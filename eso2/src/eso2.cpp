
#if defined(_MSC_VER)
#pragma warning(disable:4786)
#pragma warning(disable:4503)
#endif


#include "wit.h"

#include <map>
#include <vector>

#include <assert.h>
#include <iostream>
#include <float.h>
#include <math.h>



#include "CoinFloatEqual.h"
#include "eso2.h"
#include "globalAttrData.h"
#include "mtmData.h"
#include "featureData.h"
#include "loosePartData.h"
#include "mtmDemanBomData.h"
#include "alternatePartData.h"
#include "partInterplantData.h"
#include "partInterplantOverrideData.h"
#include "mtmInterplantData.h"
#include "mtmInterplantOverrideData.h"
#include "partDemandData.h"
#include "eso2Problem.h"


typedef std::vector<std::string> StdStringVec;

// Function to round acquire quantities
float acqQtyRnd(float x)
{
  //return ceil(x);
  return x;
}

float applyScaleToWfmv(float scale, float wfmv)
{
  //return scale*wfmv;
  return wfmv;
}


/*
------------
 
// Write partDemand in WAC*ShipVol order
void partDemandInWacTimesShipVolOrder(
                  const ESO2problem & eso2Prob,
                  bool writeHeader,
                  FILE * partDemandInWacTimesShipVolOrderFilePtr,
                  
                  bool writeZeros )
{
  if ( !writeHeader ) return;
  float wfmvScaleFactor = eso2Prob.getWfmvScaleFactor();
  float wfmv2ScaleFactor = eso2Prob.getWfmv2ScaleFactor();
  int nPeriods = eso2Prob.getNPeriods();
  {
    std::string heading;
    heading = "\"part\",\"custLoc\",\"plantLoc\",\"period\",\"demandVol\",\"wac\",\"etnFlag\"";
    if (writeHeader) fprintf(partShipReportFilePtr,"%s\n",heading.c_str());
    
    std::vector<std::string> parts;
    std::vector<std::string> custLocs;
    std::vector<std::string> plantLocs;
    eso2Prob.getPartDemands(parts,custLocs,plantLocs);

    std::map<float,int> demOrdList;
    
    // loop once for each shipped part
    size_t p;
    for ( p=0; p<parts.size(); p++ ) {
            
      std::vector<float> demandVol = eso2Prob.getPartDemandDemandVol(parts[p],custLocs[p],plantLocs[p]);
      std::vector<float> shipVol = eso2Prob.getPartDemandShipVol(parts[p],custLocs[p],plantLocs[p]);
      std::vector<float> wac = eso2Prob.getPartDemandWac(parts[p],custLocs[p],plantLocs[p]);




      
      
    } // finished p loop
  } // finished writing partShip file
} // end partDemandInWacTimesShipVol Order


-----------
*/
  
// Attempt to write report requested by Kevin
void writeShipReportForKevin(
                  const ESO2problem & eso2Prob,
                  bool writeHeader,
                  FILE * partShipReportFilePtr,
                  
                  bool writeZeros )
{
  
  float wfmvScaleFactor = eso2Prob.getWfmvScaleFactor();
  float wfmv2ScaleFactor = eso2Prob.getWfmv2ScaleFactor();
  int nPeriods = eso2Prob.getNPeriods();
  {
    std::string heading;
    heading="\"wfmvScaleFactor\",\"wfmv2ScaleFactor\",\"part\",\"custLoc\",\"plantLoc\",\"period\",\"demandVol\",\"shipVol\",\"altVol\",\"upstreamPart\",\"interPlantSrcLoc\",\"interPlantMoveVol\""; 
    if (writeHeader) fprintf(partShipReportFilePtr,"%s\n",heading.c_str());
    
    std::vector<std::string> parts;
    std::vector<std::string> custLocs;
    std::vector<std::string> plantLocs;
    eso2Prob.getPartDemands(parts,custLocs,plantLocs);
    
    // loop once for each shipped part
    size_t p;
    for ( p=0; p<parts.size(); p++ ) {
      
      //**int t;
      //**for ( t=0; t<eso2Prob.getNPeriods(); t++ ) {
      
      std::vector<float> demandVol = eso2Prob.getPartDemandDemandVol(parts[p],custLocs[p],plantLocs[p]);
      assert( demandVol.size() == nPeriods );
      std::vector<float> shipVol = eso2Prob.getPartDemandShipVol(parts[p],custLocs[p],plantLocs[p]);
      assert( shipVol.size() == nPeriods );
      std::vector<float> wac = eso2Prob.getPartDemandWac(parts[p],custLocs[p],plantLocs[p]);
      
      std::set<std::string> srcLocs = eso2Prob.getPartInterplantSrcLocs(parts[p],plantLocs[p]);
      std::set<std::string>::const_iterator it;
      for ( it=srcLocs.begin(); it!=srcLocs.end(); ++it ) {
        std::string srcLoc = *it;
        std::vector<float> moveVol = eso2Prob.getPartInterplantMoveVol(parts[p],srcLoc,plantLocs[p]);
        
        for ( size_t t=0; t<shipVol.size(); t++ ) {
          if ( moveVol[t]==0.0 && !writeZeros ) continue;
          if (moveVol[t]==0.0) continue;
          
          fprintf(partShipReportFilePtr,
            "%f,%f,\"%s\",\"%s\",\"%s\",%d,%f,%f,0.0,\" \",\"%s\",%f\n",
            wfmvScaleFactor,wfmv2ScaleFactor,parts[p].c_str(),custLocs[p].c_str(),plantLocs[p].c_str(),t+1,demandVol[t],shipVol[t],
            srcLoc.c_str(),moveVol[t]);
        } // finished t loop
      } // finished srcLoc it loop
      
      
      // determine if shipped part has an alternate
      std::set<std::string> partsUsedAsAlt = eso2Prob.getAltPartOrFeature(parts[p],plantLocs[p]);
      for ( it=partsUsedAsAlt.begin(); it!=partsUsedAsAlt.end(); ++it ) {
        std::string partUsedAsAlt = *it;
        std::vector<float> altVol = eso2Prob.getAltVol(partUsedAsAlt,parts[p],plantLocs[p]);
        
        std::set<std::string> srcLocs = eso2Prob.getPartInterplantSrcLocs(partUsedAsAlt,plantLocs[p]);
        std::set<std::string>::const_iterator it1;
        for ( it1=srcLocs.begin(); it1!=srcLocs.end(); ++it1 ) {
          std::string srcLoc = *it1;
          std::vector<float> moveVol = eso2Prob.getPartInterplantMoveVol(partUsedAsAlt,srcLoc,plantLocs[p]);
          
          for ( size_t t=0; t<shipVol.size(); t++ ) {
            if ( moveVol[t]==0.0 && !writeZeros && altVol[t]==0.0 ) continue;
            if (moveVol[t]==0.0) continue;
            
            fprintf(partShipReportFilePtr,
              "%f,%f,\"%s\",\"%s\",\"%s\",%d,%f,%f,%f,\"%s\",\"%s\",%f\n",
              wfmvScaleFactor,wfmv2ScaleFactor,parts[p].c_str(),custLocs[p].c_str(),plantLocs[p].c_str(),t+1,demandVol[t],shipVol[t],
              altVol[t],partUsedAsAlt.c_str(),
              srcLoc.c_str(),moveVol[t]);
          } // finished t loop
        } // finished srcLocs it1 loop
      } // finished partsUsedAsAlt it loop
      
      //** } // finished t loop
      
    } // finished p loop
  } // finished writing partShip file
} // Kevin's partShipReport



