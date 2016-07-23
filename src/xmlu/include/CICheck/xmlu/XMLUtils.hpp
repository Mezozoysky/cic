//  CICheck
//
//  CICheck - Copyright (C) 2016 Stanislav Demyanovich <mezozoysky@gmail.com>
//
//  This software is provided 'as-is', without any express or
//  implied warranty. In no event will the authors be held
//  liable for any damages arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute
//  it freely, subject to the following restrictions:
//
//  1.  The origin of this software must not be misrepresented;
//      you must not claim that you wrote the original software.
//      If you use this software in a product, an acknowledgment
//      in the product documentation would be appreciated but
//      is not required.
//
//  2.  Altered source versions must be plainly marked as such,
//      and must not be misrepresented as being the original software.
//
//  3.  This notice may not be removed or altered from any
//      source distribution.


/// \file
/// \brief Provides wrapped Poco::XML::* types and couple of XML utility functions
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright CICheck is released under the terms of zlib/png license

#ifndef CIC_XMLU__XML_UTILS_HPP
#define CIC_XMLU__XML_UTILS_HPP

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

Node* fetchNode( const Node* root, const std::string& nodePath, unsigned short requiredType = 0 );

std::string fetchText( const Node* root );


} //namespace xmlu
} // namespace cic


#endif /* CIC_XMLU__XML_UTILS_HPP */
