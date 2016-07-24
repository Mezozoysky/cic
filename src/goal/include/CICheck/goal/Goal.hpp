//  CICheck
//
//  CICheck - Copyright (C) 2016 Stanislav Demyanovich <mezozoysky@gmail.com>
//
//  This software is provided 'as-is', without any express or
//  implied warranty. In no event will the authors be held
//  liable for any damages arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute
//  it freely, subject to the following restrictions:
//
//  1.  The origin of this software must not be misrepresented;
//      you must not claim that you wrote the original software.
//      If you use this software in a product, an acknowledgment
//      in the product documentation would be appreciated but
//      is not required.
//
//  2.  Altered source versions must be plainly marked as such,
//      and must not be misrepresented as being the original software.
//
//  3.  This notice may not be removed or altered from any
//      source distribution.


/// \file
/// \brief Provides Goal class, the basic CICheck goal abstraction
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright CICheck is released under the terms of zlib/png license


#ifndef CIC_GOAL__GOAL_HPP
#define CIC_GOAL__GOAL_HPP

#include <string>
#include <vector>
#include <map>
#include <CICheck/goal/ARule.hpp>

namespace cic
{
namespace goal
{

class Goal
{
public:
	using Ptr = std::shared_ptr< Goal >;

	using Sequence = std::vector< std::string >;
	using RuleList = std::vector< ARule::Ptr >;
	struct TgtData
	{
		std::string successTrigger{ "" };
		std::string failureTrigger{ "" };
		std::vector< std::string > deps{};
		RuleList rules{};
	};
	using TgtMap = std::map< std::string, TgtData >;

public:
	Goal() = default;
	Goal( const Goal& other ) = delete;
	void operator=( const Goal& other ) = delete;
	virtual ~Goal() noexcept = default;
	
	virtual bool check( const std::string& tgtName );
	virtual void buildSequence( const std::string& tgtName, Sequence& seq ) const;
	virtual bool isADependsOnB( const std::string& tgtA, const std::string& tgtB ) const;

public:
	inline const std::string& name() const noexcept;
	inline const std::string& defaultTarget() const noexcept;
	inline const TgtMap& targets() const noexcept;
protected:
	inline std::string& name() noexcept;
	inline std::string& defaultTarget() noexcept;
	inline TgtMap& targets() noexcept;
private:
	std::string mName;
	std::string mDefaultTgt;
	TgtMap mTargets;
};


inline const std::string& Goal::name() const noexcept
{
	return ( mName );
}
inline std::string& Goal::name() noexcept
{
	return ( mName );
}

inline const std::string& Goal::defaultTarget() const noexcept
{
	return ( mDefaultTgt );
}
inline std::string& Goal::defaultTarget() noexcept
{
	return ( mDefaultTgt );
}

inline const Goal::TgtMap& Goal::targets() const noexcept
{
	return ( mTargets );
}
inline Goal::TgtMap& Goal::targets() noexcept
{
	return ( mTargets );
}

} // namespace goal
} // namespace cic

#endif // CIC_GOAL__GOAL_HPP
