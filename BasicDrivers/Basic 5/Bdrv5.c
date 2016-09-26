/*--

    Copyright (c) Santos Merino del Pozo.  All rights reserved.
    Use of this source code is governed by a MIT-style license which can be
    found in the LICENSE file.

    Module Name:

        Bdrv5.c

    Summary:

        This module implements the "Hello world" of deferred procedure calls
        (DPC), i.e., a timer.

    Author:

        Santos Merino del Pozo (@santitox) - Initial version

    Contact:

        santos.research@gmail.com

--*/

#include "Bdrv5.h"

_Use_decl_annotations_
VOID
TimerDPC (
    _In_ PKDPC pKdpc,
    _In_ PVOID pDeferredContext,
    _In_ PVOID pSystemArgument1,
    _In_ PVOID pSystemArgument2
    )
{
    UNREFERENCED_PARAMETER(pKdpc);
    UNREFERENCED_PARAMETER(pDeferredContext);
    UNREFERENCED_PARAMETER(pSystemArgument1);
    UNREFERENCED_PARAMETER(pSystemArgument2);

    DbgPrintEx(77, 0, "[TimerDPC] - Hello!\n");
}

_Use_decl_annotations_
VOID
Bdrv5DriverUnload (
    _In_ PDRIVER_OBJECT pDriverObject
    )
{
    UNREFERENCED_PARAMETER(pDriverObject);

    PAGED_CODE();

    KeCancelTimer(g_Timer);
    ExFreePool(g_Timer);
    ExFreePool(g_Kdpc);

    DbgPrintEx(77, 0, "[Bdrv5DriverUnload] - Timer object disabled\n");
}

_Use_decl_annotations_
NTSTATUS
DriverEntry (
    _In_ PDRIVER_OBJECT pDriverObject,
    _In_ PUNICODE_STRING pRegistryPath
    )
{
    UNREFERENCED_PARAMETER(pRegistryPath);
    PAGED_CODE();

    LONG lPeriod;
    BOOLEAN bSetTimer;
    LARGE_INTEGER Timeout = { 0 };

    //
    // Relative time at which the timer is to expire, i.e., 1 second.
    // After this time, the timer will be removed from the system timer
    // queue and set to the signaled state. The associated DPC will be
    // put in the system DPC queue
    //
    Timeout.QuadPart = RELATIVE(SECONDS(1));

    //
    // Recurring interval so the timer automatically re-queue itself
    // after, i.e., 3000ms
    //
    lPeriod = 3000;

    //
    // Set default unload function called when stopping the corresponding
    // service (e.g., sc stop from an instance of the command interpreter Cmd.exe)
    //
    pDriverObject->DriverUnload = Bdrv5DriverUnload;

    //
    // Timer and DPC objects must be allocated in non-paged pool
    //
    g_Timer = ExAllocatePool(NonPagedPool, sizeof(KTIMER));
    g_Kdpc = ExAllocatePool(NonPagedPool, sizeof(KDPC));

    KeInitializeDpc(g_Kdpc, TimerDPC, NULL);

    KeInitializeTimer(g_Timer);

    bSetTimer = KeSetTimerEx(g_Timer, Timeout, lPeriod, g_Kdpc);

    if (bSetTimer)
    {
        DbgPrintEx(77, 0, "[DriverEntry] - KeSetTimerEx (timer "
                          "object was already in the system timer queue\n");
    }

    return STATUS_SUCCESS;
}
// EoF