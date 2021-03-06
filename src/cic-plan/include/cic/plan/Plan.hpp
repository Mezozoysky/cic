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

#include "Act.hpp"
#include "Phase.hpp"
#include "Target.hpp"
#include <string>
#include <vector>
#include <map>


namespace cic
{
namespace plan
{

class Plan : public Act
{
    CLASSINFO( Plan )

public:
    using Ptr = std::shared_ptr< Plan >;

    using Sequence = std::vector< std::size_t >;
    using IndexMap = std::map< std::string, std::size_t >;

public:
    Plan();
    virtual ~Plan() noexcept = default;

    virtual void buildSequence( Sequence& seq, const Target* target ) const = 0;

    virtual void loadFromXML( Poco::XML::Element* root, cic::industry::Industry* industry ) override;
    virtual void saveToXML( Poco::XML::Element* root ) const override;

protected:
    virtual bool perform( PerformConfig* pc,
                          Report& report,
                          cic::industry::Industry& industry,
                          std::ostream& outStream,
                          std::ostream& errStream ) const override;

public:
    std::size_t getPhaseIndex( const std::string& name ) const noexcept;
    Phase::Ptr getPhase( const std::string& name ) const noexcept;

    inline const std::string& getDefaultPhaseClass() const noexcept;
    void setDefaultPhaseClass( const std::string& classId );

protected:
    virtual void onSetDefaultPhaseClass( const std::string& classId );

protected:
    virtual void onAddChild( const DAGShared& child, std::size_t index ) override;

    virtual void loadPhasesFromXML( Poco::XML::Element* root, cic::industry::Industry* industry );
    virtual void loadPhaseFromXML( Poco::XML::Element* root, cic::industry::Industry* industry );

private:
    IndexMap mIndexMap;
    std::string mDefaultPhaseClass;
};

inline const std::string& Plan::getDefaultPhaseClass() const noexcept
{
    return ( mDefaultPhaseClass );
}


} // namespace plan
} // namespace cic

#endif // CIC_PLAN__PLAN_HPP
