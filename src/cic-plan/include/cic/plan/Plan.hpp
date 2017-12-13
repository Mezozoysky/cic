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

#include "Serializable.hpp"
#include "Phase.hpp"
#include <string>
// #include <vector>
#include <map>

namespace cic
{
namespace plan
{

class Plan : Serializable
{
public:
    using Ptr = std::shared_ptr< Plan >;

    using Sequence = std::vector< std::string >;
    using PhaseMap = std::map< std::string, Phase::Ptr >;

public:
    Plan() = default;
    Plan( const Plan& other ) = delete;
    Plan& operator=( const Plan& other ) = delete;
    virtual ~Plan() noexcept = default;

    virtual bool execute( const std::string& phaseName );
    virtual void buildSequence( const std::string& phaseName, Sequence& seq ) const;
    virtual bool isADependsOnB( const std::string& tgtA, const std::string& tgtB ) const;

    virtual void loadFromXML( Poco::XML::Node* xml, Industry* industry ) override;
    virtual void saveToXML( Poco::XML::Node* xml ) const override;

public:
    inline const std::string& name() const noexcept;
    inline const std::string& defaultPhase() const noexcept;
    inline const PhaseMap& targets() const noexcept;

protected:
    inline std::string& name() noexcept;
    inline std::string& defaultPhase() noexcept;
    inline PhaseMap& targets() noexcept;

private:
    std::string mName;
    std::string mDefaultPhase;
    PhaseMap mPhases;
};


inline const std::string& Plan::name() const noexcept
{
    return ( mName );
}
inline std::string& Plan::name() noexcept
{
    return ( mName );
}

inline const std::string& Plan::defaultPhase() const noexcept
{
    return ( mDefaultPhase );
}
inline std::string& Plan::defaultPhase() noexcept
{
    return ( mDefaultPhase );
}

inline const Plan::PhaseMap& Plan::targets() const noexcept
{
    return ( mPhases );
}
inline Plan::PhaseMap& Plan::targets() noexcept
{
    return ( mPhases );
}

} // namespace plan
} // namespace cic

#endif // CIC_PLAN__PLAN_HPP
