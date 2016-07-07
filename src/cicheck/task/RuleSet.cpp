//
//  RuleSet.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 07.07.16.
//
//

#include "RuleSet.hpp"

using namespace cic::task;
using namespace cic::xmlu;

namespace cic
{


bool RuleSet::check()
{
	bool result{ true };
	for ( const auto& rule: mRules )
	{
		if ( !rule->check() )
		{
			result = false;
			break;
		}
	}
	return ( result );
}

const std::string& RuleSet::name() const
{
	return ( mName );
}

void RuleSet::setName( const std::string& name ) noexcept
{
	mName = name;
}

const std::vector< ARule::Ptr >& RuleSet::rules() const
{
	return ( mRules );
}


} // namespace cic