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
/// \brief PerformConfig class implementation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2018
/// \copyright cic is released under the terms of zlib/png license


#include <cic/plan/PerformConfig.hpp>
#include <cic/plan/Phase.hpp>
#include <fmt/format.h>
#include <Poco/Exception.h>
#include <Poco/String.h>
#include <cic/plan/Act.hpp>
#include <cic/plan/Report.hpp>
#include <Poco/DOM/Element.h>
#include <Poco/AutoPtr.h>
#include <Poco/DOM/NodeList.h>
#include <algorithm>
#include <functional>

using Poco::XML::Element;
using Poco::XML::NodeList;
using namespace fmt::literals;
using Poco::AutoPtr;
using Poco::Util::XMLConfiguration;
using cic::industry::Industry;


namespace cic
{
namespace plan
{


PerformConfig::PerformConfig()
: Serializable()
, mProperties( new XMLConfiguration() )
{
    // mProperties->loadEmpty( "props" );
}

void PerformConfig::loadFromXML( Element* root, Industry* industry )
{
    Element* elem{ root->getChildElement( "workspace" ) };
    if ( elem != nullptr )
    {
        std::string value{ Poco::trim( Poco::trim( elem->getAttribute( "value" ) ) ) }; // value can be empty
        setWorkspace( value );
    }

    elem = root->getChildElement( "reportDir" );
    if ( elem != nullptr )
    {
        std::string value{ Poco::trim( Poco::trim( elem->getAttribute( "value" ) ) ) }; // value can be empty
        setReportDir( value );
    }

    elem = root->getChildElement( "props" );
    if ( elem != nullptr )
    {
        properties().load( elem );
    }
}

void PerformConfig::saveToXML( Element* root ) const {}

void PerformConfig::setWorkspace( const std::string& workspace )
{
    onSetWorkspace( workspace );
    mWorkspace = workspace;
}

void PerformConfig::onSetWorkspace( const std::string& workspace )
{
    return;
}

void PerformConfig::setReportDir( const std::string& reportDir )
{
    onSetReportDir( reportDir );
    mReportDir = reportDir;
}

void PerformConfig::onSetReportDir( const std::string& reportDir )
{
    return;
}


} // namespace plan
} // namespace cic
