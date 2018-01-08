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
/// \brief Provides PrfrmApp, the application class for prfrm tool
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright cic is released under the terms of zlib/png license


#ifndef CIC_PRFRM__PRFRM_APP_HPP
#define CIC_PRFRM__PRFRM_APP_HPP

#include <Poco/Util/Application.h>
#include <Poco/AutoPtr.h>
#include <Poco/DOM/DOMParser.h>
#include <cic/industry/Industry.hpp>
#include <map>

namespace cic
{
namespace prfrm
{

class PrfrmApp : public Poco::Util::Application
{
public:
    using Ptr = Poco::AutoPtr< PrfrmApp >;

public:
    PrfrmApp() noexcept;
    virtual ~PrfrmApp() noexcept = default;

    void optionCallback( const std::string& name, const std::string& value );

protected:
    virtual void initialize( Poco::Util::Application& self ) override;
    virtual void uninitialize() override;
    virtual void defineOptions( Poco::Util::OptionSet& options ) override;
    virtual int main( const std::vector< std::string >& args ) override;

    virtual std::string formatHelpText() const noexcept;
    void printConfig( const std::string& rootKey ) const;

    int performTask( const std::string& planFileName,
                     const std::vector< std::string >& phaseList,
                     const Poco::Path& workspacePath,
                     const Poco::Path& reportPath,
                     bool only = false ) noexcept;

private:
    bool mIsStopRequestedByOption;

    Poco::XML::DOMParser mParser;
    industry::Industry mIndustry;
};

} // namespace prfrm
} // namespace cic

#endif // CIC_PRFRM__PRFRM_APP_HPP
