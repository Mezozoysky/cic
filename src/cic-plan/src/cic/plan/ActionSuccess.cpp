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
/// \brief ActionSuccess realisation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright cic is released under the terms of zlib/png license

#include <cic/plan/ActionSuccess.hpp>
#include <Poco/DOM/Element.h>

using Poco::XML::Element;
using cic::industry::Industry;

namespace cic
{
namespace plan
{


bool ActionSuccess::execute( std::ostream& outStream, std::ostream& errStream )
{
    return ( true );
}

void ActionSuccess::loadFromXML( Element* xml, Industry* industry )
{
    // Do nothing
}

void ActionSuccess::saveToXML( Element* root ) const {}

} // namespace plan
} // namespace cic
