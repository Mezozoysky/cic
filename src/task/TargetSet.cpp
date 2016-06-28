//
//  TargetSet.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 28.06.16.
//
//

#include <CICheck/task/TargetSet.hpp>
#include <CICheck/task/Target.hpp>
#include <limits>

namespace cic
{
namespace task
{

const std::size_t TargetSet::BAD_INDEX{ std::numeric_limits< std::size_t >::max() };

TargetSet::TargetSet() noexcept
{
}

TargetSet::~TargetSet() noexcept
{
}

const std::vector< std::string >
TargetSet::calcSequenceFor( const std::string &targetName ) const noexcept
{
    std::vector< std::string > seq;

    std::size_t idx{ index( targetName ) };
    if ( idx == BAD_INDEX )
    {
        return ( seq );
    }
    seq.push_back( targetName );

    const Target& tgt{ mTargets.at( idx ) };
    if ( tgt.deps().empty() )
    {
        return ( seq );
    }
    
    //TODO: given calculation is just useless stub, implement the real calculation
//    const Target::DepsList& tgtDeps{ tgt.deps() };
//    for ( auto dep: tgtDeps )
//    {
//        const Target& depTgt{ mTargets.at( index( dep ) ) };
//        const Target::DepsList& depDeps{ depTgt.deps() };
//        seq.insert( seq.begin(), depDeps.begin(), depDeps.end() );
//    }

    return ( seq );
}

void TargetSet::addTarget( const Target& target ) noexcept
{
    mTargets.push_back( target );
}

} // namespace task
} // namespace cic
