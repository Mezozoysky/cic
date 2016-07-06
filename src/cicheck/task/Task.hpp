//
//  Task.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 05.07.16.
//
//

#ifndef CICHECK_CICHECK__TASK_TASK_HPP
#define CICHECK_CICHECK__TASK_TASK_HPP

#include <CICheck/task/ATask.hpp>
#include <CICheck/xmlu/XMLUtils.hpp>
#include "TargetSet.hpp"
#include <string>
#include <memory>



namespace cic
{

class Task
: public task::ATask
, public xmlu::ALoadableFromXml
{
public:
	using Ptr = std::shared_ptr< Task >;

	Task() noexcept = default;
	virtual ~Task() noexcept = default;
	Task( const Task& ) = delete;
	void operator=( const Task& ) = delete;

	virtual bool check( const std::string& target ) override;
	virtual void loadFromXml( const xmlu::Node* root, tu::FactoryOwner* factories ) override;

	virtual const std::string& name() const override;
	virtual void setName( const std::string& name );
	virtual task::ATargetSet::Ptr getTargetSet() const override;
	virtual void setTargetSet( const TargetSet::Ptr& ts );
	virtual task::ACheckMap::Ptr getCheckMap() const override;
	virtual void setCheckMap( const task::ACheckMap::Ptr& cm );
protected:
	std::string mName;
	TargetSet::Ptr mTargetSet;
	task::ACheckMap::Ptr mCheckMap;
};


} // namespace cic


#endif /* CICHECK_CICHECK__TASK_TASK_HPP */
