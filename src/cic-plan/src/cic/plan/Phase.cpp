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
/// \brief plan::Phase class implementation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright cic is released under the terms of zlib/png license


#include <cic/plan/Phase.hpp>
#include <cic/plan/Action.hpp>
#include <Poco/Exception.h>
#include <Poco/String.h>
#include <Poco/DOM/Node.h>
#include <fmt/format.h>
#include <cic/plan/Report.hpp>
#include <fstream>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/NodeList.h>

using Poco::XML::Node;
using Poco::XML::NodeList;
using namespace fmt::literals;
using Poco::AutoPtr;
using Poco::XML::Element;
using cic::plan::PhaseReport;
using cic::plan::Report;
using cic::industry::Industry;

namespace cic
{
namespace plan
{

bool Phase::execute( PhaseReport* report )
{
    bool result{ true };

    for ( std::size_t i{ 0 }; i < actions().size(); ++i )
    {
        Action::Ptr action{ actions()[ i ] };
        report->actionReports().emplace_back();
        auto& actionReport( report->actionReports().back() );
        actionReport.classId() = action->getClassName();
        actionReport.outline() = action->outline();
        std::string stdoutLogName{ "action_{}_{}_stdout.log"_format( report->name(), i ) };
        std::string stderrLogName{ "action_{}_{}_stderr.log"_format( report->name(), i ) };
        actionReport.stdoutFileName() = stdoutLogName;
        actionReport.stderrFileName() = stderrLogName;
        std::ofstream stdoutLog{ stdoutLogName };
        std::ofstream stderrLog{ stderrLogName };
        if ( result )
        {
            result = action->execute( stdoutLog, stderrLog );
        }
        stdoutLog.flush();
        stdoutLog.close();
        stderrLog.flush();
        stderrLog.close();

        actionReport.success() = result;
    }
    return ( result );
};

void Phase::loadFromXML( Element* root, Industry* industry )
{
    assert( root != nullptr );

    // load dependencies
    {
        Element* elem{ root->getChildElement( "dependencies" ) };
        if ( elem != nullptr )
        {
            AutoPtr< NodeList > depNodeList{ elem->childNodes() };
            for ( std::size_t i{ 0 }; i < depNodeList->length(); ++i )
            {
                Element* depElem{ static_cast< Element* >( depNodeList->item( i ) ) };
                if ( depElem != nullptr && depElem->nodeName() == "dependency" )
                {
                    std::string depName{ Poco::trim( depElem->getAttribute( "value" ) ) };
                    if ( !depName.empty() )
                    {
                        deps().push_back( depName );
                    }
                }
            }
        }
    }

    // load actions
    {
        Element* elem{ root->getChildElement( "actions" ) };
        if ( elem != nullptr )
        {
            loadActionsFromXML( elem, industry );
        }
    }
}

void Phase::saveToXML( Element* root ) const {}

void Phase::loadActionsFromXML( Element* root, Industry* industry )
{
    AutoPtr< NodeList > list{ root->childNodes() };
    Element* elem{ nullptr };
    for ( std::size_t i{ 0 }; i < list->length(); ++i )
    {
        elem = static_cast< Element* >( list->item( i ) );
        if ( elem != nullptr && elem->nodeName() == "action" )
        {
            loadActionFromXML( elem, industry );
        }
    }
}

void Phase::loadActionFromXML( Element* root, Industry* industry )
{
    std::string classId{ root->getAttribute( "class" ) };
    if ( classId.empty() )
    {
        throw( Poco::SyntaxException{
            "Mandatory attribute 'class' isnt found or empty within the 'action' element", 8 } );
    }

    auto actionFactory( industry->getFactory< Action >() );
    if ( actionFactory == nullptr )
    {
        throw( Poco::NotFoundException{ "No factory registered for id: '{}'"_format( classId ), 8 } );
    }

    Action::Ptr action{ actionFactory->create( classId ) };
    action->loadFromXML( root, industry );
    mActions.push_back( action );
}

} // namespace plan
} // namespace cic
