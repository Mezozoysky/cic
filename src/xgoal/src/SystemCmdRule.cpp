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
/// \brief SystemCmdRule realisation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright CICheck is released under the terms of zlib/png license

#include <CICheck/xgoal/SystemCmdRule.hpp>
#include <Poco/Process.h>
#include <Poco/PipeStream.h>
#include <Poco/StreamCopier.h>
#include <fstream>
#include <Poco/Exception.h>
#include <Poco/String.h>
#include <fmt/format.h>

using namespace cic::xmlu;
using namespace fmt::literals;

namespace cic
{
namespace xgoal
{

bool SystemCmdRule::check()
{
	std::vector< std::string > opts;

	if ( !mOptions.empty() )
	{
		opts.push_back( mOptions );
	}

	Poco::Pipe outPipe;
	Poco::Pipe errPipe;
	Poco::Process::Env env;

	Poco::ProcessHandle ph{
			Poco::Process::launch(
					mCmd
					, opts
					, "/home/mezozoy/tmp"
					, nullptr
					, &outPipe
					, &errPipe
					, env
			)
	};

	Poco::PipeInputStream istrOut( outPipe );
	Poco::PipeInputStream istrErr( errPipe );

	std::ofstream ostrOut{ "rule_{}_stdout.txt"_format( name() ) };
	std::ofstream ostrErr{ "rule_{}_stderr.txt"_format( name() ) };
	Poco::StreamCopier::copyStream( istrOut, ostrOut );
	Poco::StreamCopier::copyStream( istrErr, ostrErr );

	int rc{ ph.wait() };
	return ( rc == 0 );
}

void SystemCmdRule::loadFromXML( const xmlu::Node* root, indu::Industry* industry )
{
	NodeMap* rootAttrs{ root->attributes() };

	// name
	Node* node{ rootAttrs->getNamedItem( "name" ) };
	if ( !node )
	{
		throw ( Poco::SyntaxException{ "Mandatory attribute 'name' isnt found", 8 } );
	}
	std::string rulename{ Poco::trim( node->getNodeValue() ) };
	if ( rulename.empty() )
	{
		throw ( Poco::DataException{ "Mandatory attribute 'name' is empty", 8 } );
	}
	name() = rulename;

	// cmd
	node = rootAttrs->getNamedItem( "cmd" );
	if ( !node )
	{
		throw ( Poco::SyntaxException{ "Mandatory attribute 'cmd' isnt found", 8 } );
	}
	std::string rulecmd{ Poco::trim( node->getNodeValue() ) };
	if ( rulecmd.empty() )
	{
		throw ( Poco::DataException{ "Mandatory attribute 'cmd' is empty", 8 } );
	}
	cmd() = rulecmd;

	// cmd options
	node = rootAttrs->getNamedItem( "options" );
	if ( node )
	{
		options() = Poco::trim( node->getNodeValue() );
	}
}


} // namespace xgoal
} // namespace cic
