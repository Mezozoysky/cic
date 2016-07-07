//
//  FailureRule.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 07.07.16.
//
//

#include "FailureRule.hpp"

namespace cic
{


bool FailureRule::check()
{
	return ( false );
}

void FailureRule::loadFromXml( const xmlu::Node* root, tu::FactoryOwner* factories )
{
	// Do nothing
}


} // namespace cic
