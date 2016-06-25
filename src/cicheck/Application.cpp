//  CICheck/Application.cpp
//	Project:	CICheck
//	Author:		Stanislav Demyanovich <mezozoysky@gmail.com>

#include "Application.hpp"
#include "Rules.hpp"
#include <Poco/Util/HelpFormatter.h>
#include <iostream>
#include <sstream>

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
    std::string binaryDir{ config().getString( "application.dir" ) };
    config().setString( "cic.dir.bin", binaryDir );

    Poco::Path prefixDirPath{ Poco::Path::forDirectory( binaryDir ).parent() };

    loadConfiguration();
    
    if ( !config().hasProperty( "cic.dir.share" ) )
    {
		config().setString( "cic.dir.share", Poco::Path::forDirectory( "./share/CICheck" ).makeAbsolute( prefixDirPath ).toString() );
    }
    if ( !config().hasProperty( "cic.dir.etc" ) )
    {
        config().setString( "cic.dir.etc", Poco::Path::forDirectory( "./etc/CICheck" ).makeAbsolute( prefixDirPath ).toString() );
    }

    std::cout
        << "cic.dir.bin:   " << config().getString( "cic.dir.bin" )     << std::endl
        << "cic.dir.share: " << config().getString( "cic.dir.share" )   << std::endl
        << "cic.dir.etc:   " << config().getString( "cic.dir.etc" )     << std::endl;

    // super initialize
    Poco::Util::Application::initialize ( self );

	//TODO: specific initialize stuff
    mTargetProvider = new tgt::TargetProvider();
    mTargetProvider->addDeclPath( Poco::Path::forDirectory( config().getString( "cic.dir.etc" ) ).setFileName( "targetDecls.cicheck.xml" ).toString() );
    mTargetProvider->addDeclPath( "/dev/null/targets.xml" );
}

void Application::uninitialize()
{
	//TODO: specific uninitialize stuff
    delete mTargetProvider;

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
	
	if ( mIsHelpOptionRequested )
	{
		return ( EXIT_OK );
	}
	if ( args.empty() )
	{
		std::cout << "error: no arguments given" << std::endl;
		std::cout << formatHelpText() << std::endl;
		return ( EXIT_USAGE );
	}

	std::string targetName{ args[ 0 ] };
	std::string ruleName{ args.size() > 1 ? args[ 1 ] : "build" };
	
	std::cout
		<< "target: "               << targetName	<< std::endl
		<< "requested rule: "		<< ruleName		<< std::endl;


//    std::cout
//        << "rules count: "  << Rules::count()           << std::endl
//        << "bad index: "    << Rules::badIndex()        << std::endl
//        << "first rule: "   << Rules::names[ 0 ]        << std::endl
//        << "clean index: "  << Rules::index( "clean" )  << std::endl;

    if ( Rules::index( ruleName ) == Rules::badIndex() )
    {
        std::cout << "error: unknown rule '" << ruleName << "'" << std::endl;
        std::cout << formatHelpText() << std::endl;
        return ( EXIT_USAGE );
    }

    mTargetProvider->reloadDecls();
//	if ( mTargets.isAvailable( targetName ) )
//	{
//		
//	}

    for ( std::size_t rule{ 0 }; rule <= Rules::index( ruleName ); ++rule )
    {
        std::cout <<  "-- checking: " << targetName << " " << Rules::names[ rule ] << std::endl;
    }

	return ( EXIT_OK );
}

void Application::helpOptionCallback( const std::string& name, const std::string& value )
{
	mIsHelpOptionRequested = true;
	std::cout << formatHelpText() << std::endl;
	stopOptionsProcessing();
}

std::string Application::formatHelpText() const
{
	std::ostringstream ss;

	Poco::Util::HelpFormatter hf( options() );
	hf.setCommand( commandName() );
	hf.setUsage( "[options] <target> <rule>" );
	hf.setHeader(
R"(where:
    target      target name to check
    rule        one of clean, configure, build, buildtests, runtests
options are listed below:)"
	);
	hf.setFooter( "Please dont wash your pet with this software!" );
	hf.setWidth( 80 );
	hf.format( ss );

	return ( ss.str() );
}

} // namespace cic
