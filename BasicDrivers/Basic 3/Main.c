/*--

--*/

#include <ntddk.h>
//
// Use old-style declarators (when compiling with /W4 flag), see:
// https://msdn.microsoft.com/en-us/library/b92s55e9.aspx
//
#pragma warning(disable:4131)
#define FILE_DEVICE_BASIC 0x00002a7b
#define IOCTL_Device_Function CTL_CODE(FILE_DEVICE_BASIC,\
                                       0x800,\
                                       METHOD_NEITHER,\
                                       FILE_READ_DATA | FILE_WRITE_DATA)

///////////////////////////////////////////////////////////////////////////
// Function role type declarations
//
DRIVER_UNLOAD Basic3DriverUnload;
DRIVER_DISPATCH Basic3DefaultDispatch;
_Dispatch_type_(IRP_MJ_DEVICE_CONTROL)
DRIVER_DISPATCH Basic3DispatchDeviceControl;
DRIVER_INITIALIZE DriverEntry;

///////////////////////////////////////////////////////////////////////////
// Function prototypes
//
NTSTATUS Basic3HandleIoctlSayHello(PIRP pIrp, PIO_STACK_LOCATION pIoStackIrp);

///////////////////////////////////////////////////////////////////////////
// Global variables
//
LPCWSTR g_pszDeviceName = L"\\Device\\basicDevice";
LPCWSTR g_pszDeviceLink = L"\\DosDevices\\basicDevice";

///////////////////////////////////////////////////////////////////////////
// IRP Dispatchers
//
_Use_decl_annotations_
NTSTATUS
Basic3DefaultDispatch (
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
    PAGED_CODE();

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
                      IO_NO_INCREMENT);

    DbgPrintEx(77, 0, "[DefaultDispatch]\n");
    return pIrp->IoStatus.Status;
}

_Use_decl_annotations_
NTSTATUS
Basic3DispatchDeviceControl (
    _Inout_ PDEVICE_OBJECT pDeviceObject,
    _Inout_ PIRP pIrp
    )
{
    UNREFERENCED_PARAMETER(pDeviceObject);
    PAGED_CODE();

    NTSTATUS ntStatus;
    PIO_STACK_LOCATION pIoStackIrp;
    ULONG ulIoControlCode;

    pIoStackIrp = NULL;
    pIoStackIrp = IoGetCurrentIrpStackLocation(pIrp);

    if (!pIoStackIrp)
    {
        DbgPrintEx(77, 0, "[Basic3DispatchDeviceControl] - "
                           "Failed to get a pointer to the IO stack of the caller\n");
        return STATUS_INTERNAL_ERROR;
    }

    ulIoControlCode = pIoStackIrp->Parameters.DeviceIoControl.IoControlCode;

    if (ulIoControlCode != IOCTL_Device_Function)
    {
        DbgPrintEx(77, 0, "[Basic3DispatchDeviceControl] - "
                          "IOCTL not supported\n");
        return STATUS_NOT_SUPPORTED;
    }

    DbgPrintEx(77, 0, "[Basic1DispatchDeviceControl]\n");

    //
    // Call the IOCTL handler
    //
    ntStatus = Basic3HandleIoctlSayHello(pIrp, pIoStackIrp);

    pIrp->IoStatus.Status = ntStatus;
    IoCompleteRequest(pIrp,
                      IO_NO_INCREMENT);

    return ntStatus;
}

///////////////////////////////////////////////////////////////////////////
// IOCTL Handler
//
_Use_decl_annotations_
NTSTATUS
Basic3HandleIoctlSayHello (
    _In_ PIRP pIrp,
    _In_ PIO_STACK_LOCATION pIoStackIrp
    )
{
    UNREFERENCED_PARAMETER(pIrp);
    UNREFERENCED_PARAMETER(pIoStackIrp);
    PAGED_CODE();

    DbgPrintEx(77, 0, "[Basic3HandleIoctlSayHello] - Hello!\n");

    return STATUS_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////
// Basic driver functions (i.e., entry point and unload)
//
_Use_decl_annotations_
VOID
Basic3DriverUnload (
    _In_ PDRIVER_OBJECT pDriverObject
    )
/*++

--*/
{
    PAGED_CODE();

    UNICODE_STRING szUDeviceLink;

    //
    // Delete symlink and device
    //
    RtlInitUnicodeString(&szUDeviceLink,
                         g_pszDeviceLink);
    IoDeleteSymbolicLink(&szUDeviceLink);
    IoDeleteDevice(pDriverObject->DeviceObject);

    DbgPrintEx(77, 0, "[DriverUnload]\n");
}


_Use_decl_annotations_
NTSTATUS
DriverEntry (
    _In_ PDRIVER_OBJECT pDriverObject,
    _In_ PUNICODE_STRING pRegistryPath
    )
/*++

--*/
{
    UNREFERENCED_PARAMETER(pRegistryPath);
    PAGED_CODE();

    NTSTATUS ntStatus;
    PDEVICE_OBJECT pDeviceObject;
    UNICODE_STRING szUDeviceName;
    UNICODE_STRING szUDeviceLink;

    RtlInitUnicodeString(&szUDeviceName,
                         g_pszDeviceName);
    RtlInitUnicodeString(&szUDeviceLink,
                         g_pszDeviceLink);

    //
    // Create the device
    //
    ntStatus = IoCreateDevice(pDriverObject,
                              0,
                              &szUDeviceName,
                              FILE_DEVICE_BASIC,
                              0,
                              TRUE,
                              &pDeviceObject);

    if (!NT_SUCCESS(ntStatus))
    {
        DbgPrintEx(77, 0, "[DriverEntry] - Failed to create device (%.8Xh)\n",
                   ntStatus);
        return ntStatus;
    }

    //
    // Create the symbolic link
    //
    ntStatus = IoCreateSymbolicLink(&szUDeviceLink,
                                    &szUDeviceName);

    if (!NT_SUCCESS(ntStatus))
    {
        IoDeleteDevice(pDriverObject->DeviceObject);
        DbgPrintEx(77, 0, "[DriverEntry] - Failed to create symlink (%.8Xh)\n",
                   ntStatus);
        return ntStatus;
    }

    //
    // The Unload routine performs any operations that are
    // necessary before the system unloads the driver
    //
    pDriverObject->DriverUnload = Basic3DriverUnload;

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
        pDriverObject->MajorFunction[i] = Basic3DefaultDispatch;
    }

    pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = Basic3DispatchDeviceControl;

    DbgPrintEx(77, 0, "[DriverEntry]\n");

    return STATUS_SUCCESS;
}