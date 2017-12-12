//  cic
//
//  cic - Copyright (C) 2016 Stanislav Demyanovich <mezozoysky@gmail.com>
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
/// \copyright cic is released under the terms of zlib/png license

#ifndef CIC_CHECK__XML_UTILS_HPP
#define CIC_CHECK__XML_UTILS_HPP

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
namespace check
{


Poco::AutoPtr<Poco::XML::Document> fetchDoc( const std::string& path, Poco::XML::DOMParser& parser );

Poco::XML::Node* fetchNode( const Poco::XML::Node* root,
                 const std::string& nodePath,
                 unsigned short requiredType = 0 );

std::string fetchText( const Poco::XML::Node* root );


} // namespace check
} // namespace cic


#endif /* CIC_CHECK__XML_UTILS_HPP */
