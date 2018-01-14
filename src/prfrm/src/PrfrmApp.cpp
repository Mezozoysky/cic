//  cic
//
//  cic - Copyright (C) 2017-2018 Stanislav Demyanovich <mezozoysky@gmail.com>
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
/// \brief PrfrmApp definition
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright cic is released under the terms of zlib/png license


#include "PrfrmApp.hpp"
#include <cic/plan/LinearPlan.hpp>
#include <cic/plan/DepsTreePlan.hpp>
#include <cic/plan/DepsTreePhase.hpp>
#include <fmt/format.h>
#include <sstream>
#include <fstream>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Environment.h>
#include <Poco/String.h>
#include <cic/scripting/Scripting.hpp>
#include <cic/plan/ActFailure.hpp>
#include <cic/plan/ActSuccess.hpp>
#include <cic/plan/ActShell.hpp>
#include <Poco/Logger.h>
#include <Poco/Channel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/ConsoleChannel.h>
#include <cic/plan/Report.hpp>
#include <Poco/FileStream.h>
#include <Poco/DOM/Document.h>
#include <Poco/SAX/InputSource.h>
#include <Poco/File.h>


using Poco::AutoPtr;
using Poco::XML::Document;
using Poco::XML::Document;
using Poco::XML::NamedNodeMap;
using Poco::XML::Node;
using namespace fmt::literals;
using Poco::Channel;
using Poco::ConsoleChannel;
using Poco::FileInputStream;
using Poco::FormattingChannel;
using Poco::Logger;
using Poco::PatternFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::XML::Element;
using Poco::XML::InputSource;
using cic::plan::Act;
using cic::plan::ActFailure;
using cic::plan::ActShell;
using cic::plan::ActSuccess;
using cic::plan::DepsTreePhase;
using cic::plan::DepsTreePlan;
using cic::plan::LinearPlan;
using cic::plan::Phase;
using cic::plan::PhaseReport;
using cic::plan::Plan;
using cic::plan::PlanReport;
using cic::plan::Report;
using cic::plan::Target;
using fmt::print;

namespace cic
{
namespace prfrm
{

PrfrmApp::PrfrmApp() noexcept
: Poco::Util::Application()
, mIsStopRequestedByOption{ false }
{
}

void PrfrmApp::optionCallback( const std::string& name, const std::string& value )
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
        print( "cic prfrm v.0.0.1\n" );
        stopOptionsProcessing();
    }
    else if ( name == "verbose" )
    {
        config().setBool( "cic.prfrm.options.verbose", true );
    }
    else
    {
        assert( false );
    }
}

