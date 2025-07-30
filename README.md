## gcc-md5sum - Compute MD5 message digest

A standalone `md5sum` implementation in ANSI C (almost).  The use of language
extensions or non standard language features has been avoided. 

Currently work in progress (only tested on Debian).

The  aim  is to have the same source code compile without  modification  on
Linux, VAX/VMS, and Tru64 Unix.

https://www.ietf.org/rfc/rfc1321.txt

### Compiling

To  build the application on download the source code from GitHub and unzip
it and invoke the compiler using the commands below.

Linux/Tru64 Unix:

    $ wget https://github.com/mike632t/gcc-md5sum/archive/refs/heads/stable.zip
    $ unzip stable.zip
    $ cd gcc-md5sum
    $ make test

VMS:

    $ cc gcc-md5sum.c /define=DEBUG
    $ link gcc-md5sum
    $ run gcc-md5sum

### Tested

   - Debian 12 (Bookworm), gcc 12.2.0, i686
   
   - Debian 10 (Buster), gcc 8.3.0, x64

   - Debian 10 (Buster), clang 7.0.1, x64
   
   - Debian 10 (Buster), tcc 0.9.27, x64
   
   - Tru64 UNIX 5.1b, cc 6.5-011, Alpha
   
   - VMS 5.4-3, vaxc 3.2-044, VAX

   - Open VMS 7.3, cc 6.0-001, VAX
   
   - Open VMS 7.3-2, cc 6.5-001, Alpha
   
   - Open VMS 8.4, cc 7.4-002, Alpha

Note - Due to the way data is stored on disk, the md5sum digests for a file
won't be the same on VMS systems as on UNIX.  To get the same MD5 digest as
on a UNIX/Linux system your VMS file must have the file attributes streamlf
and sequential.

You can check this using the following command.
```
$ anal/rms <filename>

 :
 :
 :

RMS FILE ATTRIBUTES

        File Organization: sequential
        Record Format: stream-LF
        Record Attributes:   carriage-return
        Maximum Record Size: 0
 :
 :
 :

```
Note  that if you compress the files using ZIP before copying them onto the
VMS system, when you extract them using UNZIP they will have the attributes
sequential + streamlf.

### Problem Reports

If you find problems or have suggestions relating to these simulators, then
please create a new [issue](https://github.com/mike632t/gcc-md5sum/issues).

Your problem report should contain:

- Architecture;

- Operating System and version;

- Compiler and version used;

- Commit ID;

- A description of the problem.

Thank you.

