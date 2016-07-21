//
//  ATargetSet.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 28.06.16.
//
//

#include <CICheck/task/ATargetSet.hpp>
#include <limits>
#include <algorithm>
#include <cassert>
#include <functional>
#include <list>
#include <Poco/Exception.h>
#include <Poco/Util/Application.h>
#include <fmt/format.h>

using namespace fmt::literals;

namespace cic
{
namespace task
{

// const std::size_t ATargetSet::BAD_INDEX{ std::numeric_limits< std::size_t >::max() };

ATargetSet::ATargetSet() noexcept
: TaskUnit()
, mDefault{ "" }
{
}

bool ATargetSet::check( const std::string& tgtName )
{
	Seq seq;
	buildSequence( tgtName, seq );

	fmt::MemoryWriter w;
	w.write( "ATargetSet checking! sequence[{}]: [", seq.size() );
	for ( const auto& tgt : seq )
	{
		w.write( " '{}'", tgt );
	}
	w.write( " ];\n" );
	fmt::print( w.str() );
	
	bool result{ true };
	for ( auto tgt : seq )
	{
		const auto& rules( mTargets.at( tgt ).rules );
		if ( rules.empty() )
		{
			fmt::print(
				stderr
				, "[notice] no rules for target '{}';"\
				  " interpreting as successfull check;\n"
				, tgt
			);
			continue;
		}
		
		for ( const auto& rule : rules )
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

void ATargetSet::buildSequence( const std::string& tgtName, Seq& seq ) const
{
	if ( mTargets.count( tgtName ) == 0 )
	{
		throw
		Poco::DataException{
			"Target '{}' isnt found with TargetSet"_format( tgtName )
			, Poco::Util::Application::EXIT_DATAERR
		};
	}

	const auto& deps( mTargets.at( tgtName ).deps );
	for ( const auto& dep : deps )
	{
		if ( dep == tgtName || isADependsOnB( dep, tgtName ) )
		{
			throw
			Poco::DataException{
				"Target '{}' cyclic dependency!"_format( tgtName )
				, Poco::Util::Application::EXIT_DATAERR
			};
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

		Seq subseq;
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

bool ATargetSet::isADependsOnB ( const std::string& tgtA, const std::string& tgtB ) const
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

std::size_t ATargetSet::getSize() const noexcept
{
	return ( mTargets.size() );
}

const std::string& ATargetSet::defaultTarget() const noexcept
{
	return ( mDefault );
}

} // namespace task
} // namespace cic
