//
//  Factory.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 30.06.16.
//
//

#ifndef CICHECK_TU__FACTORY_HPP
#define CICHECK_TU__FACTORY_HPP

#include <fmt/format.h>

namespace cic
{
namespace tu
{

template< typename AbstractionT, typename IdT = std::string >
class Factory
{
private:
	using Creator = std::function< AbstractionT*() >;

public:
	using Id = IdT;

	Factory() noexcept = default;
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
	void registerId( const Id& id ) noexcept
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
			fmt::print(
				stderr
				, "[error] Trying to register Id '{}' with factory, which Id is already registered;\n"\
				  "\tregistration ignored;\n"
				, id
			);
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
			fmt::print(
				stderr
				, "[error] Trying to create abstraction by Id '{}', which Id isnt registered with factory;\n"\
				  "\tcreation ignored;\n"
				, id
			);
			return ( a );
		}

		a = mMap.at( id )();
		return ( a );
	}

private:
    std::map< Id, Creator > mMap;
};


} // namespace tu
} // namespace cic

#endif /* CICHECK_TU__FACTORY_HPP */
