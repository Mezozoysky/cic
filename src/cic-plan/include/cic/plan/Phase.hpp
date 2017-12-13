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
/// \brief Provides XMLed version of Goal type from cic-goal library
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright cic is released under the terms of zlib/png license


#ifndef CIC_PLAN__PHASE_HPP
#define CIC_PLAN__PHASE_HPP

#include "Serializable.hpp"
#include "Action.hpp"
#include <memory>
#include <vector>

namespace Poco
{
namespace XML
{
class Node;
}
}

namespace cic
{
namespace plan
{

// class Industry;

class Phase : public Serializable
{
public:
    using Ptr = std::shared_ptr< Phase >;

    using ActionList = std::vector< Action::Ptr >;
    using StrList = std::vector< std::string >;

    Phase() = default;
    Phase( const Phase& other ) = delete;
    Phase& operator=( const Phase& other ) = delete;
    virtual ~Phase() noexcept = default;

    virtual bool execute();

    inline ActionList& actions() noexcept;
    inline const ActionList& actions() const noexcept;
    inline StrList& deps() noexcept;
    inline const StrList& deps() const noexcept;

    virtual void loadFromXML( Poco::XML::Node* root, Industry* industry ) override;
    virtual void saveToXML( Poco::XML::Node* xml ) const override;

    ActionList mActions;
    StrList mDeps;
    std::string mSuccessTrigger;
    std::string mFailureTrigger;
};

inline Phase::ActionList& Phase::actions() noexcept
{
    return ( mActions );
}

inline const Phase::ActionList& Phase::actions() const noexcept
{
    return ( mActions );
}

inline Phase::StrList& Phase::deps() noexcept
{
    return ( mDeps );
}

inline const Phase::StrList& Phase::deps() const noexcept
{
    return ( mDeps );
}

} // namespace plan
} // namespace cic

#endif // CIC_PLAN__PHASE_HPP
