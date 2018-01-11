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
/// \brief PerformConfig provides perform configuration for Act-based classes
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2018
/// \copyright cic is released under the terms of zlib/png license


#ifndef CIC_PLAN__PERFORM_CONF_HPP
#define CIC_PLAN__PERFORM_CONF_HPP

#include "Serializable.hpp"
#include <string>
#include <vector>
#include <Poco/Util/AbstractConfiguration.h>
#include <Poco/Util/XMLConfiguration.h>


namespace cic
{
namespace plan
{

class PerformConfig : public Serializable
{
    CLASSINFO( PerformConfig )

public:
    using Ptr = std::shared_ptr< PerformConfig >;

    using AConfig = Poco::Util::AbstractConfiguration;
    using AConfigPtr = Poco::AutoPtr< AConfig >;
    using Config = Poco::Util::XMLConfiguration;
    using ConfigPtr = Poco::AutoPtr< Config >;

public:
    PerformConfig();
    virtual ~PerformConfig() noexcept = default;

    virtual void loadFromXML( Poco::XML::Element* root, cic::industry::Industry* industry ) override;
    virtual void saveToXML( Poco::XML::Element* root ) const override;

    inline const std::string& getWorkDir() const noexcept;
    void setWorkDir( const std::string& workDir );
    inline const std::string& getReportDir() const noexcept;
    void setReportDir( const std::string& reportDir );
    inline const AConfig& props() const noexcept;
    inline AConfig& props() noexcept;

protected:
    inline Config& properties() noexcept;

    virtual void onSetWorkDir( const std::string& workDir );
    virtual void onSetReportDir( const std::string& reportDir );

private:
    std::string mWorkDir;
    std::string mReportDir;
    ConfigPtr mProperties;
};

inline const std::string& PerformConfig::getWorkDir() const noexcept
{
    return ( mWorkDir );
}

inline const std::string& PerformConfig::getReportDir() const noexcept
{
    return ( mReportDir );
}

inline const PerformConfig::AConfig& PerformConfig::props() const noexcept
{
    return ( *mProperties );
}

inline PerformConfig::AConfig& PerformConfig::props() noexcept
{
    return ( *mProperties );
}

inline PerformConfig::Config& PerformConfig::properties() noexcept
{
    return ( *mProperties );
}

} // namespace plan
} // namespace cic

#endif // CIC_PLAN__PERFORM_CONF_HPP
