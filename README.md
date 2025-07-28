## gcc-md5sum - Compute MD5 message digest

A standalone `md5sum` implementation in ANSI C (almost).  The use of language
extensions or non standard language features has been avoided. 

Currently work in progress (only tested on Debian).

The  aim  is to have the same source code compile without  modification  on
Linux, VAX/VMS, and Tru64 Unix.


### Compiling

To  build the application on Linux download the source code from GitHub and
unzip it (a  new folder  will  be created to automatically).

Then change directory to the new  folder run 'make'.

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

   - VMS 7.3, vaxc 6.0-001, VAX
   
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

