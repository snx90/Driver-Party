/*--

    Copyright (c) Santos Merino del Pozo.  All rights reserved.
    Use of this source code is governed by a MIT-style license which can be
    found in the LICENSE file.

    Module Name:

        Bdrv1.c

    Summary:

        This module implements a "Hello world" driver, implementing the
        minimum functionality.

    Author:

        Santos Merino del Pozo (@santitox) - Initial version

    Contact:

        santos.research@gmail.com

--*/

#include "Bdrv1.h"

_Use_decl_annotations_
VOID
Bdrv1DriverUnload (
    _In_ PDRIVER_OBJECT pDriverObject
    )
{
    //
    // Trick to surpress the W4 warning "unreferenced parameter":
    // #define UNREFERENCED_PARAMETER(P) (P)
    //
    UNREFERENCED_PARAMETER(pDriverObject);

    //
    // The Unload routine performs any operations that are
    // necessary before the system unloads the driver. In this toy
    // example we just print a message (no additional operations are
    // required in this case)
    //
    DbgPrintEx(77, 0, "[DriverUnload]\n");
}

_Use_decl_annotations_
NTSTATUS
DriverEntry (
    _In_ PDRIVER_OBJECT pDriverObject,
    _In_ PUNICODE_STRING pRegistryPath
    )
{
    UNREFERENCED_PARAMETER(pRegistryPath);
 
    //
    // Set default unload function called when stopping the corresponding
    // service (e.g., sc stop from an instance of the command interpreter Cmd.exe)
    //
    pDriverObject->DriverUnload = Bdrv1DriverUnload;
 
    DbgPrintEx(77, 0, "[DriverEntry]\n");

    return STATUS_SUCCESS;
}
// EoF