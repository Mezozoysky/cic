//
//  ATask.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 01.07.16.
//
//

#ifndef CICHECK_TASK__A_TASK_HPP
#define CICHECK_TASK__A_TASK_HPP

#include <CICheck/task/TaskUnit.hpp>
#include <CICheck/task/ATargetSet.hpp>
#include <CICheck/task/ACheckMap.hpp>
#include <string>

namespace cic
{
namespace task
{


class ATask
: public TaskUnit
{
public:
	using Ptr = std::shared_ptr< ATask >;

	virtual ~ATask() noexcept = default;

	virtual bool check( const std::string& target ) = 0;

	virtual const std::string& name() const = 0;
	virtual ATargetSet::Ptr getTargetSet() const = 0;
	virtual ACheckMap::Ptr getCheckMap() const = 0;
};


} // namespace task
} // namespace cic


#endif /* CICHECK_TASK__A_TASK_HPP */
