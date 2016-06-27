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
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/NodeIterator.h>
#include <Poco/DOM/NodeFilter.h>
#include <Poco/DOM/NamedNodeMap.h>
#include <Poco/DOM/NodeList.h>
#include <iostream>
#include <sstream>
#include <fmt/format.h>

using namespace fmt::literals;

namespace cic {
namespace task {

//Task::Task() noexcept
//: mConfig{ nullptr }
//{
//}
//
//Task::~Task() noexcept
//{
//    unload();
//}
//
//void Task::load()
//{
//    Poco::FileInputStream istr( mConfigPath.toString() );
//    unload();
//    mConfig = new TaskConfig();
//}
//
//void Task::unload() noexcept
//{
//    if ( mConfig != nullptr )
//    {
//        delete mConfig;
//        mConfig = nullptr;
//    }
//}


TaskProvider::TaskProvider() noexcept
{
}

TaskProvider::~TaskProvider() noexcept
{
}

void TaskProvider::loadDecls( const std::string& declsPath )
{
    fmt::print( "[info] loading declaration from '{}'\n", declsPath );

	Poco::FileInputStream istr{ declsPath };
	Poco::XML::InputSource input{ istr };

	try
	{
		Poco::XML::DOMParser parser;
		auto doc( parser.parse( &input ) );

		Poco::XML::Node* declsNode{ doc->getNodeByPath( "/taskDeclarations" ) };

		if ( declsNode && declsNode->nodeType() == Poco::XML::Node::ELEMENT_NODE )
		{
			Poco::XML::NodeList* nodeList{ declsNode->childNodes() };
			for ( std::size_t i{ 0 }; i < nodeList->length(); ++i )
			{
				Poco::XML::Node* node{ nodeList->item( i ) };
				if ( !node /*nerdy?*/ || node->nodeType() != Poco::XML::Node::ELEMENT_NODE )
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
						Poco::Path path{ newDeclPath };
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
							   , "[error] Unexpected xml element '{0}' inside 'TaskDeclarations' container;\n"\
							   "\tignoring '{0}';\n"
							   , node->nodeName()
					);
				}
			}
		}
		else
		{
			fmt::print( stderr, "[error] xml element 'TaskDeclarations' not found, so no Task declarations loaded;\n" );
		}
	}
	catch ( Poco::Exception& e )
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

//    void create( const std::string& name ) noexcept
//    {
//        Poco::Path binaryDirPath( true );
//        binaryDirPath.assign( "/Users/mezozoy/ws/CICheck/build/bin/Debug", Poco::Path::Style::PATH_UNIX );
//        Poco::Path taskPath( false );
//        taskPath.assign( "../share/CICheck", Poco::Path::Style::PATH_UNIX );
//        taskPath.makeAbsolute( binaryDirPath );
//        std::unique_ptr< Task > task{ new Task() };
//        mTasks.insert( std::make_pair( name, std::move( task ) ) );
//    }

bool TaskProvider::isDeclared( const std::string& name ) const noexcept
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
//    auto it( mDecls.find( name ) );
    return ( it != mDecls.end() );
}

//const Task& TaskProvider::operator [] ( const std::string& name ) const
//{
//    return ( mTasks.at( name ) );
//}

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
