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
/// \brief Provides ActShell class
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright cic is released under the terms of zlib/png license

#ifndef CIC_PLAN__ACT_SYSTEM_CMD_HPP
#define CIC_PLAN__ACT_SYSTEM_CMD_HPP

#include "Act.hpp"
#include <Poco/Process.h>

namespace cic
{
namespace plan
{

class ActShell : public Act
{
    CLASSINFO( ActShell )

public:
    using Ptr = std::shared_ptr< ActShell >;

    virtual ~ActShell() noexcept = default;

    virtual void loadFromXML( Poco::XML::Element* root, cic::industry::Industry* industry ) override;
    virtual void saveToXML( Poco::XML::Element* root ) const override;
    virtual const std::string formOutline() const noexcept override;

protected:
    virtual bool perform( PerformConfig* pc,
                          Report& report,
                          cic::industry::Industry& industry,
                          std::ostream& outStream = std::cout,
                          std::ostream& errStream = std::cerr ) const override;

public:
    inline const std::string& cmd() const noexcept;
    inline const Poco::Process::Args& args() const noexcept;
    inline const std::string& workDir() const noexcept;

protected:
    inline std::string& cmd() noexcept;
    inline Poco::Process::Args& args() noexcept;
    inline std::string& workDir() noexcept;

private:
    std::string mCmd;
    Poco::Process::Args mArgs;
    std::string mWorkDir;
};

inline const std::string& ActShell::cmd() const noexcept
{
    return ( mCmd );
}
inline std::string& ActShell::cmd() noexcept
{
    return ( mCmd );
}
inline const Poco::Process::Args& ActShell::args() const noexcept
{
    return ( mArgs );
}
inline Poco::Process::Args& ActShell::args() noexcept
{
    return ( mArgs );
}
inline const std::string& ActShell::workDir() const noexcept
{
    return ( mWorkDir );
}
inline std::string& ActShell::workDir() noexcept
{
    return ( mWorkDir );
}


} // namespace plan
} // namespace cic

#endif /* CIC_PLAN__ACT_SYSTEM_CMD_HPP */
