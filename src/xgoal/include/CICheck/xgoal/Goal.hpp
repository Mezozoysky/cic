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
/// \brief Provides XMLed version of Goal type from cic-goal library
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright CICheck is released under the terms of zlib/png license


#ifndef CIC_XGOAL__GOAL_HPP
#define CIC_XGOAL__GOAL_HPP

#include <CICheck/goal/Goal.hpp>
#include <CICheck/xmlu/LoadableFromXML.hpp>

namespace cic
{
namespace xgoal
{

class Provider;

class Goal
: public goal::Goal
, public xmlu::LoadableFromXML
{

friend class Provider;

public:
	using Ptr = std::shared_ptr< Goal >;

	Goal() = default;
	virtual ~Goal() noexcept = default;

	virtual void loadFromXML( const xmlu::Node* root, indu::Industry* industry ) override;
	virtual void loadTargetsFromXML( const xmlu::Node* root, indu::Industry* industry );
	virtual void loadTargetFromXML( const xmlu::Node* root, indu::Industry* industry );
};

} // namespace xgoal
} // namespace cic

#endif // CIC_XGOAL__GOAL_HPP
