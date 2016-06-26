//
//  TargetProvider.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 23.06.16.
//
//

#ifndef CICHECK_TargetProvider_hpp
#define CICHECK_TargetProvider_hpp

#include <string>
#include <memory>
#include <Poco/Path.h>
#include <Poco/FileStream.h>
#include <map>
#include <iostream>
#include <vector>

namespace cic {
namespace tgt {

struct TargetDecl;
class Target;

//class Target
//{
//public:
//    friend class TargetProvider;
//
//    Target() noexcept;
//    ~Target() noexcept;
//
//    void load();
//    void unload() noexcept;
//    inline bool isLoaded() const noexcept;
//
//private:
//    Poco::Path      mConfigPath;
//    TargetConfig*   mConfig;
//};

//inline bool Target::isLoaded() const noexcept
//{
//    return ( mConfig != nullptr );
//}


class TargetProvider
{
public:

    TargetProvider() noexcept;
    virtual ~TargetProvider() noexcept;

    void loadDecls( const std::string& declsPath );
    void dropDecls() noexcept;
    const std::vector< TargetDecl >& getDecls() const noexcept;

	bool isDeclared( const std::string& name ) const noexcept;
    
//	const Target& operator [] ( const std::string& name ) const;

protected:

    void declare( const TargetDecl& decl );
	void declare( const std::string& name, const std::string& path );

private:
    std::vector< TargetDecl > mDecls;
};

} // namespace tgt
} // namespace cic

#endif /* CICHECK_TargetProvider_hpp */
