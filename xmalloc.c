/* memory allocation routines with error checking.
   Copyright 1989, 90, 91, 92, 93, 94 Free Software Foundation, Inc.
   
This file is part of the libiberty library.
Libiberty is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

Libiberty is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with libiberty; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
Boston, MA 02110-1301, USA.  */

/*

@deftypefn Replacement void* xmalloc (size_t)

Allocate memory without fail.  If @code{malloc} fails, this will print
a message to @code{stderr} (using the name set by
@code{xmalloc_set_program_name},
if any) and then call @code{xexit}.  Note that it is therefore safe for
a program to contain @code{#define malloc xmalloc} in its source.

@end deftypefn

@deftypefn Replacement void* xrealloc (void *@var{ptr}, size_t @var{size})
Reallocate memory without fail.  This routine functions like @code{realloc},
but will behave the same as @code{xmalloc} if memory cannot be found.

@end deftypefn

@deftypefn Replacement void* xcalloc (size_t @var{nelem}, size_t @var{elsize})

Allocate memory without fail, and set it to zero.  This routine functions
like @code{calloc}, but will behave the same as @code{xmalloc} if memory
cannot be found.

@end deftypefn

@deftypefn Replacement void xmalloc_set_program_name (const char *@var{name})

You can use this to set the name of the program used by
@code{xmalloc_failed} when printing a failure message.

@end deftypefn

@deftypefn Replacement void xmalloc_failed (size_t)

This function is not meant to be called by client code, and is listed
here for completeness only.  If any of the allocation routines fail, this
function will be called to print an error message and terminate execution.

@end deftypefn

*/

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#if defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#endif

#include "ansidecl.h"
#include "libiberty.h"

/* The program name if set.  */
static const char *name = "";

#define HAVE_SBRK
#ifdef HAVE_SBRK
/* The initial sbrk, set when the program name is set. Not used for win32
   ports other than cygwin32.  */
static char *first_break = NULL;
#endif /* HAVE_SBRK */

void
xmalloc_set_program_name (const char *s)
{
  name = s;
#ifdef __linux__
#ifdef HAVE_SBRK
  /* Win32 ports other than cygwin32 don't have brk() */
  if (first_break == NULL)
	  first_break = (char *)sbrk(0);
#endif /* HAVE_SBRK */
#endif //__linux__
}

void
xmalloc_failed (size_t size)
{
#ifdef __linux__
#ifdef HAVE_SBRK
	extern char **environ;
	size_t allocated;

	if (first_break != NULL)
		allocated = (char *)sbrk(0) - first_break;
	else
		allocated = (char *)sbrk(0) - (char *)&environ;
	fprintf(stderr,
		"\n%s%sout of memory allocating %lu bytes after a total of %lu bytes\n",
		name, *name ? ": " : "",
		(unsigned long)size, (unsigned long)allocated);
#else /* HAVE_SBRK */
	fprintf(stderr,
		"\n%s%sout of memory allocating %lu bytes\n",
		name, *name ? ": " : "",
		(unsigned long)size);
#endif /* HAVE_SBRK */
	xexit(1);

#elif WIN32  // WIN32
=======
#ifdef WIN32  // WIN32
>>>>>>> origin/main
  fprintf (stderr,
	   "\n%s%sout of memory allocating %lu bytes\n",
	   name, *name ? ": " : "",
	   (unsigned long) size);
  xexit (1);
#else
<<<<<<< HEAD
#error There are many platform dependent issues for this OS.
#endif // __linux__
=======
#ifdef HAVE_SBRK
	extern char **environ;
	size_t allocated;

	if (first_break != NULL)
		allocated = (char *)sbrk(0) - first_break;
	else
		allocated = (char *)sbrk(0) - (char *)&environ;
	fprintf(stderr,
		"\n%s%sout of memory allocating %lu bytes after a total of %lu bytes\n",
		name, *name ? ": " : "",
		(unsigned long)size, (unsigned long)allocated);
#else /* HAVE_SBRK */
	fprintf(stderr,
		"\n%s%sout of memory allocating %lu bytes\n",
		name, *name ? ": " : "",
		(unsigned long)size);
#endif /* HAVE_SBRK */
	xexit(1);
#endif // WIN32
}  

PTR
xmalloc (size_t size)
{
  PTR newmem;

  if (size == 0)
    size = 1;
  newmem = malloc (size);
  if (!newmem)
    xmalloc_failed (size);

  return (newmem);
}

PTR
xcalloc (size_t nelem, size_t elsize)
{
  PTR newmem;

  if (nelem == 0 || elsize == 0)
    nelem = elsize = 1;

  newmem = calloc (nelem, elsize);
  if (!newmem)
    xmalloc_failed (nelem * elsize);

  return (newmem);
}

PTR
xrealloc (PTR oldmem, size_t size)
{
  PTR newmem;

  if (size == 0)
    size = 1;
  if (!oldmem)
    newmem = malloc (size);
  else
    newmem = realloc (oldmem, size);
  if (!newmem)
    xmalloc_failed (size);

  return (newmem);
}
