//  cic
//
//  cic - Copyright (C) 2016 Stanislav Demyanovich <mezozoysky@gmail.com>
//
//  This software is provided 'as-is', without any express or
//  implied warranty. In no event will the authors be held
//  liable for any damages arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute
//  it freely, subject to the following restrictions:
//
//  1.  The origin of this software must not be misrepresented;
//      you must not claim that you wrote the original software.
//      If you use this software in a product, an acknowledgment
//      in the product documentation would be appreciated but
//      is not required.
//
//  2.  Altered source versions must be plainly marked as such,
//      and must not be misrepresented as being the original software.
//
//  3.  This notice may not be removed or altered from any
//      source distribution.


/// \file
/// \brief CheckApp definition
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright cic is released under the terms of zlib/png license


#include "CheckApp.hpp"
#include <cic/plan/Plan.hpp>
#include <cic/plan/XMLUtils.hpp>
#include <fmt/format.h>
#include <sstream>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Environment.h>
#include <Poco/String.h>
#include <cic/scripting/Scripting.hpp>
#include <cic/plan/ActionFailure.hpp>
#include <cic/plan/ActionSuccess.hpp>
#include <cic/plan/ActionSystemCmd.hpp>
#include <Poco/Logger.h>
#include <Poco/Channel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/ConsoleChannel.h>
#include <cic/plan/Report.hpp>

using Poco::XML::Document;
using Poco::XML::NamedNodeMap;
using Poco::XML::Node;
using DocumentPtr = Poco::AutoPtr< Document >;
using namespace fmt::literals;
using Poco::Channel;
using Poco::ConsoleChannel;
using Poco::FormattingChannel;
using Poco::Logger;
using Poco::PatternFormatter;
using Poco::Util::AbstractConfiguration;
using cic::plan::Action;
using cic::plan::ActionFailure;
using cic::plan::ActionSuccess;
using cic::plan::ActionSystemCmd;
using cic::plan::Phase;
using cic::plan::Plan;
using cic::plan::Report;
using fmt::print;

