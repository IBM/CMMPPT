#ifndef LGFRCUSTOMER_H
#define LGFRCUSTOMER_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <iostream>

#if defined(_MSC_VER)
#define USING_WIT_DLL
#endif

/* Eventually the following line should be a a Makefile option not */
/* in this file */
// #define MOOG 
// #define SCEDEMO
// #define OMRON

/* This file defines mappings from potential customers to numbers */
/* that can be used in macro conditionals */
#define OMRONS_NUMBER 3
#define SCEDEMOS_NUMBER 2
#define MOOGS_NUMBER 1
#define GENERAL_NUMBER 0

#if defined(OMRON)
#define CUSTOMER_NUMBER OMRONS_NUMBER
#else
#if defined(MOOG)
#define CUSTOMER_NUMBER MOOGS_NUMBER
#else
#if defined(SCEDEMO)
#define CUSTOMER_NUMBER SCEDEMOS_NUMBER
#else
/* general and demos */
#define CUSTOMER_NUMBER GENERAL_NUMBER
#endif
#endif
#endif

/* Example */
/* #if CUSTOMER_NUMBER == MOOGS_NUMBER */
/*  Moog-specific code goes here */
/* #else */
/*  general code */
/* #endif */


//--------------------------------------------------------------------------
// if you want to allow the heuristic to build ahead in CMRP or ATP, then
// define LGFR_ALLOW_BUILD_AHEAD. 
//--------------------------------------------------------------------------

#if CUSTOMER_NUMBER == OMRONS_NUMBER
#define LGFR_ALLOW_BUILD_AHEAD
#endif

#if CUSTOMER_NUMBER == MOOGS_NUMBER
#define LGFR_ALLOW_BUILD_AHEAD
#endif

#if CUSTOMER_NUMBER == GENERAL_NUMBER
// #define LGFR_ALLOW_BUILD_AHEAD
#endif

//--------------------------------------------------------------------------
// Define LGFR_ALLOW_PROCURMENT_OF_UNCONSTRAINED_PARTS if ATP is to allow
// procurement of unconstrained parts
//--------------------------------------------------------------------------

#if CUSTOMER_NUMBER == MOOGS_NUMBER
#define LGFR_ALLOW_PROCURMENT_OF_UNCONSTRAINED_PARTS
#endif

#if CUSTOMER_NUMBER == GENERAL_NUMBER
// #define LGFR_ALLOW_PROCURMENT_OF_UNCONSTRAINED_PARTS
#endif

//--------------------------------------------------------------------------
// Define LGFR_NO_EQUITABILITY if ATP and CMRP initialy have equitability
// turned off
//--------------------------------------------------------------------------

#if CUSTOMER_NUMBER == MOOGS_NUMBER
#define LGFR_NO_EQUITABILITY
#endif

//--------------------------------------------------------------------------
// Define LGFR_USE_FOCUS_HORIZONS if CMRP is to use focus horizons when
// computing the fss
//--------------------------------------------------------------------------
#define LGFR_USE_FOCUS_HORIZONS

#if CUSTOMER_NUMBER == MOOGS_NUMBER
#undef  LGFR_USE_FOCUS_HORIZONS
#endif

#endif
