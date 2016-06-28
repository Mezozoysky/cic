//
//  TargetSet.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 28.06.16.
//
//

#ifndef CICHECK_TASK_TARGETSET_HPP
#define CICHECK_TASK_TARGETSET_HPP

#include <memory>
#include <vector>
#include <map>
#include <CICheck/task/Target.hpp>

namespace cic
{
namespace task
{

    
class TargetSet
{
public:
    using Ptr = std::shared_ptr< TargetSet >;
    
    template< typename TargetSetT, typename... ArgsT >
    static TargetSet::Ptr createPtr( ArgsT&&... args ) noexcept
    {
        static_assert(
                      std::is_base_of< TargetSet, TargetSetT >::value
                      , "TargetSetT should extend cic::task::TargetSet class"
                      );
        TargetSet::Ptr task{ std::make_shared< TargetSetT >( args... ) };
        return ( task );
    }

    static const std::size_t BAD_INDEX;

    TargetSet() noexcept;
    virtual ~TargetSet() noexcept;

    inline const std::size_t count() const noexcept;
    inline const bool count( const std::string& targetName ) noexcept;
    inline const std::size_t index( const std::string& targetName ) const noexcept;
    inline const std::vector< Target > & targets() const noexcept;

    const std::vector< std::string > calcSequenceFor( const std::string& targetName ) const noexcept;

protected:
    //TODO: make addTarget tamplate for various Target types with variadic args
    void addTarget( const Target& target ) noexcept;

private:
    std::vector< Target > mTargets;
    std::map< std::string, std::size_t > mIndicesByName;
};


// Inliners

inline const std::size_t TargetSet::count() const noexcept
{
    return ( mTargets.size() );
}

inline const bool TargetSet::count( const std::string& targetName ) noexcept
{
    return ( mIndicesByName.count( targetName ) );
}

inline const std::size_t TargetSet::index( const std::string& targetName ) const noexcept
{
    return ( mIndicesByName.count( targetName ) ? mIndicesByName.at( targetName ) : BAD_INDEX );
}

inline const std::vector< Target >& TargetSet::targets() const noexcept
{
    return ( mTargets );
}



} // namespace task
} // namespace cic

#endif /* CICHECK_TASK_TARGETSET_HPP */
