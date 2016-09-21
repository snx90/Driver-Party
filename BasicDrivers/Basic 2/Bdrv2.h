/*--

    Copyright (c) Santos Merino del Pozo.  All rights reserved.
    Use of this source code is governed by a MIT-style license which can be
    found in the LICENSE file.

    Module Name:

        Bdrv2.h

    Summary:

        This header contains definitions for Bdrv2.c

    Author:

        Santos Merino del Pozo (@santitox) - Initial version

    Contact:

        santos.research@gmail.com

--*/

#pragma once

#include <ntddk.h>

//
// Function prototypes and role type declarations
//
_Function_class_(DRIVER_DISPATCH)
NTSTATUS
Bdrv2DefaultDispatch (
    _Inout_ PDEVICE_OBJECT pDeviceObject,
    _Inout_ PIRP pIrp
    );

_Function_class_(DRIVER_UNLOAD)
VOID
Bdrv2DriverUnload (
    _In_ PDRIVER_OBJECT pDriverObject
    );

_Function_class_(DRIVER_INITIALIZE)
NTSTATUS
DriverEntry (
    _In_ PDRIVER_OBJECT pDriverObject,
    _In_ PUNICODE_STRING pRegistryPath
    );
// EoF