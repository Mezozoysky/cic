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
/// \brief Plan class implementation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright cic is released under the terms of zlib/png license


#include <cic/plan/Plan.hpp>
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

bool Plan::execute( const std::string& phaseName, TargetReport* report, bool skipDependencies )
{
    if ( phaseName.empty() )
    {
        throw( Poco::InvalidArgumentException{ "Target phase name is empty" } );
    }
    if ( mPhases.count( phaseName ) == 0 )
    {
        throw( Poco::NotFoundException{ "Target phase name isnt found: {}"_format( phaseName ), 8 } );
    }

    Sequence seq;
    if ( skipDependencies )
    {
        seq.push_back( phaseName );
    }
    else
    {
        buildSequence( phaseName, seq );
    }

    bool result{ true };

    for ( const auto& phName : seq )
    {
        if ( mPhases.count( phName ) == 0 )
        {
            throw( Poco::NotFoundException{ "Phase name isnt found: {}"_format( phName ), 8 } );
        }

        report->phaseReports().emplace_back();
        auto& phaseReport( report->phaseReports().back() );
        phaseReport.name() = phName;

        if ( result )
        {
            result = mPhases.at( phName )->execute( &phaseReport );
        }

        phaseReport.success() = result;
    }

    return ( result );
}

void Plan::buildSequence( const std::string& phaseName, Sequence& seq ) const
{
    if ( mPhases.count( phaseName ) == 0 )
    {
        throw( Poco::NotFoundException{ "Requested phase isnt found", 8 } );
    }

    const auto& deps( mPhases.at( phaseName )->deps() );
    for ( const auto& dep : deps )
    {
        if ( dep == phaseName || isADependsOnB( dep, phaseName ) )
        {
            throw( Poco::DataException{ "Phase '{}' cyclic dependency!"_format( phaseName ), 8 } );
        }

        bool needSkip{ false };
        for ( const auto& otherDep : deps )
        {
            if ( otherDep == dep )
            {
                continue;
            }
            if ( isADependsOnB( otherDep, dep ) )
            {
                needSkip = true;
                break;
            }
        }

        if ( needSkip )
        {
            // dont include dep into sequence as it will be included by other dep
            continue;
        }

        Sequence subseq;
        buildSequence( dep, subseq );
        seq.insert( seq.end(), subseq.begin(), subseq.end() );
    }

    seq.push_back( phaseName );

    // remove duplicates from sequence;
    auto it( seq.begin() );
    while ( it != seq.end() )
    {
        auto it2( it + 1 );
        while ( it2 != seq.end() )
        {
            if ( ( *it2 ) == ( *it ) )
            {
                it2 = seq.erase( it2 );
            }
            else
            {
                ++it2;
            }
        }
        ++it;
    }
}

bool Plan::isADependsOnB( const std::string& phaseA, const std::string& phaseB ) const
{
    if ( phaseA == phaseB )
        return ( true );

    bool depends{ false };
    for ( const auto& dep : mPhases.at( phaseA )->deps() )
    {
        if ( ( depends = isADependsOnB( dep, phaseB ) ) )
        {
            break;
        }
    }
    return ( depends );
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
    phases().insert( PhaseMap::value_type{ name, phase } );
}

void Plan::saveToXML( Element* root ) const {}

} // namespace plan
} // namespace cic
