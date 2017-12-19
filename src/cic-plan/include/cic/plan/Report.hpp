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
/// \brief Report class collects info about Plan execution
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright cic is released under the terms of zlib/png license


#ifndef CIC_PLAN__REPORT_HPP
#define CIC_PLAN__REPORT_HPP

#include "Serializable.hpp"
#include <memory>
#include <string>
#include <vector>

namespace cic
{
namespace plan
{

class Report : public Serializable
{
public:
    Report() = default;
    // Report( const Report& other ) = delete;
    Report& operator=( const Report& other ) = delete;
    virtual ~Report() noexcept = default;

    inline bool success() const noexcept;
    inline bool& success() noexcept;

private:
    bool mSuccess;
};

inline bool Report::success() const noexcept
{
    return ( mSuccess );
}

inline bool& Report::success() noexcept
{
    return ( mSuccess );
}


class ActionReport : public Report
{
    CLASSINFO( ActionReport )
public:
    using List = std::vector< ActionReport >;

    ActionReport() = default;
    // ActionReport( const Report& other ) = delete;
    ActionReport& operator=( const ActionReport& other ) = delete;
    virtual ~ActionReport() noexcept = default;

    inline const std::string& classId() const noexcept;
    inline std::string& classId() noexcept;
    inline const std::string& outline() const noexcept;
    inline std::string& outline() noexcept;
    inline const std::string& stdoutFileName() const noexcept;
    inline std::string& stdoutFileName() noexcept;
    inline const std::string& stderrFileName() const noexcept;
    inline std::string& stderrFileName() noexcept;

    virtual void loadFromXML( Poco::XML::Element* root, Industry* industry ) override;
    virtual void saveToXML( Poco::XML::Element* root ) const override;

private:
    std::string mTypeId;
    std::string mOutline;
    std::string mStdoutFileName;
    std::string mStderrFileName;
};

inline const std::string& ActionReport::classId() const noexcept
{
    return ( mTypeId );
}

inline std::string& ActionReport::classId() noexcept
{
    return ( mTypeId );
}

inline const std::string& ActionReport::outline() const noexcept
{
    return ( mOutline );
}

inline std::string& ActionReport::outline() noexcept
{
    return ( mOutline );
}

inline const std::string& ActionReport::stdoutFileName() const noexcept
{
    return ( mStdoutFileName );
}

inline std::string& ActionReport::stdoutFileName() noexcept
{
    return ( mStdoutFileName );
}

inline const std::string& ActionReport::stderrFileName() const noexcept
{
    return ( mStderrFileName );
}

inline std::string& ActionReport::stderrFileName() noexcept
{
    return ( mStderrFileName );
}


class PhaseReport : public Report
{
    CLASSINFO( PhaseReport )

public:
    using List = std::vector< PhaseReport >;

    PhaseReport() = default;
    // PhaseReport( const PhaseReport& other ) = delete;
    PhaseReport& operator=( const PhaseReport& other ) = delete;
    virtual ~PhaseReport() noexcept = default;

    inline const std::string& name() const noexcept;
    inline std::string& name() noexcept;
    inline const ActionReport::List& actionReports() const noexcept;
    inline ActionReport::List& actionReports() noexcept;

    virtual void loadFromXML( Poco::XML::Element* root, Industry* industry ) override;
    virtual void saveToXML( Poco::XML::Element* root ) const override;

private:
    std::string mName;
    ActionReport::List mActionReports;
};

inline const std::string& PhaseReport::name() const noexcept
{
    return ( mName );
}

inline std::string& PhaseReport::name() noexcept
{
    return ( mName );
}

inline const ActionReport::List& PhaseReport::actionReports() const noexcept
{
    return ( mActionReports );
}

inline ActionReport::List& PhaseReport::actionReports() noexcept
{
    return ( mActionReports );
}


class TargetReport : public Report
{
    CLASSINFO( TargetReport )
public:
    using StrList = std::vector< std::string >;

public:
    TargetReport() = default;
    // TargetReport( const TargetReport& other ) = delete;
    TargetReport& operator=( const TargetReport& other ) = delete;
    virtual ~TargetReport() noexcept = default;

    virtual void loadFromXML( Poco::XML::Element* root, Industry* industry ) override;
    virtual void saveToXML( Poco::XML::Element* root ) const override;

public:
    inline const std::string& targetPlan() const noexcept;
    inline std::string& targetPlan() noexcept;
    inline const std::string& targetPhase() const noexcept;
    inline std::string& targetPhase() noexcept;
    inline const PhaseReport::List& phaseReports() const noexcept;
    inline PhaseReport::List& phaseReports() noexcept;

private:
    std::string mTargetPlan;
    std::string mTargetPhase;
    PhaseReport::List mPhaseReports;
};

inline const std::string& TargetReport::targetPlan() const noexcept
{
    return ( mTargetPlan );
}

inline std::string& TargetReport::targetPlan() noexcept
{
    return ( mTargetPlan );
}

inline const std::string& TargetReport::targetPhase() const noexcept
{
    return ( mTargetPlan );
}

inline std::string& TargetReport::targetPhase() noexcept
{
    return ( mTargetPhase );
}

inline const PhaseReport::List& TargetReport::phaseReports() const noexcept
{
    return ( mPhaseReports );
}

inline PhaseReport::List& TargetReport::phaseReports() noexcept
{
    return ( mPhaseReports );
}

} // namespace plan
} // namespace cic

#endif // CIC_PLAN__REPORT_HPP
