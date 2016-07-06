//
//  FactoryOwner.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 02.07.16.
//
//

#ifndef CICHECK_TU__FACTORY_OWNER_HPP
#define CICHECK_TU__FACTORY_OWNER_HPP

#include <CICheck/tu/Factory.hpp>
#include <tktk/typec/TypeMap.hpp>
#include <fmt/format.h>

namespace cic
{
namespace tu
{

class FactoryOwner
{
public:
	FactoryOwner() noexcept = default;
	FactoryOwner( const FactoryOwner& ) = delete;
	virtual ~FactoryOwner() noexcept = default;
	void operator=( const FactoryOwner& ) = delete;

	template< typename AbstractionT >
	std::shared_ptr< Factory< AbstractionT > > create() noexcept
	{
		if ( has< AbstractionT >() )
		{
			fmt::print(
				stderr
				, "[error] Trying to register factory for abstraction type '{}'"\
				  ", which type is already registered;\n"\
				  "\tregistration ignored;\n"
				, typeid( AbstractionT ).name()
			);
			return ( nullptr );
		}

		auto factory{ std::make_shared< Factory< AbstractionT > >() };

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


} // namespace tu
} // namespace cic

#endif /* CICHECK_TU__ZAR_FACTORY_HPP */
