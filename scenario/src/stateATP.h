#ifndef STATEATP_H
#define STATEATP_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/dfa.h>

// Set codes.  Not using enums because subclasses can have more sets
// The following were set by the base class
// #define LGFR_DEFAULT_SET		0
// #define LGFR_UNIVERSAL_SET		1
#define    LGFR_COMMITTED_DEMANDS	2
#define    LGFR_NEWOPPOR_DEMANDS	3
#define    LGFR_FIRMFORECAST_DEMANDS	4
#ifdef USING_RISK_FORECAST
  #define    LGFR_RISKFORECAST_DEMANDS	5
  #define    LGFR_LAST_DEMANDS		6
  #define    LGFR_CAPACITY_SET          7
  #define    LGFR_MATERIAL_SET          8
#else
  #define    LGFR_LAST_DEMANDS		5
  #define    LGFR_CAPACITY_SET          6
  #define    LGFR_MATERIAL_SET          7
#endif
#define    LGFR_NULL_DEMANDS		-1


// define ATP specific states
// Note: These state names should end in PROCESSING instead of IMPLODED,
// but there is not time to change them now --- WCD, 10/10/94
#define LGFR_ATP_STATE_COMMITTED_IMPLODED		STATE_INIT
#define LGFR_ATP_STATE_NEWOPP_IMPLODED			1
#define LGFR_ATP_STATE_ATP_IMPLODED			2
#define LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY	3

//  token types
#define LGFR_ATP_TOKEN_GO_FROM_CI_TO_NOI		1
#define LGFR_ATP_TOKEN_GO_FROM_NOI_TO_ATPI		2
#define LGFR_ATP_TOKEN_GO_FROM_CI_TO_FSS		3
#define LGFR_ATP_TOKEN_GO_FROM_FSS_TO_NOI		4
#define LGFR_ATP_TOKEN_GO_FROM_FSS_TO_CI		5
#define LGFR_ATP_TOKEN_GO_FROM_NOI_TO_CI		6
#define LGFR_ATP_TOKEN_GO_FROM_ATPI_TO_CI	        7
#define LGFR_ATP_TOKEN_SUPPLY_CHANGED			8
#define LGFR_ATP_TOKEN_COMMITTED_DEMANDS_CHANGED	9
#define LGFR_ATP_TOKEN_NEWOPP_DEMANDS_CHANGED		10
#define LGFR_ATP_TOKEN_ATP_DEMANDS_CHANGED		11
#define LGFR_ATP_TOKEN_REIMPLODE			12
// #define LGFR_ATP_TOKEN_RECALCULATE_FSS			13
#define LGFR_ATP_TOKEN_GO_FROM_CI_TO_ATPI		14
#define LGFR_ATP_TOKEN_GO_FROM_ATPI_TO_NOI		15

// maximum length for the Transition table
#define LGFR_MAXLENGTH_TRANSTABLE 100

#endif
