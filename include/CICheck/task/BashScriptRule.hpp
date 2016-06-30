//
//  BashScriptRule.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 30.06.16.
//
//

#ifndef CICHECK_TASK__BASH_SCRIPT_RULE_HPP
#define CICHECK_TASK__BASH_SCRIPT_RULE_HPP

#include <map>
#include <vector>
#include <string>

namespace cic
{
namespace task
{


class AbstractRule
{
public:
	AbstractRule() noexcept;
	~AbstractRule() noexcept;

private:
	std::string mPath;
};



} // namespace task
} // namespace cic

#endif /* CICHECK_TASK__BASH_SCRIPT_RULE_HPP */
