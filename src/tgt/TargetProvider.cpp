//
//  TargetProvider.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 23.06.16.
//
//

#include <CICheck/tgt/TargetProvider.hpp>
#include <CICheck/tgt/Target.hpp>
//#include <Poco/SAX/SAXParser.h>
//#include <Poco/SAX/ContentHandler.h>
#include <iostream>
#include <sstream>
#include <fmt/format.h>

using namespace fmt::literals;

namespace cic {
namespace tgt {

//Target::Target() noexcept
//: mConfig{ nullptr }
//{
//}
//
//Target::~Target() noexcept
//{
//    unload();
//}
//
//void Target::load()
//{
//    Poco::FileInputStream istr( mConfigPath.toString() );
//    unload();
//    mConfig = new TargetConfig();
//}
//
//void Target::unload() noexcept
//{
//    if ( mConfig != nullptr )
//    {
//        delete mConfig;
//        mConfig = nullptr;
//    }
//}


TargetProvider::TargetProvider() noexcept
{
}

TargetProvider::~TargetProvider() noexcept
{
}

void TargetProvider::addDeclPath( const std::string& path ) noexcept
{
    mDeclPaths.push_back( path );
}

void TargetProvider::dropDeclPaths() noexcept
{
    mDeclPaths.clear();
}

const std::vector< std::string >& TargetProvider::getDeclPaths() const noexcept
{
    return ( mDeclPaths );
}

void TargetProvider::reloadDecls()
{
    dropDecls();

    //TODO: load all the declarations avilable by given paths

	for ( std::size_t i{ 0 }; i < mDeclPaths.size(); ++i )
    {
        Poco::Path path{ mDeclPaths[ i ] };
        std::cout << "targets file: " << path.toString() << std::endl;
        //            Poco::FileInputStream istr{ tgtsFilePath.toString() };

    }
}

void TargetProvider::dropDecls() noexcept
{
    mDecls.clear();
}

const std::vector< TargetDecl >& TargetProvider::getDecls() const noexcept
{
    return ( mDecls );
}

//    void create( const std::string& name ) noexcept
//    {
//        Poco::Path binaryDirPath( true );
//        binaryDirPath.assign( "/Users/mezozoy/ws/CICheck/build/bin/Debug", Poco::Path::Style::PATH_UNIX );
//        Poco::Path tgtPath( false );
//        tgtPath.assign( "../share/CICheck", Poco::Path::Style::PATH_UNIX );
//        tgtPath.makeAbsolute( binaryDirPath );
//        std::unique_ptr< Target > tgt{ new Target() };
//        mTargets.insert( std::make_pair( name, std::move( tgt ) ) );
//    }

bool TargetProvider::isDeclared( const std::string& name ) const noexcept
{
    auto it(
        std::find_if(
            mDecls.begin(),
            mDecls.end(),
            [ name ]( const TargetDecl& decl )
            {
                return ( decl.name == name );
            }
        )
    );
//    auto it( mDecls.find( name ) );
    return ( it != mDecls.end() );
}

//const Target& TargetProvider::operator [] ( const std::string& name ) const
//{
//    return ( mTargets.at( name ) );
//}

void TargetProvider::declare( const cic::tgt::TargetDecl &decl )
{
    mDecls.push_back( decl );
}

void TargetProvider::declare( const std::string &name, const std::string &path )
{
    declare( { name, path } );
}

} // namespace tgt
} // namespace cic
