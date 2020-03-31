#include <numeric>
#include <set>

#include <cassert>
#include <iostream>
#include <cfloat>
#include <cstdio>
#include <climits>

#include <wit.h>

#include "VisorFloatEqual.h"
#include "onHandMaterial.h"
#include "printer.h"
#include "requestQuantity.h"
#include "visorProblem1.h"


//
//----------------------
// Material Methods
//----------------------
void VISORproblem1::addMaterial    (const std::string & location, const std::string & filamentSize, const std::string & plasticType, float quantity, int sharePercent )
{
	assert(filamentSize=="1.75mm"||filamentSize=="2.85mm");
   assert(plasticType=="PETG"||plasticType=="PLA"||plasticType=="ABS"||plasticType=="ONYX");

	std::string ownMatName = ownMaterialName(location, filamentSize, plasticType );
	std::string shrMatName = shrMaterialName(location, filamentSize, plasticType );

	witAddPart(witRun(), ownMatName.c_str(), WitMATERIAL);
	witAddPart(witRun(), shrMatName.c_str(), WitCAPACITY);

	float shrQty = quantity*(float)sharePercent/100.0f;
	float ownQty = quantity-shrQty;
	witSetNameAttribute(&witGetPartSupplyVol,witSetPartSupplyVol,ownMatName,0,ownQty);
	witSetNameAttribute(&witGetPartSupplyVol,witSetPartSupplyVol,shrMatName,0,shrQty);

	std::string baseName = baseMaterialName(location, filamentSize, plasticType);
	materialBaseNames_.insert(baseName);
}

void VISORproblem1::getMaterials(
    std::vector<std::string> & locs,
    std::vector<std::string> & filamentSizes,
    std::vector<std::string> & pTypes )
{
  locs.clear();
  filamentSizes.clear();
  pTypes.clear();
#if 0
  int nMtms = materialBaseNames_.size();
  int m;
  for ( m=0; m<nMtms; m++ ) {
    locs.push_back( locationFromMaterialName(materialBaseNames_[m]) );
    filamentSizes.push_back( filamentSizeFromMaterialName(materialBaseNames_[m]) );
    pTypes.push_back( plasticTypeFromMaterialName(materialBaseNames_[m]) );
  }
#else
  std::set<std::string>::const_iterator it;
  for ( it=materialBaseNames_.begin(); it!=materialBaseNames_.end(); ++it ) {
    locs.push_back( locationFromMaterialName(*it) );
    filamentSizes.push_back( filamentSizeFromMaterialName(*it) );
    pTypes.push_back( plasticTypeFromMaterailName(*it) );
  }
#endif
}

std::vector<float> VISORproblem1::getOwnSupply(
             const std::string & loc,
             const std::string & nSize,
             const std::string & pType )
{
  std::vector<float> retVal;
  std::string name = ownMaterialName(loc,nSize,pType);
  retVal = witGetNameAttribute(witGetPartSupplyVol,name);
  return retVal;
}

std::vector<float> VISORproblem1::getSharedSupply(
             const std::string & loc,
             const std::string & nSize,
             const std::string & pType )
{
  std::vector<float> retVal;
  std::string name = shrMaterialName(loc,nSize,pType);
  retVal = witGetNameAttribute(witGetPartSupplyVol,name);
  return retVal;
}

