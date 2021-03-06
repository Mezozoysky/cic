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
/// \brief ActShell realisation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright cic is released under the terms of zlib/png license

#include <cic/plan/ActShell.hpp>
#include <Poco/Process.h>
#include <Poco/PipeStream.h>
#include <Poco/StreamCopier.h>
#include <Poco/Exception.h>
#include <Poco/String.h>
#include <fmt/format.h>
#include <Poco/DOM/Node.h>
#include <Poco/Util/Application.h>
#include <Poco/DOM/Element.h>
#include <Poco/AutoPtr.h>
#include <Poco/DOM/NodeList.h>

using Poco::AutoPtr;
using Poco::Util::Application;
using Poco::XML::Element;
using Poco::XML::Node;
using Poco::XML::NodeList;
using cic::industry::Industry;

using namespace fmt::literals;

namespace cic
{
namespace plan
{

bool ActShell::perform( PerformConfig* pc,
                        Report& report,
                        Industry& industry,
                        std::ostream& outStream,
                        std::ostream& errStream ) const
{
    outStream << "Outline: {}"_format( formOutline() ) << std::endl;
    Poco::Pipe outPipe;
    Poco::Pipe errPipe;
    Poco::Process::Env env;

    Poco::ProcessHandle ph{
        Poco::Process::launch( cmd(), args(), workDir(), nullptr, &outPipe, &errPipe, env )
    };

    Poco::PipeInputStream istrOut( outPipe );
    Poco::PipeInputStream istrErr( errPipe );

    Poco::StreamCopier::copyStream( istrOut, outStream );
    Poco::StreamCopier::copyStream( istrErr, errStream );

    int rc{ ph.wait() };
    return ( rc == 0 );
}

void ActShell::loadFromXML( Element* root, Industry* industry )
{
    std::string actCmd{ root->getAttribute( "cmd" ) };
    if ( actCmd.empty() )
    {
        throw( Poco::DataException{
            "Mandatory attribute 'cmd' isnt found or empty within the 'act' element' (class='{}')"_format(
                getClassName() ),
            8 } );
    }
    cmd() = actCmd;

    {
        Element* elem{ root->getChildElement( "args" ) };
        if ( elem != nullptr )
        {
            // load args
            AutoPtr< NodeList > argNodeList{ elem->childNodes() };
            Element* argElem{ nullptr };
            for ( std::size_t i{ 0 }; i < argNodeList->length(); ++i )
            {
                argElem = static_cast< Element* >( argNodeList->item( i ) );
                if ( argElem != nullptr && argElem->nodeName() == "arg" )
                {
                    std::string arg{ Poco::trim( argElem->getAttribute( "value" ) ) };
                    if ( !arg.empty() )
                    {
                        args().push_back( arg );
                    }
                }
            }
        }
    }

    {
        Element* elem{ root->getChildElement( "workDir" ) };
        if ( elem != nullptr )
        {
            workDir() = Poco::trim( elem->getAttribute( "value" ) );
        }
    }
}

void ActShell::saveToXML( Element* root ) const {}

const std::string ActShell::formOutline() const noexcept
{
    std::string outlineStr;
    {
        fmt::MemoryWriter writer;
        writer.write( "{} launches shell command; command: '{}';"_format( getClassName(), cmd() ) );
        writer.write( " arguments: [" );
        for ( const auto& arg : args() )
        {
            writer.write( " \"{}\""_format( arg ) );
        }
        writer.write( " ];" );
        outlineStr = writer.str();
    }
    return ( outlineStr );
}

} // namespace plan
} // namespace cic
