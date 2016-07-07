//
//  RuleBase.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 06.07.16.
//
//

#include "RuleBase.hpp"

namespace cic
{

const std::string& RuleBase::name() const
{
	return ( mName );
}

void RuleBase::setName( const std::string& name )
{
	mName = name;
}

} // namespace cic
