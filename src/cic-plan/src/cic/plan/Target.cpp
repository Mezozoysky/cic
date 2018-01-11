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
/// \brief Target class implementation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2018
/// \copyright cic is released under the terms of zlib/png license


#include <cic/plan/Target.hpp>
#include <cic/plan/Phase.hpp>
#include <fmt/format.h>
#include <Poco/Exception.h>
#include <Poco/String.h>
#include <cic/plan/Act.hpp>
#include <cic/plan/Report.hpp>
#include <Poco/DOM/Element.h>
#include <Poco/AutoPtr.h>
#include <Poco/DOM/NodeList.h>
#include <algorithm>
#include <functional>

using Poco::XML::Element;
using Poco::XML::NodeList;
using namespace fmt::literals;
using Poco::AutoPtr;
using cic::industry::Industry;


namespace cic
{
namespace plan
{


Target::Target()
: PerformConfig()
{
}

void Target::loadFromXML( Element* root, Industry* industry )
{
    assert( Poco::XML::fromXMLString( root->nodeName() ) == "target" );

    PerformConfig::loadFromXML( root, industry );

    Element* elem{ root->getChildElement( "phases" ) };
    if ( elem != nullptr )
    {
        loadPhasesFromXML( elem, industry );
    }
}

void Target::saveToXML( Element* root ) const {}

void Target::setPhases( const std::vector< std::string >& phaseList )
{
    onSetPhases( phaseList );
    mPhases = phaseList;
}

void Target::onSetPhases( const std::vector< std::string >& phaseList )
{
    return;
}

void Target::onSetPlanPath( const std::string& phaseList )
{
    return;
}

void Target::loadPhasesFromXML( Element* root, Industry* industry )
{
    AutoPtr< NodeList > list{ root->childNodes() };

    Element* elem{ nullptr };
    std::vector< std::string > phaseNames;
    for ( std::size_t i{ 0 }; i < list->length(); ++i )
    {
        elem = static_cast< Element* >( list->item( i ) );
        if ( elem != nullptr && elem->nodeName() == "phase" )
        {
            std::string value{ Poco::trim( root->getAttribute( "value" ) ) };
            if ( value.empty() )
            {
                // TODO: log warning
            }
            else
            {
                phaseNames.push_back( value );
            }
        }
    }
    setPhases( phaseNames );
}


} // namespace plan
} // namespace cic
