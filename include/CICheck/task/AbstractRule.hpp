//
//  AbstractRule.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 29.06.16.
//
//

#ifndef CICHECK_TASK__ABSTRACT_RULE_HPP
#define CICHECK_TASK__ABSTRACT_RULE_HPP

#include <map>
#include <vector>
#include <string>

namespace cic
{
namespace task
{

class AbstractRule
{
	AbstractRule() = delete;
public:
	~AbstractRule() noexcept;

private:
	std::string mName;
};



} // namespace task
} // namespace cic

#endif /* CICHECK_TASK__ABSTRACT_RULE_HPP */
