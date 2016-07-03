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
: Util::Application()
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
	Util::Application::initialize ( self );

	// Init application logger
	auto splitterChannel( new SplitterChannel() );

	auto consoleChannel( new ConsoleChannel() );
	auto rotatedFileChannel( new FileChannel( "cicheck.log" ) );

	rotatedFileChannel->setProperty( "rotation", "1M" );
	rotatedFileChannel->setProperty( "archive", "timestamp" );
	rotatedFileChannel->setProperty( "compress", "true" );
	rotatedFileChannel->setProperty( "purgeAge", "12 months" );
	rotatedFileChannel->setProperty( "rotateOnOpen", "false" );
	rotatedFileChannel->setProperty( "flush", "true" );

	splitterChannel->addChannel( consoleChannel );
	splitterChannel->addChannel( rotatedFileChannel );


	auto formatter(new PatternFormatter("|%q|%y.%m.%d %h:%M:%S.%i|%P:%T|%s| %t"));
	auto formattingChannel(new FormattingChannel(formatter, splitterChannel));

	auto& logger = Logger::get( "Application" );
	logger.setChannel( formattingChannel );
	logger.setLevel( Message::PRIO_TRACE );

	logger.information( "---------------- Start logging ----------------" );
	logger.debug( "Initializing application .." );

	mTaskProv.addSource(
			Poco::Path::forDirectory( config().getString( "cic.dir.etc" ) )
					.setFileName( "cicheck__tasks.xml" )
					.toString()
	);
	logger.debug( "TaskProvider initialized;" );

	logger.debug( ".. Done initializing application" );
}

void Application::uninitialize()
{
	logger().debug( "Uninitializing application .." );

	// TODO: uninitialize stuff here

	logger().debug( ".. Done uninitializing application" );

	Util::Application::uninitialize();
}

void Application::defineOptions( Poco::Util::OptionSet& options )
{
	Util::Application::defineOptions ( options );

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
			"* application.dir: '{}'\n"\
			"* cic.dir.share:   '{}'\n"\
			"* cic.dir.etc:     '{}'"\
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
		logger().fatal( "No arguments given!" );
		fmt::print( "{}\n", formatHelpText() );
		return ( EXIT_USAGE );
	}

	std::string taskName{ args[ 0 ] };
	std::string tgtName;

	if ( args.size() > 1 )
	{
		tgtName = args[ 1 ];
	}

	logger().information(
			"requested task: '{0}'; requested tgt: '{1}'"_format( taskName, tgtName )
	);

	// Load declarations
//	mTaskProv->loadDecls(
//		Poco::Path::forDirectory( config().getString( "cic.dir.etc" ) )
//				.setFileName( "cicheck__task_declarations.xml" )
//				.toString()
//	);

	// Output declarations
//	{
//		fmt::MemoryWriter mw;
//		mw.write( "Declared:\n" );
//		auto decls( mTaskProv->getDecls() );
//		for ( auto decl: decls )
//		{
//			mw.write( "\tname: '{}'; path: '{}';\n", decl.name, decl.path );
//		}
//		std::cout << mw.str();
//	}

//	tu::ZarFactory zarFactory;
//	zarFactory.createFactory< task::AbstractRule >();
//	auto ruleFactory( zarFactory.getFactory< task::AbstractRule >() );
//	ruleFactory->registerId< task::BashScriptRule >( "bashScript" );
//	task::AbstractRule* rptr = ruleFactory->create< task::BashScriptRule >();

	// Test for requested task declaration
//	if ( !mTaskProv->isTaskDeclared( taskName ) )
//	{
//		fmt::print(
//			stderr
//			, "[fatal] requested task '{}' is not declared;\n"\
//			 "\tterminating;\n"
//			, taskName
//		);
//		return ( EXIT_CONFIG );
//	}

	// Load task
	auto task( mTaskProv.get( taskName ) );
	if ( task == nullptr )
	{
		logger().fatal(
			"Requested task '{}' isnt provided in any way;"_format( taskName )
		);
		return ( EXIT_CONFIG );
	}

	// Output task description
//	fmt::print( "'{}' task description: '{}'\n", taskName, task->getDescription() );

	// Test for requested target
//	if ( ! task->getTargetSet()->count( tgtName ) )
//	{
//		fmt::print(
//				   stderr
//				   , "[fatal] task '{}' has no requested target '{}';\n"\
//				   "\tterminating;\n"
//				   , taskName
//				   , tgtName
//				   );
//		return ( EXIT_USAGE );
//	}

	task->perform( tgtName );

	//Output sequence for requested target
//	{
//		const task::AbstractTargetSet::Sequence seq{
//			task->getTargetSet()->calcSequenceFor( tgtName )
//		};
//		fmt::MemoryWriter mw;
//		mw.write( "\n" );
//		for ( const auto t: seq )
//		{
//			mw.write( "\t'{}'\n", t );
//		}
//		fmt::print(
//			"Sequence for target '{0}': [{1}];\n"
//			, tgtName
//			, mw.str()
//		);
//	}

//    mRuleFactory.registerId< task::BashScriptRule >( "bashScript" );
//    task::AbstractRule* rule{ mRuleFactory.create( "bashScript" ) };

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
