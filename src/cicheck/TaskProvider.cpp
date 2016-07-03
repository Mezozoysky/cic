//
//  TaskProvider.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 03.07.16.
//
//

#include "TaskProvider.hpp"
#include <fmt/format.h>
#include <Poco/String.h>

namespace cic
{


ATask::Ptr TaskProvider::get( const std::string& name )
{
	if ( !mLoaded.count( name ) )
	{
		load( name );
	}

	if ( !mLoaded.count( name ) )
	{
		return ( nullptr );
	}

	return ( mLoaded.at( name ) );
}

ATask::Ptr TaskProvider::load( const std::string &name )
{
	ATask::Ptr task{ nullptr };
	for ( auto src : mSources )
	{
		DocPtr doc{ fetchDoc( src, mParser) };
		const Node* tasksRoot = fetchXML( doc, "/tasks", Node::ELEMENT_NODE );

		if ( tasksRoot == nullptr )
		{
			fmt::print( stderr, "[error] no 'tasks' element in '{}';\n", src );
			continue;
		}

		NodeList* list{ tasksRoot->childNodes() };
		for ( std::size_t i{ 0 }; i < list->length(); ++i )
		{
			Node* taskNode{ list->item( i ) };
			if ( taskNode->nodeType() != Node::ELEMENT_NODE )
			{
				continue;
			}
			if ( taskNode->nodeName() != "task" )
			{
				fmt::print(
						stderr
						, "[error] unknown element '{}' inside 'tasks' in '{}'; ignoring;"
						, taskNode->nodeName()
						, src
				);
				continue;
			}

			NodeMap* attrs{ taskNode->attributes() };
			Node* attr{ attrs->getNamedItem( "name" ) };
			if ( attr == nullptr )
			{
				fmt::print(
						stderr
						, "[error] 'task' without 'name' in '{}'; ignoring;\n"
						, src
			   );
			}

			if ( Poco::trim( attr->getNodeValue() ) == name )
			{
				fmt::print( "TASK '{}' Is FOUND!\n", name );

				//TODO: create and load task!

				break;
			}
		}
	}

	return ( task );
}

} // namespace cic
