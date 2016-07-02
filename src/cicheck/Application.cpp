//  cicheck/Application.cpp
//	Project:	CICheck
//	Author:		Stanislav Demyanovich <mezozoysky@gmail.com>

#include "Application.hpp"
#include <Poco/Util/HelpFormatter.h>
#include <sstream>
#include <fmt/format.h>
#include <CICheck/task/Task.hpp>
#include <CICheck/tu/ZarFactory.hpp>

#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/SplitterChannel.h>

using namespace Poco;
using namespace fmt::literals;

namespace cic
{

Application::Application()
: Poco::Util::Application()
, mIsHelpOptionRequested{ false }
{
}

Application::~Application()
{
}

void Application::initialize( Poco::Util::Application& self )
{
	// Init default application directories
	std::string binaryDir{ config().getString( "application.dir" ) };
	Poco::Path prefixDirPath{ Poco::Path::forDirectory( binaryDir ).parent() };
	std::string defaultShareDir{ Poco::Path::forDirectory( "./share/CICheck" ).makeAbsolute( prefixDirPath ).toString() };
	std::string defaultEtcDir{ Poco::Path::forDirectory( "./etc/CICheck" ).makeAbsolute( prefixDirPath ).toString() };

	// TODO:
	// if ( file exists( share/cicheck.properties ) )
	// {
	//		loadConfiguration( share/cicheck.properties );
	// }

	// Load default config
	loadConfiguration();

	if ( !config().hasProperty( "cic.dir.share" ) )
	{
		config().setString( "cic.dir.share", defaultShareDir );
	}
	if ( !config().hasProperty( "cic.dir.etc" ) )
	{
		config().setString( "cic.dir.etc", defaultEtcDir );
	}

	// super initialize
	Poco::Util::Application::initialize ( self );

	// Init application logger
	AutoPtr< SplitterChannel > splitterChannel( new SplitterChannel() );

	AutoPtr< Channel > consoleChannel( new ConsoleChannel() );
	AutoPtr< FileChannel > rotatedFileChannel( new FileChannel( "cicheck.log" ) );

	rotatedFileChannel->setProperty( "rotation", "1M" );
	rotatedFileChannel->setProperty( "archive", "timestamp" );
	rotatedFileChannel->setProperty( "compress", "true" );
	rotatedFileChannel->setProperty( "purgeAge", "12 months" );
	rotatedFileChannel->setProperty( "rotateOnOpen", "false" );
	rotatedFileChannel->setProperty( "flush", "true" );

	splitterChannel->addChannel( consoleChannel );
	splitterChannel->addChannel( rotatedFileChannel );


	AutoPtr<Formatter> formatter(new PatternFormatter("|%q|%y.%m.%d %h:%M:%S.%i|%P:%T|%s| %t"));
	AutoPtr<Channel> formattingChannel(new FormattingChannel(formatter, splitterChannel));

	Logger& logger = Logger::create("cicheck", formattingChannel, Message::PRIO_TRACE);
	setLogger( logger );

	logger.information( "---------------- Start logging" );
	logger.debug( "Initializing application .." );

	mTaskProv = new task::TaskProvider();

	logger.debug( ".. Done initializing application" );
}

void Application::uninitialize()
{
	logger().debug( "Uninitializing application .." );

	delete mTaskProv;

	logger().debug( ".. Done uninitializing application" );

	Poco::Util::Application::uninitialize();
}

void Application::defineOptions( Poco::Util::OptionSet& options )
{
	Poco::Util::Application::defineOptions ( options );

	options.addOption(
			Poco::Util::Option( "help", "h", "print the help screen" )
			.required( false )
			.repeatable( false )
			.callback(
					Poco::Util::OptionCallback< Application >(
							this,
							&Application::helpOptionCallback
					)
			)
	);
}

int Application::main( const std::vector< std::string >& args )
{
	logger().debug(
			"Config:\n"
			"\tapplication.dir:   '{}'\n"\
			"\tcic.dir.share: '{}'\n"\
			"\tcic.dir.etc:   '{}'\n"\
			""_format(
					config().getString( "application.dir" )
					, config().getString( "cic.dir.share" )
					, config().getString( "cic.dir.etc" )
			)
	);

	if ( mIsHelpOptionRequested )
	{
		return ( EXIT_OK );
	}
	if ( args.empty() )
	{
		fmt::print( stderr, "[error] no arguments given\n{}\n", formatHelpText() );
		return ( EXIT_USAGE );
	}

	std::string taskName{ args[ 0 ] };
	std::string tgtName;

	if ( args.size() > 1 )
	{
		tgtName = args[ 1 ];
	}

	fmt::print( "requested task: '{0}'; requested tgt: '{1}'\n", taskName, tgtName );

	// Load declarations
	mTaskProv->loadDecls(
		Poco::Path::forDirectory( config().getString( "cic.dir.etc" ) )
				.setFileName( "cicheck__task_declarations.xml" )
				.toString()
	);

	// Output declarations
	{
		fmt::MemoryWriter mw;
		mw.write( "Declared:\n" );
		auto decls( mTaskProv->getDecls() );
		for ( auto decl: decls )
		{
			mw.write( "\tname: '{}'; path: '{}';\n", decl.name, decl.path );
		}
		std::cout << mw.str();
	}

	tu::ZarFactory zarFactory;
	zarFactory.createFactory< task::AbstractRule >();
	auto ruleFactory( zarFactory.getFactory< task::AbstractRule >() );
	ruleFactory->registerId< task::BashScriptRule >( "bashScript" );
	task::AbstractRule* rptr = ruleFactory->create< task::BashScriptRule >();

	// Test for requested task declaration
	if ( !mTaskProv->isTaskDeclared( taskName ) )
	{
		fmt::print(
			stderr
			, "[fatal] requested task '{}' is not declared;\n"\
			 "\tterminating;\n"
			, taskName
		);
		return ( EXIT_CONFIG );
	}

	// Load task
	auto task( mTaskProv->loadTask( taskName ) );
	if ( task == nullptr )
	{
		fmt::print(
			"[fatal] can't load requested task '{}'\n"\
			"\tterminating;\n"
			, taskName
		);
		return ( EXIT_CONFIG );
	}

	// Output task description
	fmt::print( "'{}' task description: '{}'\n", taskName, task->getDescription() );

	// Test for requested target
	if ( ! task->getTargetSet()->count( tgtName ) )
	{
		fmt::print(
				   stderr
				   , "[fatal] task '{}' has no requested target '{}';\n"\
				   "\tterminating;\n"
				   , taskName
				   , tgtName
				   );
		return ( EXIT_USAGE );
	}

//	task->perform( tgtName );

	//Output sequence for requested target
	{
		const task::AbstractTargetSet::Sequence seq{
			task->getTargetSet()->calcSequenceFor( tgtName )
		};
		fmt::MemoryWriter mw;
		mw.write( "\n" );
		for ( const auto t: seq )
		{
			mw.write( "\t'{}'\n", t );
		}
		fmt::print(
			"Sequence for target '{0}': [{1}];\n"
			, tgtName
			, mw.str()
		);
	}

    mRuleFactory.registerId< task::BashScriptRule >( "bashScript" );
    task::AbstractRule* rule{ mRuleFactory.create( "bashScript" ) };

	return ( EXIT_OK );
}

void Application::helpOptionCallback( const std::string& name, const std::string& value )
{
	mIsHelpOptionRequested = true;
	fmt::print( "{}", formatHelpText() );
	stopOptionsProcessing();
}

std::string Application::formatHelpText() const
{
	std::ostringstream ss;

	Poco::Util::HelpFormatter hf( options() );
	hf.setCommand( commandName() );
	hf.setUsage( "[options] [<task> [target]]" );
	hf.setHeader(
R"(where:
    task        task name to check;
    target      one of clean, configure, build, buildtests, runtests;
                default is clean;
options are listed below:)"
	);
	hf.setFooter( "Please dont wash your pet with this software!" );
	hf.setWidth( 80 );
	hf.format( ss );

	return ( ss.str() );
}

} // namespace cic
