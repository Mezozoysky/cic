//  cicheck/Application.hpp
//	Project:	CICheck
//	Author:		Stanislav Demyanovich <mezozoysky@gmail.com>

#ifndef CICHECK_CICHECK__APPLICATION_H
#define CICHECK_CICHECK__APPLICATION_H

#include <Poco/Util/Application.h>
#include <memory>
#include <CICheck/task/BashScriptRule.hpp>
#include <CICheck/task/AbstractRule.hpp>
#include <CICheck/tu/Factory.hpp>
#include "TaskProvider.hpp"

namespace cic
{

class Application
: public Poco::Util::Application
{
public:
	Application();
	virtual ~Application();

	void helpOptionCallback( const std::string& name, const std::string& value );
	void colorsOptionCallback( const std::string& name, const std::string& value );

protected:
	virtual void initialize( Poco::Util::Application& self ) override;
	virtual void uninitialize() override;
	virtual void defineOptions( Poco::Util::OptionSet& options ) override;
	int main( const std::vector< std::string >& args ) override;

    std::string formatHelpText() const;

private:
	bool mIsHelpOptionRequested;
	bool mIsColorsOptionRequested;

    TaskProvider mTaskProv;
    tu::Factory< task::AbstractRule > mRuleFactory;
};

} // namespace cic

#endif // CICHECK_CICHECK__APPLICATION_H
