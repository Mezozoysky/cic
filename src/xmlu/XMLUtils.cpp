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

Node* fetchNode(
	const Node* root
	, const std::string& nodePath
	, unsigned short requiredType
)
{
	if ( !root ) assert( false && "fetchNode: root is NULL!" );
	Node* node{ root->getNodeByPath( nodePath ) };
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

std::string fetchText(
	const Node* root
)
{
	fmt::MemoryWriter text;

	if ( root )
	{
		NodeList* nodes{ root->childNodes() };
		for ( std::size_t i{ 0 }; i < nodes->length(); ++i )
		{
			if ( nodes->item( i )->nodeType() == Node::TEXT_NODE )
			{
				text.write( nodes->item( i )->nodeValue() );
			}
		}
	}
	return ( text.str() );
}


} //namespace xmlu
} // namespace cic
