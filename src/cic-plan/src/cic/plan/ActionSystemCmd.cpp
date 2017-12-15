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
/// \brief ActionSystemCmd realisation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright cic is released under the terms of zlib/png license

#include <cic/plan/ActionSystemCmd.hpp>
#include <Poco/Process.h>
#include <Poco/PipeStream.h>
#include <Poco/StreamCopier.h>
#include <fstream>
#include <Poco/Exception.h>
#include <Poco/String.h>
#include <fmt/format.h>
#include <Poco/DOM/Node.h>
#include <Poco/DOM/NamedNodeMap.h>

using Poco::XML::NamedNodeMap;
using Poco::XML::Node;

using namespace fmt::literals;

namespace cic
{
namespace plan
{

bool ActionSystemCmd::execute()
{
    std::vector< std::string > opts;

    if ( !mArgs.empty() )
    {
        opts.push_back( mArgs );
    }

    Poco::Pipe outPipe;
    Poco::Pipe errPipe;
    Poco::Process::Env env;

    Poco::ProcessHandle ph{
        Poco::Process::launch( mCmd, opts, "/Users/mezozoy/tmp", nullptr, &outPipe, &errPipe, env )
    };

    Poco::PipeInputStream istrOut( outPipe );
    Poco::PipeInputStream istrErr( errPipe );

    std::ofstream ostrOut{ "action_{}_stdout.txt"_format( name() ) };
    std::ofstream ostrErr{ "action_{}_stderr.txt"_format( name() ) };
    Poco::StreamCopier::copyStream( istrOut, ostrOut );
    Poco::StreamCopier::copyStream( istrErr, ostrErr );

    int rc{ ph.wait() };
    return ( rc == 0 );
}

void ActionSystemCmd::loadFromXML( Node* root, Industry* industry )
{
    NamedNodeMap* rootAttrs{ root->attributes() };

    // cmd
    Node* node{ rootAttrs->getNamedItem( "cmd" ) };
    if ( !node )
    {
        throw( Poco::SyntaxException{ "Mandatory attribute 'cmd' isnt found", 8 } );
    }
    std::string actioncmd{ Poco::trim( node->getNodeValue() ) };
    if ( actioncmd.empty() )
    {
        throw( Poco::DataException{ "Mandatory attribute 'cmd' is empty", 8 } );
    }
    cmd() = actioncmd;

    // cmd args
    node = rootAttrs->getNamedItem( "args" );
    if ( node )
    {
        args() = Poco::trim( node->getNodeValue() );
    }
}

void ActionSystemCmd::saveToXML( Node* xml ) const {}

} // namespace plan
} // namespace cic
