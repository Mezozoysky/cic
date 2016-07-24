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
/// \brief xgoal::Goal class implementation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright CICheck is released under the terms of zlib/png license


#include <CICheck/xgoal/Goal.hpp>
#include <Poco/Exception.h>
#include <Poco/String.h>
#include <CICheck/xgoal/ARule.hpp>

using namespace cic::xmlu;

namespace cic
{
namespace xgoal
{

void Goal::loadFromXML( const Node *root, indu::Industry* industry )
{
	NodeMap* rootAttrs{ root->attributes() };

	// Dont load the name since name is already set with declaration name.
// 	std::string name;
// 	Node* node{ rootAttrs->getNamedItem( "name" ) };
// 	if ( node )
// 	{
// 		name = Poco::trim( node->getNodeValue() );
// 	}

	Node* node{ fetchNode( root, "/targets", Node::ELEMENT_NODE ) };
	if ( node == nullptr )
	{
		throw( Poco::SyntaxException( "Element 'targets' isnt found" ) );
	}
	loadTargetsFromXML( node, industry );
}

void Goal::loadTargetsFromXML( const xmlu::Node* root, indu::Industry* industry )
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
		if ( node->nodeName() == "target" )
		{
			loadTargetFromXML( node, industry );
		}
	}
}

void Goal::loadTargetFromXML( const xmlu::Node* root, indu::Industry* industry )
{
	NodeMap* rootAttrs{ root->attributes() };
	Node* node{ rootAttrs->getNamedItem( "name" ) };
	if ( node == nullptr )
	{
		throw ( Poco::SyntaxException{ "Mandatory attribute 'name' isnt found", 8 } );
	}
	std::string name{ Poco::trim( node->getNodeValue() ) };

	auto pair( targets().insert( TgtMap::value_type{ name, TgtData{} } ) );
	if ( !pair.second )
	{
		throw ( Poco::RuntimeException{ "Error inserting target into goal's map", 8 } );
	}
	TgtData& tgtData = ( *(pair.first) ).second;

	NodeList* nodes{ root->childNodes() };
	for ( std::size_t i{ 0 }; i < nodes->length(); ++i )
	{
		Node* node{ nodes->item( i ) };
		NodeList* list{ nullptr };
		Node* attr{ nullptr };
		if ( node->nodeName() == "dependency" )
		{
			attr = node->attributes()->getNamedItem( "value" );
			if ( attr == nullptr )
			{
				throw (
						Poco::SyntaxException{
								"Mandatory attribute 'value' isnt found in 'dependency' element"
								, 8 
						}
				);
			}
			tgtData.deps.push_back( Poco::trim( attr->getNodeValue() ) );
		}
		else if ( node->nodeName() == "dependencies" )
		{
			list = node->childNodes();
			for ( std::size_t j{ 0 }; j < list->length(); ++j )
			{
				Node* valNode{ list->item( j ) };
				if ( valNode->nodeType() != Node::ELEMENT_NODE )
				{
					continue;
				}
				if ( valNode->nodeName() == "value" )
				{
					std::string value{ Poco::trim( fetchText( valNode ) ) };
					tgtData.deps.push_back( value );
				}
				else
				{
					//TODO: mb throw?
				}
			}
		}
		else if ( node->nodeName() == "rules" )
		{
			list = node->childNodes();
			for ( std::size_t j{ 0 }; j < list->length(); ++j )
			{
				Node* tmpNode{ list->item( j ) };
				if ( tmpNode->nodeType() != Node::ELEMENT_NODE )
				{
					continue;
				}
				if ( tmpNode->nodeName() == "rule" )
				{
					std::string typeId;
					attr = tmpNode->attributes()->getNamedItem( "typeId" );
					if ( attr == nullptr )
					{
						throw (
								Poco::SyntaxException{
										"Mandatory attribute 'typeId' isnt found in 'rule' element"
										, 8
								}
						);
					}
					typeId = Poco::trim( attr->getNodeValue() );
					if ( typeId.empty() )
					{
						throw (
								Poco::DataException{
										"Mandatory attribute 'typeId' is empty"
										, 8
								}
						);
					}
					auto ruleFactory( industry->get< ARule >() );
					if ( ruleFactory == nullptr )
					{
						throw (
								Poco::NotFoundException{
										"No factory registered for given abstraction type"
										, 8
								}
						);
					}
// 					ARule::Ptr rule{ ruleFactory->create( typeId ) };
// 					poco_check_ptr( rule );
// 					rule->loadFromXML( tmpNode, industry );
// 
// 					tgtData.rules.push_back( rule );
				}
				else
				{
					//TODO: mb throw?
				}
			}
		}
		else
		{
			//TODO: mb throw?
		}
	}
}

} // namespace goal
} // namespace cic
