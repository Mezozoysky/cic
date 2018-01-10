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
/// \date 2018
/// \copyright cic is released under the terms of zlib/png license


#include <cic/plan/DepsTreePlan.hpp>
#include <cic/plan/DepsTreePhase.hpp>
#include <fmt/format.h>
#include <Poco/Exception.h>
#include <Poco/String.h>
#include <cic/plan/Act.hpp>
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

DepsTreePlan::DepsTreePlan()
: Plan()
{
    setDefaultPhaseClass( DepsTreePhase::getClassNameStatic() );
}

void DepsTreePlan::buildSequence( Sequence& seq ) const
{
    assert( getTarget()->getPhases().size() > 0 );
    seq.clear();

    {
        std::size_t index{ BAD_INDEX };
        DepsTreePhase::Ptr{ nullptr };
        for ( const auto& phaseName : getTarget()->getPhases() )
        {
            index = getPhaseIndex( phaseName );
            if ( index == BAD_INDEX )
            {
                throw Poco::DataException( "Plan has no requested phase: '{}'"_format( phaseName ), 8 );
            }
            DepsTreePhase::Ptr phase = std::static_pointer_cast< DepsTreePhase >( getChild( index ) );
            if ( phase == nullptr )
            {
                throw Poco::DataException(
                    "Requested phase exist but it isnt DepsTreePhase or corrupted: '{}'"_format( phaseName ),
                    8 );
            }

            Sequence subseq{};
            buildSubseq( subseq, phase );
            seq.insert( seq.end(), subseq.begin(), subseq.end() );
        };
    }

    // remove duplicates from subsequence;
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

    std::sort( seq.begin(), seq.end(), [this]( std::size_t indexA, std::size_t indexB ) -> bool {
        // should swap elements (return false) if A depends on B only
        return ( !this->isADependsOnB( indexA, indexB ) );
    } );
}

void DepsTreePlan::buildSubseq( Sequence& subseq, const DepsTreePhase::Ptr& phase ) const
{
    subseq.clear();

    if ( !phase->isPhony() )
    {
        for ( const auto& depName : phase->getDeps() )
        {
            if ( getPhaseIndex( depName ) == BAD_INDEX )
            {
                throw( Poco::DataException{ "Phase '{}' depends on unknown phase: '{}'"_format( phase->getName(), depName, 8 ) } );
            }
            if ( depName == phase->getName() || isADependsOnB( depName, phase->getName() ) )
            {
                throw( Poco::DataException{
                    "Phases '{}' <-> '{}' cyclic dependency!"_format( phase->getName(), depName ), 8 } );
            }

            bool needSkip{ false };
            for ( const auto& otherDepName : phase->getDeps() )
            {
                if ( getPhaseIndex( otherDepName ) == BAD_INDEX )
                {
                    throw( Poco::DataException{ "Phase '{}' depends on unknown phase: '{}'"_format( phase->getName(), otherDepName, 8 ) } );
                }
                if ( otherDepName == depName )
                {
                    continue;
                }
                if ( isADependsOnB( otherDepName, depName ) )
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

            Sequence subSubseq;
            buildSubseq( subSubseq, std::static_pointer_cast< DepsTreePhase >( getPhase( depName ) ) );
            subseq.insert( subseq.end(), subSubseq.begin(), subSubseq.end() );
        }
    }

    subseq.push_back( getPhaseIndex( phase->getName() ) );
}

bool DepsTreePlan::isADependsOnB( std::size_t indexA, std::size_t indexB ) const
{
    if ( indexA == indexB )
    {
        return ( true );
    }

    bool depends{ false };
    auto phaseA( std::static_pointer_cast< DepsTreePhase >( getChild( indexA ) ) );
    for ( const auto& dep : phaseA->getDeps() )
    {
        if ( ( depends = isADependsOnB( getPhaseIndex( dep ), indexB ) ) )
        {
            break;
        }
    }
    return ( depends );
}

bool DepsTreePlan::isADependsOnB( const std::string& nameA, const std::string& nameB ) const
{
    return ( isADependsOnB( getPhaseIndex( nameA ), getPhaseIndex( nameB ) ) );
}

void DepsTreePlan::loadFromXML( Element* root, Industry* industry )
{
    Element* elem{ root->getChildElement( "phases" ) };
    if ( elem != nullptr )
    {
        loadPhasesFromXML( elem, industry );
    }
}

void DepsTreePlan::saveToXML( Element* root ) const {}

void DepsTreePlan::loadPhaseFromXML( Element* root, Industry* industry )
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
        classId = DepsTreePhase::getClassNameStatic();
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


} // namespace plan
} // namespace cic