void writeResults(
      const ESO2problem & eso2Prob,
      bool writeHeader,
      FILE * mtmAcquirePlanFilePtr,
      FILE * featureAcquirePlanFilePtr,
      FILE * loosePartAcquirePlanFilePtr,
      FILE * partShipFilePtr,
      FILE * featureXrefFilePtr,
      FILE * altFilePtr,
      FILE * partInterplantFilePtr,

      FILE * peggedDemandPlanFilePtr,

      FILE * kpiFilePtr,
      
      double mtmTotalWfmvAtScale1,
      int    mtmNumUnique,
      double mtmSupplyQuantity,
      
      double featureTotalWfmvAtScale1,
      int    featureNumUnique,
      double featureSupplyQuantity,
      
      double loosePartTotalWfmvAtScale1,
      int    loosePartNumUnique,
      double loosePartSupplyQuantity,
      
      double  partDemandSumDemandWac,
      int     partDemandNumUnique,
      double  partDemandSumDemandVol,
      
      bool writeZeros )
{
  float wfmvScaleFactor = eso2Prob.getWfmvScaleFactor();
  float wfmv2ScaleFactor = eso2Prob.getWfmv2ScaleFactor();
  int nPeriods = eso2Prob.getNPeriods();

#if 0
  float mtmAcquireCost = 0.0f;
  float mtmDemanCost = 0.0f;
  float mtmInterplantCost = 0.0f;
  float featureAcquireCost = 0.0f;
  float loosePartAcquireCost = 0.0f;
  float partAcquireAvoidanceValue = 0.0f;
  float alternatePartCost = 0.0f;
  float partInterplantCost = 0.0f;
#endif
  
  // Open mtmAcquirePlan file and write header.
  {
    std::string heading;
    heading="\"wfmvScaleFactor\",\"wfmv2ScaleFactor\",\"mtm\",\"srcLoc\",\"acquireLoc\",\"period\",\"supply\",\"acquireQty\""; 
    if (writeHeader) fprintf(mtmAcquirePlanFilePtr,"%s\n",heading.c_str());
    
    std::vector<std::string> mtms;
    std::vector<std::string> srcLocs;
    std::vector<std::string> destLocs;
    eso2Prob.getMtmDemanLocs(mtms,srcLocs,destLocs);
    
    // loop once for each mtm
    size_t m;
    for ( m=0; m<mtms.size(); m++ ) {
      
      std::vector<float> acquireQty = eso2Prob.getMtmAcquireQty(mtms[m],srcLocs[m],destLocs[m]);
      std::vector<float> supply = eso2Prob.getMtmSupply(mtms[m],srcLocs[m]);
      //std::vector<float> wfmv = eso2Prob.getMtmWfmv(mtms[m],srcLocs[m]);
      //std::vector<float> demanCost = eso2Prob.getMtmDemanCost(mtms[m],srcLocs[m]);
      //std::vector<float> moveCost = eso2Prob.getMtmInterplantMoveCost(mtms[m],srcLocs[m],destLocs[m]);
      assert( acquireQty.size() == nPeriods );
      for ( size_t t=0; t<acquireQty.size(); t++ ) {
        if ( acquireQty[t]==0.0 && !writeZeros ) continue;
        fprintf(mtmAcquirePlanFilePtr,
          "%f,%f,\"%s\",\"%s\",\"%s\",%d,%f,%f\n",
          wfmvScaleFactor,wfmv2ScaleFactor,mtms[m].c_str(),srcLocs[m].c_str(),destLocs[m].c_str(),t+1,supply[t],acquireQty[t]);
        //mtmAcquireCost += acqQtyRnd(acquireQty[t])*applyScaleToWfmv(wfmvScaleFactor,wfmv[t]);
        //mtmDemanCost += acqQtyRnd(acquireQty[t])*demanCost[t];
        //mtmInterplantCost += acqQtyRnd(acquireQty[t])*moveCost[t];
      } // finished t loop
    } // finished m loop
  } // finished writing mtmAcquirePlan file
  
  // Open featureAcquirePlan file and write header.
  {
    std::string heading;
    heading="\"wfmvScaleFactor\",\"wfmv2ScaleFactor\",\"feature\",\"plantLoc\",\"period\",\"supply\",\"acquireQty\""; 
    if (writeHeader) fprintf(featureAcquirePlanFilePtr,"%s\n",heading.c_str());
    
    std::vector<std::string> features;
    std::vector<std::string> plantLocs;
    eso2Prob.getFeatures(features,plantLocs);
    
    // loop once for each feature
    size_t f;
    for ( f=0; f<features.size(); f++ ) {
      
      std::vector<float> acquireQty = eso2Prob.getFeatureAcquireQty(features[f],plantLocs[f]);
      std::vector<float> supply = eso2Prob.getFeatureSupply(features[f],plantLocs[f]);
      std::vector<float> wfmv = eso2Prob.getFeatureWfmv(features[f],plantLocs[f]);
      assert( acquireQty.size() == nPeriods );
      for ( size_t t=0; t<acquireQty.size(); t++ ) {
        if ( acquireQty[t]==0.0 && !writeZeros ) continue;
        fprintf(featureAcquirePlanFilePtr,
          "%f,%f,\"%s\",\"%s\",%d,%f,%f\n",
          wfmvScaleFactor,wfmv2ScaleFactor,features[f].c_str(),plantLocs[f].c_str(),t+1,supply[t],acquireQty[t]);
        //featureAcquireCost += acqQtyRnd(acquireQty[t])*applyScaleToWfmv(wfmvScaleFactor,wfmv[t]);
      } // finished t loop
    } // finished f loop
  } // finished writing featureAcquirePlan file

    
  // Open loosePartAcquirePlan file and write header.
  {
    std::string heading;
    heading="\"wfmvScaleFactor\",\"wfmv2ScaleFactor\",\"part\",\"plantLoc\",\"period\",\"supply\",\"acquireQty\""; 
    if (writeHeader) fprintf(loosePartAcquirePlanFilePtr,"%s\n",heading.c_str());
    
    std::vector<std::string> looseParts;
    std::vector<std::string> plantLocs;
    eso2Prob.getLooseParts(looseParts,plantLocs);
    
    // loop once for each loosePart
    size_t f;
    for ( f=0; f<looseParts.size(); f++ ) {
      
      std::vector<float> acquireQty = eso2Prob.getLoosePartAcquireQty(looseParts[f],plantLocs[f]);
      std::vector<float> supply = eso2Prob.getLoosePartSupply(looseParts[f],plantLocs[f]);
      std::vector<float> wfmv = eso2Prob.getLoosePartWfmv(looseParts[f],plantLocs[f]);
      assert( acquireQty.size() == nPeriods );
      for ( size_t t=0; t<acquireQty.size(); t++ ) {
        if ( acquireQty[t]==0.0 && !writeZeros ) continue;
        fprintf(loosePartAcquirePlanFilePtr,
          "%f,%f,\"%s\",\"%s\",%d,%f,%f\n",
          wfmvScaleFactor,wfmv2ScaleFactor,looseParts[f].c_str(),plantLocs[f].c_str(),t+1,supply[t],acquireQty[t]);
          //loosePartAcquireCost += acqQtyRnd(acquireQty[t])*applyScaleToWfmv(wfmvScaleFactor,wfmv[t]);
      } // finished t loop
    } // finished f loop
  } // finished writing loosePartAcquirePlan file
  
  // Open partShip file and write header.
  {
    std::string heading;
    heading="\"wfmvScaleFactor\",\"wfmv2ScaleFactor\",\"part\",\"custLoc\",\"plantLoc\",\"period\",\"demandVol\",\"shipVol\""; 
    if (writeHeader) fprintf(partShipFilePtr,"%s\n",heading.c_str());
    
    std::vector<std::string> parts;
    std::vector<std::string> custLocs;
    std::vector<std::string> plantLocs;
    eso2Prob.getPartDemands(parts,custLocs,plantLocs);
    
    // loop once for each shipped part
    size_t p;
    for ( p=0; p<parts.size(); p++ ) {
      
      std::vector<float> demandVol = eso2Prob.getPartDemandDemandVol(parts[p],custLocs[p],plantLocs[p]);
      assert( demandVol.size() == nPeriods );
      std::vector<float> shipVol = eso2Prob.getPartDemandShipVol(parts[p],custLocs[p],plantLocs[p]);
      assert( shipVol.size() == nPeriods );
      std::vector<float> wac = eso2Prob.getPartDemandWac(parts[p],custLocs[p],plantLocs[p]);
      
      for ( size_t t=0; t<shipVol.size(); t++ ) {
        if ( shipVol[t]==0.0 && !writeZeros ) continue;
        fprintf(partShipFilePtr,
          "%f,%f,\"%s\",\"%s\",\"%s\",%d,%f,%f\n",
          wfmvScaleFactor,wfmv2ScaleFactor,parts[p].c_str(),custLocs[p].c_str(),plantLocs[p].c_str(),t+1,demandVol[t],shipVol[t]);
        //partAcquireAvoidanceValue += shipVol[t]*wac[t];
      } // finished t loop
    } // finished p loop
  } // finished writing partShip file
  
  
  
  
  // Open featureXref file and write header.
  {
    std::string heading;
    heading="\"wfmvScaleFactor\",\"wfmv2ScaleFactor\",\"part\",\"feature\",\"plantLoc\",\"period\",\"xrefVol\""; 
    if (writeHeader) fprintf(featureXrefFilePtr,"%s\n",heading.c_str());
    
    std::vector<std::string> parts;
    std::vector<std::string> features;
    std::vector<std::string> plantLocs;
    eso2Prob.getFeatureXrefs(parts,features,plantLocs);
    
    // loop once for each part
    size_t e;
    for ( e=0; e<parts.size(); e++ ) {
      
      std::vector<float> xrefVol = eso2Prob.getFeatureXrefVol(parts[e],features[e],plantLocs[e]);
      assert( xrefVol.size() == nPeriods );
      //std::vector<float> xrefCost = eso2Prob.getFeatureXrefCost(parts[e],features[e],plantLocs[e]);
      for ( size_t t=0; t<xrefVol.size(); t++ ) {
        if ( xrefVol[t]==0.0 && !writeZeros ) continue;
        fprintf(featureXrefFilePtr,
          "%f,%f,\"%s\",\"%s\",\"%s\",%d,%f\n",
          wfmvScaleFactor,wfmv2ScaleFactor,parts[e].c_str(),features[e].c_str(),plantLocs[e].c_str(),t+1,xrefVol[t]);
        //alternatePartCost += ecVol[t]*ecCost[t];
      } // finished t loop
    } // finished e loop
  } // finished writing alternatePart file

  // Open alternatePart file and write header.
  {
    std::string heading;
    heading="\"wfmvScaleFactor\",\"wfmv2ScaleFactor\",\"part\",\"altPart\",\"loc\",\"period\",\"altVol\""; 
    if (writeHeader) fprintf(altFilePtr,"%s\n",heading.c_str());
    
    std::vector<std::string> partOrFeatures;
    std::vector<std::string> altParts;
    std::vector<std::string> locs;
    eso2Prob.getAlts(partOrFeatures,altParts,locs);
    
    // loop once for each altPart
    size_t e;
    for ( e=0; e<partOrFeatures.size(); e++ ) {    
      std::vector<float> altVol = eso2Prob.getAltVol(partOrFeatures[e],altParts[e],locs[e]);
      assert( altVol.size() == nPeriods );
      std::vector<float> altConvCost = eso2Prob.getAltConvCost(partOrFeatures[e],altParts[e],locs[e]);
      for ( size_t t=0; t<altVol.size(); t++ ) {
        if ( altVol[t]==0.0 && !writeZeros ) continue;
        fprintf(altFilePtr,
          "%f,%f,\"%s\",\"%s\",\"%s\",%d,%f\n",
          wfmvScaleFactor,wfmv2ScaleFactor,partOrFeatures[e].c_str(),altParts[e].c_str(),locs[e].c_str(),t+1,altVol[t]);
          //altCost += altVol[t]*altConvCost[t];
      } // finished t loop
    } // finished e loop
  } // finished writing alt file
  
  // Open partInterplant output file and write header.
  {
    std::string heading;
    heading="\"wfmvScaleFactor\",\"wfmv2ScaleFactor\",\"part\",\"srcLoc\",\"destLoc\",\"period\",\"moveVol\""; 
    if (writeHeader) fprintf(partInterplantFilePtr,"%s\n",heading.c_str());
    
    std::vector<std::string> parts;
    std::vector<std::string> srcLocs;
    std::vector<std::string> destLocs;
    eso2Prob.getPartInterplants(parts,srcLocs,destLocs);
    
    // loop once for each partInterplant part
    size_t i;
    for ( i=0; i<parts.size(); i++ ) {
      
      std::vector<float> moveVol = eso2Prob.getPartInterplantMoveVol(parts[i],srcLocs[i],destLocs[i]);
      std::vector<float> moveCost = eso2Prob.getPartInterplantMoveCost(parts[i],srcLocs[i],destLocs[i]);
      assert( moveVol.size() == nPeriods );
      for ( size_t t=0; t<moveVol.size(); t++ ) {
        if ( moveVol[t]==0.0 && !writeZeros ) continue;
        fprintf(partInterplantFilePtr,
          "%f,%f,\"%s\",\"%s\",\"%s\",%d,%f\n",
          wfmvScaleFactor,wfmv2ScaleFactor,parts[i].c_str(),srcLocs[i].c_str(),destLocs[i].c_str(),t+1,moveVol[t]);
        //partInterplantCost += moveVol[t]*moveCost[t];
      } // finished t loop
    } // finished i loop
  } // finished writing partInterplant file

    
  // Open pegged*AcquirePlan files and write header.
  {
    if (writeHeader) {
      std::string heading;
      heading="\"wfmvScaleFactor\",\"wfmv2ScaleFactor\",\"demandedPart\",\"custLoc\",\"plantLoc\",\"demandShipPeriod\",\"activity\",\"item\",\"loc\",\"activityPeriod\",\"activitySortAid\",\"qty\""; 
      fprintf(peggedDemandPlanFilePtr,"%s\n",heading.c_str());
    }
     
    std::vector<std::string> parts;
    std::vector<std::string> custLocs;
    std::vector<std::string> plantLocs;
    eso2Prob.getPartDemands(parts,custLocs,plantLocs);
    
    // loop once for each shipped part
    size_t p;
    for ( p=0; p<parts.size(); p++ ) {
      
      std::vector<float> demandVol = eso2Prob.getPartDemandDemandVol(parts[p],custLocs[p],plantLocs[p]);
      std::vector<float> shipVol = eso2Prob.getPartDemandShipVol(parts[p],custLocs[p],plantLocs[p]);

      // loop for each period
      int t;
      for (t=0; t<nPeriods; ++t ) {

        // if shipVol is not 0, then get pegging information
        if ( shipVol[t] != 0.0 ) {
          const std::set<std::string> & peggedAttributes = eso2Prob.getPartDemandPeggedAttributeNames(parts[p],custLocs[p],plantLocs[p],t);

          // loop once for each attribute pegged to demand
          std::set<std::string>::const_iterator it;
          for ( it=peggedAttributes.begin(); it!=peggedAttributes.end(); ++ it ) {

            std::vector<std::string> items,locs;
            std::vector<int> periods;
            std::vector<int> depths;
            std::vector<float> quantities;    
            eso2Prob.getPartDemandPegging(
              /*peggedAttributes[a],*/ *it,
              parts[p],custLocs[p],plantLocs[p],t,
              items, locs, periods, depths, quantities );

            // loop once for each item with attribute a pegged to demand p.
            for ( size_t i=0; i<items.size(); ++i ) {
              fprintf(peggedDemandPlanFilePtr,
                "%f,%f,\"%s\",\"%s\",\"%s\",%d,\"%s\",\"%s\",\"%s\",%d,%d,%f\n",
                wfmvScaleFactor,wfmv2ScaleFactor,parts[p].c_str(),custLocs[p].c_str(),plantLocs[p].c_str(),t+1,
                (*it).c_str(),items[i].c_str(),locs[i].c_str(),
                periods[i]+1,depths[i],quantities[i]);
            } // end of i loop
          } // End of a loop on each pegged attribute

        }
      } // end of t loop for each time period
    } // finished p loop for each shipped part
  } // finished writing pegged*AcquirePlan files
  
  // Write key performance indicator file.
  {
    std::string heading;
    heading =
      std::string("\"wfmvScaleFactor\",") +
      std::string("\"wfmv2ScaleFactor\",") +
      std::string("\"netValue\",") +

      std::string("\"sumWfmvAllMtms\",") +
      std::string("\"sumWfvmAcquiredMtms\",") +
      std::string("\"numUniqueMtms\",") +
      std::string("\"numUniqueAcquiredMtms\",") +
      std::string("\"supplyQtyAllMtms\",") +
      std::string("\"acquiredQtyMtms\",") +

      std::string("\"moveCostMtm\",") +
      std::string("\"demanCostMtm\",") +
      

      std::string("\"sumWfmvAllFeatures\",") +
      std::string("\"sumWfvmAcquiredFeatures\",") +
      std::string("\"numUniqueFeatures\",") +
      std::string("\"numUniqueAcquiredFeatures\",") +
      std::string("\"supplyQtyAllFeatures\",") +
      std::string("\"acquiredQtyFeatures\",") +

      std::string("\"conversionCostFeatures\",") +


      std::string("\"sumWfmvAllLooseParts\",") +
      std::string("\"sumWfvmAcquiredLooseParts\",") +
      std::string("\"numUniqueLooseParts\",") +
      std::string("\"numUniqueAcquiredLooseParts\",") +
      std::string("\"supplyQtyAllLooseParts\",") +
      std::string("\"acquiredQtyLooseParts\",") +
      
      std::string("\"sumWacAllPartDemands\",") +
      std::string("\"sumWacMetPartDemands\",") +
      std::string("\"numUniquePartDemandTriples\",") +
      std::string("\"numUniqueMetPartDemandTriples\",") +
      std::string("\"demandQtyAllPartDemandTriples\",") +
      std::string("\"metQtyAllPartDemandTriples\",") +

      std::string("\"moveCostParts\"");
    if (writeHeader) fprintf(kpiFilePtr,"%s\n",heading.c_str());

    double witObjVal = eso2Prob.getObjValue();
    double partMoveCost = eso2Prob.sumOfPartMoveCost();

    
    // Get metrics
    double mtmTotalWfmvWithScaleFactorTEMP;
    double mtmSumAcquireCost;
    double mtmSumMoveCost;
    double mtmSumDemanCost;
    int mtmNumUniqueTEMP;
    int mtmNumUniqueAcquired;
    double mtmSupplyQuantityTEMP;
    double mtmAcquiredQuantity;
    
    eso2Prob.mtmMetrics(
      mtmTotalWfmvWithScaleFactorTEMP,
      mtmSumAcquireCost,
      mtmSumMoveCost,
      mtmSumDemanCost,
      mtmNumUniqueTEMP,
      mtmNumUniqueAcquired,
      mtmSupplyQuantityTEMP,
      mtmAcquiredQuantity);

    //std::cout <<"mtmNumUniqueAcquired=" <<mtmNumUniqueAcquired <<" mtmAcquiredQuantity=" <<mtmAcquiredQuantity <<std::endl;
    
    double featureTotalWfmvWithScaleFactorTEMP;
    double featureSumAcquireCost;
    double featureSumXrefCost;
    int featureNumUniqueTEMP;
    int featureNumUniqueAcquired;
    double featureSupplyQuantityTEMP;
    double featureAcquiredQuantity;
    
    eso2Prob.featureMetrics(
      featureTotalWfmvWithScaleFactorTEMP,
      featureSumAcquireCost,
      featureSumXrefCost,
      featureNumUniqueTEMP,
      featureNumUniqueAcquired,
      featureSupplyQuantityTEMP,
      featureAcquiredQuantity);

    double loosePartTotalWfmvWithScaleFactorTEMP;
    double loosePartSumAcquireCost;
    double loosePartSumAltCost;
    int loosePartNumUniqueTEMP;
    int loosePartNumUniqueAcquired;
    double loosePartSupplyQuantityTEMP;
    double loosePartAcquiredQuantity;
    
    eso2Prob.loosePartMetrics(
      loosePartTotalWfmvWithScaleFactorTEMP,
      loosePartSumAcquireCost,
      loosePartSumAltCost,
      loosePartNumUniqueTEMP,
      loosePartNumUniqueAcquired,
      loosePartSupplyQuantityTEMP,
      loosePartAcquiredQuantity);
    
    double partDemandSumDemandWacTEMP;
    double partDemandSumShippedWac;
    int partDemandNumUniqueTEMP;
    int partDemandNumUniqueShipped;
    double partDemandSumDemandVolTEMP;
    double partDemandSumShipVol;
    std::map<std::string,double> sumShippedWacComponents;
    
    eso2Prob.partDemandMetrics(
      partDemandSumDemandWacTEMP,
      partDemandSumShippedWac,
      sumShippedWacComponents,
      partDemandNumUniqueTEMP,
      partDemandNumUniqueShipped,
      partDemandSumDemandVolTEMP,
      partDemandSumShipVol); 
        
    CoinRelFltEq eq(1.e-5);
    double calcObj=
      partDemandSumShippedWac -
      (mtmSumAcquireCost + featureSumAcquireCost + 
      loosePartSumAcquireCost + loosePartSumAltCost +
      mtmSumDemanCost +
      mtmSumMoveCost + partMoveCost +
      featureSumXrefCost);
    std::cout <<"calcObj=" <<calcObj <<" witObj=" <<witObjVal <<std::endl;
    //assert(eq(calcObj,witObjVal));
    std::cout <<"totalWac=" <<partDemandSumShippedWac <<std::endl;
    std::cout <<"  Sources of totalWac:" <<std::endl;
    std::map<std::string,double>::const_iterator it;
    for ( it=sumShippedWacComponents.begin(); it!=sumShippedWacComponents.end(); ++it ) {
      std::cout <<"  " <<it->first <<"=" <<it->second <<std::endl;
    }

    fprintf(kpiFilePtr,
      "%f,%f,%f,%f,%f,%d,%d,%f,%f,%f,%f,%f,%f,%d,%d,%f,%f,%f,%f,%f,%d,%d,%f,%f,%f,%f,%d,%d,%f,%f,%f\n",
      
      wfmvScaleFactor,
      wfmv2ScaleFactor,
      witObjVal,

      mtmTotalWfmvAtScale1*wfmvScaleFactor,
      mtmSumAcquireCost,
      mtmNumUnique,
      mtmNumUniqueAcquired,
      mtmSupplyQuantity,
      mtmAcquiredQuantity,
      mtmSumMoveCost,
      mtmSumDemanCost,

      featureTotalWfmvAtScale1*wfmvScaleFactor,
      featureSumAcquireCost,
      featureNumUnique,
      featureNumUniqueAcquired,
      featureSupplyQuantity,
      featureAcquiredQuantity,
      featureSumXrefCost,

      loosePartTotalWfmvAtScale1*wfmvScaleFactor,
      loosePartSumAcquireCost,
      loosePartNumUnique,
      loosePartNumUniqueAcquired,
      loosePartSupplyQuantity,
      loosePartAcquiredQuantity,
      
      partDemandSumDemandWac,
      partDemandSumShippedWac,
      partDemandNumUnique,
      partDemandNumUniqueShipped,
      partDemandSumDemandVol,
      partDemandSumShipVol,

      partMoveCost
      );
  }
  

} // end function to  writing results



