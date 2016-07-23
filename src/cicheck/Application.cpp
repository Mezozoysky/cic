//  cicheck/Application.cpp
//	Project:	CICheck
//	Author:		Stanislav Demyanovich <mezozoysky@gmail.com>

#include "Application.hpp"
#include <Poco/Util/HelpFormatter.h>
#include <sstream>
#include <fmt/format.h>

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
, mIsColorsOptionRequested{ false }
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

	Poco::Channel* consoleChannel;
	if ( mIsColorsOptionRequested )
	{
		consoleChannel = new ColorConsoleChannel();
		consoleChannel->setProperty( "enableColors", "true" );
		consoleChannel->setProperty( "traceColor", "cyan" );
		consoleChannel->setProperty( "debugColor", "lightCyan" );
		consoleChannel->setProperty( "informationColor", "white" );
		consoleChannel->setProperty( "noticeColor", "lightGreen" );
		consoleChannel->setProperty( "warningColor", "yellow" );
		consoleChannel->setProperty( "errorColor", "lightRed" );
		consoleChannel->setProperty( "criticalColor", "lightMagenta" );
		consoleChannel->setProperty( "fatalColor", "lightMagenta" );
	}
	else
	{
		consoleChannel = new ConsoleChannel();
	}

	auto rotatedFileChannel( new FileChannel( "cicheck.log" ) );
	rotatedFileChannel->setProperty( "rotation", "1M" );
	rotatedFileChannel->setProperty( "archive", "timestamp" );
	rotatedFileChannel->setProperty( "compress", "true" );
	rotatedFileChannel->setProperty( "purgeAge", "12 months" );
	rotatedFileChannel->setProperty( "rotateOnOpen", "false" );
	rotatedFileChannel->setProperty( "flush", "true" );

	auto splitterChannel( new SplitterChannel() );
	splitterChannel->addChannel( consoleChannel );
	splitterChannel->addChannel( rotatedFileChannel );

	auto formatter(new PatternFormatter("|%q|%y.%m.%d %h:%M:%S.%i|%P:%T|%s| %t"));
	auto formattingChannel(new FormattingChannel(formatter, splitterChannel));

	auto& logger = Logger::get( "Application" );
	logger.setChannel( formattingChannel );
	logger.setLevel( Message::PRIO_TRACE );

	logger.information( "---------------- Start logging ----------------" );
	logger.debug( "Initializing application .." );

	mTaskProv.init();
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
							this
							, &Application::helpOptionCallback
					)
			)
	);
	options.addOption(
			Poco::Util::Option( "colors", "c", "use colors for console logging" )
			.required( false )
			.repeatable( false )
			.callback(
					Poco::Util::OptionCallback< Application >(
							this
							, &Application::colorsOptionCallback
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
		logger().debug( "No arguments given!" );
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

	// Load task
	auto task( mTaskProv.get( taskName ) );
	if ( task == nullptr )
	{
		logger().critical(
			"Requested task '{}' isnt provided in any way;"_format( taskName )
		);
		return ( EXIT_CONFIG );
	}

	if ( !task->check( tgtName ) )
	{
		logger().information( "CHECK FAILED!\n" );
		//TODO: some onCheckFail
	}
	else
	{
		logger().information( "CHECK SUCCESSFULL!\n" );
	}

	return ( EXIT_OK );
}

void Application::helpOptionCallback( const std::string& name, const std::string& value )
{
	mIsHelpOptionRequested = true;
	fmt::print( "{}", formatHelpText() );
	stopOptionsProcessing();
}

void Application::colorsOptionCallback( const std::string &name, const std::string &value )
{
	mIsColorsOptionRequested = true;
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
