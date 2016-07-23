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

void FailureRule::loadFromXML( const xmlu::Node* root, indu::Industry* industry )
{
	// Do nothing
}


} // namespace cic
