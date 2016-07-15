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

using namespace cic::task;
using namespace cic::xmlu;
using namespace Poco;
using namespace fmt::literals;

namespace cic
{


bool SystemCmdRule::check()
{
	std::vector< std::string > opts;
	if ( mCmd.empty() )
	{
		throw ( "[error] systemCmdRule's cmd is empty;" );
	}

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
					, "/Users/mezozoy/tmp"
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

void SystemCmdRule::loadFromXml( const xmlu::Node* root, tu::FactoryOwner* factories )
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
