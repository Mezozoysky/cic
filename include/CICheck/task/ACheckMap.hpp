//
//  ACheckMap.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 29.06.16.
//
//

#ifndef CICHECK_TASK__A_CHECK_MAP_HPP
#define CICHECK_TASK__A_CHECK_MAP_HPP

#include <CICheck/task/TaskUnit.hpp>
#include <CICheck/task/ARuleSet.hpp>
#include <vector>
#include <string>

namespace cic
{
namespace task
{


class ACheckMap
: public TaskUnit
{
public:
	using Ptr = std::shared_ptr< ACheckMap >;

	virtual ~ACheckMap() noexcept = default;

	virtual bool check( const std::vector< std::string >& sequence ) = 0;

	virtual std::size_t getSize() const = 0;
	virtual std::vector< std::string >&& getKeys() const = 0;
	virtual bool hasKey( const std::string& key ) const = 0;
	virtual ARuleSet::Ptr getByKey( const std::string& key ) const = 0;
};



} // namespace task
} // namespace cic

#endif /* CICHECK_TASK__A_CHECK_MAP_HPP */
