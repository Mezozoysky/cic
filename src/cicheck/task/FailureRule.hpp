//
//  FailureRule.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 07.07.16.
//
//

#ifndef CICHECK_CICHECK__TASK_FAILURE_RULE_HPP
#define CICHECK_CICHECK__TASK_FAILURE_RULE_HPP

#include "RuleBase.hpp"

namespace cic
{


class FailureRule
: public RuleBase
{
public:
	using Ptr = std::shared_ptr< FailureRule >;

	virtual ~FailureRule() noexcept = default;

	virtual bool check() override;
	virtual void loadFromXml( const xmlu::Node* root, tu::FactoryOwner* factories ) override;

};


} // namespace cic

#endif /* CICHECK_CICHECK__TASK_FAILURE_RULE_HPP */
