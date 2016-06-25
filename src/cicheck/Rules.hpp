//
//  Rules.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 23.06.16.
//
//

#ifndef CICHECK_RULES_HPP
#define CICHECK_RULES_HPP

#include <array>
#include <string>

namespace cic
{

struct Rules
{
	Rules() = delete;
    ~Rules() = delete;

    static const std::array< std::string, 5> names;

    static std::size_t index( const std::string& name ) noexcept;
    static std::size_t count() noexcept;
    static std::size_t badIndex() noexcept;
};

} // namespace cic

#endif /* CICHECK_RULES_HPP */
