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
/// \brief Plan class implementation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright cic is released under the terms of zlib/png license


#include <cic/plan/Plan.hpp>
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

Plan::Plan()
: Act()
, mDefaultPhaseClass( "default" )
{
}

bool Plan::perform( PerformConfig* pc,
                    Report& report,
                    Industry& industry,
                    std::ostream& outStream,
                    std::ostream& errStream ) const
{
    Target* target{ static_cast< Target* >( pc ) };
    Sequence sequence;
    buildSequence( sequence, target );


    // output phase sequence
    {
        outStream << "Phase sequence:";
        for ( std::size_t index : sequence )
        {
            const std::string& name{ std::static_pointer_cast< Phase >( getChild( index ) )->getName() };
            outStream << " '{}';"_format( name );
        }
        outStream << std::endl;
    }


    bool success{ true };

    Phase::Ptr phase{};
    for ( const std::size_t& index : sequence )
    {
        phase = std::static_pointer_cast< Phase >( getChild( index ) );
        Report::Ptr phaseReport{};
        if ( success )
        {
            outStream << "Start phase '{}'..."_format( phase->getName() ) << std::endl;

            phaseReport = phase->perform( pc, industry, outStream, errStream );
            success = phaseReport->getSuccess();

            outStream << "Finished phase '{}': {}"_format( phase->getName(),
                                                             success ? "SUCCESS" : "FAILURE" ) << std::endl;
        }
        else
        {
            outStream << "Skip phase '{}'; considered as FAILURE\n";
            phaseReport.reset( industry.getFactory< Report >()->create( phase->getClassName() ) );
            phaseReport->fillWithAct( *phase );
            // phaseReport->setSuccess( false );
        }
        report.addChild( phaseReport );
    }
    return ( success );
}

void Plan::loadFromXML( Element* root, Industry* industry )
{
    Element* elem{ root->getChildElement( "phases" ) };
    if ( elem != nullptr )
    {
        loadPhasesFromXML( elem, industry );
    }
}

std::size_t Plan::getPhaseIndex( const std::string& name ) const noexcept
{
    const auto it( mIndexMap.find( name ) );
    if ( it == mIndexMap.end() )
    {
        return ( BAD_INDEX );
    }

    return ( it->second );
}

Phase::Ptr Plan::getPhase( const std::string& name ) const noexcept
{
    std::size_t index{ getPhaseIndex( name ) };
    if ( index == BAD_INDEX )
    {
        return ( nullptr );
    }

    Phase::Ptr phase{ std::static_pointer_cast< Phase >( getChild( index ) ) };
    return ( phase );
}

void Plan::setDefaultPhaseClass( const std::string& classId )
{
    onSetDefaultPhaseClass( classId );
    mDefaultPhaseClass = classId;
}

void Plan::onSetDefaultPhaseClass( const std::string& classId )
{
    return;
}

void Plan::onAddChild( const DAGShared& child, std::size_t index )
{
    Phase::Ptr phase{ std::static_pointer_cast< Phase >( child ) };
    assert( phase );
    mIndexMap.insert( std::make_pair( phase->getName(), index ) );
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
        classId = mDefaultPhaseClass;
    }

    auto phaseFactory( industry->getFactory< Phase >() );
    if ( phaseFactory == nullptr )
    {
        throw( Poco::NotFoundException{ "No factory registered for id: '{}'"_format( classId ), 8 } );
    }

    Phase::Ptr phase{ phaseFactory->create( classId ) };
    phase->loadFromXML( root, industry );
    addChild( phase );
}

void Plan::saveToXML( Element* root ) const {}

} // namespace plan
} // namespace cic
