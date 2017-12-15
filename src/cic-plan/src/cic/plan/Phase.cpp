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
/// \brief plan::Phase class implementation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright cic is released under the terms of zlib/png license


#include <cic/plan/Phase.hpp>
#include <cic/plan/Action.hpp>
#include <cic/plan/XMLUtils.hpp>
#include <cic/plan/Industry.hpp>
#include <Poco/Exception.h>
#include <Poco/String.h>
#include <Poco/DOM/Node.h>
#include <Poco/DOM/NamedNodeMap.h>
#include <fmt/format.h>

using Poco::XML::NamedNodeMap;
using Poco::XML::Node;
using Poco::XML::NodeList;
using namespace fmt::literals;

namespace cic
{
namespace plan
{

bool Phase::execute()
{
    bool result{ true };

    for ( auto action : actions() )
    {
        if ( !action->execute() )
        {
            result = false;
            break;
        }
    }
    return ( result );
};

void Phase::loadFromXML( Node* root, Industry* industry )
{
    NamedNodeMap* rootAttrs{ root->attributes() };

    // // Dont load the name since name is already set with declaration name.
    // // 	std::string name;
    // // 	Node* node{ xmlAttrs->getNamedItem( "name" ) };
    // // 	if ( node )
    // // 	{
    // // 		name = Poco::trim( node->getNodeValue() );
    // // 	}

    Node* node{ fetchNode( root, "/dependencies", Node::ELEMENT_NODE ) };
    if ( node != nullptr )
    {
        NodeList* depNodeList{ node->childNodes() };
        for ( std::size_t i{ 0 }; i < depNodeList->length(); ++i )
        {
            Node* depNode{ depNodeList->item( i ) };
            if ( depNode->nodeType() == Node::ELEMENT_NODE && depNode->nodeName() == "dependency" )
            {
                NamedNodeMap* depAttrs{ depNode->attributes() };
                Node* depValueNode{ depAttrs->getNamedItem( "value" ) };
                if ( depValueNode != nullptr )
                {
                    std::string depName{ Poco::trim( depValueNode->getNodeValue() ) };
                    if ( !depName.empty() )
                    {
                        deps().push_back( depName );
                    }
                }
            }
        }
    }

    node = fetchNode( root, "/actions", Node::ELEMENT_NODE );
    if ( node == nullptr )
    {
        throw( Poco::SyntaxException( "Element 'actions' isnt found" ) );
    }
    loadActionsFromXML( node, industry );
}

void Phase::saveToXML( Node* root ) const {}

void Phase::loadActionsFromXML( Node* root, Industry* industry )
{
    NodeList* list{ root->childNodes() };
    Node* node{ nullptr };
    for ( std::size_t i{ 0 }; i < list->length(); ++i )
    {
        node = list->item( i );
        if ( node->nodeType() != Node::ELEMENT_NODE )
        {
            continue;
        }
        if ( node->nodeName() == "action" )
        {
            loadActionFromXML( node, industry );
        }
    }
}

void Phase::loadActionFromXML( Node* root, Industry* industry )
{
    NamedNodeMap* rootAttrs{ root->attributes() };

    std::string typeId{ "" };
    Node* node = rootAttrs->getNamedItem( "typeId" );
    if ( node != nullptr )
    {
        typeId = Poco::trim( node->getNodeValue() );
    }
    if ( typeId.empty() )
    {
        throw( Poco::SyntaxException{ "Mandatory attribute 'typeId' isnt found or empty", 8 } );
    }

    auto actionFactory( industry->get< Action >() );
    if ( actionFactory == nullptr )
    {
        throw( Poco::NotFoundException{ "No factory registered for id: '{}'"_format( typeId ), 8 } );
    }

    Action::Ptr action{ actionFactory->create( typeId ) };
    action->loadFromXML( root, industry );
    mActions.push_back( action );

    // NodeList* nodes{ xml->childNodes() };
    // for ( std::size_t i{ 0 }; i < nodes->length(); ++i )
    // {
    //     Node* node{ nodes->item( i ) };
    //     NodeList* list{ nullptr };
    //     Node* attr{ nullptr };
    //     if ( node->nodeName() == "dependency" )
    //     {
    //         attr = node->attributes()->getNamedItem( "value" );
    //         if ( attr == nullptr )
    //         {
    //             throw( Poco::SyntaxException{ "Mandatory attribute 'value' isnt found in 'dependency'
    //                                           element ",
    //                                           8 } );
    //         }
    //         tgtData.deps.push_back( Poco::trim( attr->getNodeValue() ) );
    //     }
    //     else if ( node->nodeName() == "dependencies" )
    //     {
    //         list = node->childNodes();
    //         for ( std::size_t j{ 0 }; j < list->length(); ++j )
    //         {
    //             Node* valNode{ list->item( j ) };
    //             if ( valNode->nodeType() != Node::ELEMENT_NODE )
    //             {
    //                 continue;
    //             }
    //             if ( valNode->nodeName() == "value" )
    //             {
    //                 std::string value{ Poco::trim( fetchText( valNode ) ) };
    //                 tgtData.deps.push_back( value );
    //             }
    //             else
    //             {
    //                 // TODO: mb throw?
    //             }
    //         }
    //     }
    //     else if ( node->nodeName() == "actions" )
    //     {
    //         list = node->childNodes();
    //         for ( std::size_t j{ 0 }; j < list->length(); ++j )
    //         {
    //             Node* tmpNode{ list->item( j ) };
    //             if ( tmpNode->nodeType() != Node::ELEMENT_NODE )
    //             {
    //                 continue;
    //             }
    //             if ( tmpNode->nodeName() == "action" )
    //             {
    //                 std::string typeId;
    //                 attr = tmpNode->attributes()->getNamedItem( "typeId" );
    //                 if ( attr == nullptr )
    //                 {
    //                     throw( Poco::SyntaxException{
    //                         "Mandatory attribute 'typeId' isnt found in 'action' element", 8 } );
    //                 }
    //                 typeId = Poco::trim( attr->getNodeValue() );
    //                 if ( typeId.empty() )
    //                 {
    //                     throw( Poco::DataException{ "Mandatory attribute 'typeId' is empty", 8 } );
    //                 }
    //                 auto actionFactory( industry->get< Action >() );
    //                 if ( actionFactory == nullptr )
    //                 {
    //                     throw( Poco::NotFoundException{ "No factory registered for given abstraction type",
    //                                                     8 } );
    //                 }
    //                 Action::Ptr action{ actionFactory->create( typeId ) };
    //                 poco_plan_ptr( action );
    //                 action->loadFromXML( tmpNode, industry );

    //                 tgtData.actions.push_back( action );
    //             }
    //             else
    //             {
    //                 // TODO: mb throw?
    //             }
    //         }
    //     }
    //     else
    //     {
    //         // TODO: mb throw?
    //     }
    // }
}

} // namespace plan
} // namespace cic
