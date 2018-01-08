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
/// \brief Action implementation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright cic is released under the terms of zlib/png license


#include <cic/plan/Action.hpp>
#include <cic/plan/Report.hpp>
#include <cassert>

using Industry = cic::industry::Industry;

namespace cic
{
namespace plan
{

const std::string Action::formOutline() const noexcept
{
    return ( getClassName() );
}

std::shared_ptr< cic::plan::Report > Action::perform( industry::Industry& industry,
                                                      const std::ostream& outStream,
                                                      const std::ostream& errStream ) const noexcept
{
    Report::Ptr report{ industry.getFactory< Report >()->create( this->getClassName() ) };
    assert( report );
    report->fillWithAction( *this );
    bool success{ perform( *report, industry ) };
    report->setSuccess( success );
    return ( report );
}

bool Action::perform( Report& report,
                      Industry& industry,
                      std::ostream& outStream,
                      std::ostream& errStream ) const
{
    bool success{ true };

    for ( std::size_t i{ 0 }; i < getChildrenSize(); ++i )
    {
        DAGShared child{ getChild( i ) };
        std::shared_ptr< Report > childReport{};
        if ( success )
        {
            childReport = child->perform( industry );
            success = childReport->getSuccess();
        }
        else
        {
            childReport.reset( industry.getFactory< Report >()->create( child->getClassName() ) );
            childReport->fillWithAction( *child );
        }
        report.addChild( childReport );
    }
    return ( success );
};

} // namespace plan
} // namespace cic
