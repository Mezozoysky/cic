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
/// \brief Provides Industry type
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright CICheck is released under the terms of zlib/png license

#ifndef CIC_INDUSTRY__INDUSTRY_HPP
#define CIC_INDUSTRY__INDUSTRY_HPP

#include <CICheck/industry/Factory.hpp>
#include <tktk/typec/TypeMap.hpp>
#include <stdexcept>

namespace cic
{
namespace indu
{

class Industry
{
public:
	Industry() = default;
	Industry( const Industry& ) = delete;
	virtual ~Industry() noexcept = default;
	void operator=( const Industry& ) = delete;

	template< typename AbstractionT >
	std::shared_ptr< Factory< AbstractionT > > create()
	{
		if ( has< AbstractionT >() )
		{
			throw ( std::logic_error( "Factory for given AbstractionT is already registered" ) );
		}

		auto factory( std::make_shared< Factory< AbstractionT > >() );

		if ( factory != nullptr )
		{
			mFactories.insert< AbstractionT >( factory );
		}

		return ( factory );
	}

	template< typename AbstractionT >
	std::shared_ptr< Factory< AbstractionT > > get() const noexcept
	{
		if ( !( has< AbstractionT >() ) )
		{
			return ( nullptr );
		}

		return ( std::static_pointer_cast< Factory< AbstractionT > >( mFactories.at< AbstractionT >() ) );
	}

	template< typename AbstractionT >
	inline bool has() const noexcept
	{
		return ( mFactories.count< AbstractionT >() );
	}

private:
	tktk::typec::TypeMap< std::shared_ptr< FactoryMarker > > mFactories;
};


} // namespace indu
} // namespace cic

#endif /* CIC_INDUSTRY__INDUSTRY_HPP */
