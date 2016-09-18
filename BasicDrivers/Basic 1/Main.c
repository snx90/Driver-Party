/*--

--*/

//
// How to enable debug output on Windows >= Vista:
// http://www.osronline.com/article.cfm?article=295
//

#include <ntddk.h>

VOID
DriverUnload (
    _In_ PDRIVER_OBJECT pDriverObject
    )
/*++

--*/
{
    //
    // Trick to surpress the W4 warning "unreferenced parameter":
    // #define UNREFERENCED_PARAMETER(P) (P)
    //
    UNREFERENCED_PARAMETER(pDriverObject);

    DbgPrintEx(77, 0, "[DriverUnload]\n");
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
    
    DbgPrintEx(77, 0, "[DriverEntry]\n");

    return STATUS_SUCCESS;
}