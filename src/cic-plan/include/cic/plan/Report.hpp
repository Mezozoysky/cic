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
/// \brief Report class collects info about Plan execution
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright cic is released under the terms of zlib/png license


#ifndef CIC_PLAN__REPORT_HPP
#define CIC_PLAN__REPORT_HPP

#include "DAGNode.hpp"
#include "Serializable.hpp"
#include <memory>
#include <string>
#include <vector>
#include <limits>

namespace cic
{
namespace plan
{

class Act;

class Report
: public DAGNode< Report >
, public Serializable
{
    CLASSINFO( Report )

public:
    using Ptr = std::shared_ptr< Report >;

    Report() = default;
    virtual ~Report() noexcept = default;

    virtual void fillWithAct( const Act& act ) noexcept;

    inline bool getSuccess() const noexcept;
    inline void setSuccess( bool success ) noexcept;
    inline const std::string& getOutline() const noexcept;

    virtual void loadFromXML( Poco::XML::Element* root, cic::industry::Industry* industry ) override;
    virtual void saveToXML( Poco::XML::Element* root ) const override;

private:
    bool mSuccess{ false };
    std::string mOutline{};
};

inline bool Report::getSuccess() const noexcept
{
    return ( mSuccess );
}

inline void Report::setSuccess( bool success ) noexcept
{
    mSuccess = success;
}

inline const std::string& Report::getOutline() const noexcept
{
    return ( mOutline );
}


class PhaseReport : public Report
{
    CLASSINFO( PhaseReport )

public:
    using Ptr = std::shared_ptr< PhaseReport >;

    PhaseReport() = default;
    virtual ~PhaseReport() noexcept = default;

    inline const std::string& getName() const noexcept;
    inline void setName( const std::string& name ) noexcept;

    virtual void loadFromXML( Poco::XML::Element* root, cic::industry::Industry* industry ) override;
    virtual void saveToXML( Poco::XML::Element* root ) const override;

private:
    std::string mName;
};

inline const std::string& PhaseReport::getName() const noexcept
{
    return ( mName );
}

inline void PhaseReport::setName( const std::string& name ) noexcept
{
    mName = name;
}


class PlanReport : public Report
{
    CLASSINFO( PlanReport )

public:
    using StrList = std::vector< std::string >;

public:
    PlanReport() = default;
    virtual ~PlanReport() noexcept = default;

    virtual void loadFromXML( Poco::XML::Element* root, cic::industry::Industry* industry ) override;
    virtual void saveToXML( Poco::XML::Element* root ) const override;

    inline const std::string& getTargetPhase() const noexcept;
    inline void setTargetPhase( const std::string& targetPhase ) noexcept;

private:
    std::string mTargetPhase;
};

inline const std::string& PlanReport::getTargetPhase() const noexcept
{
    return ( mTargetPhase );
}

inline void PlanReport::setTargetPhase( const std::string& targetPhase ) noexcept
{
    mTargetPhase = targetPhase;
}


} // namespace plan
} // namespace cic

#endif // CIC_PLAN__REPORT_HPP
