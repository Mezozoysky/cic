//
//  RuleBase.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 06.07.16.
//
//

#ifndef CICHECK_CICHECK__TASK_RULE_BASE_HPP
#define CICHECK_CICHECK__TASK_RULE_BASE_HPP

#include <CICheck/task/ARule.hpp>
#include <string>
#include <CICheck/xmlu/XMLUtils.hpp>

namespace cic
{


class RuleBase
: public task::ARule
, public xmlu::ALoadableFromXml
{
public:
	using Ptr = std::shared_ptr< RuleBase >;

	virtual ~RuleBase() noexcept = default;

public:

	virtual const std::string& name() const override;
	virtual void setName( const std::string& name );

protected:
	std::string mName;
};


} // namespace cic

#endif /* CICHECK_CICHECK__TASK_RULE_BASE_HPP */
