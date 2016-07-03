//  cicheck/main.cpp
//	Project:	CICheck
//	Author:		Stanislav Demyanovich <mezozoysky@gmail.com>

#include "Application.hpp"
#include <Poco/Util/Application.h>
#include <Poco/AutoPtr.h>


using namespace cic;

int main( int argc, char** argv )
{
	Application app;
	app.setUnixOptions( true );
	try
	{
		app.init( argc, argv );
	}
	catch ( Poco::Exception& exc )
	{
		app.logger().log( exc );
		return Poco::Util::Application::EXIT_SOFTWARE;
	}
	return ( app.run() );
}