int
main (int argc, char * argv[])
{

  std::cout <<"eso2 Build date: " <<__DATE__ <<std::endl;
  {
    
    // Put Parms into a vector
    std::vector<std::string> args;
    for (int i=0; i<argc; i++) 
      args.push_back(argv[i]);
    
    {
      // Check parm passed to program
      bool parmError = false;
      
      if ( args.size()> 1 ) {
        if (args[1]=="?" ) {
          parmError = true; // set flag to provide usage info
        }
      }
      
      if ( ! parmError ) {
        if ( args.size()!=2 ) {
          // Wrong number of parms. Write message
          std::cerr 
            <<"-------------------------------------------------------"  <<std::endl
            <<"Wrong number of parmeters specified"                      <<std::endl
            <<"  Number parameters specified: " <<(int)args.size()-1     <<std::endl
            <<"  Expected number of parameters: 1"                       <<std::endl
            <<"-------------------------------------------------------"  <<std::endl;
          parmError = true;
        }
      }
      
      
      
      if ( parmError ) {
        // Write correct usage
        std::cerr 
          <<"-------------------------------------------------------"  <<std::endl
          <<argv[0] <<" dataDirectory "                                <<std::endl
          <<"  where:"                                                 <<std::endl
          <<"    dataDirectory:"                                       <<std::endl
          <<"      Directory where input files are located and"        <<std::endl
          <<"      output files are written."                          <<std::endl
          <<"  example usage:"                                         <<std::endl
          <<"    " <<argv[0] <<" ../example1"                          <<std::endl
          <<"-------------------------------------------------------"  <<std::endl;
        return 1;
      }
    }
    
    std::string outputDirectory=args[1];
    std::string inputDirectory=args[1];
    ESO2problem eso2Prob;
    int nPeriods;
    
    int nWfmvScaleFactors;
    bool writeZeros=true;
    {
      // Get number of solves to be done (ie number of scale factors)
      std::string globalAttrFileName = inputDirectory + "/globalAttrData.csv";
      ESO2globalAttrData globalAttrFile(globalAttrFileName);
      nWfmvScaleFactors=globalAttrFile.fieldAsInt("nWfmvScaleFactors","attrValue");

      // determine if results with value of zero are to be written to output file;
      if ( globalAttrFile.keyExists("writeZeroResults") ) {
        std::string wzr = globalAttrFile.fieldAsString("writeZeroResults","attrValue");
        if ( wzr!="yes" && wzr!="no" ) {
          std::cout 
            <<"-------------------------------------------------------"       <<std::endl
            <<"Warning: Incorrect value for writeZeroResults specified."      <<std::endl
            <<"   globalAttr filename: " <<globalAttrFile.filename().c_str()  <<std::endl
            <<"   value specified: " <<wzr.c_str()                            <<std::endl
            <<"   expected values: yes no"                                    <<std::endl
            <<"   will assume writeZeroResults is yes "                          <<std::endl
            <<"-------------------------------------------------------"       <<std::endl;
        }
        else {
          if ( wzr=="no" )
            writeZeros=false;
        }
      }
      else {
        std::cout 
          <<"-------------------------------------------------------"       <<std::endl
          <<"Warning: Attribute writeZeroResults not specified."            <<std::endl
          <<"   globalAttr filename: " <<globalAttrFile.filename().c_str()  <<std::endl
          <<"   expected values: yes no"                                    <<std::endl
          <<"   will assume writeZeroResults is yes "                       <<std::endl
          <<"-------------------------------------------------------"       <<std::endl;
      }
    }
    
    eso2Prob.setSolverLogFileName(outputDirectory+"/clpMesgFile.txt");
    bool useGlobalAttrFileSettings = true;
    createModel(eso2Prob, inputDirectory, useGlobalAttrFileSettings);
    nPeriods = eso2Prob.getNPeriods();

    
    //---------------------------------------------------------
    
    std::string mtmAcquirePlanFileName = outputDirectory+"/mtmAcquirePlanO.csv";
    FILE * mtmAcquirePlanFilePtr = fopen(mtmAcquirePlanFileName.c_str(),"w");
    
    std::string featureAcquirePlanFileName = outputDirectory+"/featureAcquirePlanO.csv";
    FILE * featureAcquirePlanFilePtr = fopen(featureAcquirePlanFileName.c_str(),"w");
    
    std::string loosePartAcquirePlanFileName = outputDirectory+"/loosePartAcquirePlanO.csv";
    FILE * loosePartAcquirePlanFilePtr = fopen(loosePartAcquirePlanFileName.c_str(),"w");
    
    std::string partShipFileName = outputDirectory+"/partShipO.csv";
    FILE * partShipFilePtr = fopen(partShipFileName.c_str(),"w");

    std::string partShipReportFileName = outputDirectory+"/partShipReportO.csv";
    FILE * partShipReportFilePtr = fopen(partShipReportFileName.c_str(),"w");
    
    std::string featureXrefFileName = outputDirectory+"/featureXrefO.csv";
    FILE * featureXrefFilePtr = fopen(featureXrefFileName.c_str(),"w");
    
    std::string altFileName = outputDirectory+"/altO.csv";
    FILE * altFilePtr = fopen(altFileName.c_str(),"w");
    
    std::string partInterplantFileName = outputDirectory+"/partInterplantO.csv";
    FILE * partInterplantFilePtr = fopen(partInterplantFileName.c_str(),"w");

    std::string peggedDemandPlanFileName = outputDirectory+"/peggedDemandPlanO.csv";
    FILE * peggedDemandPlanFilePtr = fopen(peggedDemandPlanFileName.c_str(),"w");
    
    std::string kpiFileName = outputDirectory+"/keyPerformanceIndicatorsO.csv";
    FILE * kpiFilePtr = fopen(kpiFileName.c_str(),"w");
    
    //---------------------------------------------------------
    
    // Get some metrics before prunning
    double mtmTotalWfmvAtScale1;
    double mtmSumAcquireCostTemp;
    double mtmSumMoveCostTemp;
    double mtmSumDemanCostTemp;
    int mtmNumUnique;
    int mtmNumUniqueAcquiredTemp;
    double mtmSupplyQuantity;
    double mtmAcquiredQuantityTemp;
    
    eso2Prob.mtmMetrics(
      mtmTotalWfmvAtScale1,
      mtmSumAcquireCostTemp,
      mtmSumMoveCostTemp,
      mtmSumDemanCostTemp,
      mtmNumUnique,
      mtmNumUniqueAcquiredTemp,
      mtmSupplyQuantity,
      mtmAcquiredQuantityTemp);
    
    double featureTotalWfmvAtScale1;
    double featureSumAcquireCostTemp;
    double featureSumXrefCostTemp;
    int featureNumUnique;
    int featureNumUniqueAcquiredTemp;
    double featureSupplyQuantity;
    double featureAcquiredQuantityTemp;
    
    eso2Prob.featureMetrics(
      featureTotalWfmvAtScale1,
      featureSumAcquireCostTemp,
      featureSumXrefCostTemp,
      featureNumUnique,
      featureNumUniqueAcquiredTemp,
      featureSupplyQuantity,
      featureAcquiredQuantityTemp);

    double loosePartTotalWfmvAtScale1;
    double loosePartSumAcquireCostTemp;
    double loosePartSumAltCost;
    int loosePartNumUnique;
    int loosePartNumUniqueAcquiredTemp;
    double loosePartSupplyQuantity;
    double loosePartAcquiredQuantityTemp;
    
    eso2Prob.loosePartMetrics(
      loosePartTotalWfmvAtScale1,
      loosePartSumAcquireCostTemp,
      loosePartSumAltCost,
      loosePartNumUnique,
      loosePartNumUniqueAcquiredTemp,
      loosePartSupplyQuantity,
      loosePartAcquiredQuantityTemp);
    
    double partDemandSumDemandWac;
    double partDemandSumShippedWacTemp;
    int partDemandNumUnique;
    int partDemandNumUniqueShippedTemp;
    double partDemandSumDemandVol;
    double partDemandSumShipVolTemp;
    std::map<std::string,double> sumShippedWacComponents;
    
    eso2Prob.partDemandMetrics(
      partDemandSumDemandWac,
      partDemandSumShippedWacTemp,
      sumShippedWacComponents,
      partDemandNumUnique,
      partDemandNumUniqueShippedTemp,
      partDemandSumDemandVol,
      partDemandSumShipVolTemp);
           

    //eso2Prob.writeWitData(outputDirectory + "/eso2.wit");
    eso2Prob.pruneWitModel();
    //eso2Prob.writeWitData(outputDirectory + "/example3.wit");


    std::string globalAttrFileName = inputDirectory + "/globalAttrData.csv";
    ESO2globalAttrData globalAttrFile(globalAttrFileName);

    // Loop once for each scale factor
    int s;
    for ( s=1; s<=nWfmvScaleFactors;  ++s ) {
      
      // get scale factor for this solve
      char sAsChar[100];
      sprintf(sAsChar,"%d",s);
      std::string attrName = std::string("wfmvScaleFactor")+std::string(sAsChar);
      float wfmvScaleFactor = globalAttrFile.fieldAsFloat(attrName,"attrValue");
      std::string attr2Name = std::string("wfmv2ScaleFactor")+std::string(sAsChar);
      float wfmv2ScaleFactor = globalAttrFile.fieldAsFloat(attr2Name,"attrValue");
      eso2Prob.setWfmvScaleFactor(wfmvScaleFactor,wfmv2ScaleFactor);

      // Solve
      std::cout 
        <<"Solving with wfmvScaleFactor="<<wfmvScaleFactor
        <<" and wfmv2ScaleFactor="<<wfmv2ScaleFactor
        <<". solve " <<s <<" of " <<nWfmvScaleFactors <<"."
        <<std::endl;
      eso2Prob.solveWithPegging();
      
      
      
      // Write Results
      std::cout <<"Writing results." <<std::endl;
      bool writeHeader = false;
      if ( s==1 ) writeHeader = true;
      writeResults(
        eso2Prob,
        writeHeader,
        mtmAcquirePlanFilePtr,
        featureAcquirePlanFilePtr,
        loosePartAcquirePlanFilePtr,
        partShipFilePtr,
        featureXrefFilePtr,
        altFilePtr,
        partInterplantFilePtr,
        peggedDemandPlanFilePtr,
        kpiFilePtr,
        
        mtmTotalWfmvAtScale1,
        mtmNumUnique,
        mtmSupplyQuantity,
        
        featureTotalWfmvAtScale1,
        featureNumUnique,
        featureSupplyQuantity,
        
        loosePartTotalWfmvAtScale1,
        loosePartNumUnique,
        loosePartSupplyQuantity,
        
        partDemandSumDemandWac,
        partDemandNumUnique,
        partDemandSumDemandVol,
        
        writeZeros );
      
      writeShipReportForKevin(eso2Prob,writeHeader,partShipReportFilePtr,writeZeros );

      
    } // end of s loop

    //eso2Prob.reportPartDemand("0000000P1690","93B","93B",24-1);
    //eso2Prob.reportPartDemand("p1","cust","sim",3-1);
    //eso2Prob.reportPartDemand("pn1","980","980",1-1);
    //eso2Prob.reportPartDemand("0000053P6023","980","980",12-1);
    //eso2Prob.reportPartDemand("u3","kgn","wat",4-1);
    //eso2Prob.reportPartDemand("u3","kgn","wat",1-1);
    //eso2Prob.reportPartDemand("u3","kgn","wat",2-1);


    
    //--------------------------------------
    fclose(mtmAcquirePlanFilePtr);
    fclose(featureAcquirePlanFilePtr);
    fclose(partShipFilePtr);
    fclose(partShipReportFilePtr);
    fclose(featureXrefFilePtr);
    fclose(partInterplantFilePtr);
    fclose(peggedDemandPlanFilePtr);
    fclose(kpiFilePtr);
    //--------------------------------------


#if 0
      // Get list of shortages and print 
      if ( shortages )
      {          
        std::vector<std::string> resourceDescriptions;
        std::vector<int> periods;
        std::vector<float> shortageVols;
        eso2Prob.getResourceShortages(resourceDescriptions,periods,shortageVols );
        if ( periods.size()>0 )
          printf("Resource Shortages:");
        int s;
        for ( s=0; s<periods.size(); s++ ) {
          printf("%d, %f, %s\n",periods[s],shortageVols[s],resourceDescriptions[s].c_str());
        }
      }
#endif
      
  }

  std::cout <<"eso2 application is finished." <<std::endl;
  return 0;
}