namespace cic
{
namespace check
{

CheckApp::CheckApp() noexcept
: Poco::Util::Application()
, mIsStopRequestedByOption{ false }
{
}

void CheckApp::optionCallback( const std::string& name, const std::string& value )
{
    if ( name == "help" )
    {
        mIsStopRequestedByOption = true;
        print( "{}\n", formatHelpText() );
        stopOptionsProcessing();
    }
    else if ( name == "version" )
    {
        mIsStopRequestedByOption = true;
        print( "cic check v.0.0.1\n" );
        stopOptionsProcessing();
    }
    else if ( name == "verbose" )
    {
        config().setBool( "cic.check.options.verbose", true );
    }
    else if ( name == "only" )
    {
        config().setBool( "cic.check.options.only", true );
    }
    else
    {
        assert( false );
    }
}

void CheckApp::initialize( Poco::Util::Application& self )
{
    Application::initialize( self );

    Poco::Channel* consoleChannel{ new ConsoleChannel() };
    //    auto formatter( new PatternFormatter( "|%q|%y.%m.%d %h:%M:%S.%i|%P:%T|%s| %t" ) );
    auto formatter( new PatternFormatter( "|%q|%y.%m.%d %h:%M:%S.%i|%P| %t" ) );
    auto formattingChannel( new FormattingChannel( formatter, consoleChannel ) );
    auto& logger = Logger::get( "Application" );
    logger.setChannel( formattingChannel );
    logger.setLevel( Poco::Message::PRIO_DEBUG );

    // Define initial cic home path
    Poco::Path homePath;
    if ( Poco::Environment::has( "CIC_HOME" ) )
    {
        homePath = Poco::Path::forDirectory( Poco::Environment::get( "CIC_HOME" ) );
    }
    else
    {
        homePath = Poco::Path::forDirectory( config().getString( "application.dir" ) ).parent();
    }

    // try load custom config
    bool usingCustomConfig{ loadConfiguration() > 0 };

    if ( !config().hasProperty( "cic.homeDir" ) )
    {
        config().setString( "cic.homeDir", homePath.toString() );
    }
    else
    {
        homePath = Poco::Path::forDirectory( config().getString( "cic.homeDir" ) );
    }

    // define cic bin path
    Poco::Path binPath{ homePath };
    binPath.pushDirectory( "bin" );

    if ( !config().hasProperty( "cic.binDir" ) )
    {
        config().setString( "cic.binDir", binPath.toString() );
    }
    else
    {
        binPath = Poco::Path::forDirectory( config().getString( "cic.binDir" ) );
    }

    // define cic share path
    Poco::Path sharePath{ homePath };
    sharePath.pushDirectory( "share" ).pushDirectory( "cic" );

    if ( !config().hasProperty( "cic.shareDir" ) )
    {
        config().setString( "cic.shareDir", sharePath.toString() );
    }
    else
    {
        sharePath = Poco::Path::forDirectory( config().getString( "cic.shareDir" ) );
    }

    // define app share path
    Poco::Path appSharePath{ sharePath };
    appSharePath.pushDirectory( "check" );

    if ( !config().hasProperty( "cic.check.shareDir" ) )
    {
        config().setString( "cic.check.shareDir", appSharePath.toString() );
    }
    else
    {
        appSharePath = Poco::Path::forDirectory( config().getString( "cic.check.shareDir" ) );
    }

    // define cic etc path
    Poco::Path etcPath{ homePath };
    etcPath.pushDirectory( "etc" ).pushDirectory( "cic" );

    if ( !config().hasProperty( "cic.etcDir" ) )
    {
        config().setString( "cic.etcDir", etcPath.toString() );
    }
    else
    {
        etcPath = Poco::Path::forDirectory( config().getString( "cic.etcDir" ) );
    }

    // define app etc path
    Poco::Path appETCPath{ etcPath };
    appETCPath.pushDirectory( "check" );

    if ( !config().hasProperty( "cic.check.etcDir" ) )
    {
        config().setString( "cic.check.etcDir", appETCPath.toString() );
    }
    else
    {
        appETCPath = Poco::Path::forDirectory( config().getString( "cic.check.etcDir" ) );
    }


    if ( !usingCustomConfig )
    {
        // Load default configuration
        std::string defCfg{ appETCPath.setFileName( "check.properties" ).toString() };
        try
        {
            loadConfiguration( defCfg );
        }
        catch ( Poco::FileNotFoundException& exc )
        {
            logger.warning(
                "Default check configuration isnt provided: {}"
                ""_format( exc.displayText() ) );
        }
    }


    {
        auto planFactory = mIndustry.create< Plan >();
        planFactory->registerId< Plan >( "default" );
        auto phaseFactory = mIndustry.create< Phase >();
        phaseFactory->registerId< Phase >( "default" );
        auto actionFactory = mIndustry.create< Action >();
        actionFactory->registerId< ActionSuccess >( "success" );
        actionFactory->registerId< ActionFailure >( "failure" );
        actionFactory->registerId< ActionSystemCmd >( "systemCmd" );
    }
}

void CheckApp::uninitialize()
{
    Poco::Util::Application::uninitialize();
}

void CheckApp::defineOptions( Poco::Util::OptionSet& options )
{
    Poco::Util::Application::defineOptions( options );

    options.addOption(
        Poco::Util::Option( "help", "h", "print help and exit" )
            .required( false )
            .repeatable( false )
            .callback( Poco::Util::OptionCallback< CheckApp >( this, &CheckApp::optionCallback ) ) );

    options.addOption(
        Poco::Util::Option( "version", "v", "print version/copyright info and exit" )
            .required( false )
            .repeatable( false )
            .callback( Poco::Util::OptionCallback< CheckApp >( this, &CheckApp::optionCallback ) ) );

    options.addOption(
        Poco::Util::Option( "verbose", "V", "be verbose" )
            .required( false )
            .repeatable( false )
            .callback( Poco::Util::OptionCallback< CheckApp >( this, &CheckApp::optionCallback ) ) );

    options.addOption(
        Poco::Util::Option( "only", "1", "execute specified phase only, skip any dependencies" )
            .required( false )
            .repeatable( false )
            .callback( Poco::Util::OptionCallback< CheckApp >( this, &CheckApp::optionCallback ) ) );

    options.addOption( Poco::Util::Option( "workspace", "w", "workspace path to operate within" )
                           .required( false )
                           .repeatable( false )
                           .argument( "path", true )
                           .binding( "cic.check.workspace", &config() ) );
}

int CheckApp::main( const std::vector< std::string >& args )
{
    if ( mIsStopRequestedByOption )
    {
        return ( EXIT_OK );
    }
    if ( args.empty() )
    {
        logger().fatal( "No arguments given!" );
        fmt::print( "{}\n", formatHelpText() );
        return ( EXIT_USAGE );
    }

    //    print( "Start configuration:\n" );
    //    printConfig( "" );

    Poco::Path pwdPath{ Poco::Path::forDirectory( config().getString( "system.currentDir" ) ) };
    Poco::Path workspacePath;
    std::string workspaceDir{ config().getString( "cic.check.workspace", "" ) };
    if ( workspaceDir.empty() )
    {
        workspacePath = pwdPath;
    }
    else
    {
        workspacePath = Poco::Path::forDirectory( workspaceDir );
        if ( workspacePath.isRelative() )
        {
            workspacePath.makeAbsolute( pwdPath );
        }
    }
    workspaceDir = workspacePath.toString();
    config().setString( "cic.check.workspace", workspaceDir );

    bool verbose{ config().getBool( "cic.check.options.verbose", false ) };
    if ( verbose )
    {
        logger().information(
            fmt::format( "Config:\n"
                         "\t cic home dir:       : '{}'\n"
                         "\t cic binary dir      : '{}'\n"
                         "\t cic share dir       : '{}'\n"
                         "\t cic etc dir         : '{}'\n"
                         "\t application dir     : '{}'\n"
                         "\t cic check share dir : '{}'\n"
                         "\t cic check etc dir   : '{}'\n"
                         "\t cic check workspace : '{}'",
                         config().getString( "cic.homeDir" ),
                         config().getString( "cic.binDir" ),
                         config().getString( "cic.shareDir" ),
                         config().getString( "cic.etcDir" ),
                         config().getString( "application.dir" ),
                         config().getString( "cic.check.shareDir" ),
                         config().getString( "cic.check.etcDir" ),
                         workspaceDir ) );
    }

    std::string planName{ args[ 0 ] };
    std::string phaseName;

    if ( args.size() > 1 )
    {
        phaseName = args[ 1 ];
    }

    if ( verbose )
    {
        logger().information(
            fmt::format( "Requested plan: '{}';{}\n",
                         planName,
                         phaseName.empty() ? "" : " Requested phase: '{}';"_format( phaseName ) ) );
    }

    Poco::Path planPath{ config().getString( "cic.check.shareDir" ) };
    planPath.pushDirectory( "plans" ).setBaseName( planName ).setExtension( "xml" );
    DocumentPtr doc{ plan::fetchDoc( planPath.toString(), mParser ) };
    Node* planRoot{ plan::fetchNode( doc, "/plan", Node::ELEMENT_NODE ) };
    std::string typeId{ "default" };
    NamedNodeMap* attrs{ planRoot->attributes() };
    Node* attr{ attrs->getNamedItem( "typeId" ) };
    if ( attr )
    {
        typeId = Poco::trim( attr->getNodeValue() );
    }
    auto factory = mIndustry.get< Plan >();
    assert( factory != nullptr );
    Plan::Ptr plan{ factory->create( typeId ) };
    plan->loadFromXML( planRoot, &mIndustry );

    bool result{ false };
    bool only{ config().getBool( "cic.check.options.only", false ) };
    Report report;
    try
    {
        result = plan->execute( phaseName, &report, only );
    }
    catch ( Poco::Exception& exc )
    {
        logger().critical(
            "Fail to execute plan '{}' for phase '{}': {}"
            ""_format( planName, phaseName, exc.displayText() ) );
        return ( exc.code() );
    }

    if ( !result )
    {
        logger().information( "PLAN EXECUTION FAILURE" );
        // TODO: Failure analysis / reporting
    }
    else
    {
        logger().information( "PLAN EXECUTION SUCCESS" );
    }

    // print( "Finish configuration:\n" );
    //    printConfig( "" );

    scripting::foo();
    return ( EXIT_OK );
}

std::string CheckApp::formatHelpText() const noexcept
{
    std::ostringstream ss;

    Poco::Util::HelpFormatter hf( options() );
    hf.setCommand( commandName() );
    hf.setUsage( "[options] [<plan> [phase]]" );
    hf.setHeader(
        R"(where:
		plan        plan name to check;
		phase      specific phase to check; plan's default phase used if no specified;

		!Note: plan should but not must to provide default phase

		options are listed below:)" );
    hf.setFooter( "MEMENTO MORI!" );
    hf.setWidth( 80 );
    hf.format( ss );

    return ( ss.str() );
}

void CheckApp::printConfig( const std::string& rootKey ) const
{
    AbstractConfiguration::Keys keys;
    config().keys( rootKey, keys );
    if ( keys.size() == 0 )
    {
        print( "CONFIG:: '{}' -> '{}';\n"_format( rootKey, config().getString( rootKey, "--hz--" ) ) );
    }
    else
    {
        for ( auto& key : keys )
        {
            std::string newRoot;
            if ( rootKey.empty() )
            {
                printConfig( key );
            }
            else
            {
                printConfig( "{0}.{1}"_format( rootKey, key ) );
            }
        }
    }
}


} // namespace check
} // namespace cic
