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
/// \brief Provides Plan class, the basic cic plan abstraction
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright cic is released under the terms of zlib/png license


#ifndef CIC_PLAN__PLAN_HPP
#define CIC_PLAN__PLAN_HPP

#include "Action.hpp"
#include "Phase.hpp"
#include <string>
#include <vector>
#include <map>


namespace cic
{
namespace plan
{

class Plan : public Action
{
    CLASSINFO( Plan )

public:
    using Ptr = std::shared_ptr< Plan >;

    using Sequence = std::vector< std::size_t >;
    using IndexMap = std::map< std::string, std::size_t >;

public:
    Plan() = default;
    virtual ~Plan() noexcept = default;

    virtual bool perform( Report& report, cic::industry::Industry& industry ) const override;

    virtual void buildSequence( Sequence& seq ) const = 0;

    virtual void loadFromXML( Poco::XML::Element* root, cic::industry::Industry* industry ) override;
    virtual void saveToXML( Poco::XML::Element* root ) const override;

    std::size_t getPhaseIndex( const std::string& name ) const noexcept;
    Phase::Ptr getPhase( const std::string& name ) const noexcept;

    inline const std::vector< std::string >& getTargetPhases() const noexcept;
    void setTargetPhases( const std::vector< std::string >& phaseList );

protected:
    virtual void onSetTargetPhases( const std::vector< std::string >& phaseList );

    virtual void onAddChild( const DAGShared& child, std::size_t index ) override;

    virtual void loadPhasesFromXML( Poco::XML::Element* root, cic::industry::Industry* industry );
    virtual void loadPhaseFromXML( Poco::XML::Element* root, cic::industry::Industry* industry );

private:
    IndexMap mIndexMap;
    std::vector<std::string> mTargetPhases;
};

inline const std::vector< std::string >& Plan::getTargetPhases() const noexcept
{
    return ( mTargetPhases );
}

} // namespace plan
} // namespace cic

#endif // CIC_PLAN__PLAN_HPP
