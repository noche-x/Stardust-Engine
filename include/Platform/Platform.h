#pragma once

#define PLATFORM_PSP 	0
#define PLATFORM_WIN 	1
#define PLATFORM_NIX 	2
#define PLATFORM_VITA 	3
#define PLATFORM_XXX 	-1

//Switch this line of code for other build support

#if CURRENT_PLATFORM == PLATFORM_PSP
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspthreadman.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <string.h>
#include <pspnet.h>
#include <psputility.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <pspnet_resolver.h>
#include <psphttp.h>
#include <pspsdk.h>
#include <pspwlan.h>
#include <sys/socket.h>
#include <unistd.h> 
#include <queue>
#elif CURRENT_PLATFORM == PLATFORM_VITA
#include <vitasdk.h> 
// FIXME
typedef uint64_t u64;
typedef uint32_t u32;
#endif


namespace Stardust::Platform {
#ifndef CURRENT_PLATFORM
#error No Platform was Defined!
#endif

	/*
	Init Platform performs all needed platform initialization code
	*/
	void initPlatform(const char* appName = "APP");

	/*
	Exit Platform performs all needed platform exit code (this is for forced exits)
	*/
	void exitPlatform();

	/*
	Performs all per frame operations
	*/
	void platformUpdate();
}