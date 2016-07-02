//
//  BashScriptRule.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 30.06.16.
//
//

#ifndef CICHECK_TASK__BASH_SCRIPT_RULE_HPP
#define CICHECK_TASK__BASH_SCRIPT_RULE_HPP

#include <CICheck/task/AbstractRule.hpp>

namespace cic
{
namespace task
{


class BashScriptRule
: public AbstractRule
{
public:
	BashScriptRule() noexcept;
	virtual ~BashScriptRule() noexcept = default;

public:
	inline const std::string& getPath() const noexcept;
protected:
	inline void setPath( const std::string& path ) noexcept;

private:
	std::string mPath;
};

// Inliners
inline const std::string& BashScriptRule::getPath() const noexcept
{
	return ( mPath );
}
inline void BashScriptRule::setPath( const std::string& path ) noexcept
{
	mPath = path;
}


} // namespace task
} // namespace cic

#endif /* CICHECK_TASK__BASH_SCRIPT_RULE_HPP */
