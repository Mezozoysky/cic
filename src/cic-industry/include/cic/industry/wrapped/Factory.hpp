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
/// \brief Provides Factory template
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright industry is released under the terms of zlib/png license

#ifndef INDUSTRY__FACTORY_HPP
#define INDUSTRY__FACTORY_HPP

#include <functional>
#include <unordered_map>

namespace industry
{

/// \brief Marker interface for factory to derive from
class FactoryMarker
{
public:
    /// Default constructor
    FactoryMarker() noexcept = default;
    /// Default copy constructor
    FactoryMarker( const FactoryMarker& ) = default;
    /// Virtual destructor for inheritance
    virtual ~FactoryMarker() noexcept = default;
    /// Default assign operator
    FactoryMarker& operator=( const FactoryMarker& ) = default;
};

/// \brief Factory template
template < typename AbstractionT, typename IdT = std::string >
class Factory : public FactoryMarker
{
public:
    /// Alias type for object creation functions
    using Creator = std::function< AbstractionT*() >;

    using Id = IdT; ///< Alias type for specified id type
    using Abstraction = AbstractionT; ///< Alias type for specified abstraction type

    /// Default constructor
    Factory() = default;
    /// Virtual destructor for possible inheritance
    virtual ~Factory() noexcept = default;

    /// \brief Create an object of specified derived type ReqT and return pointer to it
    template < typename ReqT >
    static ReqT* create();

    /// \brief Register id for specified derived type ReqT with default creator
    template < typename RegT >
    bool registerId( const Id& id ) noexcept;

    /// \brief Register id for specified derived type ReqT with given creator
    template < typename RegT >
    bool registerId( const Id& id, const Creator& creator ) noexcept;

    /// \brief Create an object of type registered with the id and return pointer to it
    AbstractionT* create( const IdT& id ) const;

private:
    std::unordered_map< Id, Creator > mMap; ///< Ids to object creation function map
};


template < typename AbstractionT, typename IdT >
template < typename ReqT >
ReqT* Factory< AbstractionT, IdT >::create()
{
    static_assert( std::is_default_constructible< ReqT >::value,
                   "ReqT should have default constructor" );

    static_assert( std::is_base_of< AbstractionT, ReqT >::value,
                   "ReqT should extend AbstractionT" );

    return ( new ReqT() );
}

template < typename AbstractionT, typename IdT >
template < typename RegT >
bool Factory< AbstractionT, IdT >::registerId( const IdT& id ) noexcept
{
    static_assert( std::is_default_constructible< RegT >::value,
                   "RegT should have default constructor" );

    static_assert( std::is_base_of< AbstractionT, RegT >::value,
                   "RegT should extend AbstractionT" );

    if ( mMap.count( id ) )
    {
        return ( false );
    }

    mMap.insert( std::pair< Id, Creator >( id, std::bind( Factory::create< RegT > ) ) );
    return ( true );
}

template < typename AbstractionT, typename IdT >
template < typename RegT >
bool Factory< AbstractionT, IdT >::registerId(
const IdT& id, const Factory< AbstractionT, IdT >::Creator& creator ) noexcept
{
    static_assert( std::is_default_constructible< RegT >::value,
                   "RegT should be default constructible" );

    static_assert( std::is_base_of< AbstractionT, RegT >::value,
                   "RegT should extend AbstractionT" );

    if ( mMap.count( id ) )
    {
        return ( false );
    }

    mMap.insert( std::pair< Id, Creator >( id, creator ) );
    return ( true );
}

template < typename AbstractionT, typename IdT >
AbstractionT* Factory< AbstractionT, IdT >::create( const IdT& id ) const
{
    AbstractionT* a{ nullptr };

    if ( mMap.count( id ) )
    {
        a = mMap.at( id )();
    }
    return ( a );
}

} // namespace industry

#endif // INDUSTRY__FACTORY_HPP
