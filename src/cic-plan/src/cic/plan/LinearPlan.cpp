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
/// \brief Linear plan
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2018
/// \copyright cic is released under the terms of zlib/png license


#include <cic/plan/LinearPlan.hpp>
#include <fmt/format.h>
#include <Poco/Exception.h>
#include <Poco/String.h>
#include <cic/plan/Act.hpp>
#include <cic/plan/Report.hpp>
#include <Poco/DOM/Element.h>
#include <Poco/AutoPtr.h>
#include <Poco/DOM/NodeList.h>

using Poco::XML::Element;
using Poco::XML::NodeList;
using namespace fmt::literals;
using Poco::AutoPtr;
using cic::industry::Industry;


namespace cic
{
namespace plan
{

LinearPlan::LinearPlan()
: Plan()
{
}

void LinearPlan::buildSequence( Sequence& seq ) const
{
    assert( getTargetPhases().size() > 0 );
    seq.clear();

    Sequence phaseIndices;

    {
        std::size_t index{ BAD_INDEX };
        for ( const auto& phaseName : getTargetPhases() )
        {
            index = getPhaseIndex( phaseName );
            if ( index == BAD_INDEX )
            {
                throw Poco::DataException( "Plan has no requested phase: '{}'"_format( phaseName ), 8 );
            }
            phaseIndices.push_back( index );

            assert( phaseIndices.size() > 0 );
            if ( phaseIndices.size() > 1 )
            {
                std::sort( phaseIndices.begin(), phaseIndices.end() );
            }
        }
    }

    {
        std::size_t prevIMin{ 0 };
        for ( auto it( phaseIndices.begin() ); it != phaseIndices.end(); ++it )
        {
            Phase::Ptr phase{ std::static_pointer_cast< Phase >( getChild( *it ) ) };
            if ( !phase->isPhony() )
            {
                for ( std::size_t i{ prevIMin }; i < *it; ++i )
                {
                    seq.push_back( i );
                }
            }
            seq.push_back( *it );
        }
    }
}


} // namespace plan
} // namespace cic
