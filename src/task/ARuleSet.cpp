//
//  RuleSet.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 29.06.16.
//
//

#include <CICheck/task/ARuleSet.hpp>

namespace cic
{
namespace task
{


bool ARuleSet::check()
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

const std::string& ARuleSet::name() const noexcept
{
	return ( mName );
}

std::size_t ARuleSet::getSize() const noexcept
{
	return ( mRules.size() );
}

const std::vector< ARule::Ptr >& ARuleSet::rules() const noexcept
{
	return ( mRules );
}


} // namespace task
} // namespace cic