//
//----------------------
// Printer Methods
//----------------------
void VISORproblem1::addPrinter(
    const std::string & name,
    const std::string & location,
    float prodRate,
    bool F175, bool F285, bool petg, bool pla, bool abs, bool onyx)
{
	std::string printerNm      = printerName(name, location );
	std::string printerOperNm  = printerOperName(name, location );
	std::string noSupplyPartNm = noSupplyPartName(name, location );
	std::string visorPartNm    = visorPartName(name, location );

	// Add parts: Printer, NoSupply, and produced visor
	witAddPart(witRun(), printerNm.c_str(),      WitCAPACITY);
	witAddPart(witRun(), noSupplyPartNm.c_str(), WitMATERIAL);
	witAddPart(witRun(), visorPartNm.c_str(),    WitCAPACITY);

	// Add opertaion
	witAddOperation(witRun(),printerOperNm.c_str());

	// Add bom connecting operation to printer and noSupplyPart
	witAddBomEntry(witRun(), printerOperNm.c_str(), printerNm.c_str());
	witAddBomEntry(witRun(), printerOperNm.c_str(), noSupplyPartNm.c_str());

	// Connect operation to produced visor
	witAddBopEntry(witRun(), printerOperNm.c_str(), visorPartNm.c_str());

	// Set printer supply volume to be the number that can be produced in a day
	std::vector<float> sv=floatToStlVec(prodRate);
   witSetNameAttribute(witSetPartSupplyVol,printerNm,sv);

   float sumSupplyVol=1;
   for ( int i=0; i<sv.size();i++ ) sumSupplyVol += sv[i];

   // Add Demand for visor and set demandVol to big M
   witAddDemand(witRun(),visorPartNm.c_str(),"demand");
   std::vector<float> dv=floatToStlVec(sumSupplyVol);
   witSetDemandAttribute(witSetDemandDemandVol,visorPartNm,"demand",dv);

   std::vector<float> cumShipRew=floatToStlVec(10.);
   witSetDemandAttribute(witSetDemandCumShipReward,visorPartNm,"demand",cumShipRew);
   std::vector<float> shipRew=floatToStlVec(100.);
   witSetDemandAttribute(witSetDemandShipReward,visorPartNm,"demand",shipRew);

   //Subs Boms for all materaials printer can use
   {
      std::vector<std::string> matLoc, filamentSize, plasticType;
      getMaterials( matLoc, filamentSize, plasticType );

      float * ownSubCost = floatToConstFloatStar(0.0f);
      float * ownShrSubCost = floatToConstFloatStar(1.0f);
      float * shrSubCost = floatToConstFloatStar(2.0f);
      float * shipOffset = floatToConstFloatStar(1.0f);

      //Loop once for each material
      int nSubBomEntries = 0;
      for (int i=0; i<matLoc.size(); i++)
      {
      	// Determine if i'th material can be used with this printer
      	//std::cout <<F175 <<" " <<F285 <<" " <<filamentSize[i]+"\n";
      	//std::cout <<petg <<" " <<pla <<" " <<abs <<" " <<onyx <<" " <<plasticType[i]+"\n";
      	//std::cout <<location <<" " <<matLoc[i] <<"\n";
      	bool nozOk  = ( F175 && filamentSize[i]=="1.75mm" ) || ( F285 && filamentSize[i]=="2.85mm" );
      	bool typeOk = ( petg && plasticType[i]=="PETG" )
      	            ||( pla  && plasticType[i]=="PLA" )
      	            ||( abs  && plasticType[i]=="ABS" )
      	            ||( onyx && plasticType[i]=="ONYX" );
      	//std::cout <<"nozOk " <<nozOk <<"typeOk " <<typeOk <<std::endl;
      	if (nozOk && typeOk)
      	{
      		// Material is good to use, so add SubBom Entry
	         std::string shrMatName = shrMaterialName(matLoc[i], filamentSize[i], plasticType[i] );
            witAddSubsBomEntry(witRun(),printerOperNm.c_str(),1,shrMatName.c_str());
            
            // Set the consRate to be 1/60  (60 per kg)
            float * consRate = floatToConstFloatStar(1./60.);
            witSetSubsBomEntryConsRate(witRun(),printerOperNm.c_str(),1,nSubBomEntries,consRate);
            delete [] consRate;

            if ( location==matLoc[i] )
            {
            	// ownd Share sub cost
               witSetSubsBomEntrySubCost(witRun(),printerOperNm.c_str(),1,nSubBomEntries,ownShrSubCost);
            }
            else {
            	// Share sub cost
               witSetSubsBomEntrySubCost(witRun(),printerOperNm.c_str(),1,nSubBomEntries,shrSubCost);
               // set shipping offset 1
               witSetSubsBomEntryOffset(witRun(),printerOperNm.c_str(),1,nSubBomEntries,shipOffset);

            }
            nSubBomEntries++;

            // if material is local then add own supply
            if ( location==matLoc[i] )
            {
            	std::string ownMatName = ownMaterialName(matLoc[i], filamentSize[i], plasticType[i] );
               witAddSubsBomEntry(witRun(),printerOperNm.c_str(),1,ownMatName.c_str());
               witSetSubsBomEntrySubCost(witRun(),printerOperNm.c_str(),1,nSubBomEntries,ownSubCost);
               nSubBomEntries++;
            }
      	}
      }
      delete [] ownSubCost;
      delete [] ownShrSubCost;
      delete [] shrSubCost;
      delete [] shipOffset;
   }

	std::string baseName = basePrinterName(name,location);
	printerBaseNames_.insert(baseName);
}

void VISORproblem1::getPrinters(
    std::vector<std::string> & names,
    std::vector<std::string> & locs )
{
  names.clear();
  locs.clear();
  std::set<std::string>::const_iterator it;
  for ( it=printerBaseNames_.begin(); it!=printerBaseNames_.end(); ++it ) {
  	 names.push_back( printerFromPrinterName(*it) );
    locs.push_back( locationFromPrinterName(*it) );
  }
}

