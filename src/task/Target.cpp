//
//  Target.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 28.06.16.
//
//

#include <CICheck/task/Target.hpp>

namespace cic
{
namespace task
{

Target::Target( const std::string& name ) noexcept
: mName{ name }
{
}

Target::Target( const std::string& name, const std::vector< std::string > & deps ) noexcept
: mName{ name }
, mDeps{ deps }
{
}

void Target::addDep( const std::string &name ) noexcept
{
    mDeps.push_back( name );
}

void Target::addDeps( const DepsList &deps ) noexcept
{
    mDeps = deps;
}

} // namespace task
} // namespace cic
