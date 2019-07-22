// File:   sceDefin.h

#ifndef LGFRSCEDEFINE_H
#define LGFRSCEDEFINE_H
#include <stdlib.h>

#include <mcl/src/mdsPragma.h>

// USE THIS FOR MEMORY DEBUGGING
// #ifndef ELIMINATE_OLD_MAIN
// #define ELIMINATE_OLD_MAIN
// #endif

#ifndef STR
#  define STR(x)    #x
#endif

#ifndef XSTR
#  define XSTR(x)   STR(x)
#endif




#define SCE_OPT_IMPLODE
 
// #if defined (__OS2__) || defined (WIN32) || defined (__WINDOWS__)
// #define DOS_FILE_NAMES
// #endif

#define MANDATORY 1
#define PURELY_OPTIONAL  0
#define OPTIONAL_WITH_MESSAGE  -1

#define DEMAND_TYPE_ERROR 0
#define DEMAND_TYPE_INDP 1
#define DEMAND_TYPE_TINP 2

#define SCE_WIT_MESSAGING_OFF  0
#define SCE_WIT_MESSAGING_ON   1


// these CONSTANTS should really be implemented as
// enums, but we're in a rush!
#define SCE_FEATURE_TYPE_CUSTOMER_CHOICE  0
#define SCE_FEATURE_TYPE_PURELY_OPTIONAL  1
#define SCE_FEATURE_TYPE_MANDATORY        2
#define SCE_FEATURE_TYPE_MWO              3


#define PEG_TYPE_SUPPLY       1
#define PEG_TYPE_CONSVOL      2
#define PEG_TYPE_EXECVOL      3
#define PEG_TYPE_SIDEVOL      4
#define PEG_TYPE_PRODVOL      5
#define PEG_TYPE_INTERPLANT   6



#define ENABLE_NEGATIVE_DEMANDS
#define ALLOW_QUOTED_STRING_PARAMS
// #define SCE_USE_WIT_MRP
// #define BUILDING_BLOCKS_VIA_MULTI_BOPS
// #define FEATURE_SETS_VIA_PROPORTIONAL_ROUTING
// #define MULTI_SITE_BOM
#define SCRUBBER_MODE_ENABLED
#define PEGGING_ENABLED
#define MARK_BOGONS
#define FORCE_BOGONS
#define PRODVOLPEGGING
// #define ALTERNATE_AS_SUB
#define MULTI_ATTRIBUTE_DEMAND_CLASS
//  #define VARIABLE_PERIODS

#endif









