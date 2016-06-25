//
//  Target.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 25.06.16.
//
//

#ifndef CICHECK_TGT_TARGET_HPP
#define CICHECK_TGT_TARGET_HPP

#include <string>

namespace cic {
namespace tgt {

struct TargetDecl
{
    std::string name;
    std::string path;

    TargetDecl( const std::string& name, const std::string& path ) noexcept;
};

class Target
{
    Target() = delete;
    Target( const Target& ) = delete;
    void operator = ( const Target& ) = delete;

public:
    explicit Target( const std::string& name, const std::string& path ) noexcept;
    explicit Target( const TargetDecl& decl ) noexcept;
    virtual ~Target() noexcept;

    inline const TargetDecl& getDecl() const noexcept;

private:
    TargetDecl mDecl;
};


//
// Inliners
//

inline const TargetDecl& Target::getDecl() const noexcept
{
    return ( mDecl );
}


} // namespace tgt
} // namespace cic

#endif /* CICHECK_TGT_TARGET_HPP */
