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
/// \brief Provides LoadableFromXML abstract class
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright CICheck is released under the terms of zlib/png license

#ifndef CIC_XMLU__LOADABLE_FROM_XML_HPP
#define CIC_XMLU__LOADABLE_FROM_XML_HPP

#include <CICheck/xmlu/XMLUtils.hpp>
#include <CICheck/industry/Industry.hpp>

namespace cic
{
namespace xmlu
{

class LoadableFromXML
{
public:
	LoadableFromXML() noexcept = default;
	virtual ~LoadableFromXML() noexcept = default;

	virtual void loadFromXML( const Node* root, indu::Industry* industry ) = 0;
};


} //namespace xmlu
} // namespace cic

#endif // CIC_XMLU__LOADABLE_FROM_XML_HPP
