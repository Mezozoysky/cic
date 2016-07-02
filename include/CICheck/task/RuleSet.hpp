//
//  RuleSet.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 29.06.16.
//
//

#ifndef CICHECK_TASK__RULE_SET_HPP
#define CICHECK_TASK__RULE_SET_HPP

#include <vector>
#include <CICheck/task/AbstractRule.hpp>
#include <memory>

namespace cic
{
namespace task
{

class RuleSet
{
public:
    using ListImpl = std::vector< std::unique_ptr< AbstractRule > >;
	RuleSet() noexcept = default;
	~RuleSet() noexcept = default;


private:
    ListImpl mRules;
};



} // namespace task
} // namespace cic

#endif /* CICHECK_TASK__RULE_SET_HPP */