std::vector<float> VISORproblem1::getPrinterProdRate(
             const std::string & name,
             const std::string & loc )
{
  std::vector<float> retVal;
  std::string nm = printerName(name,loc);
  retVal = witGetNameAttribute(witGetPartSupplyVol,nm);
  return retVal;
}
std::vector<float> VISORproblem1::getPrinterShipVol(
             const std::string & name,
             const std::string & loc )
{
  std::vector<float> retVal;
  std::string visorPartNm = visorPartName(name, loc);
  retVal = witGetDemandAttribute(witGetDemandShipVol,visorPartNm,"demand");
  return retVal;
}

std::vector<float> VISORproblem1::getPrinterProdVol(
             const std::string & name,
             const std::string & loc )
{
  std::vector<float> retVal;
  std::string operNm = printerOperName(name, loc);
  retVal = witGetNameAttribute(witGetOperationExecVol,operNm);
  return retVal;
}
  std::vector<float> getPrinterProdVol(const std::string & name, const std::string & loc );
//------------------------------------------
// Subs Bom Entry methods
//-----------------------------------------
void VISORproblem1::getSubVol(
            std::vector<std::string> & printerName, std::vector<std::string> & printerLoc,
            std::vector<std::string> & matLoc, std::vector<std::string> & matSize, std::vector<std::string> &matType,
            std::vector< std::vector<float>> &subVol, std::vector<std::string> & own )
{
      printerName.clear();
      printerLoc.clear();
      matLoc.clear();
      matSize.clear();
      matType.clear();
      subVol.clear();
      own.clear();
      witAttr objItrState;
      witGetObjItrState(witRun(), &objItrState);
      while( true )   {
         witAdvanceObjItr(witRun());
         witGetObjItrState(witRun(), &objItrState);
         if (objItrState==WitINACTIVE) break;
         if( objItrState == WitAT_SUB_ENTRY ) {
           char * operationName;
           int bomEntry;
           int subEntry;
           witGetObjItrSubsBomEntry(witRun(),&operationName, &bomEntry, &subEntry);
           std::vector<float> sv = witGetSubArcAttribute(witGetSubsBomEntrySubVol,operationName,bomEntry,subEntry);

           printerName.push_back( printerFromPrinterName(operationName) );
           printerLoc.push_back( locationFromPrinterName(operationName) );

           char * consPart;
           witGetSubsBomEntryConsumedPart(witRun(),operationName,bomEntry,subEntry,&consPart);

           matLoc.push_back(locationFromMaterialName(consPart));
           matSize.push_back(filamentSizeFromMaterialName(consPart));;
           matType.push_back(plasticTypeFromMaterailName(consPart));
           subVol.push_back(sv);
           if( ownSupply(consPart) )own.push_back("yes");
           else own.push_back("no");

           witFree(consPart);
           witFree(operationName);
        }
     }
}

// -----------------------------
// solver methods
// ----------------------------
void VISORproblem1::solve(bool useOptImplode)
{
	if (useOptImplode) witOptImplode(witRun());
	else witHeurImplode(witRun());
}




//-------------------------------------------------------------------------
// material Name Methods
//-------------------------------------------------------------------------
std::string VISORproblem1::ownMaterialName(const std::string & location, const std::string & filamentSize, const std::string & plasticType )
{
  return "ownSupply: "+baseMaterialName(location, filamentSize, plasticType);
}
std::string VISORproblem1::shrMaterialName(const std::string & location, const std::string & filamentSize, const std::string & plasticType )
{
  return "shareableSupply: "+baseMaterialName(location, filamentSize, plasticType);
}
std::string VISORproblem1::baseMaterialName(const std::string & location, const std::string & filamentSize, const std::string & plasticType )
{
  return " Size "+filamentSize+" Type "+plasticType+" at "+location;
}
std::string VISORproblem1::locationFromMaterialName(const std::string & matName)
{
  return textAfter(matName," at ");
}
std::string VISORproblem1::filamentSizeFromMaterialName(const std::string & matName)
{
  return textBetween(matName," Size "," Type ");
}
std::string VISORproblem1::plasticTypeFromMaterailName(const std::string & matName)
{
  return textBetween(matName," Type "," at ");
}
bool VISORproblem1::ownSupply(const std::string & matName)
{
   bool retVal;
   if ( beginsWith(matName,"shareableSupply: ") )
      retVal=false;
   else
   {
   	//assert( beginsWith(matName,"shareableSupply: ") );
   	retVal = true;
   }
   return retVal;
}

