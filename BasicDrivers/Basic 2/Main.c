/*--

--*/

#include <ntddk.h>

VOID
DriverUnload (
    _In_ PDRIVER_OBJECT pDriverObject
    )
/*++

--*/
{
    UNREFERENCED_PARAMETER(pDriverObject);

    DbgPrintEx(77, 0, "[DriverUnload]\n");
}


NTSTATUS
DefaultDispatch (
    _In_ PDEVICE_OBJECT pDeviceObject,
    _In_ PIRP pIrp
    )
/*++
    This function just completes all IRP's that come its way.
    However, as there is no user-mode application communicating
    with the driver, no debug output corresponding with DefaultDispatch
    should be seen.
--*/
{
    UNREFERENCED_PARAMETER(pDeviceObject);

    //
    // Set the IRP status information, indicating either success
    // or a hardware failure. In this toy example, result is always
    // successful.
    //
    pIrp->IoStatus.Status = STATUS_SUCCESS;

    //
    // We just indicate that all processing has been finished.
    // Thereby, the IO manager can check whether (higher-level)
    // drivers on the stack called IoCompletion. If so, they are
    // called until the top layer is reached, hence completing
    // the IRP.
    //
    IoCompleteRequest(pIrp,
                      IO_NO_INCREMENT
                      );
    
    DbgPrintEx(77, 0, "[DefaultDispatch]\n");
    return pIrp->IoStatus.Status;
}


NTSTATUS
DriverEntry (
    _In_ PDRIVER_OBJECT pDriverObject,
    _In_ PUNICODE_STRING pRegistryPath
    )
/*++

--*/
{
    UNREFERENCED_PARAMETER(pRegistryPath);

    //
    // The Unload routine performs any operations that are
    // necessary before the system unloads the driver
    //
    pDriverObject->DriverUnload = DriverUnload;

    //
    // We assign the same degault dispatch routine to all
    // IRP function codes. Otherwise, If we don't do this
    // any (unrecognized) IRP will result in 
    //          STATUS_INVALID_DEVICE_REQUEST
    // which is a bad programming practice. As suggested
    // by the MSDN documentation, a default dispatch routine
    // should forward the IRP to the next (lower-level)
    // driver.
    //
    for (ULONG i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
    {
        pDriverObject->MajorFunction[i] = DefaultDispatch;
    }

    DbgPrintEx(77, 0, "[DriverEntry]\n");

    return STATUS_SUCCESS;
}