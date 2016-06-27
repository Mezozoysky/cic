//
//  TaskProvider.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 23.06.16.
//
//

#ifndef CICHECK_TASK_TASKPROVIDER_HPP
#define CICHECK_TASK_TASKPROVIDER_HPP

#include <string>
#include <memory>
#include <Poco/Path.h>
#include <Poco/FileStream.h>
#include <map>
#include <iostream>
#include <vector>
#include <Poco/DOM/DOMParser.h>

namespace cic {
namespace task {

struct TaskDecl;
class Task;

//class Task
//{
//public:
//    friend class TaskProvider;
//
//    Task() noexcept;
//    ~Task() noexcept;
//
//    void load();
//    void unload() noexcept;
//    inline bool isLoaded() const noexcept;
//
//private:
//    Poco::Path      mConfigPath;
//    TaskConfig*   mConfig;
//};

//inline bool Task::isLoaded() const noexcept
//{
//    return ( mConfig != nullptr );
//}


class TaskProvider
{
public:

	TaskProvider() noexcept;
	virtual ~TaskProvider() noexcept;

	void loadDecls( const std::string& declsPath );
	void dropDecls() noexcept;
	const std::vector< TaskDecl >& getDecls() const noexcept;

	bool isDeclared( const std::string& name ) const noexcept;

//	const Task& operator [] ( const std::string& name ) const;

protected:

	void declare( const TaskDecl& decl );
	void declare( const std::string& name, const std::string& path );

private:
	std::vector< TaskDecl > mDecls;
	Poco::XML::DOMParser mParser;
};

} // namespace task
} // namespace cic

#endif /* CICHECK_TASK_TASKPROVIDER_HPP */
