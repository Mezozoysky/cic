//
//  TaskDecl.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 27.06.16.
//
//

#ifndef CICHECK_TASK__TASK_DECL_HPP
#define CICHECK_TASK__TASK_DECL_HPP

#include <string>

namespace cic {
namespace task {

struct TaskDecl
{
    std::string name;
    std::string path;
    
    TaskDecl( const std::string& name, const std::string& path ) noexcept;
};
    
} // namespace task
} // namespace cic

#endif /* CICHECK_TASK__TASK_DECL_HPP */
