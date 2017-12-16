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
#include <cic/plan/XMLUtils.hpp>
#include <Poco/Process.h>
#include <Poco/PipeStream.h>
#include <Poco/StreamCopier.h>
#include <fstream>
#include <iostream>
#include <Poco/Exception.h>
#include <Poco/String.h>
#include <fmt/format.h>
#include <Poco/DOM/Node.h>
#include <Poco/DOM/NamedNodeMap.h>
#include <Poco/Util/Application.h>

using Poco::Util::Application;
using Poco::XML::NamedNodeMap;
using Poco::XML::Node;
using Poco::XML::NodeList;

using namespace fmt::literals;

namespace cic
{
namespace plan
{

bool ActionSystemCmd::execute()
{
    Poco::Pipe outPipe;
    Poco::Pipe errPipe;
    Poco::Process::Env env;

    Poco::ProcessHandle ph{
        Poco::Process::launch( cmd(), args(), workDir(), nullptr, &outPipe, &errPipe, env )
    };

    Poco::PipeInputStream istrOut( outPipe );
    Poco::PipeInputStream istrErr( errPipe );

    std::ofstream ostrOut{ "action_systemCmd_{}_stdout.txt"_format( cmd() ) };
    std::ofstream ostrErr{ "action_systemCmd_{}_stderr.txt"_format( cmd() ) };

    Poco::StreamCopier::copyStream( istrOut, std::cout );
    Poco::StreamCopier::copyStream( istrErr, std::cerr );

    // Poco::StreamCopier::copyStream( istrOut, ostrOut );
    // Poco::StreamCopier::copyStream( istrErr, ostrErr );

    int rc{ ph.wait() };
    return ( rc == 0 );
}

void ActionSystemCmd::loadFromXML( Node* root, Industry* industry )
{
    Application& app{ Application::instance() };

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

    node = fetchNode( root, "/args", Node::ELEMENT_NODE );
    if ( node != nullptr )
    {
        // load args
        NodeList* argNodeList{ node->childNodes() };
        Node* argNode{ nullptr };
        for ( std::size_t i{ 0 }; i < argNodeList->length(); ++i )
        {
            argNode = argNodeList->item( i );
            if ( argNode->nodeType() != Node::ELEMENT_NODE )
            {
                continue;
            }
            if ( argNode->nodeName() == "arg" )
            {
                NamedNodeMap* argAttrs{ argNode->attributes() };
                Node* argValueNode{ argAttrs->getNamedItem( "value" ) };
                if ( argValueNode != nullptr )
                {
                    std::string arg{ Poco::trim( argValueNode->getNodeValue() ) };
                    if ( !arg.empty() )
                    {
                        args().push_back( arg );
                    }
                }
            }
        } // for

        Poco::Path workspacePath{ app.config().getString( "cic.check.workspace" ) };
        Poco::Path workDirPath{ workspacePath };

        node = fetchNode( root, "/workDir", Node::ELEMENT_NODE );
        if ( node != nullptr )
        {
            NamedNodeMap* wdAttrs{ node->attributes() };
            Node* wdValueNode{ wdAttrs->getNamedItem( "value" ) };
            if ( wdValueNode != nullptr )
            {
                std::string wd{ Poco::trim( wdValueNode->getNodeValue() ) };
                if ( !wd.empty() )
                {
                    workDirPath = Poco::Path::forDirectory( wd );
                    if ( workDirPath.isRelative() )
                        {
                            workDirPath.makeAbsolute( workspacePath );
                        }
                }
            }
        }
        workDir() = workDirPath.toString();
    }
}

void ActionSystemCmd::saveToXML( Node* root ) const {}

} // namespace plan
} // namespace cic
