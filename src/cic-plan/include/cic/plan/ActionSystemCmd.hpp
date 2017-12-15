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
/// \brief Provides ActionSystemCmd class
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright cic is released under the terms of zlib/png license
///
/// \details ActionSystemCmd class presents the action which plans system command for exit
/// code == 0

#ifndef CIC_PLAN__ACTION_SYSTEM_CMD_HPP
#define CIC_PLAN__ACTION_SYSTEM_CMD_HPP

#include "Action.hpp"
#include <Poco/Process.h>

namespace cic
{
namespace plan
{

class ActionSystemCmd : public Action
{
public:
    using Ptr = std::shared_ptr< ActionSystemCmd >;

    virtual ~ActionSystemCmd() noexcept = default;

    virtual bool execute() override;
    virtual void loadFromXML( Poco::XML::Node* root, Industry* industry ) override;
    virtual void saveToXML( Poco::XML::Node* root ) const override;

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

inline const std::string& ActionSystemCmd::cmd() const noexcept
{
    return ( mCmd );
}
inline std::string& ActionSystemCmd::cmd() noexcept
{
    return ( mCmd );
}
    inline const Poco::Process::Args& ActionSystemCmd::args() const noexcept
{
    return ( mArgs );
}
    inline Poco::Process::Args& ActionSystemCmd::args() noexcept
{
    return ( mArgs );
}
inline const std::string& ActionSystemCmd::workDir() const noexcept
{
    return ( mWorkDir );
}
inline std::string& ActionSystemCmd::workDir() noexcept
{
    return ( mWorkDir );
}


} // namespace plan
} // namespace cic

#endif /* CIC_PLAN__ACTION_SYSTEM_CMD_HPP */
