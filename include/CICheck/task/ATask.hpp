//
//  ATask.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 01.07.16.
//
//

#ifndef CICHECK_TASK__A_TASK_HPP
#define CICHECK_TASK__A_TASK_HPP

#include <string>
#include <memory>
#include <CICheck/task/AbstractTargetSet.hpp>

namespace cic
{
namespace task
{

class ATask
: public std::enable_shared_from_this< ATask >
{
public:
	using Ptr = std::shared_ptr< ATask >;

	ATask() noexcept = default;
	virtual ~ATask() noexcept = default;
	ATask( const ATask& ) = delete;
	void operator=( const ATask& ) = delete;

	virtual void perform( const std::string& target ) = 0;

public:
	inline const std::string& getName() const noexcept;

protected:
	inline void setName( const std::string& name ) noexcept;

private:
	std::string mName;
};

//
// Inliners
//

inline const std::string& ATask::getName() const noexcept
{
	return ( mName );
}

inline void ATask::setName( const std::string& name ) noexcept
{
	mName = name;
}

} // namespace task
} // namespace cic


#endif /* CICHECK_TASK__A_TASK_HPP */
