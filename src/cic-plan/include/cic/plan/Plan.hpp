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
/// \brief Provides Plan class, the basic cic plan abstraction
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright cic is released under the terms of zlib/png license


#ifndef CIC_PLAN__PLAN_HPP
#define CIC_PLAN__PLAN_HPP

#include "Action.hpp"
#include "Phase.hpp"
#include <string>
#include <vector>


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
    using PhaseList = std::vector< Phase::Ptr >;

public:
    Plan() = default;
    Plan( const Plan& other ) = delete;
    Plan& operator=( const Plan& other ) = delete;
    virtual ~Plan() noexcept = default;

    virtual bool perform( Report& report, cic::industry::Industry& industry ) const override;
    // virtual bool perform( const std::string& phaseName, TargetReport* report, bool skipDependencies = false
    // );
    virtual void buildSequence( const std::string& phaseName, Sequence& seq ) const;

    virtual void loadFromXML( Poco::XML::Element* root, cic::industry::Industry* industry ) override;
    virtual void saveToXML( Poco::XML::Element* root ) const override;

    inline std::size_t getPhasesCount() const noexcept;
    inline const PhaseList& getPhases() const noexcept;
    inline Phase::Ptr getPhase( std::size_t index ) const noexcept;
    Phase::Ptr getPhase( const std::string& name ) const noexcept;
    inline void addPhase( const Phase::Ptr& phase ) noexcept;
    inline void addPhases( const PhaseList& phases ) noexcept;


protected:
    virtual void loadPhasesFromXML( Poco::XML::Element* root, cic::industry::Industry* industry );
    virtual void loadPhaseFromXML( Poco::XML::Element* root, cic::industry::Industry* industry );

private:
    PhaseList mPhases;
};


// Inliners

inline std::size_t Plan::getPhasesCount() const noexcept
{
    return ( mPhases.size() );
}

inline const Plan::PhaseList& Plan::getPhases() const noexcept
{
    return ( mPhases );
}

inline Phase::Ptr Plan::getPhase( std::size_t index ) const noexcept
{
    Phase::Ptr phase{};
    if ( index < getPhasesCount() )
    {
        phase = mPhases[ index ];
    }
    return ( phase );
}

inline void Plan::addPhase( const Phase::Ptr& phase ) noexcept
{
    mPhases.push_back( phase );
}

inline void Plan::addPhases( const Plan::PhaseList& phases ) noexcept
{
    for ( const auto& phase : phases )
    {
        mPhases.push_back( phase );
    }
}

} // namespace plan
} // namespace cic

#endif // CIC_PLAN__PLAN_HPP
