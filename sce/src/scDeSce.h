#ifndef LGFR_SCE_SCHDDEST_H
#define LGFR_SCE_SCHDDEST_H

#include <scenario/src/schdDesc.h>

// Schedule description strings specifically created for Iman

// Note: Schedule descriptor abbreviations must be less than 16 characters 
//       long.

// LgFrScheduleFactory::abbreviationOf() can be used to get the abbreviation
// for a descriptor.

// -------------------------------------
// Sce Descriptors
// -------------------------------------
const LgFrScheduleDescriptor
     LGFRInterPlantSupplyVolume("Inter Plant Supply Volume", "IP Sup Vol");

#endif
