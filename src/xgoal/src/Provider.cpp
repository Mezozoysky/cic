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
/// \brief xgoal::Provider realisation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright CICheck is released under the terms of zlib/png license

#include <CICheck/xgoal/Provider.hpp>
#include <Poco/Exception.h>
#include <Poco/String.h>
#include <Poco/Path.h>

using namespace cic::xmlu;

namespace cic
{
namespace xgoal
{

void Provider::loadDecls( const std::string& declsPath )
{
	DocPtr doc{ fetchDoc( declsPath, mParser) };
	Node* declsRoot{ fetchNode( doc, "/declarations", Node::ELEMENT_NODE ) };
	if ( !declsRoot )
	{
		throw ( Poco::DataException( "'declarations' element isnt found", 8 ) );
	}
	NodeList* list{ declsRoot->childNodes() };
	Node* decl{ nullptr };
	for ( std::size_t i{ 0 }; i < list->length(); ++i )
	{
		decl = list->item( i );
		if ( decl->nodeType() != Node::ELEMENT_NODE )
		{
			continue;
		}
		if ( decl->nodeName() == "decl-goal" )
		{
			// Goal declaration found
			loadDecl( decl, declsPath );
		}
		else
		{
			// Unkonown element is found inside 'declarations'
		}
	}
}

void Provider::loadDecl( const Node* root, const std::string& declsPath )
{
	NodeMap* rootAttrs{ root->attributes() };

	Node* node{ rootAttrs->getNamedItem( "name" ) };
	if ( !node )
	{
		throw ( Poco::DataException( "No 'name' attribute found", 8 ) );
	}
	auto name( Poco::trim( node->getNodeValue() ) );
	if ( name.empty() )
	{
		throw ( Poco::DataException( "'name' attribute is empty", 8 ) );
	}

	node = rootAttrs->getNamedItem( "path" );
	if ( !node )
	{
		throw ( Poco::DataException( "No 'path' attribute found", 8 ) );
	}
	auto path( Poco::trim( node->getNodeValue() ) );
	if ( path.empty() )
	{
		throw ( Poco::DataException( "'path' attribute is empty", 8 ) );
	}
	Poco::Path p{ Poco::Path::forDirectory( path ) };
	if ( p.isRelative() )
	{
		p.makeAbsolute( Poco::Path( declsPath ).parent() );
		path = p.toString();
	}

	GoalDecl decl;
	decl.path = path;
	mDecls.insert( { name, decl } );
}

void Provider::dropDecls() noexcept
{
	mDecls.clear();
}

goal::Goal::Ptr Provider::get ( const std::string& goalName )
{
	if ( !isDeclared( goalName ))
	{
		throw ( Poco::InvalidArgumentException( "Requested goal isnt declared", 8 ) );
	}
	if ( mDecls.at( goalName ).goal == nullptr )
	{
		return ( reload( goalName ) );
	}

	return ( mDecls.at( goalName ).goal );
}

goal::Goal::Ptr Provider::reload ( const std::string& goalName )
{
	if ( !isDeclared( goalName ))
	{
		throw ( Poco::InvalidArgumentException( "Requested goal isnt declared", 8 ) );
	}

	GoalDecl& decl{ mDecls.at( goalName ) };
	std::string src{ Poco::Path::forDirectory( decl.path ).setFileName( "goal.xml" ).toString() };
	DocPtr doc{ fetchDoc( src, mParser ) };

	const Node* root = fetchNode( doc, "goal", Node::ELEMENT_NODE );
	if ( root == nullptr )
	{
		throw ( Poco::DataException( "Element 'goal' not found;\n", 8 ) );
	}

	NodeMap* attrs{ root->attributes() };
	Node* attr{ attrs->getNamedItem( "name" ) };
	if ( attr == nullptr )
	{
		throw ( Poco::DataException( "Attribute 'name' for element 'goal' isnt found", 8 ) );
	}
	auto name{ Poco::trim( attr->getNodeValue() ) };

	std::string typeId{ "default" };
	attr = attrs->getNamedItem( "typeId" );
	if ( attr )
	{
		typeId = Poco::trim( attr->getNodeValue() );
	}

/*
	auto goal( xgoal::Goal::Ptr( mIndustry.get< Goal >()->create( typeId ) ) );
	goal->loadFromXML( root, &mIndustry );
	decl.goal = goal;
*/
}

void Provider::drop( const std::string& goalName )
{
	if ( !isDeclared( goalName ) )
	{
		throw ( Poco::InvalidArgumentException( "Goal isnt declared" ) );
	}
	mDecls.at( goalName ).goal = nullptr;
}

void Provider::dropAll() noexcept
{
	for ( auto& decl : mDecls )
	{
		decl.second.goal = nullptr;
	}
}



} // namespace xgoal
} // namespace cic
