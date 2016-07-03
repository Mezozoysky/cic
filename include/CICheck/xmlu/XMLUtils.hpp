//
//  XMLUtils.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 01.07.16.
//
//

#ifndef CICHECK_XMLU__XML_UTILS_HPP
#define CICHECK_XMLU__XML_UTILS_HPP

#include <Poco/DOM/Node.h>
#include <Poco/DOM/NodeList.h>
#include <Poco/DOM/NamedNodeMap.h>
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/Document.h>
#include <Poco/SAX/InputSource.h>
#include <Poco/XML/XMLException.h>
#include <Poco/AutoPtr.h>

namespace cic
{
namespace xmlu
{

using Exception = Poco::XML::XMLException; // ?? is it ugly?
using Node = Poco::XML::Node;
using NodeList = Poco::XML::NodeList;
using NodeMap = Poco::XML::NamedNodeMap;
using Parser = Poco::XML::DOMParser;
using Doc = Poco::XML::Document;
using DocPtr = Poco::AutoPtr< Doc >;
using Input = Poco::XML::InputSource;


DocPtr fetchDoc( const std::string& path, Parser& parser );

const Node* fetchXML(
	const Node* root
	, const std::string& nodePath
	, unsigned short requiredType = 0 //Node::ELEMENT_NODE
);

std::string fetchValue(
	const Node* root
	, const std::string& nodePath
	, const std::string& defaultValue = ""
);

class LoadableFromXML
{
public:
	LoadableFromXML() noexcept = default;
	virtual ~LoadableFromXML() noexcept = default;

	virtual void loadFromXML( const Node* node ) = 0;

	virtual void loadFromXMLFile(
		const std::string& path
		, Parser& parser
		, const std::string& nodePath = "/"
	);
};


} //namespace xmlu
} // namespace cic


#endif /* CICHECK_XMLU__XML_UTILS_HPP */
