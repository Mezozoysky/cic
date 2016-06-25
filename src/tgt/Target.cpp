//
//  Target.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 25.06.16.
//
//

#include <CICheck/tgt/Target.hpp>

namespace cic {
namespace tgt {


TargetDecl::TargetDecl( const std::string& name, const std::string& path ) noexcept
: name ( name )
, path ( path )
{
}


Target::Target( const std::string& name, const std::string& path ) noexcept
: mDecl{ name, path }
{
}

Target::~Target() noexcept
{
}

} // namespace tgt
} // namespace cic