//-------------------------------------------------------------------------
// printer Name Methods
//-------------------------------------------------------------------------
std::string VISORproblem1::printerName(const std::string & name, const std::string & location )
{
  return basePrinterName(name,location);
}
std::string VISORproblem1::printerOperName(const std::string & name, const std::string & location )
{
  return "Make on "+basePrinterName(name,location);
}
std::string VISORproblem1::noSupplyPartName(const std::string & name, const std::string & location )
{
  return "No supply part for "+basePrinterName(name,location);
}
std::string VISORproblem1::visorPartName(const std::string & name, const std::string & location )
{
  return "Visor made on "+basePrinterName(name,location);
}
std::string VISORproblem1::basePrinterName(const std::string & name, const std::string & location )
{
  return "Printer: "+name+" at-> "+location;
}
std::string VISORproblem1::printerFromPrinterName(const std::string & baseName)
{
  return textBetween(baseName,"Printer: "," at-> ");
}
std::string VISORproblem1::locationFromPrinterName(const std::string & baseName)
{
  return textAfter(baseName," at-> ");
}


//-------------------------------------------------------------------------
// text utilities Methods
//-------------------------------------------------------------------------
std::string VISORproblem1::textBetween(
                        const std::string & src,
                        const std::string & begMark,
                        const std::string & endMark)
{
  size_t begPos = src.find(begMark);
  assert( begPos != std::string::npos );
  begPos = begPos+begMark.size();

  size_t endPos = src.find(endMark);
  assert( endPos != std::string::npos );

  size_t len = endPos-begPos;
  std::string retVal = src.substr(begPos,len);
  return retVal;
}
std::string VISORproblem1::textAfter(
                        const std::string & src,
                        const std::string & begMark)
{
  size_t begPos = src.find(begMark);
  assert( begPos != std::string::npos );
  begPos = begPos+begMark.size();

  size_t endPos = src.size();

  size_t len = endPos-begPos;
  std::string retVal = src.substr(begPos,len);
  return retVal;
}
bool VISORproblem1::beginsWith(
                        const std::string & src,
                        const std::string & begString)
{
  size_t begPos = src.find(begString);
  if ( begPos==0 ) return true;
  else return false;
}
bool VISORproblem1::contains(
                        const std::string & haystack,
                        const std::string & needle)
{
  size_t pos = haystack.find(needle);
  if ( pos!=std::string::npos ) return true;
  else return false;
}



//-------------------------------------------------------------------------
// Utilities for invoking Wit Methods
//-------------------------------------------------------------------------
// Set element of Part/Operation float* attribute, given period and float
void VISORproblem1::witSetNameAttribute(
    witGetNameFloatStarStar witGetFunc,
    witSetNameFloatStar     witSetFunc,
    const std::string & name,
    int period,
    float flt )
{
  assert( period>= 0 );
  assert( period<getNPeriods() );
  float * floatStar;
  witGetFunc(mutableWitRun(),name.c_str(),&floatStar);
  floatStar[period] = flt;
  witSetFunc(witRun(),name.c_str(),floatStar);
  witFree(floatStar);
}

// Set Part/Operation float* attribute, given std::vector<float>
void VISORproblem1::witSetNameAttribute(
    witSetNameFloatStar     witSetFunc,
    const std::string & name,
    const std::vector<float> & value )
{
  float * fltStar = stlVecToFloatStar(value);
  witSetFunc(witRun(),name.c_str(),fltStar);
  delete [] fltStar;
}

// Set Part/Operation float* attribute, given float
void VISORproblem1::witSetNameAttribute(
    witSetNameFloatStar     witSetFunc,
    const std::string & name,
    float value )
{
  float * fltStar = floatToConstFloatStar(value);
  witSetFunc(witRun(),name.c_str(),fltStar);
  delete [] fltStar;
}

// Set Part/Operation bound attribute, given three std::vector<float> vectors
void VISORproblem1::witSetNameBoundAttribute(
    witSetName3FloatStar     witSetFunc,
    const std::string & name,
    const std::vector<float> & hlb,
    const std::vector<float> & slb,
    const std::vector<float> & hub )
{
  float * hlbFltStar = stlVecToFloatStar(hlb);
  float * slbFltStar = stlVecToFloatStar(slb);
  float * hubFltStar = stlVecToFloatStar(hub);
  witSetFunc(witRun(),name.c_str(),hlbFltStar, slbFltStar, hubFltStar);
  delete [] hlbFltStar;
  delete [] slbFltStar;
  delete [] hubFltStar;
}
// Get Part/Operation bound attributes, given three std::vector<float> vectors
void VISORproblem1::witGetNameBoundAttribute(
    witGetName3FloatStar     witGetFunc,
    const std::string & name,
    std::vector<float> & hlb,
    std::vector<float> & slb,
    std::vector<float> & hub )const
{
  float * hlbFltStar;
  float * slbFltStar;
  float * hubFltStar;
  witGetFunc(mutableWitRun(),name.c_str(),&hlbFltStar, &slbFltStar, &hubFltStar);
  hlb=witFloatStarToStlVec(hlbFltStar);
  slb=witFloatStarToStlVec(slbFltStar);
  hub=witFloatStarToStlVec(hubFltStar);
  witFree(hlbFltStar);
  witFree(slbFltStar);
  witFree(hubFltStar);
}



