////////////////////////////////////////////////////////////////////////////
// Name:        CommonHeader.h
// Purpose:     Includes shared by all other files. 
// Authors:     Paul Koning, Joe Stanton, Bill Galcher, Steve Zoppi, Dale Sinder
// Created:     03/26/2005
// Copyright:   (c) Paul Koning, Joe Stanton, Dale Sinder
// Licence:     see pterm-license.txt
/////////////////////////////////////////////////////////////////////////////

#ifndef __COMMONHEADER__
#define __COMMONHEADER__ 1

// ============================================================================
// declarations
// ============================================================================

// vvvvvvv This should be in the C++ Compiler directives for Visual Studio
//#define _CRT_SECURE_NO_WARNINGS 1  // for MSVC to not be such a pain

#define RESIDENTMSEC 30 // msec to give resident before return to z80

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include <string.h>
#include <wchar.h>

#include <fcntl.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <stdio.h> 



#ifdef __BORLANDC__
#pragma hdrstop
#endif





extern "C"
{
#if defined (_WIN32)
#include <winsock.h>
#include <process.h>
#include <io.h>
#else
#include <fcntl.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#endif
#include <stdlib.h>
#include <time.h>

typedef unsigned char u8;
typedef short u16;



#if wxUSE_LIBGNOMEPRINT
#include "wx/html/forcelnk.h"
    //FORCE_LINK (gnome_print)
#endif

}



#define None 0xffffffff
#define KEY(a, b, c, d) (((a) & 0xff) + (((b) & 0xff) << 8) + \
                         + (((c) & 0xff) << 16) + (((d) & 0xff) << 24))
#define KEY1(a) KEY ((a), None, None, None)
// typedef struct 
// {
//     u32 u;
//     u32 p;
// } ukey;

typedef enum { both, niu, ascii } connMode;


// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    Pterm_ToggleMenuBar = 1,
    Pterm_ToggleStatusBar,
    Pterm_ToggleStretchMode,
    Pterm_ToggleAspectMode,
    Pterm_ToggleLock,
    Pterm_RestoreLocation,
    Pterm_SetScaleEntry,
    Pterm_CopyScreen = Pterm_SetScaleEntry + 32,
    Pterm_ConnectAgain,
    Pterm_ResetMtutor,
    Pterm_SessionSettings,
    Pterm_SaveScreen,
    Pterm_SaveAudio,
    Pterm_HelpKeys,
    Pterm_HelpIndex,
    Pterm_PastePrint,
    Pterm_FullScreen,
    Pterm_Resize,
    Pterm_Boot,

    // timers
    Pterm_Timer,        // display pacing
    Pterm_Mclock,       // pterm clock
    Pterm_Dclock,       // disk clock
    Pterm_Mz80,
    Pterm_PasteTimer,   // paste key generation pacing
    //other items
    Pterm_Exec,         // execute URL
    Pterm_MailTo,       // execute email client
    Pterm_SearchThis,   // execute search URL
    Pterm_Macro0,
    Pterm_Macro1,
    Pterm_Macro2,
    Pterm_Macro3,
    Pterm_Macro4,
    Pterm_Macro5,
    Pterm_Macro6,
    Pterm_Macro7,
    Pterm_Macro8,
    Pterm_Macro9,


};

// it is important that these be kept in sync 
// with the -jump-
// in unit route in ptermhelp/nptermhelp 
// block 1-d, microroute
//
// jump    context,x,index,keyboard,profile,
//         connect,title,emulate, display,
//         paste, local, connect2, x


enum 
{
    helpContextGenericIndex = 0,
    helpContextKeyboard,
    helpContextProfiles,
    helpContextConnection,
    helpContextTitle,
    helpContextEmulation,
    helpContextDisplay,
    helpContextPasting,
    helpContextLocal,
    helpContextConnecting
};

enum
{
    osOther = 0,
    osMac,
    osWin,
    osLinux
};


#endif    // __COMMONHEADER__
