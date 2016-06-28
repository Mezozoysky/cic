//
//  Task.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 25.06.16.
//
//

#include <CICheck/task/Task.hpp>
#include <Poco/FileStream.h>
#include <Poco/Path.h>
#include <Poco/SAX/InputSource.h>
#include <Poco/DOM/Document.h>
#include <cassert>
#include <Poco/AutoPtr.h>
#include <Poco/DOM/NodeList.h>
#include <fmt/format.h>
#include <Poco/String.h>
#include <Poco/DOM/NamedNodeMap.h>
#include <CICheck/task/LinearTargetSet.hpp>


using namespace Poco;

namespace cic
{
namespace task
{

Task::Task( const std::string& name, const std::string& path ) noexcept
: mDecl{ name, path }
{
}

Task::Task( const TaskDecl& decl ) noexcept
: mDecl{ decl }
{
}

Task::~Task() noexcept
{
}

bool Task::load( XML::DOMParser &parser )
{
	AutoPtr<XML::Document> doc;

	{
		FileInputStream istr{
			Path::forDirectory( getDecl().path )
					.setFileName( "task.xml" )
					.toString()
		};
		XML::InputSource input{ istr };
		doc = parser.parse( &input );
	}

	if ( doc.isNull() ) return ( false );

	if ( !loadTask( doc.get() ) ) return ( false );

	return ( true );
}

bool Task::loadTask( const XML::Node* node )
{
	XML::Node* taskNode{ node->getNodeByPath( "/task" ) };
	if ( !taskNode || taskNode->nodeType() != XML::Node::ELEMENT_NODE ) return ( false );
	if ( !loadDescription( taskNode ) ) return ( false );
	if ( !loadTargetSet( taskNode ) ) return ( false );

	return ( true );
}

bool Task::loadDescription( const XML::Node* node )
{
	XML::Node* descNode{ node->getNodeByPath( "/description" ) };
	if ( !descNode || descNode->nodeType() != XML::Node::ELEMENT_NODE ) return ( false );

	mDescription = descNode->innerText();
	trimInPlace( mDescription );

	return ( true );
}

bool Task::loadTargetSet( const XML::Node* node )
{
	XML::Node* tsNode{ node->getNodeByPath( "/targetSet" ) };
	if ( tsNode )
	{
		if ( tsNode->nodeType() != XML::Node::ELEMENT_NODE ) return ( false );

		XML::Node* useAttr( tsNode->attributes()->getNamedItem( "use" ) );
		if ( !useAttr )
		{
			fmt::print(
				stderr
				, "[error] custom TargetSet support is not implemented yet;\n"\
				  "\tused TargetSet should be specified via 'use' attribute;\n"\
				  "\tto use default TargetSet: '<targetSet use=\"default\">';\n"\
				  "\tTargetSet loading terminated;\n"
			);
			return ( false );
		}
		if ( !( useAttr->getNodeValue() == "default" ) )
		{
			fmt::print(
				stderr
				, "[error] attribute 'use' support no values, except 'default';\n"\
				  "so the only default target set implemented;\n"\
				  "TargetSet loading terminated;\n"
			);
			return ( false );
		}
	}

	const AbstractTargetSet::Sequence seq{
		"clean"
		, "configure"
		, "build"
		, "buildtests"
		, "runtests"
	};

	mTargetSet = AbstractTargetSet::createPtr<LinearTargetSet>( seq );

	return ( true );
}



} // namespace task
} // namespace cic
