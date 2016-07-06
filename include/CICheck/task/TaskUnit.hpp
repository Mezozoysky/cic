//
//  TaskUnit.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 06.07.16.
//
//

#ifndef CICHECK_TASK__TASK_UNIT_HPP
#define CICHECK_TASK__TASK_UNIT_HPP

#include <memory>

namespace cic
{
namespace task
{


class TaskUnit
: public std::enable_shared_from_this< TaskUnit >
{
public:
	using Ptr = std::shared_ptr< TaskUnit >;

	TaskUnit() noexcept = default;
	virtual ~TaskUnit() noexcept = default;
	TaskUnit( const TaskUnit& ) = delete;
	void operator=( const TaskUnit& ) = delete;
};


} // namespace task
} // namespace cic


#endif /* CICHECK_TASK__TASK_UNIT_HPP */
