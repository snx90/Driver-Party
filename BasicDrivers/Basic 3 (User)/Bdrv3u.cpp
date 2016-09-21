/*--

    Copyright (c) Santos Merino del Pozo.  All rights reserved.
    Use of this source code is governed by a MIT-style license which can be
    found in the LICENSE file.

    Module Name:

        Bdrv3u.cpp

    Summary:

        This module implements a basic user-mode application communicating
        with "Basic 3" driver.

    Author:

        Santos Merino del Pozo (@santitox) - Initial version

    Contact:

        santos.research@gmail.com

--*/

#include "Bdrv3u.h"

namespace stdexp = std::experimental; 

_Use_decl_annotations_
BOOL
IsServiceInstalled (
    _In_ LPCWSTR ServiceName
    )
{
    BOOL bIsServiceInstalled;

    //
    // Checks whether the indicated service has been installed (e.g., sc create)
    // on the system or not. It has been taken from Satoshi's DrvLoader, see:
    //      https://github.com/tandasat/DrvLoader
    //
    const auto scmHandle = stdexp::make_unique_resource(
        OpenSCManager(nullptr, nullptr, GENERIC_READ), &CloseServiceHandle);
    bIsServiceInstalled = CloseServiceHandle(OpenService(
        scmHandle.get(), ServiceName, GENERIC_READ));

    return bIsServiceInstalled;
}

INT
main (
    _In_ INT argc,
    _In_ PCHAR argv[]
    )
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    DWORD dwError;
    BOOL bfInstalled;
    BOOL bfSendIOCTL;
    HANDLE hDevice;
    LPWSTR pszFullDeviceName;

    dwError = ERROR_SUCCESS;
    pszFullDeviceName = new WCHAR[MAX_PATH];

    //
    // Driver (labelled "basic3") is installed manually in advance
    //
    bfInstalled = IsServiceInstalled(L"basic3");

    if (!bfInstalled)
    {
        fwprintf(stderr, L"[IsServiceInstalled] - ERROR (Service is not installed)\n");
        return EXIT_FAILURE;
    }

    //
    // Create a \\.\Global\Xxx device name to use with CreateFile()
    //
    wsprintf(pszFullDeviceName, L"\\\\.\\Global\\%s", L"basicDevice");

    //
    // Open a handle to the device
    //
    hDevice = CreateFile(pszFullDeviceName,
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         NULL,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         NULL);

    if (hDevice == INVALID_HANDLE_VALUE)
    {
        dwError = GetLastError();
        fwprintf(stderr, L"[CreateFile] - ERROR (%.8Xh)\n", dwError);
        return EXIT_FAILURE;
    }

    //
    // Send an IOCTL (i.e., IOCTL_Device_Function) to the device
    //
    bfSendIOCTL = DeviceIoControl(hDevice,
                                  IOCTL_Device_Function,
                                  NULL,
                                  0,
                                  NULL,
                                  0,
                                  NULL,
                                  NULL);

    if (!bfSendIOCTL)
    {
        dwError = GetLastError();
        fwprintf(stderr, L"[DeviceIoControl] - ERROR (%.8Xh)\n", dwError);
    }

    //
    // Close handle and exit
    //
    CloseHandle(hDevice);
    if (dwError == ERROR_SUCCESS)
    {
        return EXIT_SUCCESS;
    }
    else
    {
        return EXIT_FAILURE;
    }
}
// EoF