//
//  TaskDecl.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 27.06.16.
//
//

#include <CICheck/task/TaskDecl.hpp>

namespace cic {
namespace task {


TaskDecl::TaskDecl( const std::string& name, const std::string& path ) noexcept
: name ( name )
, path ( path )
{
}

void TaskDecl::operator=( const cic::task::TaskDecl &other ) noexcept
{
	name = other.name;
	path = other.path;
}


} // namespace task
} // namespace cic
