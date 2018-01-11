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
/// \brief Provides DepsTreePlan class, the basic cic plan abstraction
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2018
/// \copyright cic is released under the terms of zlib/png license


#ifndef CIC_PLAN__DEPS_TREE_PLAN_HPP
#define CIC_PLAN__DEPS_TREE_PLAN_HPP

#include "Plan.hpp"


namespace cic
{
namespace plan
{


class DepsTreePhase;

class DepsTreePlan : public Plan
{
    CLASSINFO( DepsTreePlan )

public:
    using Ptr = std::shared_ptr< DepsTreePlan >;

    DepsTreePlan();
    virtual ~DepsTreePlan() noexcept = default;

    virtual void buildSequence( Sequence& seq, const Target* target ) const override;
    virtual bool isADependsOnB( std::size_t indexA, std::size_t indexB ) const;
    virtual bool isADependsOnB( const std::string& nameA, const std::string& nameB ) const;

    virtual void loadFromXML( Poco::XML::Element* root, cic::industry::Industry* industry ) override;
    virtual void saveToXML( Poco::XML::Element* root ) const override;

protected:
    virtual void buildSubseq( Sequence& subseq, const std::shared_ptr< DepsTreePhase >& phase ) const;

    virtual void loadPhaseFromXML( Poco::XML::Element* root, cic::industry::Industry* industry ) override;

};


} // namespace plan
} // namespace cic

#endif // CIC_PLAN__DEPS_TREE_PLAN_HPP
