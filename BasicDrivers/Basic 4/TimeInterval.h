/*--

    Copyright (c) Santos Merino del Pozo.  All rights reserved.
    Use of this source code is governed by a MIT-style license which can be
    found in the LICENSE file.

    Module Name:

        TimeInterval.h

    Summary:

        This header contains definitions simplifying time interval
        specifications as provided in:

        http://www.osronline.com/article.cfm?article=261 (thanks!)

        See also:

        http://www.techtalkz.com/microsoft-device-drivers/297882-kedelayexecutionthread-not-waiting.html

    Author:

        Santos Merino del Pozo (@santitox) - Initial version
        Rob Green (from NTDEV list) - Original version

    Contact:

        santos.research@gmail.com

--*/

#pragma once

#define ABSOLUTE(wait) (wait)

//
// To be used when an interval of e.g., 5 seconds is
// desired (+ SECONDS macro, see below)
//
#define RELATIVE(wait) (-(wait))

#define NANOSECONDS(nanos) \
(((signed __int64)(nanos)) / 100L)

#define MICROSECONDS(micros) \
(((signed __int64)(micros)) * NANOSECONDS(1000L))

#define MILLISECONDS(milli) \
(((signed __int64)(milli)) * MICROSECONDS(1000L))

#define SECONDS(seconds) \
(((signed __int64)(seconds)) * MILLISECONDS(1000L))