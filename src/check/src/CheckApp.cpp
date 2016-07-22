//  CICheck
//
//  CICheck - Copyright (C) 2016 Stanislav Demyanovich <mezozoysky@gmail.com>
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
/// \copyright CICheck is released under the terms of zlib/png license


#include "CheckApp.hpp"
#include <fmt/format.h>
#include <sstream>
#include <Poco/Util/HelpFormatter.h>

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

void CheckApp::helpOptionCallback ( const std::string& name, const std::string& value )
{
	mIsHelpOptionRequested = true;
	fmt::print( "{}\n", formatHelpText() );
	stopOptionsProcessing();
}

void CheckApp::initialize ( Poco::Util::Application& self )
{
	// Init default application directories
	std::string binaryDir{ config().getString( "application.dir" ) };
	Poco::Path prefixDirPath{ Poco::Path::forDirectory( binaryDir ).parent() };
	std::string defaultShareDir{
			Poco::Path::forDirectory( "./share/CICheck" ).makeAbsolute( prefixDirPath ).toString()
	};
	std::string defaultEtcDir{
			Poco::Path::forDirectory( "./etc/CICheck" ).makeAbsolute( prefixDirPath ).toString()
	};

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

	Poco::Util::Application::initialize ( self );

	//TODO: configure logging!

	//logger.information( "---------------- Start logging ----------------" );
	//logger.debug( "Initializing CheckApp .." );
	// TODO: initialize stuff here
	//logger.debug( ".. Done initializing CheckApp" );

}

void CheckApp::uninitialize()
{
	//logger().debug( "Uninitializing CheckApp .." );
	// TODO: uninitialize stuff here
	//logger().debug( ".. Done uninitializing CheckApp" );

	Poco::Util::Application::uninitialize();
}

void CheckApp::defineOptions( Poco::Util::OptionSet& options )
{
    Poco::Util::Application::defineOptions ( options );

	options.addOption(
		Poco::Util::Option( "help", "h", "print the help screen" )
		.required( false )
		.repeatable( false )
		.callback(
			Poco::Util::OptionCallback< CheckApp >(
				this
				, &CheckApp::helpOptionCallback
			)
		)
	);

}

int CheckApp::main( const std::vector< std::string >& args )
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

	std::string goalName{ args[ 0 ] };
	std::string tgtName;

	if ( args.size() > 1 )
	{
		tgtName = args[ 1 ];
	}

	logger().information(
		"requested goal: '{}'; requested target: '{}'"_format( goalName, tgtName )
	);

	// TODO: check goal/target

	return ( EXIT_OK );
}

std::string CheckApp::formatHelpText() const noexcept
{
	std::ostringstream ss;

	Poco::Util::HelpFormatter hf( options() );
	hf.setCommand( commandName() );
	hf.setUsage( "[options] [<goal> [target]]" );
	hf.setHeader(
		R"(where:
		goal        goal name to check;
		target      specific target to check; goal's default target used if no specified;

		!Note: goal should but not must to provide default target

		options are listed below:)"
	);
	hf.setFooter( "MEMENTO MORI!" );
	hf.setWidth( 80 );
	hf.format( ss );

	return ( ss.str() );
}


} // namespace check
} // namespace cic
