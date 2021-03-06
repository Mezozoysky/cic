//  cic
//
//  cic - Copyright (C) 2017 Stanislav Demyanovich <mezozoysky@gmail.com>
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
/// \brief Provides ActFailure class - the act which always plans failed
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright cic is released under the terms of zlib/png license

#ifndef CIC_PLAN__ACT_FAILURE_HPP
#define CIC_PLAN__ACT_FAILURE_HPP

#include "Act.hpp"


namespace cic
{
namespace plan
{


class ActFailure : public Act
{
    CLASSINFO( ActFailure );

public:
    using Ptr = std::shared_ptr< ActFailure >;

    ActFailure() = default;
    virtual ~ActFailure() noexcept = default;

    virtual void loadFromXML( Poco::XML::Element* xml, cic::industry::Industry* industry ) override;
    virtual void saveToXML( Poco::XML::Element* xml ) const override;

protected:
    virtual bool perform( PerformConfig* pc,
                          Report& report,
                          cic::industry::Industry& industry,
                          std::ostream& outStream = std::cout,
                          std::ostream& errStream = std::cerr ) const override;
};


} // namespace plan
} // namespace cic

#endif /* CIC_PLAN__ACT_FAILURE_HPP */