// Set element of Bop/Bop Entry float* attribute, given period and float
void VISORproblem1::witSetArcAttribute(
    witGetNameIndexFloatStarStar witGetFunc,
    witSetNameIndexFloatStar     witSetFunc,
    const std::string & opName,
    int index,
    int period,
    float flt )
{
  assert( period>= 0 );
  assert( period<getNPeriods() );
  //int bei = getBopEntryIndex(opName,partName);
  //assert( bei!=-1 );
  float * floatStar;
  witGetFunc(mutableWitRun(),opName.c_str(),index,&floatStar);
  floatStar[period] = flt;
  witSetFunc(witRun(),opName.c_str(),index,floatStar);
  witFree(floatStar);
}

// Set element of Bop/Bop Entry float* attribute, given std::vector<float>
void VISORproblem1::witSetArcAttribute(
    witSetNameIndexFloatStar     witSetFunc,
    const std::string & opName,
    int index,
    const std::vector<float> & value )
{
  float * fltStar = stlVecToFloatStar(value);
  witSetFunc(witRun(),opName.c_str(),index,fltStar);
  delete []  fltStar;
}

// Set element of Bop/Bop Entry float* attribute, given float
void VISORproblem1::witSetArcAttribute(
    witSetNameIndexFloatStar     witSetFunc,
    const std::string & opName,
    int index,
    float value )
{
  float * fltStar = floatToConstFloatStar(value);
  witSetFunc(witRun(),opName.c_str(),index,fltStar);
  delete []  fltStar;
}



// Set element on demand float* attribute, given period and fltValue
void VISORproblem1::witSetDemandAttribute(
    witGetDblNameFloatStarStar      witGetFunc,
    witSetDblNameFloatStar          witSetFunc,
    const std::string & partName,
    const std::string & demandName,
    int period,
    float flt )
{
  assert( period>= 0 );
  assert( period<getNPeriods() );
  float * floatStar;
  witGetFunc(mutableWitRun(),partName.c_str(),demandName.c_str(),&floatStar);
  floatStar[period] = flt;
  witSetFunc(witRun(),partName.c_str(),demandName.c_str(),floatStar);
  witFree(floatStar);
}

// Set element on demand float* attribute, given stl::vector<float>
void VISORproblem1::witSetDemandAttribute(
    witSetDblNameFloatStar          witSetFunc,
    const std::string & partName,
    const std::string & demandName,
    const std::vector<float> & fltVec )
{
  float * floatStar = stlVecToFloatStar(fltVec);
  witSetFunc(witRun(),partName.c_str(),demandName.c_str(),floatStar);
  delete [] floatStar;
}

// Get vector of wit part/Operation float* attribute
std::vector<float> VISORproblem1::witGetNameAttribute(
    witGetNameFloatStarStar witGetFunc,
    const std::string & name ) const
{
  float * attr;
  witGetFunc(mutableWitRun(),name.c_str(),&attr);
  std::vector<float> retVal = witFloatStarToStlVec(attr);
  witFree(attr);
  return retVal;
}

// Get vector of bom/bop float* attribute
std::vector<float> VISORproblem1::witGetArcAttribute(
                                     witGetNameIndexFloatStarStar     witGetFunc,
                                     const std::string & opName,
                                     int index )const
{
  float * attr;
  witGetFunc(mutableWitRun(),opName.c_str(),index,&attr);
  std::vector<float> retVal = witFloatStarToStlVec(attr);
  witFree(attr);
  return retVal;
}

// Get vector of bom/bop int* attribute
std::vector<int> VISORproblem1::witGetArcAttribute(
                                     witGetNameIndexIntStarStar     witGetFunc,
                                     const std::string & opName,
                                     int index )const
{
  int * attr;
  witGetFunc(mutableWitRun(),opName.c_str(),index,&attr);
  std::vector<int> retVal = witIntStarToStlVec(attr);
  witFree(attr);
  return retVal;
}



// Get vector of sub float* attribute
std::vector<float> VISORproblem1::witGetSubArcAttribute(
                                     witGetNameIndexIndexFloatStarStar     witGetFunc,
                                     const std::string & opName,
                                     int index, int subIndex )const
{
  float * attr;
  witGetFunc(mutableWitRun(),opName.c_str(),index,subIndex,&attr);
  std::vector<float> retVal = witFloatStarToStlVec(attr);
  witFree(attr);
  return retVal;
}

