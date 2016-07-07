//
//  ATargetSet.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 28.06.16.
//
//

#ifndef CICHECK_TASK__A_TARGET_SET_HPP
#define CICHECK_TASK__A_TARGET_SET_HPP

#include <CICheck/task/TaskUnit.hpp>
#include <vector>
#include <string>

namespace cic
{
namespace task
{


class ATargetSet
: public TaskUnit
{
public:
	using Ptr = std::shared_ptr< ATargetSet >;

	static const std::size_t BAD_INDEX;

	virtual ~ATargetSet() noexcept = default;

	virtual std::vector< std::string > calcSequenceFor( const std::string& targetName ) const = 0;

	virtual std::size_t getSize() const = 0;
	virtual std::size_t getIndexFor( const std::string& targetName ) const = 0;
	virtual const std::vector< std::string >& targets() const = 0;
	virtual const std::string& defaultTarget() const = 0;
};


} // namespace task
} // namespace cic

#endif /* CICHECK_TASK__A_TARGET_SET_HPP */
