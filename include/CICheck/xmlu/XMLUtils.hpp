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
#include <CICheck/tu/FactoryOwner.hpp>

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

Node* fetchNode(
	const Node* root
	, const std::string& nodePath
	, unsigned short requiredType = 0 //Node::ELEMENT_NODE
);

std::string fetchText(
	const Node* root
);

class ALoadableFromXml
{
public:
	ALoadableFromXml() noexcept = default;
	virtual ~ALoadableFromXml() noexcept = default;

	virtual void loadFromXml( const Node* root, tu::FactoryOwner* factories ) = 0;
};


} //namespace xmlu
} // namespace cic


#endif /* CICHECK_XMLU__XML_UTILS_HPP */
