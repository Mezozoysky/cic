//
//  LinearTargetSet.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 29.06.16.
//
//

#include <CICheck/task/LinearTargetSet.hpp>

namespace cic
{
namespace task
{

LinearTargetSet::LinearTargetSet( const Sequence& targetNames ) noexcept
: AbstractTargetSet()
{
    for ( auto name: targetNames )
    {
        Target tgt{ name };
        addTarget( std::move( tgt ) );
    }
}

LinearTargetSet::~LinearTargetSet() noexcept
{
}


const LinearTargetSet::Sequence LinearTargetSet::calcSequenceFor( const std::string &targetName ) const
{
    Sequence seq;

    auto idx( index( targetName ) );
    if ( idx == BAD_INDEX )
    {
        return ( seq );
    }
    const TargetList& list{ targets() };
    for ( std::size_t i{ 0 }; i <= idx; ++i )
    {
        seq.push_back( list.at( i ).name() );
    }

    return ( seq );
}

} // namespace task
} // namespace cic
