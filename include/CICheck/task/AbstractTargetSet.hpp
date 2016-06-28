//
//  TargetSet.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 28.06.16.
//
//

#ifndef CICHECK_TASK__ABSTRACT_TARGET_SET_HPP
#define CICHECK_TASK__ABSTRACT_TARGET_SET_HPP

#include <memory>
#include <vector>
#include <map>
#include <CICheck/task/Target.hpp>

namespace cic
{
namespace task
{

class AbstractTargetSet
{
public:
	using Ptr = std::shared_ptr< AbstractTargetSet >;

	template< typename TargetSetT, typename... ArgsT >
	static typename TargetSetT::Ptr createPtr( ArgsT&&... args ) noexcept
	{
		static_assert(
					  std::is_base_of< AbstractTargetSet, TargetSetT >::value
					  , "TargetSetT should extend cic::task::AbstractTargetSet class"
					  );
		typename TargetSetT::Ptr task{ std::make_shared< TargetSetT >( args... ) };
		return ( task );
	}

public:
	using Sequence = std::vector< std::string >;
	using TargetList = std::vector< Target >;
	using IndexMap = std::map< std::string, std::size_t >;

	static const std::size_t BAD_INDEX;

	virtual ~AbstractTargetSet() noexcept;

	inline const std::size_t size() const noexcept;
	inline const std::size_t count( const std::string& targetName ) noexcept;
	inline const std::size_t index( const std::string& targetName ) const noexcept;
	inline const TargetList& targets() const noexcept;

	virtual const Sequence calcSequenceFor( const std::string& targetName ) const = 0;

protected:
	AbstractTargetSet() noexcept;

protected:
	void addTarget( Target&& target ) noexcept;

private:
	TargetList mTargets;
	IndexMap mIndexMap;
};

// Inliners

inline const std::size_t AbstractTargetSet::size() const noexcept
{
	return ( mTargets.size() );
}

inline const std::size_t AbstractTargetSet::count( const std::string& targetName ) noexcept
{
	return ( mIndexMap.count( targetName ) );
}

inline const std::size_t AbstractTargetSet::index( const std::string& targetName ) const noexcept
{
	return ( mIndexMap.count( targetName ) ? mIndexMap.at( targetName ) : BAD_INDEX );
}

inline const std::vector< Target >& AbstractTargetSet::targets() const noexcept
{
	return ( mTargets );
}


} // namespace task
} // namespace cic

#endif /* CICHECK_TASK__ABSTRACT_TARGET_SET_HPP */
