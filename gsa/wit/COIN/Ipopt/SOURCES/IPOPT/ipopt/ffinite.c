/* Copyright (C) 2002, Carnegie Mellon University and others.
   All Rights Reserved.
   This code is published under the Common Public License.  */

/* $Id: ffinite.c,v 1.1 2002/11/24 21:28:39 andreasw Exp $  */
/*
    This routine implements the Fortran interface to the C function finite
    Returns nonzero, if the argument is not Inf or NaN, returns nonzero
    if the argument is finite.
*/

/*  
    Author:  Andreas Waechter     05/01/02  Release as Version IPOPT 2.0
*/

/*  Make sure we do the correct casting from C int to Fortran INTEGER */
#ifdef __osf__
typedef int integer;
#else
typedef long int integer;
#endif

#include <math.h>
#ifdef WIN32
#include <float.h>
#endif

#ifdef _AIX
#define ffinite_ ffinite
#endif

#ifdef __MINGW32__
integer ffinite_ (double *number)
#elif WIN32
integer __stdcall FFINITE(double *number)
#else
integer ffinite_ (double *number)
#endif
{
  integer ret_val;

#ifdef WIN32
  ret_val = (integer) _finite(*number);
#else
  ret_val = (integer) finite(*number);
#endif

  return ret_val;
}
