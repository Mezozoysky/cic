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
/// \brief ARule - basic rule abstraction
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright CICheck is released under the terms of zlib/png license


#ifndef CIC_GOAL__A_RULE_HPP
#define CIC_GOAL__A_RULE_HPP

#include <memory>
#include <string>

namespace cic
{
namespace goal
{

class ARule
{
public:
	using Ptr = std::shared_ptr< ARule >;

	ARule() = default;
	ARule( const ARule& other ) = delete;
	void operator=( const ARule& other ) = delete;
	virtual ~ARule() noexcept = default;

	virtual bool check() = 0;

public:
	inline const std::string& name() const noexcept;
protected:
	inline std::string& name() noexcept;
private:
	std::string mName{ "" };
};

inline const std::string& ARule::name() const noexcept
{
	return ( mName );
}

inline std::string& ARule::name() noexcept
{
	return (mName);
}

} // namespace goal
} // namespace cic

#endif // CIC_GOAL__A_RULE_HPP
