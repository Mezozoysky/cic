//
//  RuleSet.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 07.07.16.
//
//

#ifndef CICHECK_CICHECK__TASK_RULE_SET_HPP
#define CICHECK_CICHECK__TASK_RULE_SET_HPP

#include <CICheck/task/ARuleSet.hpp>
#include "RuleBase.hpp"
#include <vector>
#include <CICheck/xmlu/XMLUtils.hpp>


namespace cic
{


class RuleSet
: public task::ARuleSet
, public xmlu::ALoadableFromXml
{
public:
	using Ptr = std::shared_ptr< RuleSet >;

	virtual ~RuleSet() noexcept = default;

public:
	virtual bool check() override;
	virtual void loadFromXml( const xmlu::Node* root, tu::FactoryOwner* factories ) override;

	virtual const std::string& name() const override;
	virtual void setName( const std::string& name ) noexcept;
	virtual std::size_t getSize() const override;
	virtual const std::vector< task::ARule::Ptr >& rules() const override;

protected:
	std::string mName;
	std::vector< task::ARule::Ptr > mRules;
};


} // namespace cic

#endif /* CICHECK_CICHECK__TASK_RULE_SET_HPP */
