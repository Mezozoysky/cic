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
#include <Poco/DOM/Node.h>
#include <cic/plan/Action.hpp>

using Poco::XML::Node;
using namespace fmt::literals;

namespace cic
{
namespace plan
{


bool Plan::execute( const std::string& phaseName, bool skipDependencies )
{
    if ( phaseName.empty() )
    {
        throw( Poco::InvalidArgumentException{ "Requested phase name is empty" } );
    }
    if ( mPhases.count( phaseName ) == 0 )
    {
        throw( Poco::NotFoundException{ "Requested phase name isnt found: {}"_format( phaseName ), 8 } );
    }

    bool result{ true };

    Sequence seq;
    if ( skipDependencies )
    {
        seq.push_back( phaseName );
    }
    else
    {
        buildSequence( phaseName, seq );
    }

    for ( const auto& phName : seq )
    {
        const auto& actions( mPhases.at( phName )->actions() );
        if ( actions.empty() )
        {
            // 			fmt::print(
            // 				stderr
            // 				, "[notice] no actions for phase '{}';"\
// 				  " interpreting as successfull plan;\n"
            // 				, tgt
            // 			);
            continue; // interpreting plan as successfull since no actions to check
        }

        for ( auto action : actions )
        {
            if ( !action->execute() )
            {
                result = false;
                break;
            }
        }
        if ( !result )
        {
            break;
        }
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

void Plan::loadFromXML( Node* root, Industry* industry ) {}
void Plan::saveToXML( Node* root ) const {}

} // namespace plan
} // namespace cic
