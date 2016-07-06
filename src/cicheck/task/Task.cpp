//
//  Task.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 05.07.16.
//
//

#include "Task.hpp"

using namespace cic::task;
using namespace cic::xmlu;

namespace cic
{


bool Task::check( const std::string &target )
{
	fmt::print( "TASK CHECK!\n" );
	return ( false );
}

void Task::loadFromXml( const Node *root, tu::FactoryOwner *factories )
{
	fmt::print( "TASK LOAD FROM XML!\n" );
	NodeMap* rootAttrs{ root->attributes() };

	// name
	Node* node{ rootAttrs->getNamedItem( "name" ) };
	if ( !node )
	{
		fmt::print( stderr, "[notice] name attribute isnt specified!\n" );
	}
	else
	{
		auto name( node->getNodeValue() );
		if ( !mName.empty() )
		{
			if ( mName != name )
			{
				fmt::print(
						stderr
						, "[warning] different names: old '{}' and new '{}'; new name ignored;\n"
						, mName
						, name
				);
			}
		}
		else
		{
			mName = name;
		}
	}

	// targetSet
	node = fetchNode( root, "/targetSet" );
	NodeMap* attrs{ nullptr };
	Node* attr{ nullptr };
	if ( !node )
	{
		fmt::print( "TARGET SET NOT FOUND!\n" );
		//TODO: throw here?
	}
	else
	{
		fmt::print( "TARGET SET FOUND!\n" );
		attrs = node->attributes();

		std::string typeId = "default";
		if ( attrs )
		{
			attr = attrs->getNamedItem( "typeId" );
			std::string typeId;
			if ( attr )
			{
				typeId = attr->getNodeValue();
			}
			else
			{
				typeId = "default";
			}
		}
		fmt::print( "TARGET SET TYPEID: '{}'\n", typeId );
		auto ts = TargetSet::Ptr( factories->get< TargetSet >()->create( typeId ) );
		ts->loadFromXml( node, factories );

		mTargetSet = ts;
	}
}
	

const std::string& Task::name()  const
{
	return ( mName );
}

void Task::setName( const std::string& name )
{
	mName = name;
}

ATargetSet::Ptr Task::getTargetSet() const
{
	return ( std::static_pointer_cast< ATargetSet >( mTargetSet ) );
}

void Task::setTargetSet( const TargetSet::Ptr& ts )
{
	mTargetSet = ts;
}

ACheckMap::Ptr Task::getCheckMap() const
{
	return ( mCheckMap );
}

void Task::setCheckMap( const ACheckMap::Ptr& cm )
{
	mCheckMap = cm;
}
	

} // namespace cic
