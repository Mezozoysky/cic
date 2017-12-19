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
/// \brief Report implementation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright cic is released under the terms of zlib/png license


#include <cic/plan/Report.hpp>
#include <Poco/DOM/Element.h>

using Poco::XML::Element;

namespace cic
{
namespace plan
{

void ActionReport::loadFromXML( Element* root, Industry* industry )
{
    // Do nothing
}

void ActionReport::saveToXML( Element* root ) const {}

void PhaseReport::loadFromXML( Element* root, Industry* industry )
{
    // Do nothing
}

void PhaseReport::saveToXML( Element* root ) const {}

void TargetReport::loadFromXML( Element* root, Industry* industry )
{
    // Do nothing
}

void TargetReport::saveToXML( Element* root ) const {}

} // namespace plan
} // namespace cic