void PrfrmApp::initialize( Poco::Util::Application& self )
{
    Application::initialize( self );

    Poco::Channel* consoleChannel{ new ConsoleChannel() };
    //    auto formatter( new PatternFormatter( "|%q|%y.%m.%d %h:%M:%S.%i|%P:%T|%s| %t" ) );
    auto formatter( new PatternFormatter( "|%q|%y.%m.%d %h:%M:%S.%i|%P| %t" ) );
    auto formattingChannel( new FormattingChannel( formatter, consoleChannel ) );
    auto& logger = Logger::get( "Application" );
    logger.setChannel( formattingChannel );
    logger.setLevel( Poco::Message::PRIO_DEBUG );

    // define initial cic home path
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
    logger.debug( "usingCustomConfig: {}"_format( usingCustomConfig ) );

    // define cic home path
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
    appSharePath.pushDirectory( "prfrm" );

    if ( !config().hasProperty( "cic.prfrm.shareDir" ) )
    {
        config().setString( "cic.prfrm.shareDir", appSharePath.toString() );
    }
    else
    {
        appSharePath = Poco::Path::forDirectory( config().getString( "cic.prfrm.shareDir" ) );
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
    appETCPath.pushDirectory( "prfrm" );

    if ( !config().hasProperty( "cic.prfrm.etcDir" ) )
    {
        config().setString( "cic.prfrm.etcDir", appETCPath.toString() );
    }
    else
    {
        appETCPath = Poco::Path::forDirectory( config().getString( "cic.prfrm.etcDir" ) );
    }


    if ( !usingCustomConfig )
    {
        // Load default configuration
        std::string defCfg{ appETCPath.setFileName( "prfrm.properties" ).toString() };
        try
        {
            loadConfiguration( defCfg );
        }
        catch ( Poco::FileNotFoundException& exc )
        {
            logger.warning(
                "Default prfrm configuration isnt provided: {}"
                ""_format( exc.displayText() ) );
        }
    }


    {
        auto reportFactory( mIndustry.registerFactory< Report >() );
        auto planFactory( mIndustry.registerFactory< Plan >() );
        planFactory->registerId< DepsTreePlan >( DepsTreePlan::getClassNameStatic() );
        planFactory->registerId< DepsTreePlan >( "default" );
        reportFactory->registerId< PlanReport >( DepsTreePlan::getClassNameStatic() );
        planFactory->registerId< LinearPlan >( LinearPlan::getClassNameStatic() );
        reportFactory->registerId< PlanReport >( LinearPlan::getClassNameStatic() );
        auto phaseFactory = mIndustry.registerFactory< Phase >();
        phaseFactory->registerId< DepsTreePhase >( DepsTreePhase::getClassNameStatic() );
        phaseFactory->registerId< DepsTreePhase >( "default" );
        reportFactory->registerId< PhaseReport >( DepsTreePhase::getClassNameStatic() );
        phaseFactory->registerId< Phase >( Phase::getClassNameStatic() );
        reportFactory->registerId< PhaseReport >( Phase::getClassNameStatic() );
        auto actFactory = mIndustry.registerFactory< Act >();
        actFactory->registerId< ActSuccess >( ActSuccess::getClassNameStatic() );
        reportFactory->registerId< Report >( ActSuccess::getClassNameStatic() );
        actFactory->registerId< ActFailure >( ActFailure::getClassNameStatic() );
        reportFactory->registerId< Report >( ActFailure::getClassNameStatic() );
        actFactory->registerId< ActShell >( ActShell::getClassNameStatic() );
        reportFactory->registerId< Report >( ActShell::getClassNameStatic() );
    }
}

void PrfrmApp::uninitialize()
{
    Poco::Util::Application::uninitialize();
}

void PrfrmApp::defineOptions( Poco::Util::OptionSet& options )
{
    Poco::Util::Application::defineOptions( options );

    options.addOption(
        Poco::Util::Option( "help", "h", "print usage info, ignore all other options and exit;" )
            .required( false )
            .repeatable( false )
            .callback( Poco::Util::OptionCallback< PrfrmApp >( this, &PrfrmApp::optionCallback ) ) );

    options.addOption(
        Poco::Util::Option( "version",
                            "v",
                            "print version/copyright info, ignore all other options and exit" )
            .required( false )
            .repeatable( false )
            .callback( Poco::Util::OptionCallback< PrfrmApp >( this, &PrfrmApp::optionCallback ) ) );

    options.addOption(
        Poco::Util::Option( "verbose", "V", "be verbose" )
            .required( false )
            .repeatable( false )
            .callback( Poco::Util::OptionCallback< PrfrmApp >( this, &PrfrmApp::optionCallback ) ) );

    options.addOption( Poco::Util::Option( "workspace", "w", "path to workspace directory to operate within" )
                           .required( false )
                           .repeatable( false )
                           .argument( "path", true )
                           .binding( "cic.prfrm.workspace", &config() ) );

    options.addOption(
        Poco::Util::Option( "reportdir", "r", "path to directory in wich to save report and perform log" )
            .required( false )
            .repeatable( false )
            .argument( "path", true )
            .binding( "cic.prfrm.reportdir", &config() ) );
}

int PrfrmApp::main( const std::vector< std::string >& args )
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

    Poco::Path pwdPath{ Poco::Path::forDirectory( config().getString( "system.currentDir" ) ) };
    Poco::Path sharePlansPath{ Poco::Path::forDirectory( config().getString( "cic.prfrm.shareDir" ) ) };
    sharePlansPath.pushDirectory( "plans" );

    // fetch/clarify given file path
    Poco::Path givenFilePath{ Poco::trim( args[ 0 ] ) };
    if ( givenFilePath.isRelative() )
    {
        Poco::Path tmpPath{ givenFilePath };
        tmpPath.makeAbsolute( pwdPath );
        Poco::File file{ tmpPath };
        if ( !file.exists() )
        {
            tmpPath.assign( givenFilePath );
            tmpPath.makeAbsolute( sharePlansPath );
        }
        file = tmpPath;
        if ( !file.exists() )
        {
            logger().error( "File '{}' isnt found"_format( givenFilePath.toString() ) );
            return ( EXIT_DATAERR );
        }
        givenFilePath.assign( tmpPath );
    }

    Target::Ptr target{ std::make_shared< Target >() };
    Plan::Ptr plan;

    // form/load plan and target using the given file
    {
        AutoPtr< Document > doc;
        // load xml doc
        {
            FileInputStream istr{ givenFilePath.toString() };
            InputSource input{ istr };
            doc = mParser.parse( &input );
        }

        Element* root{ doc->documentElement() };

        if ( root->nodeName() == "plan" ) // given file contains plan
        {
            // load plan from the given file
            std::string planClass{ Poco::trim( root->getAttribute( "class" ) ) };
            if ( planClass.empty() )
            {
                planClass = "default";
            }
            auto factory = mIndustry.getFactory< Plan >();
            assert( factory != nullptr );
            plan.reset( factory->create( planClass ) );
            plan->loadFromXML( root, &mIndustry );
            // set given file path as plan path for empty target
            target->setPlanPath( givenFilePath.toString() );
        }
        else if ( root->nodeName() == "target" ) // given file contains target
        {
            // load target from the given file
            target->loadFromXML( root, &mIndustry );
            if ( target->getPlanPath().empty() )
            {
                logger().error( "Target '{}' has no reference to plan file" );
                return ( EXIT_DATAERR );
            }

            // clarify target's plan path
            {
                Poco::Path planFilePath{ target->getPlanPath() };

                Poco::Path tmpPath;
                Poco::File tmpFile;
                if ( planFilePath.isAbsolute() )
                {
                    tmpFile = planFilePath;
                    if ( !tmpFile.exists() )
                    {
                        logger().error( "Plan path '{}' isnt found"_format( planFilePath.toString() ) );
                        return ( EXIT_DATAERR );
                    }
                }
                if ( planFilePath.isRelative() )
                {
                    tmpPath = planFilePath;
                    tmpPath.makeAbsolute( givenFilePath.parent() ); // try relative to target parent dir
                    tmpFile = tmpPath;
                    if ( tmpFile.exists() )
                    {
                        planFilePath = tmpPath;
                    }
                }
                if ( planFilePath.isRelative() )
                {
                    tmpPath = planFilePath;
                    tmpPath.makeAbsolute( sharePlansPath );
                    tmpFile = tmpPath;
                    if ( tmpFile.exists() )
                    {
                        planFilePath = tmpPath;
                    }
                }
                if ( planFilePath.isRelative() )
                {
                    logger().error( "Relative plan path '{}' isnt found"_format( planFilePath.toString() ) );
                    return ( EXIT_DATAERR );
                }

                target->setPlanPath( planFilePath.toString() );
            }

            // load the plan from target's plan path
            {
                AutoPtr< Document > planDoc;
                {
                    FileInputStream istr{ target->getPlanPath() };
                    InputSource input{ istr };
                    planDoc = mParser.parse( &input );
                }
                Element* planRoot{ planDoc->documentElement() };

                std::string planClass{ Poco::trim( planRoot->getAttribute( "class" ) ) };
                if ( planClass.empty() )
                {
                    planClass = "default";
                }
                auto factory = mIndustry.getFactory< Plan >();
                assert( factory != nullptr );
                plan.reset( factory->create( planClass ) );
                plan->loadFromXML( planRoot, &mIndustry );
            }
        }
    } // target and plan are formed/loaded

    // fetch/clarify workspace path
    {
        Poco::Path workspacePath{};
        std::string workspace{ config().getString( "cic.prfrm.workspace", "" ) };
        if ( workspace.empty() ) // workspace option isnt spacified
        {
            if ( target->getWorkspace().empty() )
            {
                target->setWorkspace( pwdPath.toString() );
            }
            else
            {
                workspacePath = Poco::Path::forDirectory( target->getWorkspace() );
                if ( workspacePath.isRelative() )
                {
                    workspacePath.makeAbsolute( pwdPath );
                    target->setWorkspace( workspacePath.toString() );
                }
            }
        }
        else
        {
            workspacePath = Poco::Path::forDirectory( workspace );
            if ( workspacePath.isRelative() )
            {
                workspacePath.makeAbsolute( pwdPath );
                target->setWorkspace( workspacePath.toString() );
            }
        }
        // TODO: Shure for workspace directory exists
        config().setString( "cic.prfrm.workspace", target->getWorkspace() );
    }

    // fetch/clarify report dir path
    {
        Poco::Path reportDirPath{};
        std::string reportDir{ config().getString( "cic.prfrm.reportDir", "" ) };
        if ( reportDir.empty() ) // report-dir option isnt spacified
        {
            if ( target->getReportDir().empty() )
            {
                target->setReportDir( pwdPath.toString() );
            }
            else
            {
                reportDirPath = Poco::Path::forDirectory( target->getReportDir() );
                if ( reportDirPath.isRelative() )
                {
                    reportDirPath.makeAbsolute( pwdPath );
                    target->setReportDir( reportDirPath.toString() );
                }
            }
        }
        else
        {
            reportDirPath = Poco::Path::forDirectory( reportDir );
            if ( reportDirPath.isRelative() )
            {
                reportDirPath.makeAbsolute( pwdPath );
                target->setReportDir( reportDirPath.toString() );
            }
        }
        // TODO: Shure for report directory exists
        config().setString( "cic.prfrm.reportDir", target->getReportDir() );
    }

    // fetch/clarify phase list
    {
        std::vector< std::string > phaseList;
        if ( args.size() > 1 )
        {
            for ( std::size_t i{ 1 }; i < args.size(); ++i )
            {
                phaseList.push_back( Poco::trim( args[ i ] ) );
            }
        }

        if ( phaseList.empty() )
        {
            if ( target->getPhases().empty() )
            {
                logger().error( "Target phases are not specified" );
                return ( EXIT_USAGE );
            }
        }
        else
        {
            target->setPhases( phaseList );
        }
    }

    // verbose output
    bool verbose{ config().getBool( "cic.prfrm.options.verbose", false ) };
    if ( verbose )
    {
        logger().information(
            fmt::format( "Config:\n"
                         "\t cic home dir:       : '{}';\n"
                         "\t cic binary dir      : '{}';\n"
                         "\t cic share dir       : '{}';\n"
                         "\t cic etc dir         : '{}';\n"
                         "\t application dir     : '{}';\n"
                         "\t cic prfrm share dir : '{}';\n"
                         "\t cic prfrm etc dir   : '{}';\n"
                         "\t target workspace    : '{}';\n"
                         "\t target report dir   : '{}';",
                         config().getString( "cic.homeDir" ),
                         config().getString( "cic.binDir" ),
                         config().getString( "cic.shareDir" ),
                         config().getString( "cic.etcDir" ),
                         config().getString( "application.dir" ),
                         config().getString( "cic.prfrm.shareDir" ),
                         config().getString( "cic.prfrm.etcDir" ),
                         target->getWorkspace(),
                         target->getReportDir() ) );
    }

    // perform target and save report/log/etc
    std::shared_ptr< Report > report{};
    std::ofstream outStream{};
    Poco::Path logFilePath{ Poco::Path::forDirectory( target->getReportDir() ) };
    {
        logFilePath.setFileName( "perform_target.log" );
        outStream.open( logFilePath.toString() );
    }
    if ( givenFilePath.toString() == target->getPlanPath() ) // given file is plan
    {
        outStream << "Start performing custom target for plan '{}'..."_format( target->getPlanPath() )
                  << std::endl;
    }
    else // given file is target
    {
        outStream << "Start performing target '{}'..."_format( givenFilePath.toString() ) << std::endl;
    }
    report = plan->Act::perform( target.get(), mIndustry, outStream, outStream );
    outStream << "Finished performing custom target for plan '{}': {}"_format( target->getPlanPath(),
                                                                               report->getSuccess() ?
                                                                                   "SUCCESS" :
                                                                                   "FAILURE" )
              << std::endl;
    outStream.close();

    logger().information( "PERFORM {}"_format( report->getSuccess() ? "SUCCESS" : "FAILURE" ) );
    logger().information( "Perform log saved into '{}'"_format( logFilePath.toString() ) );

    return ( EXIT_OK );
}

std::string PrfrmApp::formatHelpText() const noexcept
{
    std::ostringstream ss;

    Poco::Util::HelpFormatter hf( options() );
    hf.setCommand( commandName() );
    hf.setUsage( "[options] <plan_file> [phases...]" );
    hf.setHeader(
        R"(where:
    plan_file   path to target plan file; if relative, considered relative
                to target workspace path; if path not found in workspace,
                considered relative to application share dir; if the path
                still not found considered configuration error;
    phases...   list of 1 or more phases to perform; plan's default phases
                used if no specified; if plan doesnt provide default phases,
                considered configuration error;

    options are listed below:)" );
    hf.setFooter( "Live long and prosper!" );
    hf.setWidth( 80 );
    hf.format( ss );

    return ( ss.str() );
}

void PrfrmApp::printConfig( const std::string& rootKey ) const
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


} // namespace prfrm
} // namespace cic
