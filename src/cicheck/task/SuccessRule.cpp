//
//  SuccessRule.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 06.07.16.
//
//

#include "SuccessRule.hpp"

namespace cic
{


bool SuccessRule::check()
{
	return ( true );
}

void SuccessRule::loadFromXml( const xmlu::Node* root, tu::FactoryOwner* factories )
{
	// Do nothing
}


} // namespace cic
