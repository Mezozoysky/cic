//
//  TaskProvider.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 03.07.16.
//
//

#ifndef CICHECK_CICHECK__TASK_PROVIDER_HPP
#define CICHECK_CICHECK__TASK_PROVIDER_HPP

#include "task/Task.hpp"
#include <vector>
#include <string>
#include <map>
#include <CICheck/xmlu/XMLUtils.hpp>
#include <CICheck/industry/Industry.hpp>
#include <Poco/Logger.h>

using namespace cic::task;
using namespace cic::xmlu;

namespace cic
{


class TaskProvider
{
public:
	TaskProvider() = default;
	virtual ~TaskProvider() noexcept = default;

	virtual void init();
	virtual ATask::Ptr get( const std::string& name);

	inline void addSource( const std::string& fileName ) noexcept;
	inline void setSources( std::vector< std::string >&& sources ) noexcept;
	inline const std::vector< std::string >& sources() const noexcept;
	inline std::vector< std::string >& sources() noexcept;

	inline Poco::Logger& logger() const noexcept;
protected:
	virtual Task::Ptr load( const std::string& name );

private:
	std::vector< std::string > mSources;
	std::map< std::string, task::ATask::Ptr > mLoaded;
	xmlu::Parser mParser;
	indu::Industry mIndustry;
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

inline Poco::Logger& TaskProvider::logger() const noexcept
{
	return ( Poco::Logger::get( "TaskProvider" ) );
}

} // namespace cic

#endif // CICHECK_CICHECK__TASK_PROVIDER_HPP
