//
//  CheckMap.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 06.07.16.
//
//

#include "CheckMap.hpp"


using namespace cic::task;
using namespace cic::xmlu;

namespace cic
{


bool CheckMap::check( const std::vector< std::string >& sequence )
{
	fmt::print( "CHECK MAP CHECK!" );
	bool result{ true };
	for ( auto tgt : sequence )
	{
		if ( !mRulesets.at( tgt )->check() )
		{
			result = false;
			break;
		}
	}
	return ( result );
}

void CheckMap::loadFromXml( const Node* root, tu::FactoryOwner* factories )
{
	fmt::print( "CHECK MAP LOAD FROM XML!\n" );
	NodeMap* rootAttrs{ root->attributes() };

//!!!ERROR development stopped here

	// load targets
	NodeList* list{ root->childNodes() };

	for ( std::size_t i{ 0 }; i < list->length(); ++i )
	{
		Node* node{ list->item( i ) };
		if ( node->nodeType() != Node::ELEMENT_NODE )
		{
			continue;
		}
		if ( node->nodeName() != "check" )
		{
			fmt::print(
					   stderr
					   , "[error] unknown element '{}' inside 'checkMap'; ignoring;"
					   , node->nodeName()
					   );
			continue;
		}

		NodeMap* attrs{ node->attributes() };
		Node* attr{ attrs->getNamedItem( "target" ) };
		if ( attr == nullptr )
		{
			fmt::print(
					   stderr
					   , "[error] 'check' without 'target'; ignoring;\n"
					   );
			continue;
		}

		std::string target{ attr->getNodeValue() };

		Node* rsNode{ fetchNode( node, "/ruleSet" ) };
		if ( !node )
		{
			fmt::print( "RULE SET NOT FOUND!\n" );
		}
		else
		{
			fmt::print( "RULE SET FOUND!\n" );
		}

		//TODO: get typeId, create and load
	}


}

std::size_t CheckMap::getSize() const
{
	return ( mRulesets.size() );
}

std::vector< std::string >&& CheckMap::getKeys() const
{
	std::vector< std::string > keys;
	for ( const auto& pair : mRulesets )
	{
		keys.push_back( pair.first );
	}
	return ( std::move( keys ) );
}

bool CheckMap::hasKey( const std::string& key ) const
{
	return ( mRulesets.count( key ) );
}

ARuleSet::Ptr CheckMap::getByKey( const std::string &key ) const
{
	return ( mRulesets.at( key ) );
}


} // namespace cic