// Get vector of sub int* attribute
std::vector<int> VISORproblem1::witGetSubArcAttribute(
                                     witGetNameIndexIndexIntStarStar     witGetFunc,
                                     const std::string & opName,
                                     int index, int subIndex )const
{
  int * attr;
  witGetFunc(mutableWitRun(),opName.c_str(),index,subIndex,&attr);
  std::vector<int> retVal = witIntStarToStlVec(attr);
  witFree(attr);
  return retVal;
}

// Get vector of sub int attribute
int VISORproblem1::witGetSubArcAttribute(
                                     witGetNameIndexIndexIntStar     witGetFunc,
                                     const std::string & opName,
                                     int index, int subIndex )const
{
  int retVal;
  witGetFunc(mutableWitRun(),opName.c_str(),index,subIndex,&retVal);
  return retVal;
}

// Set sub float* attribute, given period and fltValue
void VISORproblem1::witSetSubArcAttribute(
    witGetNameIndexIndexFloatStarStar witGetFunc,
    witSetNameIndexIndexFloatStar     witSetFunc,
    const std::string & opName,
    int index, int subIndex,
    int period,
    float flt )
{
  assert( period>= 0 );
  assert( period<getNPeriods() );
  float * floatStar;
  witGetFunc(mutableWitRun(),opName.c_str(),index,subIndex,&floatStar);
  floatStar[period] = flt;
  witSetFunc(witRun(),opName.c_str(),index,subIndex,floatStar);
  witFree(floatStar);
}





// Get vector of wit demand float* attribute
std::vector<float> VISORproblem1::witGetDemandAttribute(
    witGetDblNameFloatStarStar witGetFunc,
    const std::string & partName,
    const std::string & demandName )const
{
  float * attr;
  witGetFunc(mutableWitRun(),partName.c_str(),demandName.c_str(),&attr);
  std::vector<float> retVal = witFloatStarToStlVec(attr);
  witFree(attr);
  return retVal;
}






//--------------------------------------------------------

void VISORproblem1::setTitle( const std::string & title)
{
  witSetTitle(witRun(),title.c_str());
}

std::string VISORproblem1::getTitle() const
{
  char * t;
  witGetTitle(mutableWitRun(),&t);
  std::string retVal(t);
  witFree(t);
  return retVal;
}

void VISORproblem1::setNPeriods( int nP )
{
  witSetNPeriods(witRun(),nP);
  nPeriods_ = nP;
}
// Set OSL Logfile name
void VISORproblem1::setSolverLogFileName(const std::string & name)
{
  witSetSolverLogFileName(witRun(),name.c_str());
}

void VISORproblem1::writeWitData( std::string filename )
{
  witSetMesgFileAccessMode(mutableWitRun(),WitFALSE,"w");
  witWriteData(mutableWitRun(), filename.c_str() );
}












WitRun * VISORproblem1::mutableWitRun() const { return wr_; }
WitRun * VISORproblem1::witRun() { return wr_; }



