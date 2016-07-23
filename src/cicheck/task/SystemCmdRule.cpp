//
//  SystemCmdRule.cpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 07.07.16.
//
//

#include "SystemCmdRule.hpp"
#include <Poco/Process.h>
#include <Poco/PipeStream.h>
#include <Poco/StreamCopier.h>
#include <fstream>
#include <Poco/Exception.h>
#include <fmt/format.h>

using namespace cic::task;
using namespace cic::xmlu;
using namespace Poco;
using namespace fmt::literals;

namespace cic
{


bool SystemCmdRule::check()
{
	fmt::print( "SYSTEM CMD RULE {} CHECKS!\n", mName );
	std::vector< std::string > opts;

	if ( !mOptions.empty() )
	{
		opts.push_back( mOptions );
	}

	Pipe outPipe;
	Pipe errPipe;
	Process::Env env;

	ProcessHandle ph{
			Process::launch(
					mCmd
					, opts
					, "/home/mezozoy/tmp"
					, nullptr
					, &outPipe
					, &errPipe
					, env
			)
	};

	PipeInputStream istrOut( outPipe );
	PipeInputStream istrErr( errPipe );

	std::ofstream ostrOut{ "rule_{}_stdout.txt"_format( mName ) };
	std::ofstream ostrErr{ "rule_{}_stderr.txt"_format( mName ) };
	StreamCopier::copyStream( istrOut, ostrOut );
	StreamCopier::copyStream( istrErr, ostrErr );

	int rc{ ph.wait() };
	return ( rc == 0 );
}

void SystemCmdRule::loadFromXML( const xmlu::Node* root, indu::Industry* industry )
{
	fmt::print( "SYSTEM CMD RULE LOAD FROM XML!\n" );
	NodeMap* rootAttrs{ root->attributes() };

	// name
	Node* node{ rootAttrs->getNamedItem( "name" ) };
	if ( !node )
	{
		fmt::print( stderr, "[notice] name attribute isnt specified!\n" );
	}
	else
	{
		mName = node->getNodeValue();
	}

	// cmd
	node = rootAttrs->getNamedItem( "cmd" );
	if ( !node )
	{
		throw "[error] rule without 'cmd' attribute;";
	}
	mCmd = node->getNodeValue();

	// cmd options
	node = rootAttrs->getNamedItem( "options" );
	if ( node )
	{
		mOptions = node->getNodeValue();
	}
}


} // namespace cic
