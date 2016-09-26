/*--

    Copyright (c) Santos Merino del Pozo.  All rights reserved.
    Use of this source code is governed by a MIT-style license which can be
    found in the LICENSE file.

    Module Name:

        Bdrv5.h

    Summary:

        This header contains definitions for Bdrv5.c

    Author:

        Santos Merino del Pozo (@santitox) - Initial version

    Contact:

        santos.research@gmail.com

--*/

#pragma once
//
// Use old-style declarators (when compiling with /W4 flag), see:
// https://msdn.microsoft.com/en-us/library/b92s55e9.aspx
//
#pragma warning(disable:4131)

#include <ntddk.h>
#include "../Basic 4/TimeInterval.h"

//
// Global variables
//

//
// Timer object
//
PKTIMER g_Timer;

//
// DPC object
//
PKDPC g_Kdpc;

//
// Function prototypes and role type declarations
//
_Function_class_(KDEFERRED_ROUTINE)
_IRQL_requires_(DISPATCH_LEVEL)
_IRQL_requires_same_
VOID
TimerDPC (
    _In_ PKDPC pKdpc,
    _In_ PVOID pDeferredContext,
    _In_ PVOID pSystemArgument1,
    _In_ PVOID pSystemArgument2
    );

_Function_class_(DRIVER_UNLOAD)
VOID
Bdrv5DriverUnload (
    _In_ PDRIVER_OBJECT pDriverObject
    );

_Function_class_(DRIVER_INITIALIZE)
NTSTATUS
DriverEntry (
    _In_ PDRIVER_OBJECT pDriverObject,
    _In_ PUNICODE_STRING pRegistryPath
    );
// EoF