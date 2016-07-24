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
/// \brief xgoal::Provider is a utility class for loading XMLed goals
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright CICheck is released under the terms of zlib/png license

#ifndef CIC_XGOAL__PROVIDER_HPP
#define CIC_XGOAL__PROVIDER_HPP

#include <CICheck/xmlu/XMLUtils.hpp>
#include <CICheck/xgoal/Goal.hpp>
#include <vector>
#include <CICheck/industry/Industry.hpp>

namespace cic
{
namespace xgoal
{


class Provider
{
public:
	struct GoalDecl
	{
		std::string path{ "" };
		goal::Goal::Ptr goal{ nullptr };
	};

public:
	Provider() = default;
	virtual ~Provider() noexcept = default;

	virtual void loadDecls( const std::string& declsPath );
	virtual void loadDecl( const xmlu::Node* root );

	inline bool empty() const noexcept;

private:
	std::map< std::string, GoalDecl > mDecls;
	indu::Industry mIndustry;
	xmlu::Parser mParser;
};

inline bool Provider::empty() const noexcept
{
	return ( mDecls.empty() );
}

} // namespace xgoal
} // namespace cic

#endif // CIC_XGOAL__PROVIDER_HPP
