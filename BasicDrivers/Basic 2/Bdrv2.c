/*--

    Copyright (c) Santos Merino del Pozo.  All rights reserved.
    Use of this source code is governed by a MIT-style license which can be
    found in the LICENSE file.

    Module Name:

        Bdrv2.c

    Summary:

        This module extends "Basic 1" project, so the same (default) dispatch
        function is assigned to all IRP codes.

    Author:

        Santos Merino del Pozo (@santitox) - Initial version

    Contact:

        santos.research@gmail.com

--*/

#include "Bdrv2.h"

_Use_decl_annotations_
NTSTATUS
Bdrv2DefaultDispatch (
    _In_ PDEVICE_OBJECT pDeviceObject,
    _In_ PIRP pIrp
    )
{
    UNREFERENCED_PARAMETER(pDeviceObject);

    //
    // Set the IRP status information, indicating either success
    // or a hardware failure. In this case (where no additional 
    // processing is requried), result is always successful
    //
    pIrp->IoStatus.Status = STATUS_SUCCESS;

    //
    // We just indicate that all processing has been finished.
    // Thereby, the IO manager can check whether (higher-level)
    // drivers on the stack called IoCompletion. If so, they are
    // called until the top layer is reached, hence completing
    // the IRP
    //
    IoCompleteRequest(pIrp,
                      IO_NO_INCREMENT);
 
    DbgPrintEx(77, 0, "[DefaultDispatch]\n");

    return pIrp->IoStatus.Status;
}

_Use_decl_annotations_
VOID
Bdrv2DriverUnload (
    _In_ PDRIVER_OBJECT pDriverObject
    )
{
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
    pDriverObject->DriverUnload = Bdrv2DriverUnload;

    //
    // We assign the same default dispatch routine to all IRP function
    // codes. Otherwise, If we don't do this any (unrecognized) IRP
    // will result in STATUS_INVALID_DEVICE_REQUEST which is a bad
    // programming practice. As suggested by the MSDN documentation,
    // a default dispatch routine should forward the IRP to the next
    // (lower-level) driver
    //
    for (ULONG i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
    {
        //
        // We just complete all IRPs that come its way.
        // However, as there is no user-mode application communicating with
        // the driver, no debug output corresponding with DefaultDispatch
        // should be seen. For a more elaborated driver, see "Basic 3" which
        // includes a user-land application sending IOCTLs to the driver
        //
        pDriverObject->MajorFunction[i] = Bdrv2DefaultDispatch;
    }

    DbgPrintEx(77, 0, "[DriverEntry]\n");

    return STATUS_SUCCESS;
}
// EoF