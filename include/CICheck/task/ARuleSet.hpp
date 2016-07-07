//
//  ARuleSet.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 29.06.16.
//
//

#ifndef CICHECK_TASK__A_RULE_SET_HPP
#define CICHECK_TASK__A_RULE_SET_HPP

#include <CICheck/task/TaskUnit.hpp>
#include <vector>
#include <CICheck/task/ARule.hpp>

namespace cic
{
namespace task
{

class ARuleSet
: public TaskUnit
{
public:
	using Ptr = std::shared_ptr< ARuleSet >;

	virtual ~ARuleSet() noexcept = default;

	virtual bool check() = 0;

	virtual const std::string& name() const = 0;
	virtual const std::vector< ARule::Ptr >& rules() const = 0;

};



} // namespace task
} // namespace cic

#endif /* CICHECK_TASK__A_RULE_SET_HPP */
