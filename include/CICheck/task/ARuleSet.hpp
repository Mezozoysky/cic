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

	virtual bool check();

	virtual const std::string& name() const noexcept;
	virtual std::size_t getSize() const noexcept;
	virtual const std::vector< ARule::Ptr >& rules() const noexcept;

protected:
	std::string mName;
	std::vector< task::ARule::Ptr > mRules;
};



} // namespace task
} // namespace cic

#endif /* CICHECK_TASK__A_RULE_SET_HPP */
