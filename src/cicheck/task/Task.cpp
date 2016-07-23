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
	if ( mTargetSet == nullptr ) throw "NO TARGET SET IN TASK!";
// 	if ( mCheckMap == nullptr ) throw "NO CHECK MAP IN TASK!";
// 	std::vector< std::string > seq{ mTargetSet->calcSequenceFor( target ) };
	bool result{ false };
	result = mTargetSet->check( target );
	return ( result );
}

void Task::loadFromXML( const Node *root, indu::Industry* industry )
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
		}
		fmt::print( "TARGET SET TYPEID: '{}'\n", typeId );
		auto ts = TargetSet::Ptr( industry->get< TargetSet >()->create( typeId ) );
		ts->loadFromXML( node, industry );

		mTargetSet = ts;
	}

	// checkMap
	node = fetchNode( root, "/checkMap" );
	if ( !node )
	{
		fmt::print( "CHECK MAP NOT FOUND!\n" );
		//TODO: throw here?
	}
	else
	{
		fmt::print( "CHECK MAP FOUND!\n" );
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
		}
		fmt::print( "CHECK MAP TYPEID: '{}'\n", typeId );
		auto cm = CheckMap::Ptr( industry->get< CheckMap >()->create( typeId ) );
		cm->loadFromXML( node, industry );

		mCheckMap = cm;
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

void Task::setCheckMap( const CheckMap::Ptr& cm )
{
	mCheckMap = cm;
}
	

} // namespace cic
