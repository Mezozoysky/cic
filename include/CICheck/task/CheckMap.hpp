//
//  CheckMap.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 29.06.16.
//
//

#ifndef CICHECK_TASK__CHECK_MAP_HPP
#define CICHECK_TASK__CHECK_MAP_HPP

#include <map>
#include <vector>
#include <CICheck/task/RuleSet.hpp>

namespace cic
{
namespace task
{

class RuleSet;

class CheckMap
{
public:
	using MapImpl = std::map< std::string, RuleSet >;

	CheckMap() noexcept;
	~CheckMap() noexcept;

private:
	MapImpl mMap;
};



} // namespace task
} // namespace cic

#endif /* CICHECK_TASK__CHECK_MAP_HPP */
