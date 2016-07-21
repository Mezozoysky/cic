//
//  RuleSet.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 07.07.16.
//
//

#include "RuleSet.hpp"
#include <Poco/String.h>
#include <Poco/Exception.h>
#include <Poco/Util/Application.h>

using namespace cic::task;
using namespace cic::xmlu;

namespace cic
{

void RuleSet::loadFromXml( const xmlu::Node* root, tu::FactoryOwner* factories )
{
	fmt::print( "RULE SET LOAD FROM XML!\n" );
	NodeMap* rootAttrs{ root->attributes() };

	// load rules
	NodeList* list{ root->childNodes() };

	for ( std::size_t i{ 0 }; i < list->length(); ++i )
	{
		Node* node{ list->item( i ) };
		if ( node->nodeType() != Node::ELEMENT_NODE )
		{
			continue;
		}
		if ( node->nodeName() != "rule" )
		{
			fmt::print(
					   stderr
					   , "[error] unknown element '{}' inside 'ruleSet'; ignoring;"
					   , node->nodeName()
					   );
			continue;
		}

		NodeMap* attrs{ node->attributes() };
		std::string typeId;
		Node* attr{ attrs->getNamedItem( "typeId" ) };
		if ( !attr )
		{
			throw
				Poco::SyntaxException{
					"'rule' element without 'typeId' attribute;"
					, Poco::Util::Application::EXIT_DATAERR
				};
		}
		else
		{
			typeId = Poco::trim( attr->getNodeValue() );
		}
		fmt::print( "RULE TYPEID: '{}'\n", typeId );
		auto rule = RuleBase::Ptr( factories->get< RuleBase >()->create( typeId ) );
		rule->loadFromXml( node, factories );
		mRules.push_back( rule );
	}
}

void RuleSet::setName( const std::string& name ) noexcept
{
	mName = name;
}


} // namespace cic