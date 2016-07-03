//
//  TaskProvider.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 03.07.16.
//
//

#ifndef CICHECK_CICHECK__TASK_PROVIDER_HPP
#define CICHECK_CICHECK__TASK_PROVIDER_HPP

#include <CICheck/task/ATask.hpp>
#include <vector>
#include <string>
#include <map>
#include <CICheck/xmlu/XMLUtils.hpp>

using namespace cic::task;
using namespace cic::xmlu;

namespace cic
{


class TaskProvider
{
public:
	TaskProvider() = default;
	virtual ~TaskProvider() noexcept = default;

	virtual ATask::Ptr get( const std::string& name);

	inline void addSource( const std::string& fileName ) noexcept;
	inline void setSources( std::vector< std::string >&& sources ) noexcept;
	inline const std::vector< std::string >& sources() const noexcept;
	inline std::vector< std::string >& sources() noexcept;

protected:
	virtual ATask::Ptr load( const std::string& name );

private:
	std::vector< std::string > mSources;
	std::map< std::string, task::ATask::Ptr > mLoaded;
	xmlu::Parser mParser;
};

// Inliners
inline void TaskProvider::addSource( const std::string& fileName ) noexcept
{
	mSources.push_back( fileName );
}

inline void TaskProvider::setSources( std::vector< std::string >&& sources ) noexcept
{
	mSources = std::move( sources );
}

inline const std::vector< std::string >& TaskProvider::sources() const noexcept
{
	return ( mSources );
}

inline std::vector< std::string >& TaskProvider::sources() noexcept
{
	return ( mSources );
}

} // namespace cic

#endif // CICHECK_CICHECK__TASK_PROVIDER_HPP
