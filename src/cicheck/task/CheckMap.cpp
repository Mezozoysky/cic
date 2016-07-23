//
//  CheckMap.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 06.07.16.
//
//

#include "CheckMap.hpp"
#include <Poco/String.h>
#include <fmt/format.h>


using namespace cic::task;
using namespace cic::xmlu;

namespace cic
{


bool CheckMap::check( const std::vector< std::string >& sequence )
{
	fmt::MemoryWriter w;
	w.write( "CHECK MAP CHECK with SEQUENCE[{}]: [", sequence.size() );
	for ( const auto& tgt : sequence )
	{
		w.write( " '{}'", tgt );
	}
	w.write( " ];\n" );
	fmt::print( w.str() );

	bool result{ true };
	for ( auto tgt : sequence )
	{
		if ( !mRulesets.count( tgt ) )
		{
			fmt::print(
					stderr
					, "[notice] no rules for target '{}';"\
					  " interpreting as successfull check;\n"
					, tgt
			);
			continue;
		}
		if ( !mRulesets.at( tgt )->check() )
		{
			result = false;
			break;
		}
	}
	return ( result );
}

void CheckMap::loadFromXML( const Node* root, indu::Industry* industry )
{
	fmt::print( "CHECK MAP LOAD FROM XML!\n" );
	NodeMap* rootAttrs{ root->attributes() };

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

		std::string key{ Poco::trim( attr->getNodeValue() ) };
		RuleSet::Ptr val{ nullptr };

		Node* rsNode{ fetchNode( node, "/ruleSet" ) };
		if ( !rsNode )
		{
			fmt::print( "RULE SET NOT FOUND!\n" );
		}
		else
		{
			fmt::print( "RULE SET FOUND!\n" );
			attrs = rsNode->attributes();

			std::string typeId = "default";
			if ( attrs )
			{
				attr = attrs->getNamedItem( "typeId" );
				std::string typeId;
				if ( attr )
				{
					typeId = attr->getNodeValue();
				}
			}
			fmt::print( "RULE SET TYPEID: '{}'\n", typeId );
			val = RuleSet::Ptr( industry->get< RuleSet >()->create( typeId ) );
			val->loadFromXML( rsNode, industry );
		}

		if ( !key.empty() && val != nullptr && val->getSize() != 0 )
		{
			mRulesets.insert( std::pair< std::string, RuleSet::Ptr >( key, val ) );
		}
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
