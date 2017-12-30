//  cic
//
//  cic - Copyright (C) 2017 Stanislav Demyanovich <mezozoysky@gmail.com>
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
/// \brief Plan class implementation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright cic is released under the terms of zlib/png license


#include <cic/plan/Plan.hpp>
#include <cic/plan/Phase.hpp>
#include <fmt/format.h>
#include <Poco/Exception.h>
#include <Poco/String.h>
#include <cic/plan/Action.hpp>
#include <cic/plan/Report.hpp>
#include <Poco/DOM/Element.h>
#include <Poco/AutoPtr.h>
#include <Poco/DOM/NodeList.h>

using Poco::XML::Element;
// using Poco::XML::Node;
using Poco::XML::NodeList;
using namespace fmt::literals;
using Poco::AutoPtr;
using cic::industry::Industry;


namespace cic
{
namespace plan
{

bool Plan::perform( Report& report, Industry& industry ) const
{
    // TODO: Apply system wide options
    // TODO: Apply application wide options
    // TODO: Apply plan wide options

    Sequence sequence;
    buildSequence( "" /*targetPhase*/, sequence );

    bool success{ true };

    for ( const std::size_t& index : sequence )
    {
        auto phase( getPhases()[ index ] );
        Report::Ptr phaseReport{};
        if ( success )
        {
            phaseReport = phase->Action::perform( industry );
            success = phaseReport->getSuccess();
        }
        else
        {
            phaseReport.reset( industry.getFactory< Report >()->create( phase->getClassName() ) );
            phaseReport->fillWithAction( *phase );
            // phaseReport->setSuccess( false );
        }
        report.addChildReport( phaseReport );
    }
    return ( success );
}

void Plan::buildSequence( const std::string& phaseName, Sequence& seq ) const
{
    for ( std::size_t index{ 0 }; index < getPhasesCount(); ++index )
    {
        seq.push_back( index );
        if ( getPhases()[ index ]->getName() == phaseName )
        {
            break;
        }
    }
}

void Plan::loadFromXML( Element* root, Industry* industry )
{
    Element* elem{ root->getChildElement( "phases" ) };
    if ( elem != nullptr )
    {
        loadPhasesFromXML( elem, industry );
    }
}

void Plan::loadPhasesFromXML( Element* root, Industry* industry )
{
    AutoPtr< NodeList > list{ root->childNodes() };

    Element* elem{ nullptr };
    for ( std::size_t i{ 0 }; i < list->length(); ++i )
    {
        elem = static_cast< Element* >( list->item( i ) );
        if ( elem != nullptr && elem->nodeName() == "phase" )
        {
            loadPhaseFromXML( elem, industry );
        }
    }
}

void Plan::loadPhaseFromXML( Element* root, Industry* industry )
{
    std::string name{ root->getAttribute( "name" ) };
    if ( name.empty() )
    {
        throw( Poco::SyntaxException{
            "Mandatory attribute 'name' isnt found or empty within the 'phase' element", 8 } );
    }

    std::string classId{ root->getAttribute( "class" ) };
    if ( classId.empty() )
    {
        classId = "default";
    }

    auto phaseFactory( industry->getFactory< Phase >() );
    if ( phaseFactory == nullptr )
    {
        throw( Poco::NotFoundException{ "No factory registered for id: '{}'"_format( classId ), 8 } );
    }

    Phase::Ptr phase{ phaseFactory->create( classId ) };
    phase->loadFromXML( root, industry );
    addPhase( phase );
}

void Plan::saveToXML( Element* root ) const {}

Phase::Ptr Plan::getPhase( const std::string& name ) const noexcept
{
    Phase::Ptr phase{};

    for ( auto it( mPhases.begin() ); it != mPhases.end(); ++it )
    {
        const auto& currPhase = *it;
        if ( currPhase->getName() == name )
        {
            phase = currPhase;
            break;
        }
    }

    return ( phase );
}

} // namespace plan
} // namespace cic
