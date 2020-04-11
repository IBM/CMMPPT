#ifndef VISOR_visorUtil_h
#define VISOR_visorUtil_h

#include <string>
#include "printer.h"
#include "requestQuantity.h"
#include "onHandMaterial.h"
#include "visorProdPlan.h"
#include "visorProblem1.h"
#include "visorProblem2.h"

void readOnHandMaterial(const std::string &onHandMaterialFileName, VISORproblem1 &printingProb);

void readPrinter(std::string &printerFileName,
                 VISORproblem1 *printingProbPtr,
                 VISORproblem2 *allocProbPtr);

void readRequestQuantity(std::string &requestQuantityFileName,
                         VISORproblem2 &allocProb);

void readVisorProdPlan(std::string visorProdPlanFileName,
                       VISORproblem2 &allocProb);

void copyImplosion1ShipVolToImplosion2SupplyVol(
    VISORproblem1 &printingProb,
    VISORproblem2 &allocProb);

#endif
