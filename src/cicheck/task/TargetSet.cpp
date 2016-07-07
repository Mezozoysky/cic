//
//  TargetSet.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 06.07.16.
//
//

#include "TargetSet.hpp"
#include <fmt/format.h>

using namespace cic::xmlu;
using namespace cic::task;

namespace cic
{


std::vector< std::string > TargetSet::calcSequenceFor( const std::string& targetName ) const
{
	std::vector< std::string > seq;

	auto idx( getIndexFor( targetName ) );
	if ( idx != BAD_INDEX )
	{
		for ( std::size_t i{ 0 }; i <= idx; ++i )
		{
//			seq.push_back( mTargets.at( i ).name() );
			seq.push_back( mTargets.at( i ) );
		}
	}
	return ( seq );
}

void TargetSet::loadFromXml( const xmlu::Node* root, tu::FactoryOwner* factories )
{
	fmt::print( "TARGET SET LOAD FROM XML!\n" );
	NodeMap* rootAttrs{ root->attributes() };

	// load default target
	Node* attr{ rootAttrs->getNamedItem( "default" ) };
	if ( attr )
	{
		mDefault = attr->getNodeValue();
	}

	// load targets
	NodeList* list{ root->childNodes() };

	for ( std::size_t i{ 0 }; i < list->length(); ++i )
	{
		Node* node{ list->item( i ) };
		if ( node->nodeType() != Node::ELEMENT_NODE )
		{
			continue;
		}
		if ( node->nodeName() != "target" )
		{
			fmt::print(
					stderr
					, "[error] unknown element '{}' inside 'targetSet'; ignoring;"
					, node->nodeName()
			);
			continue;
		}

		NodeMap* attrs{ node->attributes() };
		Node* attr{ attrs->getNamedItem( "name" ) };
		if ( attr == nullptr )
		{
			fmt::print(
					   stderr
					   , "[error] 'target' without 'name'; ignoring;\n"
			);
		}

		mTargets.push_back( attr->getNodeValue() );
	}

}

std::size_t TargetSet::getSize() const
{
	return ( mTargets.size() );
}

std::size_t TargetSet::getIndexFor( const std::string& targetName ) const
{
	std::size_t idx{ 0 };
	auto it( mTargets.begin() );
	while ( it != mTargets.end() )
	{
		if ( *it == targetName )
		{
			break;
		}
		++idx;
		++it;
	}

	return ( it != mTargets.end() ? idx : BAD_INDEX );
}

const std::vector< std::string >& TargetSet::targets() const
{
	return ( mTargets );
}

const std::string& TargetSet::defaultTarget() const
{
	return ( mDefault );
}



} // namespace cic
