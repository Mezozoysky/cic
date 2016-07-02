//
//  AbstractRule.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 29.06.16.
//
//

#ifndef CICHECK_TASK__ABSTRACT_RULE_HPP
#define CICHECK_TASK__ABSTRACT_RULE_HPP

#include <map>
#include <vector>
#include <string>

namespace cic
{
namespace task
{

class AbstractRule
{
public:
	AbstractRule() noexcept = default;
	virtual ~AbstractRule() noexcept = default;

public:
	inline const std::string& getName() const noexcept;
protected:
	inline void setName( const std::string& name ) noexcept;

private:
	std::string mName;
};

// Inliners
inline const std::string& AbstractRule::getName() const noexcept
{
	return ( mName );
}
inline void AbstractRule::setName( const std::string& name ) noexcept
{
	mName = name;
}


} // namespace task
} // namespace cic

#endif /* CICHECK_TASK__ABSTRACT_RULE_HPP */
