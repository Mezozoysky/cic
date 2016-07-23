//  CICheck
//
//  CICheck - Copyright (C) 2016 Stanislav Demyanovich <mezozoysky@gmail.com>
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
/// \date 2016
/// \copyright CICheck is released under the terms of zlib/png license

#ifndef CIC_INDUSTRY__FACTORY_HPP
#define CIC_INDUSTRY__FACTORY_HPP

#include <functional>
#include <map>
#include <stdexcept>

namespace cic
{
namespace indu
{

class FactoryMarker
{
public:
	FactoryMarker() noexcept = default;
	FactoryMarker( const FactoryMarker& ) = delete;
	virtual ~FactoryMarker() noexcept = default;
	void operator=( const FactoryMarker& ) = delete;
};

template< typename AbstractionT, typename IdT = std::string >
class Factory
: public FactoryMarker
{
private:
	using Creator = std::function< AbstractionT*() >;

public:
	using Id = IdT;

	Factory() = default;
	Factory( const Factory& ) = delete;
	virtual ~Factory() noexcept = default;
	void operator=( const Factory& ) = delete;

	template< typename ReqT >
	static ReqT* create() noexcept
	{
		static_assert(
			std::is_default_constructible< ReqT >::value
			, "ReqT should have default constructor"
		);

		static_assert(
			std::is_base_of< AbstractionT, ReqT >::value
			, "ReqT should extend AbstractionT"
		);

		return ( new ReqT() );
    }

	template< typename RegT >
	void registerId( const Id& id )
	{
		static_assert(
			std::is_default_constructible< RegT >::value
			, "RegT should have default constructor"
		);

		static_assert(
			std::is_base_of< AbstractionT, RegT >::value
			, "RegT should extend AbstractionT"
		);

		//TODO: some template for testing various types on nullness is needed

        if ( mMap.count( id ) )
        {
			throw ( std::invalid_argument( "Given id is already registered" ) );
			return;
        }

		mMap.insert(
			std::pair< Id, Creator >( id, std::bind( Factory::create< RegT > ) )
		);
    }

	AbstractionT* create( const Id& id ) noexcept
	{
		AbstractionT* a{ nullptr };

		if ( !mMap.count( id ) )
		{
			throw ( std::out_of_range( "Given id isnt registered" ) );
		}

		a = mMap.at( id )();
		return ( a );
	}

private:
    std::map< Id, Creator > mMap;
};


} // namespace indu
} // namespace cic

#endif /* CIC_INDUSTRY__FACTORY_HPP */