// default constructor
VISORproblem1::VISORproblem1()
:
wr_(NULL),
nPeriods_(30),
materialBaseNames_(),
printerBaseNames_()
{
  witNewRun( &wr_ );
  witInitialize( witRun() );
  //witSetOslMesgFileName(witRun(),WitSTDOUT);

  // Turn off WIT informational messages
  witSetMesgTimesPrint( witRun(), WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
  // Turn off warning msg about long names.
  witSetMesgTimesPrint( witRun(), WitTRUE, 338, 0);

  // Turn on some interesting informational messages
  witSetMesgTimesPrint( witRun(), WitTRUE, 729, UCHAR_MAX);
  witSetMesgTimesPrint( witRun(), WitTRUE, 730, UCHAR_MAX);

  //WIT0729I Deleting selected objects.
  //         Number of objects selected by the user:  158900
  //         Number of objects selected by WIT:       160629
  //         Number of objects selected total:        319529

  //WIT0730I Object purge complete.
  //         Number of objects before purge:  332170
  //         Number of objects deleted:       319529
  //         Number of objects after purge:    12641

  /* Turn off warning msg:
  WIT0749W One or more BOP entries have been rejected for possible explosion,
         because they had positive effective production rates (= yieldRate *
         productRate) that were below the value of expCutoff. The largest
         effective production rate that was rejected and the corresponding BOP
         entry and execution period are given below:

            expCutoff:             0.01
            Largest Rejected Rate: 0.008
            Operation:             "disassemble_26353AU"
            BOP Entry Index:       4
            Exec. Period:          0
  */
  //witSetMesgTimesPrint( witRun(), WitTRUE, 749, 0);




    witSetIndependentOffsets( witRun(), WitTRUE );
    witSetNPeriods(witRun(),30);
  //witSetObjChoice( witRun(), 1 );

  //witSetUseFocusHorizons( witRun(), WitFALSE );

  //witSetExecEmptyBom( witRun(), WitFALSE );
  //witSetMultiRoute( witRun(), WitTRUE );
  //witSetIndependentOffsets( witRun(), WitTRUE );

  //witSetAccAfterOptImp(witRun(),WitTRUE );
  //witSetAccAfterSoftLB(witRun(),WitTRUE );

  //witSetPostSolveRecovery(witRun(),WitTRUE);

  //witSetGroupPipMode(witRun(),WitTRUE);

  //ESO2probAppData * problemAppData = new ESO2probAppData;
  //witSetAppData(witRun(),problemAppData);

}

// destructor
VISORproblem1::~VISORproblem1()
{
  gutsOfDestructor();
}

// copy constructor. Not yet suported
VISORproblem1::VISORproblem1( const VISORproblem1& source ):
wr_(NULL),
nPeriods_(source.nPeriods_)
{
  gutsOfCopy(source);
}

// assignment operator. Not yet supported
VISORproblem1&
VISORproblem1::operator=(const VISORproblem1& rhs)
{
  if (this != &rhs) {
    if ( wr_ != NULL ) gutsOfDestructor();
    wr_ = NULL;
    gutsOfCopy(rhs);
  }
  return *this;
}

// copy object
void VISORproblem1::gutsOfCopy( const VISORproblem1& source )
{
  // Copy Wit Run
  assert(wr_==NULL);
  witNewRun( &wr_ );

  // Turn off WIT informational messages
  witSetMesgTimesPrint( wr_, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
  // Turn off warning msg about long names.
  witSetMesgTimesPrint( wr_, WitTRUE, 338, 0);

  witCopyData(wr_,source.wr_);
  nPeriods_ = source.nPeriods_;

#if 0
  // Copy global app data
  {
    //ESO2probAppData * srcAppData;
    //ESO2probAppData * sinkAppData;
    //witGetAppData(source.mutableWitRun(), (void**)&srcAppData);
    //sinkAppData = new ESO2probAppData(*srcAppData);
    //witSetAppData(witRun(), sinkAppData);
  }

  // copy partDemand appData
  {
    std::vector<std::string> parts;
    std::vector<std::string> custLocs;
    std::vector<std::string> plantLocs;
    getPartDemands( parts, custLocs, plantLocs);
    int p;
    for (p=0; p<parts.size(); ++p ) {
      ESO2partDemandAppData * srcAppData;
      ESO2partDemandAppData * sinkAppData;
      std::string dName = partDemandName(parts[p],custLocs[p],plantLocs[p]);
      witGetDemandAppData(source.mutableWitRun(),dName.c_str(),dName.c_str(),(void**)&srcAppData);
      sinkAppData = new ESO2partDemandAppData(*srcAppData);
      witSetDemandAppData(witRun(),dName.c_str(),dName.c_str(),sinkAppData);
    }
  }
  // copy looseParts appData
  {
    std::vector<std::string> parts;
    std::vector<std::string> locs;
    getLooseParts( parts, locs);
    int p;
    for (p=0; p<parts.size(); ++p ) {
      ESO2loosePartAppData * srcAppData;
      ESO2loosePartAppData * sinkAppData;
      std::string opName = acquireLoosePartName(parts[p],locs[p]);
      witGetOperationAppData(source.mutableWitRun(),opName.c_str(),(void**)&srcAppData);
      sinkAppData = new ESO2loosePartAppData(*srcAppData);
      witSetOperationAppData(witRun(),opName.c_str(),sinkAppData);
    }
  }

  // copy features appData
  {
    std::vector<std::string> features;
    std::vector<std::string> locs;
    getFeatures( features, locs);
    int p;
    for (p=0; p<features.size(); ++p ) {
      ESO2featureAppData * srcAppData;
      ESO2featureAppData * sinkAppData;
      std::string opName = acquireFeatureName(features[p],locs[p]);
      witGetOperationAppData(source.mutableWitRun(),opName.c_str(),(void**)&srcAppData);
      sinkAppData = new ESO2featureAppData(*srcAppData);
      witSetOperationAppData(witRun(),opName.c_str(),sinkAppData);
    }
  }

#endif

}

// destructor
void VISORproblem1::gutsOfDestructor()
{

  // Delete global app data
  {
    //ESO2probAppData * probAppData;
    //witGetAppData(mutableWitRun(),  (void**)&probAppData);
    //delete probAppData;
  }

  // delete partDemand appData
  {
    //std::vector<std::string> parts;
    //std::vector<std::string> custLocs;
    //std::vector<std::string> plantLocs;
    //getPartDemands( parts, custLocs, plantLocs);
    //int p;
    //for (p=0; p<parts.size(); ++p ) {
      //ESO2partDemandAppData * srcAppData;
      //std::string dName = partDemandName(parts[p],custLocs[p],plantLocs[p]);
      //witGetDemandAppData(mutableWitRun(),dName.c_str(),dName.c_str(),(void**)&srcAppData);
      //delete srcAppData;
    //}
  }

  // delete the witRun
  witDeleteRun(witRun());
}



#ifdef NDEBUG
#undef NDEBUG
#endif
//------------------------------------------------------------------------------
// Test function
//------------------------------------------------------------------------------
void
VISORproblem1::test()
{
  //OsiRelFltEq eq(1e-05);
  VisorRelFltEq eq(1.e-05);

  {
    VISORproblem1 prob;

    prob.setNPeriods(12);
    assert( prob.getNPeriods() == 12 );

    prob.setTitle("wittitle");
    assert( prob.getTitle() == "wittitle");

  }



  {
  	 // Test materials
    VISORproblem1 prob;
    assert(prob.getNPeriods()==30);
    prob.setNPeriods(25);
    assert(prob.getNPeriods()==25);

    prob.addMaterial("Briarcliff","1.75mm","PLA",200.0,75);

    std::vector<std::string> location, filamentSize, plasticType;
    prob.getMaterials( location, filamentSize, plasticType );
    assert( location.size()==1 );
    assert( filamentSize.size()==1 );
    assert( plasticType.size()==1 );
    assert( location[0]=="Briarcliff" );
    assert( filamentSize[0]=="1.75mm" );
    assert( plasticType[0]=="PLA" );

    std::vector<float> sv=prob.getOwnSupply("Briarcliff","1.75mm","PLA");
    assert( eq(sv[0],50.) );
    assert( eq(sv[1],0.0) );
    sv=prob.getSharedSupply("Briarcliff","1.75mm","PLA");
    assert( eq(sv[0],150.) );
    assert( eq(sv[3],0.0) );
    assert( sv.size()==25 );


    prob.addMaterial("Amawalk",   "1.75mm","ABS",100.0,33);
    prob.getMaterials( location, filamentSize, plasticType );
    assert( location.size()==2 );
    sv=prob.getOwnSupply("Amawalk",   "1.75mm","ABS");
    assert( eq(sv[0],67.) );
    assert( eq(sv[24],0.0) );
    sv=prob.getSharedSupply("Amawalk",   "1.75mm","ABS");
    assert( eq(sv[0],33.) );
    assert( eq(sv[20],0.0) );


    // Test printers
    prob.addPrinter("DigiLab3D45","Kitchawan Rd", 30.f,   true, false,    true, true, false, false);
    std::vector<std::string> printerName, printerLoc;
    prob.getPrinters( printerName, printerLoc );
    assert( printerName.size()==1 );
    assert( printerLoc.size()==1 );
    assert( printerName[0]=="DigiLab3D45" );
    assert( printerLoc[0]=="Kitchawan Rd" );
    std::vector<float> pr=prob.getPrinterProdRate("DigiLab3D45","Kitchawan Rd");
    assert( eq(pr[0],30.) );
    assert( eq(pr[20],30.) );
    std::vector<float> shipVol=prob.getPrinterShipVol("DigiLab3D45","Kitchawan Rd");
    assert( eq(shipVol[0],0.) );
    assert( eq(shipVol[10],0.) );

    // Check Subs Boms
    {
      std::vector<std::string> printerName, printerLoc;
      std::vector<std::string> matLoc, matSize, matType, own;
      std::vector< std::vector<float>> subVol;
    	prob.getSubVol(
            printerName, printerLoc,
            matLoc, matSize, matType,
            subVol, own );
      assert(printerName[0]=="DigiLab3D45" );
      assert(printerLoc[0]=="Kitchawan Rd" );
      assert(matLoc[0]=="Briarcliff" );
      assert(matSize[0]=="1.75mm" );
      assert(matType[0]=="PLA" );
      assert(own[0]=="no" );


      prob.addPrinter("DigiLab3D45","Amawalk", 10.f,   true, false,    false, false, true, false);
      prob.getSubVol(
            printerName, printerLoc,
            matLoc, matSize, matType,
            subVol, own );
      assert(subVol.size()==3);

      //    for( int i=0; i<printerName.size(); i++)
      //    {
      //    	std::cout <<printerName[i]+" "+printerLoc[i]+" "+matLoc[i]+" "+matSize[i]+" "+matType[i]+" "+own[i]+"\n";
      //    }

    }


    //std::vector<float> vol = p1.getPartDemandShipVol("0000000P1413","980","980");
    //assert(eq(vol[0],0.0));
   }

}


