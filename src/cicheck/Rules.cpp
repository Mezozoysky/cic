//
//  Rules.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 23.06.16.
//
//

#include "Rules.hpp"

namespace cic
{

const std::array<std::string, 5 > Rules::names{
	"clean"
    , "configure"
    , "build"
    , "buildtests"
    , "runtests"
};

std::size_t Rules::index( const std::string& name ) noexcept
{
    std::size_t idx{ badIndex() };

    for ( int i{ 0 }; i < names.size(); ++i )
    {
        if ( name == names[ i ] )
        {
            idx = i;
            break;
        }
    }

    return ( idx );
}

std::size_t Rules::count() noexcept
{
    return ( names.size() );
}

std::size_t Rules::badIndex() noexcept
{
    return ( names.size() );
}


} // namespace cic
