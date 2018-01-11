//  cic
//
//  cic - Copyright (C) 2017-2018 Stanislav Demyanovich <mezozoysky@gmail.com>
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
/// \brief Target class provides perform configuration for Plan-based classes
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2018
/// \copyright cic is released under the terms of zlib/png license


#ifndef CIC_PLAN__TARGET_HPP
#define CIC_PLAN__TARGET_HPP

#include "PerformConfig.hpp"
#include <string>
#include <vector>


namespace cic
{
namespace plan
{

class Target : public PerformConfig
{
    CLASSINFO( Target )

public:
    using Ptr = std::shared_ptr< Target >;

public:
    Target();
    virtual ~Target() noexcept = default;

    virtual void loadFromXML( Poco::XML::Element* root, cic::industry::Industry* industry ) override;
    virtual void saveToXML( Poco::XML::Element* root ) const override;

    inline const std::string& getPlanPath() const noexcept;
    void setPlanPath( const std::string& planPath );
    inline const std::vector< std::string >& getPhases() const noexcept;
    void setPhases( const std::vector< std::string >& phaseList );

protected:
    virtual void onSetPlanPath( const std::string& planPath );
    virtual void onSetPhases( const std::vector< std::string >& phaseList );

    virtual void loadPhasesFromXML( Poco::XML::Element* root, cic::industry::Industry* industry );

private:
    std::string mPlanPath;
    std::vector< std::string > mPhases;
};

inline const std::string& Target::getPlanPath() const noexcept
{
    return ( mPlanPath );
}

inline const std::vector< std::string >& Target::getPhases() const noexcept
{
    return ( mPhases );
}


} // namespace plan
} // namespace cic

#endif // CIC_PLAN__TARGET_HPP
