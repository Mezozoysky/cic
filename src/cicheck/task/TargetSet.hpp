//
//  TargetSet.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 06.07.16.
//
//

#ifndef CICHECK_CICHECK__TASK_TARGET_SET_HPP
#define CICHECK_CICHECK__TASK_TARGET_SET_HPP

#include <CICheck/task/ATargetSet.hpp>
#include <CICheck/xmlu/XMLUtils.hpp>
#include <vector>
#include <string>

namespace cic
{


class TargetSet
: public task::ATargetSet
, public xmlu::ALoadableFromXml
{
public:
	using Ptr = std::shared_ptr< TargetSet >;

	TargetSet() noexcept = default;
	virtual ~TargetSet() noexcept = default;
	TargetSet( const TargetSet& other ) = delete;
	void operator=( const TargetSet& other ) = delete;

	virtual std::vector< std::string >&& calcSequenceFor( const std::string& targetName ) const override;
	virtual void loadFromXml( const xmlu::Node* root, tu::FactoryOwner* factories ) override;

	virtual std::size_t getSize() const override;
	virtual std::size_t getIndexFor( const std::string& targetName ) const override;
	virtual const std::vector< std::string >& targets() const override;
	virtual const std::string& defaultTarget() const override;

protected:
	std::vector< std::string > mTargets;
	std::string mDefault;
};


} // namespace cic

#endif /* CICHECK_CICHECK__TASK_TARGET_SET_HPP */