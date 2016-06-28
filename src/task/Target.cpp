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

Target::Target( const std::string& name, std::vector< std::string >&& deps ) noexcept
: mName{ name }
, mDeps{ std::move( deps ) }
{
}

Target::Target( Target&& other ) noexcept
: mName{ std::move( other.mName ) }
, mDeps{ std::move( other.mDeps ) }
{
}

Target::~Target() noexcept
{
}

void Target::addDep( const std::string& name ) noexcept
{
	mDeps.push_back( name );
}

void Target::addDeps( const DepsList& deps ) noexcept
{
	mDeps = deps;
}

} // namespace task
} // namespace cic
