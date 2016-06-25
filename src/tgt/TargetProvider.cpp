//
//  TargetProvider.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 23.06.16.
//
//

#include <CICheck/tgt/TargetProvider.hpp>
#include <CICheck/tgt/Target.hpp>
#include <Poco/SAX/SAXParser.h>
#include <Poco/SAX/ContentHandler.h>
#include <sstream>

namespace cic {
namespace tgt {

class DeclReader
: public Poco::XML::ContentHandler
{
public:
    DeclReader()
    : mLocator{ nullptr }
    {
    }

    virtual void setDocumentLocator( const Poco::XML::Locator* locator ) override
    {
        mLocator = locator;
    }

    virtual void startDocument() override
    {
        std::cout << "START DOCUMENT" << std::endl;
		std::cout << getLocation()    << std::endl;
    }

    virtual void endDocument() override
    {
        std::cout << "END DOCUMENT" << std::endl;
		std::cout << getLocation()  << std::endl;
    }

	virtual void startElement(
					  const Poco::XML::XMLString& uri
					  , const Poco::XML::XMLString& localName
					  , const Poco::XML::XMLString& qname
					  , const Poco::XML::Attributes& attrList
	) override
    {
		std::cout << "START ELEMENT \"" << localName << "\"" << std::endl;
		std::cout << getLocation()      << std::endl;
    }

	virtual void endElement(
					const Poco::XML::XMLString& uri
					, const Poco::XML::XMLString& localName
					, const Poco::XML::XMLString& qname
	) override
	{
		std::cout << "END ELEMENT \"" << localName << "\"" << std::endl;
		std::cout << getLocation()    << std::endl;
	}

	virtual void characters( const Poco::XML::XMLChar ch[], int start, int length ) override
	{
		std::cout << "CHARACTERS \"" << std::string( ch + start, length ) << "\"" << std::endl;
		std::cout << getLocation()   << std::endl;
	}

	virtual void ignorableWhitespace( const Poco::XML::XMLChar ch[], int start, int length ) override
	{
		std::cout << "IGNORABLE WHITESPACE \"" << std::string( ch + start, length ) << "\"" << std::endl;
		std::cout << getLocation()             << std::endl;
	}

	virtual void processingInstruction( const Poco::XML::XMLString& target, const Poco::XML::XMLString& data ) override
	{
		std::cout << "PROCESSING INSTRUCTION" << std::endl;
		std::cout << getLocation()            << std::endl;
	}

	virtual void startPrefixMapping( const Poco::XML::XMLString& prefix, const Poco::XML::XMLString& uri ) override
	{
		std::cout << "START PREFIX MAPPING \"" << prefix << "\"" << std::endl;
		std::cout << getLocation()             << std::endl;
	}

	virtual void endPrefixMapping( const Poco::XML::XMLString& prefix ) override
	{
		std::cout << "END PREFIX MAPPING \"" << prefix << "\"" << std::endl;
		std::cout << getLocation()           << std::endl;
	}

	virtual void skippedEntity(const Poco::XML::XMLString& name) override
	{
		std::cout << "SKIPPED ENTIY \"" << name << "\"" << std::endl;
		std::cout << getLocation()      << std::endl;
	}


protected:
	std::string getLocation()
	{
		std::ostringstream ostr;
		if ( mLocator )
		{
			ostr << mLocator->getSystemId()
			     << ", line: " << mLocator->getLineNumber()
			     << ", col: "  << mLocator->getColumnNumber();
		}
		else
		{
			ostr << "location unknown";
		}
		return ( ostr.str() );
	}

private:
    const Poco::XML::Locator* mLocator;
};

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

	DeclReader declReader;
	Poco::XML::SAXParser parser;
	parser.setContentHandler( &declReader );

	for ( std::size_t i{ 0 }; i < mDeclPaths.size(); ++i )
    {
        Poco::Path path{ mDeclPaths[ i ] };
        std::cout << "targets file: " << path.toString() << std::endl;
        //            Poco::FileInputStream istr{ tgtsFilePath.toString() };

        try
        {
            parser.parse( path.toString() );
        } catch( Poco::Exception& e )
        {
            std::cerr << "[error] " << e.displayText() << std::endl;
        }
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
