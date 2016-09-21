/*--

    Copyright (c) Santos Merino del Pozo.  All rights reserved.
    Use of this source code is governed by a MIT-style license which can be
    found in the LICENSE file.

    Module Name:

        Bdrv3u.h

    Summary:

        This header contains definitions for Bdrv3u.cpp

    Author:

        Santos Merino del Pozo (@santitox) - Initial version

    Contact:

        santos.research@gmail.com

--*/

#pragma once

#include <Windows.h>
#include <winioctl.h>
#include <stdio.h>
#include <windef.h>

//
// See:
// P0052 - Generic Scope Guard and RAII Wrapper for the Standard Library
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0052r0.pdf
//
#include "../ScopedResource/unique_resource.h"

//
// We need the CTL_CODE definition
//
#include "../Basic 3/Common.h"

_Success_(return != 0)
_Check_return_
BOOL
IsServiceInstalled (
    _In_ LPCWSTR ServiceName
    );
// EoF