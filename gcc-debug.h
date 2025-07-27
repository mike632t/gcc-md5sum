/*
 * gcc-debug.h
 *
 * Copyright(C) 2013   MT
 *
 * Define debug macro.
 *
 * This  program is free software: you can redistribute it and/or modify it
 * under  the terms of the GNU General Public License as published  by  the
 * Free  Software Foundation, either version 3 of the License, or (at  your
 * option) any later version.
 *
 * This  program  is distributed in the hope that it will  be  useful,  but
 * WITHOUT   ANY   WARRANTY;   without even   the   implied   warranty   of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You  should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * 17 Aug 13  0.1.0001  - Initial version - MT
 * 07 Dec 18            - Updated to allow debugging to be turned on or off
 *                        by setting  DEBUG true or false - MT
 * 22 Aug 21            - Prints Current filename and line number - MT
 *                      - Added verbose macro - MT
 * 03 Jan 22  0.2.0005  - Changed both macros to allow DEBUG and VERBOSE to
 *                        be defined from the command line - MT
 * 09 Jun 25  0.3.0006  - Changed to use stdout instead of stderr - MT
 * 25 Jul 25  0.4.0007  - Added profile and error macros - MT
 *
 */

#ifndef debug /* Don't redefine macro if already defined. */
#if defined(DEBUG) /* Execute code if DEBUG is True */ 
#define debug(code) do {fprintf(stdout, "\nDebug\t: %s line : %d : ", \
            __FILE__, __LINE__); code;} while(0)
#else
#define debug(code)
#endif
#endif

#ifndef verbose /* Don't redefine macro if already defined. */
#if defined(VERBOSE) /* Execute code if VERBOSE is True */ 
#define verbose(code) do {fprintf(stdout, "Verbose\t: %s line : %d : ", \
            __FILE__, __LINE__); code;} while(0)
#else
#define verbose(code)
#endif
#endif

#ifndef profile /* Don't redefine macro if already defined. */
#include <stdio.h>
#include <time.h>
#if DEBUG /* Execute code if DEBUG is True */ 
#define profile(__code) do { /* Time how long code takes to execute */  \
   struct timespec __t_start, __t_end; \
   double __d_elapsed; \
   clock_gettime(CLOCK_MONOTONIC, &__t_start); \
   __code \
   clock_gettime(CLOCK_MONOTONIC, &__t_end); \
   __d_elapsed = (__t_end.tv_sec - __t_start.tv_sec) + \
                    (__t_end.tv_nsec - __t_start.tv_nsec) / 1e9; \
   fprintf(stdout, "%0.9fs\n", __d_elapsed); \
} while(0)
#else
#define profile(__code) do { /* Just run it */ \
   __code \
} while(0)
#endif
#endif

#ifndef error /* Don't redefine macro if already defined. */
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#define error(i_errno, s_format, ...) /* Display an error message and exit */ \
    do { \
        int _err = (i_errno); \
        if (!_err) _err = -1; \
        fprintf(stderr, "%s: ", NAME); /* Depends on NAME being defined */ \
        fprintf(stderr, s_format, ##__VA_ARGS__); \
        fprintf(stderr, "\n"); \
        exit(_err); \
    } while (0)
#endif

#ifndef warning /* Don't redefine macro if already defined. */
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#define warning(i_errno, s_format, ...) /* Display an warning message */ \
    do { \
        int _err = (i_errno); \
        if (!_err) _err = -1; \
        fprintf(stderr, "%s: ", NAME); /* Depends on NAME being defined */ \
        fprintf(stderr, s_format, ##__VA_ARGS__); \
        fprintf(stderr, "\n"); \
    } while (0)
#endif
