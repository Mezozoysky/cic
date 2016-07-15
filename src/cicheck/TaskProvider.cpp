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
#include "task/Task.hpp"
#include "task/TargetSet.hpp"
#include "task/CheckMap.hpp"
#include "task/RuleSet.hpp"
#include "task/RuleBase.hpp"
#include "task/SuccessRule.hpp"
#include "task/FailureRule.hpp"
#include "task/SystemCmdRule.hpp"
#include <Poco/Path.h>
#include <Poco/Util/Application.h>

using namespace fmt::literals;

namespace cic
{

void TaskProvider::init()
{
	//TODO: write the normal logger daclator
	auto& l(
		Poco::Logger::create(
				"TaskProvider"
				, Poco::Logger::get( "Application" ).getChannel()
		)
	);
	l.setLevel( Poco::Logger::get( "Application" ).getLevel() );

	// fill factories
	auto taskFactory( mFactories.create< Task >() );
	taskFactory->registerId< Task >( "default" );

	auto tsFactory( mFactories.create< TargetSet >() );
	tsFactory->registerId< TargetSet >( "default" );

	auto cmFactory( mFactories.create< CheckMap >() );
	cmFactory->registerId< CheckMap >( "default" );

	auto rsFactory( mFactories.create< RuleSet >() );
	rsFactory->registerId< RuleSet >( "default" );

	auto ruleFactory( mFactories.create< RuleBase >() );
	ruleFactory->registerId< SuccessRule >( "success" );
	ruleFactory->registerId< FailureRule >( "failure" );
	ruleFactory->registerId< SystemCmdRule >( "systemCmd" );
}

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

Task::Ptr TaskProvider::load( const std::string &name )
{
	Task::Ptr task{ nullptr };
	for ( auto src : mSources )
	{
		DocPtr doc{ fetchDoc( src, mParser) };
		const Node* tasksRoot = fetchNode( doc, "/tasks", Node::ELEMENT_NODE );

		if ( tasksRoot == nullptr )
		{
			logger().error( "Element 'tasks' not found in '{}';\n", src );
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
				logger().error(
						"Unexpected element '{}' inside element 'tasks' in '{}';"\
						" ignoring task;"_format( taskNode->nodeName(), src )
				);
				continue;
			}

			NodeMap* attrs{ taskNode->attributes() };
			Node* attr{ attrs->getNamedItem( "name" ) };
			if ( attr == nullptr )
			{
				logger().error(
						"Attribute 'name' isnt specified inside element 'task' in '{}';"\
						" ignoring task;\n"_format( src )
				);
				continue;
			}

			if ( Poco::trim( attr->getNodeValue() ) == name )
			{
				logger().debug( "Task '{}' declaration is found in '{}';\n"\
							    ""_format( name, src )
				);

				//
				// creating and loading task starts here!
				//
				std::string typeId{ "default" };
				attr = attrs->getNamedItem( "fileRef" );
				if ( !attr )
				{
					attr = attrs->getNamedItem( "typeId" );
					if ( attr )
					{
						typeId = attr->getNodeValue();
					}
					task = Task::Ptr( mFactories.get< Task >()->create( typeId ) );
					task->loadFromXml( taskNode, &mFactories );
					mLoaded.insert( std::pair< std::string, ATask::Ptr >( name, task ) );
				}
				else
				{
					Poco::Path fileRef{ attr->getNodeValue() };
					std::string taskFile;
					if ( fileRef.isAbsolute() )
					{
						taskFile = fileRef.toString();
					}
					else
					{
						auto& app( Poco::Util::Application::instance() );
						taskFile = Poco::Path::forDirectory(
								app.config().getString( "cic.dir.share" )
						).pushDirectory( "tasks" ).append( fileRef ).toString();
					}


					DocPtr taskDoc = fetchDoc( taskFile, mParser );
					if ( taskDoc.isNull() )
					{
						logger().error( "Can't parse xml from file '{}';"_format( taskFile ) );
					}
					else
					{
						taskNode = fetchNode( taskDoc.get(), "/task" );
						if ( taskNode )
						attrs = taskNode->attributes();
						attr = attrs->getNamedItem( "typeId" );
						if ( attr )
						{
							typeId = attr->getNodeValue();
						}
						task = Task::Ptr( mFactories.get< Task >()->create( typeId ) );
						task->setName( name );
						task->loadFromXml( taskNode, &mFactories );
						mLoaded.insert( std::pair< std::string, ATask::Ptr >( name, task ) );
					}
				}

				break;
			}
		}
	}

	return ( task );
}

} // namespace cic
