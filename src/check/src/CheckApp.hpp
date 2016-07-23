//  CICheck
//
//  CICheck - Copyright (C) 2016 Stanislav Demyanovich <mezozoysky@gmail.com>
//
//  This software is provided 'as-is', without any express or
//  implied warranty. In no event will the authors be held
//  liable for any damages arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute
//  it freely, subject to the following restrictions:
//
//  1.  The origin of this software must not be misrepresented;
//      you must not claim that you wrote the original software.
//      If you use this software in a product, an acknowledgment
//      in the product documentation would be appreciated but
//      is not required.
//
//  2.  Altered source versions must be plainly marked as such,
//      and must not be misrepresented as being the original software.
//
//  3.  This notice may not be removed or altered from any
//      source distribution.


/// \file
/// \brief Provides CheckApp, the application class for cic-check tool
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2016
/// \copyright CICheck is released under the terms of zlib/png license


#ifndef CIC_CHECK__CHECK_APP_HPP
#define CIC_CHECK__CHECK_APP_HPP

#include <Poco/Util/Application.h>
#include <Poco/AutoPtr.h>
#include <map>
#include <CICheck/goal/Goal.hpp>
#include <CICheck/xmlu/XMLUtils.hpp>

namespace cic
{
namespace check
{

class CheckApp
: public Poco::Util::Application
{
public:
	using Ptr = Poco::AutoPtr< CheckApp >;

	struct GoalDecl
	{
		std::string path{ "" };
		goal::Goal::Ptr goal{ nullptr };
	};

public:
	CheckApp() noexcept;
	virtual ~CheckApp() noexcept = default;

	void helpOptionCallback( const std::string& name, const std::string& value );

protected:
	virtual void initialize( Poco::Util::Application& self ) override;
	virtual void uninitialize() override;
	virtual void defineOptions( Poco::Util::OptionSet& options ) override;
	virtual int main( const std::vector< std::string >& args ) override;

	virtual std::string formatHelpText() const noexcept;
	virtual void loadDecls( const std::string& declsPath );

private:
	bool mIsHelpOptionRequested;

	xmlu::Parser mParser;
	std::map< std::string, GoalDecl > mGoalDecls;
};

} // namespace check
} // namespace cic

#endif // CIC_CHECK__CHECK_APP_HPP
