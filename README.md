# Driver Party
A collection of (very) basic Windows drivers providing a reference implementation for learning purposes (inspired by Rootkit.com).

## Motivation
(blah) Maybe, other day...

## Driver Installation
To install and uninstall the driver, use the 'sc' command. For installation:

    >sc create <driver_label> type= kernel binPath= <sys_driver_path>
    >sc start <driver_label>

And for uninstallation:

    >sc stop <driver_label>
    >sc delete <driver_label>

## Note
The project has been tested on Win8.1 32-bit running inside an x86-based virtual machine.
If you are running the examples on >= Vista, you (may) need to enable debug output on Windows.
See: http://www.osronline.com/article.cfm?article=295


# License
This software is released under the MIT License, see LICENSE.