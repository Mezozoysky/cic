//  CICheck
//
//  CICheck - Copyright (C) 2016 Stanislav Demyanovich <mezozoysky@gmail.com>
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
/// \brief Goal class implementation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright CICheck is released under the terms of zlib/png license


#include <CICheck/goal/Goal.hpp>
#include <fmt/format.h>
#include <Poco/Exception.h>

using namespace fmt::literals;

namespace cic
{
namespace goal
{


bool Goal::check ( const std::string& tgtName )
{
	bool result{ true };

	Sequence seq;
	buildSequence( tgtName, seq );

	for ( const auto& tgt : seq )
	{
		RuleList& rules{ mTargets.at( tgtName ).rules };
		if ( rules.empty() )
		{
			fmt::print(
				stderr
				, "[notice] no rules for target '{}';"\
				  " interpreting as successfull check;\n"
				, tgtName
			);
			continue;
		}

		for ( auto rule : rules )
		{
			if ( !rule->check() )
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

void Goal::buildSequence ( const std::string& tgtName, Sequence& seq ) const
{
	if ( mTargets.count( tgtName ) == 0 )
	{
		throw (
				Poco::DataException{
					"Target '{}' isnt found with TargetSet"_format( tgtName )
					, 8
				}
		);
	}
	
	const auto& deps( mTargets.at( tgtName ).deps );
	for ( const auto& dep : deps )
	{
		if ( dep == tgtName || isADependsOnB( dep, tgtName ) )
		{
			throw (
					Poco::DataException{
						"Target '{}' cyclic dependency!"_format( tgtName )
						, 8
					}
			);
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
	
	seq.push_back( tgtName );
	
	//remove duplicates from sequence;
	auto it( seq.begin() );
	while ( it != seq.end() )
	{
		auto it2( it + 1 );
		while ( it2 != seq.end() )
		{
			if ( (*it2) == (*it) )
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

bool Goal::isADependsOnB ( const std::string& tgtA, const std::string& tgtB ) const
{
	if ( tgtA == tgtB ) return ( true );
	
	bool depends{ false };
	for ( const auto& dep : mTargets.at( tgtA ).deps )
	{
		if ( depends = isADependsOnB( dep, tgtB ) )
		{
			break;
		}
	}
	return ( depends );
}

} // namespace goal
} // namespace cic
