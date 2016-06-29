//
//  Task.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 25.06.16.
//
//

#ifndef CICHECK_TASK__TASK_HPP
#define CICHECK_TASK__TASK_HPP

#include <string>
#include <CICheck/task/TaskDecl.hpp>
#include <memory>
#include <type_traits>
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/Node.h>
#include <CICheck/task/AbstractTargetSet.hpp>

namespace cic {
namespace task {



class Task
{
    Task() = delete;
    Task( const Task& ) = delete;
    void operator = ( const Task& ) = delete;

public:
	using Ptr = std::shared_ptr< Task >;

	template< typename TaskT, typename... ArgsT >
	static Task::Ptr createPtr( ArgsT&&... args ) noexcept
	{
		static_assert(
			std::is_base_of< Task, TaskT >::value
			, "TaskT should extend cic::task::Task class"
		);
		Task::Ptr task{ std::make_shared< TaskT >( args... ) };
		return ( task );
	}

	explicit Task( const std::string& name, const std::string& path ) noexcept;
	explicit Task( const TaskDecl& decl ) noexcept;
	virtual ~Task() noexcept;

	virtual bool load( Poco::XML::DOMParser& parser );

	inline const TaskDecl& getDecl() const noexcept;
	inline const std::string& getDescription() const noexcept;
	inline AbstractTargetSet::Ptr getTargetSet() noexcept;

protected:
	virtual bool loadTask( const Poco::XML::Node* node );
	virtual bool loadDescription( const Poco::XML::Node* node );
	virtual bool loadTargetSet( const Poco::XML::Node* node );

private:
	TaskDecl mDecl;
	std::string mDescription;
	AbstractTargetSet::Ptr mTargetSet;
};


//
// Inliners
//

inline const TaskDecl& Task::getDecl() const noexcept
{
	return ( mDecl );
}

inline const std::string& Task::getDescription() const noexcept
{
	return ( mDescription );
}

inline AbstractTargetSet::Ptr Task::getTargetSet() noexcept
{
	return ( mTargetSet );
}

} // namespace task
} // namespace cic

#endif /* CICHECK_TASK__TASK_HPP */
