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
/// \brief XML utility functions definitions
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright cic is released under the terms of zlib/png license

#include <cic/plan/XMLUtils.hpp>
#include <Poco/FileStream.h>
#include <cassert>
#include <fmt/format.h>

using Poco::XML::XMLException; // ?? is it ugly?
using Poco::XML::Node;
using Poco::XML::NodeList;
using Poco::XML::NamedNodeMap;
using Poco::XML::DOMParser;
using Poco::XML::Document;
using DocumentPtr = Poco::AutoPtr< Document >;
using Poco::XML::InputSource;

using namespace fmt::literals;

namespace cic
{
namespace plan
{

DocumentPtr fetchDoc( const std::string& path, DOMParser& parser )
{
    DocumentPtr doc{ nullptr };

    if ( path.empty() )
    {
        return ( doc );
    }

    Poco::FileInputStream istr{ path };
    InputSource input{ istr };
    doc = parser.parse( &input );
    return ( doc );
}

Node* fetchNode( const Node* root,
                 const std::string& nodePath,
                 unsigned short requiredType )
{
    if ( !root )
        assert( false && "fetchNode: root is NULL!" );
    Node* node{ root->getNodeByPath( nodePath ) };
    if ( !node )
    {
        fmt::print( stderr,
                    "[error] '{}' node has no path '{}';\n",
                    root->nodeName(),
                    nodePath );
        return ( nullptr );
    }
    if ( requiredType && ( requiredType != node->nodeType() ) )
    {
        fmt::print( stderr,
                    "[error] found node '{}' with nodeType={} but required type {};\n",
                    node->nodeName(),
                    node->nodeType(),
                    requiredType );
        return ( nullptr );
    }
    return ( node );
}

std::string fetchText( const Node* root )
{
    fmt::MemoryWriter text;

    if ( root )
    {
        NodeList* nodes{ root->childNodes() };
        for ( std::size_t i{ 0 }; i < nodes->length(); ++i )
        {
            if ( nodes->item( i )->nodeType() == Node::TEXT_NODE )
            {
                text.write( nodes->item( i )->nodeValue() );
            }
        }
    }
    return ( text.str() );
}


} // namespace plan
} // namespace cic
