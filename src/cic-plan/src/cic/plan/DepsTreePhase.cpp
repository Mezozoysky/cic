//  cic
//
//  cic - Copyright (C) 2017-2018 Stanislav Demyanovich <mezozoysky@gmail.com>
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
/// \brief DepsTreePhase - phase with possible dependencies
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2018
/// \copyright cic is released under the terms of zlib/png license


#include <cic/plan/DepsTreePhase.hpp>
#include <Poco/Exception.h>
#include <Poco/String.h>
#include <fmt/format.h>
#include <fstream>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/NodeList.h>

using Poco::XML::Node;
using Poco::XML::NodeList;
using namespace fmt::literals;
using Poco::AutoPtr;
using Poco::XML::Element;
using cic::industry::Industry;
using cic::plan::Report;

namespace cic
{
namespace plan
{

void DepsTreePhase::setDeps( const StrList& deps )
{
    onSetDeps( deps );
    //TODO: add deps 1 by 1
    mDeps = deps;
}

void DepsTreePhase::onSetDeps( const StrList& deps )
{
    //do nothing for a while
}

void DepsTreePhase::loadFromXML( Element* root, Industry* industry )
{
    assert( root != nullptr );

    // load basic phase
    Phase::loadFromXML( root, industry );

    // load dependencies
    {
        Element* elem{ root->getChildElement( "dependencies" ) };
        if ( elem != nullptr )
        {
            AutoPtr< NodeList > depNodeList{ elem->childNodes() };
            for ( std::size_t i{ 0 }; i < depNodeList->length(); ++i )
            {
                Element* depElem{ static_cast< Element* >( depNodeList->item( i ) ) };
                if ( depElem != nullptr && depElem->nodeName() == "dependency" )
                {
                    std::string depName{ Poco::trim( depElem->getAttribute( "value" ) ) };
                    if ( !depName.empty() )
                    {
                        deps().push_back( depName );
                    }
                }
            }
        }
    }
}

void DepsTreePhase::saveToXML( Element* root ) const
{
    Phase::saveToXML( root );
    //TODO: save dependencies
}


} // namespace plan
} // namespace cic
