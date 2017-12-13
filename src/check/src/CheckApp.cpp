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
#include <cic/check/Plan.hpp>
#include <cic/check/XMLUtils.hpp>
#include <fmt/format.h>
#include <sstream>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Environment.h>
#include <Poco/String.h>
#include <cic/scripting/Scripting.hpp>

using Poco::XML::Document;
using Poco::XML::Node;
using Poco::XML::NamedNodeMap;
using DocumentPtr = Poco::AutoPtr< Document >;
using namespace fmt::literals;

namespace cic
{
namespace check
{

CheckApp::CheckApp() noexcept
: Poco::Util::Application()
, mIsHelpOptionRequested{ false }
{
}

void CheckApp::helpOptionCallback( const std::string& name, const std::string& value )
{
    mIsHelpOptionRequested = true;
    fmt::print( "{}\n", formatHelpText() );
    stopOptionsProcessing();
}

void CheckApp::initialize( Poco::Util::Application& self )
{
    // Init default application directories
    Poco::Path defaultHomePath;
    if ( Poco::Environment::has( "CIC_HOME" ) )
    {
        defaultHomePath = Poco::Path::forDirectory( Poco::Environment::get( "CIC_HOME" ) );
    }
    else
    {
        defaultHomePath = Poco::Path::forDirectory( config().getString( "application.dir" ) ).parent();
    }

    Poco::Path defaultBinPath{ defaultHomePath };
    defaultBinPath.pushDirectory( "bin" );
    Poco::Path defaultSharePath{ defaultHomePath };
    defaultSharePath.pushDirectory( "share" ).pushDirectory( "cic" ).pushDirectory( "check" );

    // 	std::string defaultShareDir{
    // 			Poco::Path::forDirectory( "./share/cic" ).makeAbsolute( prefixDirPath ).toString()
    // 	};

    // Load custom application config
    // ( cic-check.properties placed beside the application executable or above its path )
    bool usingCustomConfig{ loadConfiguration() > 0 };

    if ( !config().hasProperty( "cic.dir.home" ) )
    {
        config().setString( "cic.dir.home", defaultHomePath.toString() );
    }

    if ( !config().hasProperty( "cic.dir.bin" ) )
    {
        config().setString( "cic.dir.bin", defaultBinPath.toString() );
    }

    if ( !config().hasProperty( "cic.dir.share" ) )
    {
        config().setString( "cic.dir.share", defaultSharePath.toString() );
    }

    if ( !usingCustomConfig )
    {
        // Load default configuration
        std::string defCfg{ defaultSharePath.setFileName( "check.properties" ).toString() };
        try
        {
            loadConfiguration( defCfg );
        }
        catch ( Poco::FileNotFoundException& exc )
        {
            logger().warning(
                "Default cic-check configuration isnt provided: {}"
                ""_format( exc.displayText() ) );
        }
    }

    // Application-level properties
    // 	if ( !config().hasProperty( "cic.check.property_name" ) )
    // 	{
    // 		config().setString( "cic.check.property_name", propertyValue );
    // 	}

    Poco::Util::Application::initialize( self );

    // TODO: configure logging!
    logger().setLevel( Poco::Message::PRIO_TRACE );

    // logger.information( "---------------- Start logging ----------------" );
    // logger.debug( "Initializing CheckApp .." );

    // TODO: initialize stuff here
    {
        auto planFactory = mIndustry.create< Plan >();
        planFactory->registerId< Plan >( "default" );
        auto phaseFactory = mIndustry.create< Phase >();
        phaseFactory->registerId< Phase >( "default" );
        auto ruleFactory = mIndustry.create< Rule >();
        ruleFactory->registerId< SuccessRule >( "success" );
        ruleFactory->registerId< FailureRule >( "failure" );
        ruleFactory->registerId< SystemCmdRule >( "systemCmd" );
    }
    // logger.debug( ".. Done initializing CheckApp" );
}

void CheckApp::uninitialize()
{
    // logger().debug( "Uninitializing CheckApp .." );
    // TODO: uninitialize stuff here
    // logger().debug( ".. Done uninitializing CheckApp" );

    Poco::Util::Application::uninitialize();
}

void CheckApp::defineOptions( Poco::Util::OptionSet& options )
{
    Poco::Util::Application::defineOptions( options );

    options.addOption(
        Poco::Util::Option( "help", "h", "print the help screen" )
            .required( false )
            .repeatable( false )
            .callback( Poco::Util::OptionCallback< CheckApp >( this, &CheckApp::helpOptionCallback ) ) );
}

int CheckApp::main( const std::vector< std::string >& args )
{
    logger().debug(
        "Config:\n"
        "* cic.dir.home:  '{}'\n"
        "* cic.dir.bin:   '{}'\n"
        "* cic.dir.share: '{}'"
        ""_format( config().getString( "cic.dir.home" ),
                   config().getString( "cic.dir.bin" ),
                   config().getString( "cic.dir.share" ) ) );

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

    std::string planName{ args[ 0 ] };
    std::string phaseName;

    if ( args.size() > 1 )
    {
        phaseName = args[ 1 ];
    }

    logger().information(
        "Requested plan: '{}';{}"
        ""_format( planName, phaseName.empty() ? "" : " Requested phase: '{}';"_format( phaseName ) ) );

    // TODO: load and check plan/phase
    // try
    // {
    //     mPlanProvider.loadDecls( Poco::Path::forDirectory( config().getString( "cic.dir.share" ) )
    //                                  .pushDirectory( "check" )
    //                                  .setFileName( "declarations.xml" )
    //                                  .toString() );
    // }
    // catch ( Poco::FileNotFoundException& exc )
    // {
    //     logger().error(
    //         "Error while loading declarations: {}"
    //         ""_format( exc.displayText() ) );
    // }


    // if ( mPlanProvider.isDeclsEmpty() )
    // {
    //     logger().critical( "No declarations provided" );
    //     return ( EXIT_DATAERR );
    // }

    // if ( !mPlanProvider.isDeclared( planName ) )
    // {
    //     logger().critical( "Plan '{}' isnt declared;"_format( planName ) );
    //     return ( EXIT_DATAERR );
    // }

    Poco::Path planPath{ config().getString( "cic.dir.share" ) };
    planPath.pushDirectory( "plans" ).setBaseName( planName ).setExtension( "xml" );
    DocumentPtr doc{ fetchDoc( planPath.toString(), mParser ) };
    Node* planRoot{ fetchNode( doc, "/plan", Node::ELEMENT_NODE ) };
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

    // try
    // {
    //     plan = mPlanProvider.get( planName );
    // }
    // catch ( Poco::Exception& exc )
    // {
    //     logger().critical( "Error loading plan '{}': {}"_format( planName, exc.displayText() ) );
    //     return ( exc.code() );
    // }

    bool result{ false };
    try
    {
        result = plan->check( phaseName );
    }
    catch ( Poco::Exception& exc )
    {
        logger().critical(
            "Error checking plan '{}' for phase '{}': {}"
            ""_format( planName, phaseName, exc.displayText() ) );
        return ( exc.code() );
    }

    if ( !result )
    {
        logger().information( "CHECK FAILED" );
        // TODO: Failure analysis / reporting
    }
    else
    {
        logger().information( "CHECK SUCCESSFULL" );
    }

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


} // namespace check
} // namespace cic
