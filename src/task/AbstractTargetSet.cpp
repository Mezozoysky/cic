//
//  TargetSet.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 28.06.16.
//
//

#include <CICheck/task/AbstractTargetSet.hpp>
#include <limits>

namespace cic
{
namespace task
{

const std::size_t AbstractTargetSet::BAD_INDEX{ std::numeric_limits< std::size_t >::max() };

AbstractTargetSet::AbstractTargetSet() noexcept
{
}

AbstractTargetSet::~AbstractTargetSet() noexcept
{
}

void AbstractTargetSet::addTarget( Target&& target ) noexcept
{
	mTargets.emplace_back( std::move( target ) );
	mIndexMap.insert( IndexMap::value_type( mTargets.back().name(), mTargets.size() - 1 ) );
}


//const std::vector< std::string >
//AbstractTargetSet::calcSequenceFor( const std::string &targetName ) const noexcept
//{
//    std::vector< std::string > seq;
//
//    std::size_t idx{ index( targetName ) };
//    if ( idx == BAD_INDEX )
//    {
//        return ( seq );
//    }
//    seq.push_back( targetName );
//
//    const Target& tgt{ mTargets.at( idx ) };
//    if ( tgt.deps().empty() )
//    {
//        return ( seq );
//    }
//
//    //TODO: given calculation is just useless stub, implement the real calculation
//    //    const Target::DepsList& tgtDeps{ tgt.deps() };
//    //    for ( auto dep: tgtDeps )
//    //    {
//    //        const Target& depTgt{ mTargets.at( index( dep ) ) };
//    //        const Target::DepsList& depDeps{ depTgt.deps() };
//    //        seq.insert( seq.begin(), depDeps.begin(), depDeps.end() );
//    //    }
//
//    return ( seq );
//}

} // namespace task
} // namespace cic
