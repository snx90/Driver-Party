/*--

    Copyright (c) Santos Merino del Pozo.  All rights reserved.
    Use of this source code is governed by a MIT-style license which can be
    found in the LICENSE file.

    Module Name:

        Common.h

    Summary:

        This header contains common definitions that may be used by a
        user-mode application.

    Author:

        Santos Merino del Pozo (@santitox) - Initial version

    Contact:

        santos.research@gmail.com

--*/

#pragma once

#define IOCTL_Device_Function CTL_CODE(FILE_DEVICE_UNKNOWN,\
                                       0x800,\
                                       METHOD_NEITHER,\
                                       FILE_ANY_ACCESS)
// EoF