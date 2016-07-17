//
//  AbstractRule.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 29.06.16.
//
//

#include <CICheck/task/ARule.hpp>

namespace cic
{
namespace task
{

const std::string& ARule::name() const noexcept
{
	return ( mName );
}

} // namespace task
} // namespace cic
