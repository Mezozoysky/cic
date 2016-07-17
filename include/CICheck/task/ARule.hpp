//
//  ARule.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 29.06.16.
//
//

#ifndef CICHECK_TASK__A_RULE_HPP
#define CICHECK_TASK__A_RULE_HPP

#include <CICheck/task/TaskUnit.hpp>
#include <string>

namespace cic
{
namespace task
{

class ARule
: public TaskUnit
{
public:
	using Ptr = std::shared_ptr< ARule >;

	virtual ~ARule() noexcept = default;

public:
	virtual bool check() = 0;

	virtual const std::string& name() const noexcept;

protected:
	std::string mName;
};


} // namespace task
} // namespace cic

#endif /* CICHECK_TASK__A_RULE_HPP */
