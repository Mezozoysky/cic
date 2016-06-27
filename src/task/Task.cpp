//
//  Task.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 25.06.16.
//
//

#include <CICheck/task/Task.hpp>

namespace cic
{
namespace task
{

Task::Task( const std::string& name, const std::string& path ) noexcept
: mDecl{ name, path }
{
}

Task::Task( const TaskDecl& decl ) noexcept
: mDecl{ decl }
{
}

Task::~Task() noexcept
{
}

} // namespace task
} // namespace cic
