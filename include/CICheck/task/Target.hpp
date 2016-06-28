//
//  Target.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 28.06.16.
//
//

#ifndef CICHECK_TASK_TARGET_HPP
#define CICHECK_TASK_TARGET_HPP

#include <string>
#include <vector>

namespace cic
{
namespace task
{

class Target
{
public:
	using DepsList = std::vector< std::string >;

	Target( const std::string& name ) noexcept;
	Target( const std::string& name, std::vector< std::string >&& deps ) noexcept;
	Target( Target&& other ) noexcept;
	~Target() noexcept;

	inline const std::string& name() const noexcept;
	inline const DepsList& deps() const noexcept;

protected:
	void addDep( const std::string& name ) noexcept;
	void addDeps( const DepsList& deps ) noexcept;

private:
	std::string mName;
	DepsList mDeps;
};

// Inliners
inline const std::string& Target::name() const noexcept
{
	return ( mName );
}

inline const Target::DepsList& Target::deps() const noexcept
{
	return ( mDeps );
}


} // namespace task
} // namespace cic

#endif /* CICHECK_TASK_TARGET_HPP */
