//
//  AbstractTask.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 01.07.16.
//
//

#ifndef CICHECK_TASK__ABSTRACT_TASK_HPP
#define CICHECK_TASK__ABSTRACT_TASK_HPP

#include <string>
#include <CICheck/task/TaskDecl.hpp>
#include <memory>
#include <type_traits>
#include <CICheck/task/AbstractTargetSet.hpp>

namespace cic
{
namespace task
{

class AbstractTask
: public std::enable_shared_from_this< AbstractTask >
{
public:
	using Ptr = std::shared_ptr< AbstractTask >;

	AbstractTask() noexcept = default;
	virtual ~AbstractTask() noexcept = default;
	AbstractTask( const AbstractTask& ) = delete;
	void operator=( const AbstractTask& ) = delete;

	virtual void perform( const std::string& target ) = 0;

public:
	inline const TaskDecl& getDecl() const noexcept;
	inline const std::string& getDescription() const noexcept;
	inline AbstractTargetSet::Ptr getTargetSet() noexcept;

protected:
	inline void setDecl( const TaskDecl& decl ) noexcept;
	inline void setDescription( const std::string& desc ) noexcept;
	inline void setTargetSet( const AbstractTargetSet::Ptr& ts ) noexcept;

private:
	TaskDecl mDecl;
	std::string mDescription;
	AbstractTargetSet::Ptr mTargetSet;
};

//
// Inliners
//

inline const TaskDecl& AbstractTask::getDecl() const noexcept
{
	return ( mDecl );
}

inline void AbstractTask::setDecl( const TaskDecl& decl ) noexcept
{
	mDecl = decl;
}

inline const std::string& AbstractTask::getDescription() const noexcept
{
	return ( mDescription );
}

inline void AbstractTask::setDescription( const std::string& desc ) noexcept
{
	mDescription = desc;
}

inline AbstractTargetSet::Ptr AbstractTask::getTargetSet() noexcept
{
	return ( mTargetSet );
}

inline void AbstractTask::setTargetSet( const AbstractTargetSet::Ptr& ts ) noexcept
{
	mTargetSet = ts;
}

} // namespace task
} // namespace cic


#endif /* CICHECK_TASK__ABSTRACT_TASK_HPP */
