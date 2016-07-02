//
//  XMLUtils.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 01.07.16.
//
//

#include <CICheck/xmlu/XMLUtils.hpp>
#include <Poco/FileStream.h>
#include <cassert>
#include <fmt/format.h>


using namespace fmt::literals;

namespace cic
{
namespace xmlu
{

DocPtr fetchDoc( const std::string& path, Parser& parser )
{
	DocPtr doc{ nullptr };

	if ( path.empty() ) return ( doc );

	Poco::FileInputStream istr{ path };
	Input input{ istr };
	doc = parser.parse( &input );
	return ( doc );
}

const Node* fetchXML(
	const Node* root
	, const std::string& nodePath
	, unsigned short requiredType
)
{
	if ( !root ) assert( false && "fetchXML: root is NULL!" );
	const Node* node{ root->getNodeByPath( nodePath ) };
	if ( !node )
	{
		fmt::print(
			stderr
			, "[error] '{}' node has no path '{}';\n"
			, root->nodeName()
			, nodePath
		);
		return ( nullptr );
	}
	if ( requiredType && ( requiredType != node->nodeType() ) )
	{
		fmt::print(
			stderr
			, "[error] found node '{}' with nodeType={} but required type {};\n"
			, node->nodeName()
			, node->nodeType()
			, requiredType
		);
		return ( nullptr );
	}
	return ( node );
}

std::string fetchValue(
	const Node* root
	, const std::string& nodePath
	, const std::string& defaultValue
)
{
	auto node( fetchXML( root, nodePath ) );
	if ( !node )
	{
		return ( defaultValue );
	}

	std::string value{ node->getNodeValue() };
	if ( value.empty() )
	{
		return ( defaultValue );
	}
	return ( value );
}


void LoadableFromXML::loadFromXMLFile(
	const std::string& path
	, Parser& parser
	, const std::string& nodePath
)
{
	DocPtr doc = fetchDoc( path, parser );
	if ( doc.isNull() )
	{
		throw Exception( "Can't parse file '{}';"_format( path ) );
	}

	loadFromXML( fetchXML( doc.get(), nodePath ) );
}


} //namespace xmlu
} // namespace cic
