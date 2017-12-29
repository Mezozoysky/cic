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

#include "Action.hpp"
#include <memory>
#include <vector>

namespace cic
{
namespace plan
{

class Phase : public Action
{
    CLASSINFO( Phase );

public:
    using Ptr = std::shared_ptr< Phase >;

    using ActionList = std::vector< Action::Ptr >;

    Phase();
    virtual ~Phase() noexcept = default;

    virtual void loadFromXML( Poco::XML::Element* root, cic::industry::Industry* industry ) override;
    virtual void saveToXML( Poco::XML::Element* root ) const override;

    inline const std::string& getName() const noexcept;
    inline void setName( const std::string& name ) noexcept;
    inline const ActionList& getActions() const noexcept;
    inline void setActions( const ActionList& actions ) noexcept;

protected:
    virtual bool perform( Report& report, cic::industry::Industry& industry ) const override;

    virtual void loadActionsFromXML( Poco::XML::Element* root, cic::industry::Industry* industry );
    virtual void loadActionFromXML( Poco::XML::Element* root, cic::industry::Industry* industry );


private:
    std::string mName;
    ActionList mActions;
};

inline const std::string& Phase::getName() const noexcept
{
    return ( mName );
}

inline void Phase::setName( const std::string& name ) noexcept
{
    mName = name;
}

inline const Phase::ActionList& Phase::getActions() const noexcept
{
    return ( mActions );
}

inline void Phase::setActions( const ActionList& actions ) noexcept
{
    mActions = actions;
}

} // namespace plan
} // namespace cic

#endif // CIC_PLAN__PHASE_HPP
