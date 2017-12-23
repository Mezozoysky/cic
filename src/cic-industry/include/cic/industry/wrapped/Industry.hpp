//  industry
//
//  industry - Copyright (C) 2017 Stanislav Demyanovich <mezozoysky@gmail.com>
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
/// \brief Provides Industry type
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright industry is released under the terms of zlib/png license

#ifndef INDUSTRY__INDUSTRY_HPP
#define INDUSTRY__INDUSTRY_HPP

#include "Factory.hpp"
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace industry
{

/// \brief Centralized storage for factories
template < typename IdT = std::string >
class Industry
{
private:
    using MarkerPtr = std::shared_ptr< FactoryMarker >;
    using TypeMap = std::unordered_map< std::type_index, MarkerPtr >;
    using Id = IdT;

public:
    /// Default constructor
    Industry() = default;
    ///< Virtual destructor for possible inheritance
    virtual ~Industry() noexcept = default;
    Industry( const Industry& ) = delete;
    Industry& operator=( const Industry& ) = delete;

    /// Create and register factory for specified abstraction type
    template < typename AbstractionT >
    std::shared_ptr< Factory< AbstractionT, IdT > > registerFactory() noexcept;

    /// Register given factory for specified abstraction type
    template < typename AbstractionT >
    bool registerFactory(
    const std::shared_ptr< Factory< AbstractionT, IdT > >& factory ) noexcept;

    /// Get registered factory for specified abstraction type
    template < typename AbstractionT >
    std::shared_ptr< Factory< AbstractionT, IdT > > getFactory() const noexcept;

    /// Check if factory is registered for specified abstraction type
    template < typename AbstractionT >
    bool hasFactory() const noexcept;

    /// Register given factory for given type index
    bool registerFactory( const std::type_index& index,
                          const MarkerPtr& factory ) noexcept;

    /// Get register factory for given index type
    MarkerPtr getFactory( const std::type_index& index ) const noexcept;

    /// Check if factory is registered for given type index
    inline bool hasFactory( const std::type_index& index ) const noexcept;

private:
    TypeMap mFactories; ///< Abstraction type index to factory map
};


template < typename IdT >
template < typename AbstractionT >
std::shared_ptr< Factory< AbstractionT, IdT > >
Industry< IdT >::registerFactory() noexcept
{
    std::type_index index{ typeid( AbstractionT ) };
    auto factory( std::make_shared< Factory< AbstractionT, IdT > >() );

    if ( registerFactory( index, factory ) )
    {
        return ( factory );
    }

    return ( nullptr );
}

template < typename IdT >
template < typename AbstractionT >
bool Industry< IdT >::registerFactory(
const std::shared_ptr< Factory< AbstractionT, IdT > >& factory ) noexcept
{
    std::type_index index{ typeid( AbstractionT ) };
    return ( registerFactory( index, factory ) );
}

template < typename IdT >
template < typename AbstractionT >
std::shared_ptr< Factory< AbstractionT, IdT > > Industry< IdT >::getFactory() const
noexcept
{
    std::type_index index{ typeid( AbstractionT ) };
    return (
    std::static_pointer_cast< Factory< AbstractionT, IdT > >( getFactory( index ) ) );
}

template < typename IdT >
template < typename AbstractionT >
bool Industry< IdT >::hasFactory() const noexcept
{
    std::type_index index{ typeid( AbstractionT ) };
    return ( hasFactory( index ) );
}

template < typename IdT >
bool Industry< IdT >::registerFactory( const std::type_index& index,
                                       const MarkerPtr& factory ) noexcept
{
    if ( factory == nullptr )
    {
        return ( false );
    }
    if ( hasFactory( index ) )
    {
        return ( false );
    }

    mFactories[ index ] = factory;
    return ( true );
}

template < typename IdT >
typename Industry< IdT >::MarkerPtr
Industry< IdT >::getFactory( const std::type_index& index ) const noexcept
{
    if ( !( hasFactory( index ) ) )
    {
        return ( nullptr );
    }

    return ( mFactories.at( index ) );
}

template < typename IdT >
inline bool Industry< IdT >::hasFactory( const std::type_index& index ) const noexcept
{
    return ( mFactories.count( index ) );
}


} // namespace industry

#endif // INDUSTRY__INDUSTRY_HPP
