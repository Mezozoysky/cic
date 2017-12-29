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
/// \brief Action implementation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright cic is released under the terms of zlib/png license


#include <cic/plan/Action.hpp>
#include <cic/plan/Report.hpp>

using Industry = cic::industry::Industry;

namespace cic
{
namespace plan
{

const std::string Action::formOutline() const noexcept
{
    return ( getClassName() );
}

std::shared_ptr< cic::plan::Report > Action::perform( industry::Industry& industry ) const noexcept
{
    Report::Ptr report{ industry.getFactory< Report >()->create( this->getClassName() ) };
    // assert( report );
    report->fillWithAction( *this );
    bool success{ perform( *report, industry ) };
    report->setSuccess( success );
    return ( report );
}

} // namespace plan
} // namespace cic
