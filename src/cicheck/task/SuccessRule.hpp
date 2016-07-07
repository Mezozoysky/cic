//
//  SuccessRule.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 06.07.16.
//
//

#ifndef CICHECK_CICHECK__TASK_SUCCESS_RULE_HPP
#define CICHECK_CICHECK__TASK_SUCCESS_RULE_HPP

#include "RuleBase.hpp"

namespace cic
{


class SuccessRule
: public RuleBase
{
public:
	using Ptr = std::shared_ptr< SuccessRule >;

	virtual ~SuccessRule() noexcept = default;

	virtual bool check() override;
};


} // namespace cic

#endif /* CICHECK_CICHECK__TASK_SUCCESS_RULE_HPP */
