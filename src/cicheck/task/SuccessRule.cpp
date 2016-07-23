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

void SuccessRule::loadFromXML( const xmlu::Node* root, indu::Industry* industry )
{
	// Do nothing
}


} // namespace cic
