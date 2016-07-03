//
//  ZarFactory.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 02.07.16.
//
//

#ifndef CICHECK_TU__ZAR_FACTORY_HPP
#define CICHECK_TU__ZAR_FACTORY_HPP

#include <CICheck/tu/Factory.hpp>
#include <tktk/typec/TypeMap.hpp>
#include <fmt/format.h>

namespace cic
{
namespace tu
{

class ZarFactory
{
public:
	ZarFactory() noexcept = default;
	ZarFactory( const ZarFactory& ) = delete;
	virtual ~ZarFactory() noexcept = default;
	void operator=( const ZarFactory& ) = delete;

	template< typename AbstractionT >
	std::shared_ptr< Factory< AbstractionT > > createFactory() noexcept
	{
		if ( hasFactory< AbstractionT >() )
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
	std::shared_ptr< Factory< AbstractionT > > getFactory() const noexcept
	{
		if ( !hasFactory< AbstractionT >() )
		{
			return ( nullptr );
		}

		return ( std::static_pointer_cast< Factory< AbstractionT > >( mFactories.at< AbstractionT >() ) );
	}

	template< typename AbstractionT >
	inline bool hasFactory() const noexcept
	{
		return ( mFactories.count< AbstractionT >() );
	}

private:
	tktk::typec::TypeMap< std::shared_ptr< FactoryMarker > > mFactories;
};


} // namespace tu
} // namespace cic

#endif /* CICHECK_TU__ZAR_FACTORY_HPP */
