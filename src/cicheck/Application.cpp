//  CICheck/Application.cpp
//	Project:	CICheck
//	Author:		Stanislav Demyanovich <mezozoysky@gmail.com>

#include "Application.hpp"
#include "Rules.hpp"
#include <Poco/Util/HelpFormatter.h>
#include <sstream>
#include <fmt/format.h>
#include <CICheck/task/Task.hpp>

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

	fmt::print( "cic.dir.bin:   {}\n", config().getString( "cic.dir.bin" ) );
	fmt::print( "cic.dir.share: {}\n", config().getString( "cic.dir.share" ) );
	fmt::print( "cic.dir.etc:   {}\n", config().getString( "cic.dir.etc" ) );

    // super initialize
    Poco::Util::Application::initialize ( self );


    mTaskProv = new task::TaskProvider();
}

void Application::uninitialize()
{
	delete mTaskProv;

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
		fmt::print( stderr, "[error] no arguments given\n{}\n", formatHelpText() );
		return ( EXIT_USAGE );
	}

	std::string taskName{ args[ 0 ] };
	std::string ruleName{ args.size() > 1 ? args[ 1 ] : "build" };
	
	fmt::print( "requested task: '{0}'; requested rule: '{1}'\n", taskName, ruleName );

//    std::cout
//        << "rules count: "  << Rules::count()           << std::endl
//        << "bad index: "    << Rules::badIndex()        << std::endl
//        << "first rule: "   << Rules::names[ 0 ]        << std::endl
//        << "clean index: "  << Rules::index( "clean" )  << std::endl;

    if ( Rules::index( ruleName ) == Rules::badIndex() )
    {
		fmt::print( "error: unknown rule '{}'\n{}\n", ruleName, formatHelpText() );
        return ( EXIT_USAGE );
    }

    mTaskProv->loadDecls(
        Poco::Path::forDirectory(
            config().getString( "cic.dir.etc" )
        ).setFileName( "cicheck__task_declarations.xml" ).toString()
    );

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

    if ( !mTaskProv->isDeclared( taskName ) )
    {
        fmt::print( stderr, "[fatal] requested task '{}' is not declared; terminating;", taskName );
        return ( EXIT_UNAVAILABLE );
    }
    for ( std::size_t rule{ 0 }; rule <= Rules::index( ruleName ); ++rule )
    {
		fmt::print( "-- checking task '{}', rule '{}'\n", taskName, Rules::names[ rule ] );
    }

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
