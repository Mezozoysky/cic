//
//  SystemCmdRule.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 07.07.16.
//
//

#ifndef CICHECK_CICHECK__TASK_SYSTEM_CMD_RULE_HPP
#define CICHECK_CICHECK__TASK_SYSTEM_CMD_RULE_HPP

#include "RuleBase.hpp"

namespace cic
{


class SystemCmdRule
: public RuleBase
{
public:
	using Ptr = std::shared_ptr< SystemCmdRule >;

	virtual ~SystemCmdRule() noexcept = default;

	virtual bool check() override;
	virtual void loadFromXml( const xmlu::Node* root, tu::FactoryOwner* factories ) override;

protected:
	std::string mCmd;
	std::string mOptions;
};


} // namespace cic

#endif /* CICHECK_CICHECK__TASK_SYSTEM_CMD_RULE_HPP */
