//
//  TaskProvider.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 23.06.16.
//
//

#include <CICheck/task/TaskProvider.hpp>
#include <CICheck/task/Task.hpp>
#include <Poco/SAX/InputSource.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/NodeIterator.h>
#include <Poco/DOM/NodeFilter.h>
#include <Poco/DOM/NamedNodeMap.h>
#include <Poco/DOM/NodeList.h>
#include <Poco/FileStream.h>
#include <iostream>
#include <sstream>
#include <fmt/format.h>
#include <Poco/AutoPtr.h>

using namespace fmt::literals;
using namespace Poco;

namespace cic {
namespace task {


TaskProvider::TaskProvider() noexcept
{
}

TaskProvider::~TaskProvider() noexcept
{
}

void TaskProvider::loadDecls( const std::string& declsPath )
{
	fmt::print( "[info] loading declaration from '{}'\n", declsPath );

	FileInputStream istr{ declsPath };
	XML::InputSource input{ istr };

	try
	{
		AutoPtr< XML::Document > doc{ mParser.parse( &input ) };

		XML::Node* declsNode{ doc->getNodeByPath( "/taskDeclarations" ) };

		if ( declsNode && declsNode->nodeType() == XML::Node::ELEMENT_NODE )
		{
			XML::NodeList* nodeList{ declsNode->childNodes() };
			for ( std::size_t i{ 0 }; i < nodeList->length(); ++i )
			{
				XML::Node* node{ nodeList->item( i ) };
				if ( !node /*nerdy?*/ || node->nodeType() != XML::Node::ELEMENT_NODE )
				{
					continue;
				}

				if ( node->nodeName() == "decl" )
				{
					auto attrs( node->attributes() );

					auto attr( attrs->getNamedItem( "name" ) );
					if ( !attr )
					{
						fmt::print(
								   stderr
								   , "[error] found declaration without 'name' attribute;\n"\
								   "\tignoring declaration;"
						);
						continue;
					}

					std::string newDeclName{ attr->getNodeValue() };
					std::string newDeclPath;

					attr = attrs->getNamedItem( "path" );
					if ( !attr )
					{
						newDeclPath = "_decl_path_internal_(_not_implemented_)_";
						fmt::print(
								   stderr
								   , "[error] found declaration without 'path' attribute;\n"\
								   "\tinternal path feature is not implemented;\n"\
								   "\tignoring declaration;\n"
						);
						continue;
					}
					else
					{
						newDeclPath = attr->getNodeValue();
						Path path{ newDeclPath };
						if ( path.isRelative() )
						{
							newDeclPath = "_decl_path_relative_(_not_implemented_)_";
							fmt::print(
									   stderr
									   , "[error] found declaration with relative value in 'path' attribute;\n"\
									   "\trelative path feature is not implemented;\n"\
									   "\tignoring declaration;\n"
							);
							continue;
						}
					}
					declare( newDeclName, newDeclPath );
				}
				else
				{
					fmt::print(
							   stderr
							   , "[error] Unexpected xml element '{0}' inside 'taskDeclarations' container;\n"\
							   "\tignoring '{0}';\n"
							   , node->nodeName()
					);
				}
			}
		}
		else
		{
			fmt::print( stderr, "[error] xml element 'taskDeclarations' not found, so no Task declarations loaded;\n" );
		}
	}
	catch ( ::Poco::Exception& e )
	{
		fmt::print( stderr, "[error] {}\n", e.displayText() );
	}
}

void TaskProvider::dropDecls() noexcept
{
    mDecls.clear();
}

const std::vector< TaskDecl >& TaskProvider::getDecls() const noexcept
{
    return ( mDecls );
}

Task::Ptr TaskProvider::loadTask( const std::string &name )
{
	Task::Ptr task{ nullptr };

	if ( mTasks.find( name ) != mTasks.end() )
	{
		fmt::print(
				   "[error] trying to load task '{}' wich is already loaded;\n"\
				   "\tnull-value will be returned;\n"
				   , name
		);
		return ( task );
	}

	auto it(
		std::find_if(
			mDecls.begin()
			, mDecls.end()
			, [ name ]( const TaskDecl& decl ) { return ( decl.name == name ); }
		)
	);
	if ( it == mDecls.end() )
	{
		fmt::print(
				   "[error] trying to load task '{}' which isnt declared;\n"\
				   "\tnull-value will be returned;\n"
				   , name
		);
		return ( task );
	}

	task = Task::createPtr< Task >( *it );
	if ( !task->load( mParser ) )
	{
		return ( nullptr );
	}

	mTasks.insert( std::pair< std::string, Task::Ptr >( name, task ) );

	return ( task );
}

void TaskProvider::dropTask( const std::string &name ) noexcept
{
	auto it( mTasks.find( name ) );
	if ( it != mTasks.end() )
	{
		mTasks.erase( it );
	}
	else
	{
		fmt::print( "[error] trying to drop task '{}' which isnt loaded;'n", name );
	}
}

void TaskProvider::dropTasks() noexcept
{
	mTasks.clear();
}

Task::Ptr TaskProvider::getTask( const std::string &name )
{
	Task::Ptr task{ nullptr };

	if ( !isTaskLoaded( name ) )
	{
		task = loadTask( name );
	}
	else
	{
		task = mTasks.at( name );
	}

	return ( task );
}

bool TaskProvider::isTaskDeclared( const std::string& name ) const noexcept
{
    auto it(
        std::find_if(
            mDecls.begin(),
            mDecls.end(),
            [ name ]( const TaskDecl& decl )
            {
                return ( decl.name == name );
            }
        )
    );
    return ( it != mDecls.end() );
}

bool TaskProvider::isTaskLoaded( const std::string& name ) const noexcept
{
	return ( mTasks.count( name ) );
}

void TaskProvider::declare( const cic::task::TaskDecl &decl )
{
    mDecls.push_back( decl );
}

void TaskProvider::declare( const std::string &name, const std::string &path )
{
    declare( { name, path } );
}

} // namespace task
} // namespace cic
