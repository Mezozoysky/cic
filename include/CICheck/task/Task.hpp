//
//  Task.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 25.06.16.
//
//

#ifndef CICHECK_TASK_TASK_HPP
#define CICHECK_TASK_TASK_HPP

#include <string>
#include <CICheck/task/TaskDecl.hpp>

namespace cic {
namespace task {

class Task
{
    Task() = delete;
    Task( const Task& ) = delete;
    void operator = ( const Task& ) = delete;

public:
    explicit Task( const std::string& name, const std::string& path ) noexcept;
    explicit Task( const TaskDecl& decl ) noexcept;
    virtual ~Task() noexcept;

    inline const TaskDecl& getDecl() const noexcept;

private:
    TaskDecl mDecl;
//    std::string mDescription;
};


//
// Inliners
//

inline const TaskDecl& Task::getDecl() const noexcept
{
    return ( mDecl );
}


} // namespace task
} // namespace cic

#endif /* CICHECK_TASK_TASK_HPP */
