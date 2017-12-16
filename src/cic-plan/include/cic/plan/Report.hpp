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
    struct ActionReport
    {
        std::string typeId;
        std::string outline;
        std::string stdoutFileName;
        std::string stderrFileName;
        bool success;
    };
    using ActionReportList = std::vector< ActionReport >;

    struct PhaseReport
    {
        std::string name;
        bool success;
        ActionReportList actionReports;
    };
    using PhaseReportList = std::vector< PhaseReport >;

    using StrList = std::vector< std::string >;

public:
    Report() = default;
    Report( const Report& other ) = delete;
    Report& operator=( const Report& other ) = delete;
    virtual ~Report() noexcept = default;

    virtual void loadFromXML( Poco::XML::Node* xml, Industry* industry );
    virtual void saveToXML( Poco::XML::Node* xml ) const;

public:
    inline const std::string& targetPlan() const noexcept;
    inline std::string& targetPlan() noexcept;
    inline const std::string& targetPhase() const noexcept;
    inline std::string& targetPhase() noexcept;
    inline const PhaseReportList& phaseReports() const noexcept;
    inline PhaseReportList& phaseReports() noexcept;
    inline bool success() const noexcept;
    inline bool& success() noexcept;

private:
    std::string mTargetPlan;
    std::string mTargetPhase;
    PhaseReportList mPhaseReports;
    bool mSuccess;
};

inline const std::string& Report::targetPlan() const noexcept
{
    return ( mTargetPlan );
}

inline std::string& Report::targetPlan() noexcept
{
    return ( mTargetPlan );
}

inline const std::string& Report::targetPhase() const noexcept
{
    return ( mTargetPlan );
}

inline std::string& Report::targetPhase() noexcept
{
    return ( mTargetPhase );
}

inline const Report::PhaseReportList& Report::phaseReports() const noexcept
{
    return ( mPhaseReports );
}

inline Report::PhaseReportList& Report::phaseReports() noexcept
{
    return ( mPhaseReports );
}

inline bool Report::success() const noexcept
{
    return ( mSuccess );
}

inline bool& Report::success() noexcept
{
    return ( mSuccess );
}

} // namespace plan
} // namespace cic

#endif // CIC_PLAN__REPORT_HPP
