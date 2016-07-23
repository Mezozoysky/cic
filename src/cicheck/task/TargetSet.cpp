//
//  TargetSet.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 06.07.16.
//
//

#include "TargetSet.hpp"
#include <fmt/format.h>
#include <Poco/String.h>
#include <Poco/Util/Application.h>
#include "RuleBase.hpp"

using namespace cic::xmlu;
using namespace cic::task;

namespace cic
{


// std::vector< std::string > TargetSet::calcSequenceFor( const std::string& targetName ) const
// {
// 	std::vector< std::string > seq;
// 
// 	auto idx( getIndexFor( targetName ) );
// 	if ( idx != BAD_INDEX )
// 	{
// 		for ( std::size_t i{ 0 }; i <= idx; ++i )
// 		{
// 			//seq.push_back( mTargets.at( i ).name() );
// 			seq.push_back( mTargets.at( i ) );
// 		}
// 	}
// 	return ( seq );
// }

void TargetSet::loadFromXML( const xmlu::Node* root, indu::Industry* industry )
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
		if ( node->nodeName() == "target" )
		{
			loadTargetFromXML( node, industry );
		}
		else
		{
			fmt::print(
					stderr
					, "[error] unknown element '{}' inside 'targetSet'; ignoring;"
					, node->nodeName()
			);
			continue;
		}
	}

}

void TargetSet::loadTargetFromXML( const Node* root, indu::Industry* industry )
{
	NodeMap* attrs{ root->attributes() };
	Node* attr{ attrs->getNamedItem( "name" ) };
	if ( attr == nullptr )
	{
		fmt::print(
			stderr
			, "[error] 'target' without 'name'; ignoring;\n"
		);
		return;
	}

	std::string tgtName{ attr->getNodeValue() };
	auto pair( mTargets.insert( TgtMap::value_type{ tgtName, TgtData{} } ) );
	if ( !pair.second )
	{
		//TODO: log or do something as target isnt inserted into mTargets map
	}
	TgtData& tgtData = ( *(pair.first) ).second;
	
	NodeList* nodes{ root->childNodes() };
	for ( std::size_t i{ 0 }; i < nodes->length(); ++i )
	{
		Node* node{ nodes->item( i ) };
		NodeList* tmpList{ nullptr };
		if ( node->nodeName() == "dependency" )
		{
			Node* valueNode{ node->attributes()->getNamedItem( "value" ) };
			if ( valueNode == nullptr )
			{
				fmt::print(
					stderr
					, "[error] 'dependency' without 'value'; ignoring;\n"
				);
				continue;
			}
			tgtData.deps.push_back( valueNode->getNodeValue() );
		}
		else if ( node->nodeName() == "dependencies" )
		{
			tmpList = node->childNodes();
			for ( std::size_t j{ 0 }; j < tmpList->length(); ++j )
			{
				Node* tmpNode{ tmpList->item( j ) };
				if ( tmpNode->nodeType() != Node::ELEMENT_NODE )
				{
					continue;
				}
				if ( tmpNode->nodeName() == "value" )
				{
					std::string value{ Poco::trim( fetchText( tmpNode ) ) };
					tgtData.deps.push_back( value );
				}
				else
				{
					fmt::print(
							"** Found node '{}' with value '{}'  in 'dependencies'\n"
							, tmpNode->nodeName()
							, tmpNode->getNodeValue()
					);
				}
			}
		}
		else if ( node->nodeName() == "rules" )
		{
			tmpList = node->childNodes();
			for ( std::size_t j{ 0 }; j < tmpList->length(); ++j )
			{
				Node* tmpNode{ tmpList->item( j ) };
				if ( tmpNode->nodeType() != Node::ELEMENT_NODE )
				{
					continue;
				}
				if ( tmpNode->nodeName() == "rule" )
				{
					//TODO: Load rule
					NodeMap* ruleAttrs{ tmpNode->attributes() };
					std::string typeId;
					Node* ruleAttr{ ruleAttrs->getNamedItem( "typeId" ) };
					if ( !ruleAttr )
					{
						throw
						Poco::SyntaxException{
							"'rule' element without mandatory 'typeId' attribute;"
							, Poco::Util::Application::EXIT_DATAERR
						};
					}
					else
					{
						typeId = Poco::trim( ruleAttr->getNodeValue() );
					}
					fmt::print( "RULE TYPEID: '{}'\n", typeId );
					auto rule = RuleBase::Ptr( industry->get< RuleBase >()->create( typeId ) );
					rule->loadFromXML( tmpNode, industry );
					
					tgtData.rules.push_back( rule );
				}
				else
				{
					fmt::print(
						"** Found node '{}' with value '{}'  in 'rules'\n"
						, tmpNode->nodeName()
						, tmpNode->getNodeValue()
					);
				}
			}
		}
		else
		{
			fmt::print( "** Found node '{}' in 'target'\n", node->nodeName() );
		}
	}
}

// const std::vector< std::string >& TargetSet::targets() const
// {
// 	return ( mTargets );
// }
// 
// const std::string& TargetSet::defaultTarget() const
// {
// 	return ( mDefault );
// }



} // namespace cic
