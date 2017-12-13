//  cic
//
//  cic - Copyright (C) 2016 Stanislav Demyanovich <mezozoysky@gmail.com>
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
/// \brief Action - basic rule abstraction
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright cic is released under the terms of zlib/png license


#ifndef CIC_PLAN__ACTION_HPP
#define CIC_PLAN__ACTION_HPP

#include "Serializable.hpp"
#include <memory>
#include <string>

namespace cic
{
namespace plan
{

class Action : public Serializable
{
public:
    using Ptr = std::shared_ptr< Action >;

    Action() = default;
    Action( const Action& other ) = delete;
    Action& operator=( const Action& other ) = delete;
    virtual ~Action() noexcept = default;

    virtual bool execute() = 0;

public:
    inline const std::string& name() const noexcept;

protected:
    inline std::string& name() noexcept;

private:
    std::string mName;
};

inline const std::string& Action::name() const noexcept
{
    return ( mName );
}

inline std::string& Action::name() noexcept
{
    return ( mName );
}

} // namespace plan
} // namespace cic

#endif // CIC_PLAN__ACTION_HPP
