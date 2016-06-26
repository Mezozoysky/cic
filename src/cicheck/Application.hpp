//  CICheck/Application.hpp
//	Project:	CICheck
//	Author:		Stanislav Demyanovich <mezozoysky@gmail.com>

#ifndef CICHECK_APPLICATION_H
#define CICHECK_APPLICATION_H

#include <Poco/Util/Application.h>
#include <memory>
#include <CICheck/tgt/TargetProvider.hpp>

namespace cic
{

class Application
: public Poco::Util::Application
{
public:
	Application();
	virtual ~Application();

	void helpOptionCallback( const std::string& name, const std::string& value );

protected:
	virtual void initialize( Poco::Util::Application& self ) override;
	virtual void uninitialize() override;
	virtual void defineOptions( Poco::Util::OptionSet& options ) override;
	int main( const std::vector< std::string >& args ) override;

    std::string formatHelpText() const;

private:
	bool mIsHelpOptionRequested;

    tgt::TargetProvider* mTgtProv;
};

} // namespace cic

#endif // CICHECK_APPLICATION_H
