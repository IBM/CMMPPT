/* $Id$ */
// Copyright (C) 2011, International Business Machines
// Corporation and others.  All Rights Reserved.
// This code is licensed under the terms of the Eclipse Public License (EPL).

#include "VisorFinite.h"
//#include "CoinUtilsConfig.h"

#ifdef HAVE_CFLOAT
#include <cfloat>
#else
#ifdef HAVE_FLOAT_H
#include <float.h>
#endif
#endif

#ifdef HAVE_CMATH
#include <cmath>
#else
#ifdef HAVE_MATH_H
#include <math.h>
#endif
#endif

#ifdef HAVE_CIEEEFP
#include <cieeefp>
#else
#ifdef HAVE_IEEEFP_H
#include <ieeefp.h>
#endif
#endif

bool VisorFinite(double val)
{
#ifdef VISOR_C_FINITE
  return VISOR_C_FINITE(val) != 0;
#else
  //return val != DBL_MAX && val != -DBL_MAX;
  return val != std::numeric_limits<double>::max() && val != -std::numeric_limits<double>::max();
#endif
}

bool VisorIsnan(double val)
{
#ifdef VISOR_C_ISNAN
  return COIN_C_ISNAN(val) != 0;
#else
  return false;
#endif
}

/* vi: softtabstop=2 shiftwidth=2 expandtab tabstop=2
*/