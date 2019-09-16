//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "MsgBuilder.C"
//
// Contains the implementation of class MsgBuilder.
//------------------------------------------------------------------------------

#include <MsgBuilder.h>
#include <MsgMgr.h>
#include <Msg.h>

//------------------------------------------------------------------------------

WitMsgBuilder::WitMsgBuilder (WitMsgMgr * theMsgMgr):
      myMsgMgr_   (theMsgMgr),
      info_       (MclLevel::info    ()),
      warning_    (MclLevel::warning ()),
      severe_     (MclLevel::severe  ()),
      fatal_      (MclLevel::fatal   ()),
      currentMsg_ (NULL)
   {
   }

//------------------------------------------------------------------------------

WitMsgBuilder::~WitMsgBuilder ()
   {
   }

//------------------------------------------------------------------------------

void WitMsgBuilder::makeMsgFrag (WitMsgFragID theFragID, const char * textVal)
   {
   new MclMsgFrag (myMsgMgr_->myMclFacility (), theFragID, textVal);
   }

//------------------------------------------------------------------------------

void WitMsgBuilder::makeMsg (
      WitMsgID         theMsgID,
      int              theExtNum,
      const MclLevel & theLevel,
      const char *     textVal)
   {
   if (DEBUG_MODE)
      checkExtNumOrder (theExtNum);

   currentMsg_ =
      new WitMsg (myMsgMgr_, theMsgID, theLevel, textVal, theExtNum);
   }

//------------------------------------------------------------------------------

void WitMsgBuilder::checkExtNumOrder (int theExtNum)
   {
   if (currentMsg_ == NULL)
      return;

   if (theExtNum >= currentMsg_->externalNumber ())
      return;

   fprintf (stderr,
      "\n"
      "FATAL ERROR encountered by WIT.\n"
      "\n"
      "Attempt to create a message whose external number is lower than that\n"
      "of the previously created message.\n"
      "\n"
      "      Previous External Number: %d\n"
      "      Current  External Number: %d\n",
      currentMsg_->externalNumber (),
      theExtNum);

   exitWitNonMsg (4);
   }

//------------------------------------------------------------------------------
// buildMsgFrags
//------------------------------------------------------------------------------

void WitMsgBuilder::buildMsgFrags ()
   {
   makeMsgFrag (
      "trueFrag",
      "TRUE");

   makeMsgFrag (
      "falseFrag",
      "FALSE");

   makeMsgFrag (
      "trueWdFrag",
      "true");

   makeMsgFrag (
      "falseWdFrag",
      "false");

   makeMsgFrag (
      "capacityFrag",
      "Capacity");

   makeMsgFrag (
      "materialFrag",
      "Material");

   makeMsgFrag (
      "rawMaterialFrag",
      "Raw Material");

   makeMsgFrag (
      "productFrag",
      "Product");

   makeMsgFrag (
      "hardLowerBoundFrag",
      "Hard lower bound");

   makeMsgFrag (
      "softLowerBoundFrag",
      "Soft lower bound");

   makeMsgFrag (
      "hardUpperBoundFrag",
      "Hard upper bound");

   makeMsgFrag (
      "informationalMsgFrag",
      "informational");

   makeMsgFrag (
      "warningMsgFrag",
      "warning");

   makeMsgFrag (
      "errorMsgFrag",
      "error");

   makeMsgFrag (
      "severeMsgFrag",
      "severe");

   makeMsgFrag (
      "fatalMsgFrag",
      "fatal");

   makeMsgFrag (
      "maxViolMandECFrag",
      "stockVol[t] == 0.0, due to mandEC");

   makeMsgFrag (
      "maxViolExecutableFrag",
      "execVol[t] == 0.0, when not executable");

   makeMsgFrag (
      "maxViolTotSubVolFrag",
      "total subVol <= execVol");

   makeMsgFrag (
      "maxViolCumShipVolFrag",
      "cumShipVol <= cumDemandVol");

   makeMsgFrag (
      "maxViolShipVolFrag",
      "shipVol <= demandVol");

   makeMsgFrag (
      "maxViolHardLBFrag",
      "vector >= hardLB");

   makeMsgFrag (
      "maxViolHardUBFrag",
      "vector <= hardUB");

   makeMsgFrag (
      "maxViolLotSizeFrag",
      "execVol must be lot size feasible.");

   makeMsgFrag (
      "optImpFrag",
      "Optimizing Implosion");

   makeMsgFrag (
      "extOptImpFrag",
      "External Optimizing Implosion");

   makeMsgFrag (
      "heurImpAllFrag",
      "Heuristic Implosion/Allocation");

   makeMsgFrag (
      "heurOptImpFrag",
      "Heuristic Implosion for Optimizing Implosion");

   makeMsgFrag (
      "userInputFrag",
      "User Input");

   makeMsgFrag (
      "partFrag",
      "Part");

   makeMsgFrag (
      "operationFrag",
      "Operation");

   makeMsgFrag (
      "demandFrag",
      "Demand");

   makeMsgFrag (
      "bomEntryFrag",
      "BOM Entry");

   makeMsgFrag (
      "subEntryFrag",
      "Substitute BOM Entry");

   makeMsgFrag (
      "bopEntryFrag",
      "BOP Entry");

   makeMsgFrag (
      "nstnFrag",
      "NSTN");

   makeMsgFrag (
      "asapFrag",
      "ASAP");

   makeMsgFrag (
      "hardLBFrag",
      "Hard Lower Bounds");

   makeMsgFrag (
      "softLBFrag",
      "Soft Lower Bounds");

   makeMsgFrag (
      "hardUBFrag",
      "Hard Upper Bounds");

   makeMsgFrag (
      "lpFrag",
      "LP");

   makeMsgFrag (
      "mipFrag",
      "MIP");

   makeMsgFrag (
      "stochLpFrag",
      "stochastic LP");

   makeMsgFrag (
      "yesFrag",
      "Yes");

   makeMsgFrag (
      "noFrag",
      "No");
   }
      //
      // End of buildMsgFrags ().

//------------------------------------------------------------------------------
// buildMsgs
//------------------------------------------------------------------------------

void WitMsgBuilder::buildMsgs ()
   {
   //---------------------------------------------------------------------------

   makeMsg (
      "inputErrorRepSmsg",
      74,
      severe_,
      "WIT has detected and reported errors.  These errors "
      "prevent WIT from continuing.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "fopenSmsg",
      75,
      severe_,
      "An error was encountered by the \"fopen\" function when attempting "
      "to open a file.\n\n"
      "File name:           \"%1$s\"\n"
      "File opening mode:   \"%2$s\"\n"
      "Fopen error number:  %3$d\n"
      "Fopen error message: \"%4$s\"");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "paramMustBeDblSmsg",
      76,
      severe_,
      "Input error in control parameter file \"%1$s\".\n"
      "Parameter \"%2$s\" was specified as \"%3$s\".\n"
      "This parameter must be a double.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "paramMustBeIntSmsg",
      77,
      severe_,
      "Input error in control parameter file \"%1$s\".\n"
      "Parameter \"%2$s\" was specified as \"%3$s\".\n"
      "This parameter must be an integer.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "unrecParamSmsg",
      78,
      severe_,
      "Input error in control parameter file \"%1$s\".\n"
      "Unrecognized control parameter \"%2$s\" was read.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "callocSmsg",
      81,
      severe_,
      "Memory allocation error when attempting\n"
      "to obtain %1$d bytes of memory.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "witStopsSmsg",
      90,
      severe_,
      "WIT has encountered an error condition and "
      "is now terminating execution of the program.\n"
      "The return code is %1$d.");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "fcloseSmsg",
      92,
      severe_,
      "Error from fclose when closing the file \"%1$s\".");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "tempnamSmsg",
      96,
      severe_,
      "Error from operating system function tempnam or tmpnam.\n"
      "The system error number was %1$d, and the message was:\n"
      "%2$s");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "headingMsg",
      97,
      info_,
      "%1$s\n"
      "Constrained Materials Management and Production Planning Tool\n"
      "Release         %2$s\n"
      "Build Date:     %3$s\n"
      "Build Type:     %4$s\n"
      "Platform:       %5$s\n"
      "CPLEX Embedded? %6$m\n"
      "Run Started At: %7$s");

   currentMsg_->preceedingLineFeeds   (0);

   //---------------------------------------------------------------------------

   makeMsg (
      "witFuncCalled",
      98,
      info_,
      "WIT function %1$s entered.");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "setStringAttrMsg",
      99,
      info_,
      "%1$s changed from \"%2$s\" to \"%3$s\".");

   //---------------------------------------------------------------------------

   makeMsg (
      "witFileParam",
      100,
      info_,
      "The file \"%1$s\" will be accessed.");

   //---------------------------------------------------------------------------

   makeMsg (
      "titleMsg",
      101,
      info_,
      "Problem Title:\n"
      "\n"
      "   %1$s");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "chgMsgsTimesPrintSm",
      106,
      info_,
      "The number of times %1$m messages will be displayed "
      "has been changed to %2$d.");

   //---------------------------------------------------------------------------

   makeMsg (
      "chgMsgTimesPrintSm",
      107,
      info_,
      "The number of times messages number %1$d will be displayed "
      "has been changed to %2$d.");

   //---------------------------------------------------------------------------

   makeMsg (
      "invalidExtNumWmsg",
      108,
      warning_,
      "When calling function %1$s, message number %2$d was specified. "
      "This message does not exist.\n"
      "No action will be taken by function %1$s.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "setIntAttrMsg",
      117,
      info_,
      "%1$s changed from %2$d to %3$d.");

   //---------------------------------------------------------------------------

   makeMsg (
      "setDblAttrMsg",
      118,
      info_,
      "%1$s changed from %2$f to %3$f.");

   //---------------------------------------------------------------------------

   makeMsg (
      "getAttrMsg",
      120,
      info_,
      "%1$s returned.");

   //---------------------------------------------------------------------------

   makeMsg (
      "partIdDdMsg",
      127,
      info_,
      "Part \"%1$s\":");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "partScalarsDdMsg",
      128,
      info_,
      "   Part Category:     %1$12m\n"
      "   Part Index:        %2$12d\n"
      "   Single-Source?            %3$5b\n"
      "   Bounded Lead Time?        %4$5b\n"
      "   Scrap Allowed?            %5$5b");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "extSupplyVolDdMsg",
      131,
      info_,
      "   External supply volumes:%1v$8.0f");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "stockCostDdMsg",
      132,
      info_,
      "   Stock variable cost coefficients:%1v$8.2f");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "demandIdDdMsg",
      134,
      info_,
      "Part          \"%1$s\",\n"
      "Demand Stream \"%2$s\":");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "demandScalarsDdMsg",
      135,
      info_,
      "   Demand Stream Index:    %1$5d\n"
      "   Search Increment:       %2$9.3f\n"
      "   Late Shipments Allowed? %3$5b\n"
      "   Pref. Build Ahead:      %4$5b\n"
      "   Integer shipVols?       %5$5b");

   //---------------------------------------------------------------------------

   makeMsg (
      "demVolDdMsg",
      138,
      info_,
      "   Demand Volumes:%1v$8.0f");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "demShipRewardDdMsg",
      140,
      info_,
      "   Shipment variable reward coefficients:%1v$8.2f");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "demCumShipRewardDdMsg",
      141,
      info_,
      "   Cumulative shipment variable reward coefficients:%1v$8.2f");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "consBomHeadingDdMsg",
      142,
      info_,
      "   Consuming BOM Entries:\n"
      "      Consuming Operation  BOM Entry #");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "consBomDataDdMsg",
      143,
      info_,
      "             %1$12s  %2$-8d");

   //---------------------------------------------------------------------------

   makeMsg (
      "cycleTime34DdMsg",
      144,
      info_,
      "   WIT 3.4 Cycle Times:%1v$8.1f");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "executableDdMsg",
      145,
      info_,
      "   Executable?%1v$8b");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "mecDdMsg",
      146,
      info_,
      "   Mandatory E/C?%1v$8b");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "execCostDdMsg",
      148,
      info_,
      "   Execution variable cost coefficients:%1v$8.2f");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "scrapCostDdMsg",
      149,
      info_,
      "   Scrap variable cost coefficients:%1v$8.2f");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "bomEntryDdMsg",
      151,
      info_,
      "BOM Entry:\n"
      "\n"
      "   Consuming Operation: %1$12s\n"
      "   BOM Entry Number:        %2$8d\n"
      "   Consumed Part:       %3$12s\n"
      "   Global BOM Entry Number: %4$8d\n"
      "   Single-Source?           %5$8b");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "impactPeriodDdMsg",
      152,
      info_,
      "   Impact Periods:%1v$8d");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "shipSchedDataMsg",
      153,
      info_,
      "%1$-14s  %2$-14s  %3$3d  %4$14s");

   //---------------------------------------------------------------------------

   makeMsg (
      "prodSchedDataMsg",
      154,
      info_,
      "%1$-14s  %2$3d  %3$11s");

   //---------------------------------------------------------------------------

   makeMsg (
      "preProcMsg",
      155,
      info_,
      "Preprocessing.");

   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "heuristicMsg",
      156,
      info_,
      "Performing heuristic implosion.");

   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "solveOptProblemMsg",
      157,
      info_,
      "Solving the %1$m problem with CPLEX.");

   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "optSolnFoundMsg",
      159,
      info_,
      "Optimal solution found.");

   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "sglObjValueMsg",
      160,
      info_,
      "Objective function value: %1$.0f");

   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "genLpMipProbMsg",
      162,
      info_,
      "Generating the %1$m problem.");

   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "partNameTypeApMsg",
      167,
      info_,
      "Part name is \"%1$s\" and is of category \"%2$s\".");

   //---------------------------------------------------------------------------

   makeMsg (
      "dupPartApMsg",
      169,
      severe_,
      "The part \"%1$s\" can not be added because it already exists.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "attrCategoryErrMsg",
      172,
      severe_,
      "Attribute %1$s can not be specified when the part category is %2$s.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "setVecDblAttrMsg",
      174,
      info_,
      "Attribute %1$s set to:%2v$11f");

   //---------------------------------------------------------------------------

   makeMsg (
      "demandNameAdMsg",
      177,
      info_,
      "Demand stream \"%2$s\" is being added to part \"%1$s\".");

   //---------------------------------------------------------------------------

   makeMsg (
      "undefinedPartMsg",
      178,
      severe_,
      "The part \"%1$s\" is not defined.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "duplicateDemNameAdMsg",
      179,
      severe_,
      "Demand \"%1$s\" has already been defined");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "addBomEntryMsg",
      180,
      info_,
      "A BOM entry for the consumption of part \"%2$s\" "
      "is being added to operation \"%1$s\"");

   //---------------------------------------------------------------------------

   makeMsg (
      "setBooleanAttrMsg",
      183,
      info_,
      "%1$s changed from %2$b to %3$b.");

   //---------------------------------------------------------------------------

   makeMsg (
      "effProdRateDdMsg",
      189,
      info_,
      "   Effective Production Rates:%1v$8.2f");

   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "timingMismatchFmsg",
      192,
      fatal_,
      "Timing section mismatch.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "undefinedDemandMsg",
      196,
      severe_,
      "The demand \"%1$s\" for part \"%2$s\" is not defined.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "categorySizeDdMsg",
      198,
      info_,
      "   Number of %1$-8m Parts:                 %2$5d");

   //---------------------------------------------------------------------------

   makeMsg (
      "dimenHeadingDdMsg",
      199,
      info_,
      "Problem Dimensions:\n");

   //---------------------------------------------------------------------------

   makeMsg (
      "dimenDataDdMsg",
      200,
      info_,
      "   Total Number of Parts:                 %01$8d\n"
      "   Number of Operations:                  %02$8d\n"
      "   Number of Demand Streams:              %03$8d\n"
      "   Number of Periods:                     %04$8d\n"
      "   Number of Bound Sets Specified:        %05$8d\n"
      "   Total Number of Bound Sets:            %06$8d\n"
      "   Number of BOM Entries:                 %07$8d\n"
      "   Number of Substitute BOM Entries:      %08$8d\n"
      "   Number of BOP Entries:                 %09$8d\n"
      "   Number of Data Objects:                %10$8d");

   //---------------------------------------------------------------------------

   makeMsg (
      "subEntryDdMsg",
      202,
      info_,
      "Substitute BOM Entry:\n"
      "\n"
      "   Consuming Operation: %01$12s\n"
      "   BOM Entry Number:    %02$12d\n"
      "   Replaced Part:       %03$12s\n"
      "   Substitute Number:   %04$12d\n"
      "   Consumed Part:       %05$12s\n"
      "   Global Substitute Number: %06$7d\n"
      "   Integer subVols?     %07$12b\n"
      "   Explosion Allowed?   %08$12b\n"
      "   Netting Allowed?     %09$12b\n"
      "   MRP Netting Allowed? %10$12b\n"
      "   Exp/Net Aversion:    %11$12.3f");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "boundHeadingDdMsg",
      203,
      info_,
      "      Bound Set Index:    %1$8d");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "boundDataDdMsg",
      204,
      info_,
      "      %1$m:%2v$8.2f");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "consSubsHeadingDdMsg",
      250,
      info_,
      "   Consuming Substitutes:\n"
      "      Consuming Operation  Replaced Part  BOM Entry #  Sub #");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "consSubsDataDdMsg",
      251,
      info_,
      "             %1$12s   %2$12s  %3$-8d     %4$-8d");

   //---------------------------------------------------------------------------

   makeMsg (
      "subSchedHeadingMsg",
      252,
      info_,
      "Execution Volumes Due To Substitutes:");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "subSchedDataMsg",
      253,
      info_,
      "%1$-14s  %2$-14s  %3$8d  %4$3d  %5$-14s  %6$8d  %7$11s");

   //---------------------------------------------------------------------------

   makeMsg (
      "setMsgBoolAttrMsg",
      259,
      info_,
      "The %1$s attribute for message number %2$d has been set to %3$b.");

   //---------------------------------------------------------------------------

   makeMsg (
      "setMsgTypeBoolAttrMsg",
      260,
      info_,
      "The %1$s attribute for %2$m messages has been set to %3$b.");

   //---------------------------------------------------------------------------

   makeMsg (
      "noPartsSmsg",
      262,
      severe_,              
      "Optimizing implosion was invoked "
      "when the input data includes no parts.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "categoryErrMsgAdd",
      264,
      severe_,
      "%1$s can not be invoked for a part with category %2$s.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "demPriDdMsg",
      267,
      info_,
      "   Priorities:%1v$8d");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "setVecBoolAttrMsg",
      268,
      info_,
      "Attribute %1$s set to:%2v$b");

   //---------------------------------------------------------------------------

   makeMsg (
      "setVecIntAttrMsg",
      269,
      info_,
      "Attribute %1$s set to:%2v$11d");

   //---------------------------------------------------------------------------

   makeMsg (
      "setBoundVecAttrMsg",
      272,
      info_,
      "The %2$m vector of %1$s is set to:%3v$11f");

   //---------------------------------------------------------------------------

   makeMsg (
      "parmsAsbeMsg",
      274,
      info_,
      "A substitute BOM entry for the consumption of part \"%3$s\" "
      "is being added to BOM entry index %2$d of operation \"%1$s\".");

   //---------------------------------------------------------------------------

   makeMsg (
      "subCostDdMsg",
      275,
      info_,
      "   Substitute variable cost coefficients:%1v$8.2f");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "emptyBom",
      277,
      severe_,
      "Attempt to access a BOM entry of an operation whose "
      "Bill-Of-Manufacturing is empty.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "callBeforeInitSMsg",
      285,
      severe_,
      "%1$s must not be called until after either witInitialize or witCopyData "
      "has been called for the same WitRun.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "reqSchedDataMsg",
      286,
      info_,
      "%1$-14s  %2$3d  %3$11.3f");

   //---------------------------------------------------------------------------

   makeMsg (
      "mrpMsg",
    289,
      info_,
      "Performing MRP.");

   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "inputAttribSmsg",
      301,
      severe_,
      "   Attribute          : %1$s");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "inputPeriodSmsg",
      303,
      severe_,
      "   Period number      : %1$d");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "inputPartNameSmsg",
      304,
      severe_,
      "   Part name          : %1$s");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "inputDemandNameSmsg",
      305,
      severe_,
      "   Demand name        : %1$s");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "inputConsumedPartNameSmsg",
      306,
      severe_,
      "   Consumed part name : %1$s");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "inputBomIndexSmsg",
      307,
      severe_,
      "   BOM entry index    : %1$d");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "inputSubsBomEntryIndexSmsg",
      308,
      severe_,
      "   Substitute index   : %1$d");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "apiFunctionSmsg",
      310,
      severe_,
      "   Function           : %1$s");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "notAllowedDataChangeSmsg",
      311,
      severe_,
      "Attribute can not be set after %1$s has been invoked.");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "boundSetRangeSmsg",
      312,
      severe_,
      "The condition that "
      "0.0 <= hard lower bound <= soft lower bound <= hard upper bound "
      "is not satisfied.\n"
      "Hard lower bound = %1$f\n"
      "Soft lower bound = %2$f\n"
      "Hard upper bound = %3$f");

   currentMsg_->delayedSevere         (true);
   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "materialReqdSmsg",
      316,
      severe_,
      "The attribute is only valid when part category = material.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "dblLowEqSmsg",
      317,
      severe_,
      "Attribute is out of range.\n"
      "   Value specified    : %1$f\n"
      "   Range              : value >= %2$f");

   currentMsg_->delayedSevere         (true);
   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "dblLowSmsg",
      318,
      severe_,
      "Attribute is out of range.\n"
      "   Value specified    : %1$f\n"
      "   Range              : value > %2$f");

   currentMsg_->delayedSevere         (true);
   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "intLowEqUpEqImmSmsg",
      319,
      severe_,
      "Attribute is out of range.\n"
      "   Value specified    : %1$d\n"
      "   Range              : %2$d <= value <= %3$d");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "intLowEqSmsg",
      321,
      severe_,
      "Attribute is out of range.\n"
      "   Value specified    : %1$d\n"
      "   Range              : value >= %2$d");

   currentMsg_->delayedSevere         (true);
   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "intLowEqImmSmsg",
      322,
      severe_,
      "Attribute is out of range.\n"
      "   Value specified    : %1$d\n"
      "   Range              : value >= %2$d");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "unknownPartCategorySmsg",
      323,
      severe_,
      "Unrecognized part category specified.\n"
      "   Value specified    : %1$s\n"
      "   Allowed values     : WitCAPACITY, WitMATERIAL");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "bomEntryIndexRangeSmsg",
      324,
      severe_,
      "BOM Entry index is out of range.\n"
      "   Value specified    : %1$d\n"
      "   Range              : %2$d <= value < %3$d");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "subsBomEntryIndexRangeSmsg",
      325,
      severe_,
      "Substitute number is out of range.\n"
      "   Value specified    : %1$d\n"
      "   Range              : %2$d <= value < %3$d");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "partNameMsg",
      326,
      info_,
      "Part name is %1$s.");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "demandNameMsg",
      327,
      info_,
      "Demand name is %1$s.");

   //---------------------------------------------------------------------------

   makeMsg (
      "bomEntryIndexMsg",
      329,
      info_,
      "BOM entry index is %1$d.");

   //---------------------------------------------------------------------------

   makeMsg (
      "subsBomEntryIndexMsg",
      330,
      info_,
      "Substitute BOM entry index is %1$d.");

   //---------------------------------------------------------------------------

   makeMsg (
      "mpsFileMsg",
      335,
      info_,
      "Printing an MPS file of the optimization problem.\n");

   currentMsg_->testable              (false);
   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "nullStringSmsg",
      336,
      severe_,
      "A null string was specified as the \"%1$s\" argument.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "longNameWmsg",
      338,
      warning_,
      "One or more of the names given in the input "
      "is more than %1$d characters long. "
      "This may cause overflows in the output fields.\n"
      "The longest name was: \"%2$s\".");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "fgetsError",
      340,
      severe_,
      "Error from fgets when reading file " "\"%1$s\".\n"
      "Fgets error number was %2$d, and the message was:\n"
      "%3$s");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "periodRangeSmsg",
      342,
      severe_,
      "Period number is out of range.\n"
      "   Value specified    : %1$d\n"
      "   Range              : %2$d <= value < %3$d");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "paramReadErrorConflict",
      343,
      severe_,
      "Input error in control parameter file \"%1$s\".\n"
      "The parameter \"%2$s\" was specified as \"%3$s\", while\n"
      "the parameter \"%4$s\" was specified as \"%5$s\".\n"
      "This combination is not allowed.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "writePreprocMsg",
      344,
      info_,
      "Writing the preprocessing output.");

   currentMsg_->testable              (false);
   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "writeEchoMsg",
      345,
      info_,
      "Writing the echo output.");

   currentMsg_->testable              (false);
   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "writeReqSchedMsg",
      348,
      info_,
      "Writing the MRP supply requirements schedule.");

   currentMsg_->testable              (false);
   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "writeExecSchedMsg",
      349,
      info_,
      "Writing the execution schedule.");

   currentMsg_->testable              (false);
   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "writeShipSchedMsg",
      350,
      info_,
      "Writing the shipment schedule.");

   currentMsg_->testable              (false);
   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "writeCriticalListMsg",
      351,
      info_,
      "Writing the critical part list.");

   currentMsg_->testable              (false);
   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "normalTermMsg",
      352,
      info_,
      "Normal termination.");

   currentMsg_->testable              (false);
   currentMsg_->dupToStdout           (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "bomEntryWoSubsSmsg",
      353,
      severe_,
      "BOM entry does not have any substitute BOM entries.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "inputFileNameSmsg",
      355,
      severe_,
      "   Input file name    : %1$s");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "inputFileLineNumberSmsg",
      356,
      severe_,
      "   Input file line number   : %1$d");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "dblParamTooLowSmsg",
     369,
      severe_,
      "Input error in control parameter file \"%1$s\".\n"
      "Parameter \"%2$s\" was specified as \"%3$s\".\n"
      "This parameter must be >= %4$f.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "intParamTooLowSmsg",
     370,
      severe_,
      "Input error in control parameter file \"%1$s\".\n"
      "Parameter \"%2$s\" was specified as \"%3$s\".\n"
      "This parameter must be >= %4$d.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "intParamTooHighSmsg",
     371,
      severe_,
      "Input error in control parameter file \"%1$s\".\n"
      "Parameter \"%2$s\" was specified as \"%3$s\".\n"
      "This parameter must be <= %4$d.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "unknownFileFormatSmsg",
     372,
      severe_,
      "Unrecognized file format specified. Format specified was \"%1$s\". "
      "Valid selections are WitBSV or WitCSV.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "fileFormatMsg",
      373,
      info_,
      "The file format is \"%1$s.\"");

   //---------------------------------------------------------------------------

   makeMsg (
      "criticalListMsg",
      374,
      info_,
      "%1$-14s  %2$3d");

   //---------------------------------------------------------------------------

   makeMsg (
      "shipSchedDataCsvMsg",
      376,
      info_,
      "%1$s,%2$s,%3$d,%4$s");

   //---------------------------------------------------------------------------

   makeMsg (
      "prodSchedDataCsvMsg",
      377,
      info_,
      "%1$s,%2$d,%3$s");

   //---------------------------------------------------------------------------

   makeMsg (
      "subSchedDataCsvMsg",
      378,
      info_,
      "%1$s,%2$s,%3$d,%4$d,%5$s,%6$d,%7$s");

   //---------------------------------------------------------------------------

   makeMsg (
      "reqSchedDataCsvMsg",
      379,
      info_,
      "%1$s,%2$d,%3$.3f");

   //---------------------------------------------------------------------------

   makeMsg (
      "criticalListCsvMsg",
      380,
      info_,
      "%1$s,%2$d");

   //---------------------------------------------------------------------------

   makeMsg (
      "formCriticalListMsg",
      383,
      info_,
      "Constructing the critical part list.");

   //---------------------------------------------------------------------------

   makeMsg (
      "nCshipRelaxedMsg",
      390,
      info_,
      "%1$d nonnegativities on cumulative shipment variables relaxed by "
      "screening.");

   //---------------------------------------------------------------------------

   makeMsg (
      "nVarsFixedMsg",
      391,
      info_,
      "%1$d variables fixed by screening implosion constraints.");

   //---------------------------------------------------------------------------

   makeMsg (
      "nDualElimMsg",
      392,
      info_,
      "%1$d dual infeasibilities eliminated by reconstruction.");

   //---------------------------------------------------------------------------

   makeMsg (
      "initObjMsg",
      393,
      info_,
      "Objective function evaluated at the initial solution:");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "boundCountsMsg",
      395,
      info_,
      "Bound Counts:\n"
      "   Number of bound sets specified: %1$8d\n"
      "   Number of bounded variables:    %2$8d\n"
      "   Number of slbv variables:       %3$8d");

   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "writeOptSolnMsg",
      396,
      info_,
      "Writing the optimization solution.");

   currentMsg_->testable              (false);
   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "writeOptProblemMsg",
      397,
      info_,
      "Writing the optimization problem.");

   currentMsg_->testable              (false);
   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "cpuTimesMsg",
      398,
      info_,
      "CPU Times:");

   currentMsg_->testable              (false);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "sectionTimeMsg",
      399,
      info_,
      "   %1$-27s %2$8.2f seconds.");

   currentMsg_->testable              (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "memoryErrorSmsg",
      404,
      severe_,
      "Memory allocation error:\n\n"
      "There is not enough memory available for WIT to continue running.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "mpsFileError",
      406,
      severe_,
      "Error: ioStat = %1$d, when closing MPS file.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "compFSSMsg",
      408,
      info_,
      "Computing the focussed shortage schedule.");

   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "writeSolnMsg",
      410,
      info_,
      "Writing the comprehensive implosion solution.");

   currentMsg_->testable              (false);
   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "fssWithUserSolnSmsg",
      415,
      severe_,
      "Attempted to compute a focussed shortage schedule when the current "
      "implosion solution was user-specified.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "noFssSmsg",
      416,
      severe_,
      "Attempted to obtain a focussed shortage volume "
      "when the focussed shortage schedule has not yet been computed.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "heurOptStarterMsg",
      419,
      info_,
      "The solution to a heuristic implosion will be used "
      "as the initial solution.");

   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "accOptStarterMsg",
      420,
      info_,
      "This will be an accelerated optimizing implosion.\n"
      "The solution to the previous optimizing implosion will be used as the "
      "initial solution.");

   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "schedOptStarterMsg",
      421,
      info_,
      "The production and shipment schedules will be used "
      "as the initial solution.");

   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "crashOptStarterMsg",
      422,
      info_,
      "The solver's crash routine will be used to generate the initial "
      "solution.");

   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "updateOptProblemMsg",
      424,
      info_,
      "Updating the optimization problem.");

   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "minLotSizeDdMsg",
      425,
      info_,
      "   Minimum Lot Sizes:%1v$8.2f");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "incLotSizeDdMsg",
      426,
      info_,
      "   Incremental Lot Sizes:%1v$8.2f");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "optImpMsg",
      429,
      info_,
      "Performing optimizing implosion.");

   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "reSolveLpMsg",
      431,
      info_,
      "Re-solving the LP problem with CPLEX.");

   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "accAfterSoftLBNotOptImpMsg",
     432,
      warning_,
      "accAfterSoftLB is TRUE and accAfterOptImp is FALSE.\n"
      "This is probably not a useful combination of settings\n"
      "for these two attributes.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "negPartCostWmsg",
      446,
      warning_,
      "Attribute \"%1$s\" of part \"%2$s\" is being set to a negative value "
      "of %3$f in period %4$d.\n"
      "A negative value is permitted, "
      "but it may cause the objective to go to positive infinity.");

   currentMsg_->maxTimesIssued        (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "negSubCostWmsg",
      447,
      warning_,
      "Attribute \"%1$s\" of the substitute BOM entry with "
      "operation \"%2$s\", "
      "bomEntryIndex %3$d, "
      "subsBomEntryIndex %4$d, "
      "and consumedPart \"%5$s\" "
      "is being set to a negative value of %6$f in period %7$d.\n"
      "A negative value is permitted, "
      "but it may cause the objective to go to positive infinity.");

   currentMsg_->maxTimesIssued        (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "infeasWmsg",
      448,
      warning_,
      "WIT has computed an infeasible implosion solution.\n"
      "This may have been caused by any of the following:\n"
      "\n"
      "   A numerical problem\n"
      "   An infeasible user-specified starting solution\n"
      "      to heuristic implosion / allocation, (if any)\n"
      "   A bug in WIT\n"
      "\n"
      "Type of implosion: %1$m\n"
      "Maximum constraint violation: %2$g");

   currentMsg_->testable              (false);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "genericMsg",
      450,
      info_,
      "%1$s");

   currentMsg_->testable              (false);
   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "setAccInitSmsg",
      455,
      severe_,
      "Attempted to set \"optInitMethod\" to \"accelerated\".");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "infeasObjWmsg",
     456,
      warning_,
      "Attempted to evaluate the objective functions when the current "
      "implosion solution is not feasible. "
      "The resulting objective function values may not be meaningful.");

   //---------------------------------------------------------------------------

   makeMsg (
      "unpostprocessedFssSmsg",
      458,
      severe_,
      "Attempted to compute a focussed shortage schedule "
      "when the WitRun is in an unpostprocessed state.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "unknownOptStarterSmsg",
      459,
      severe_,
      "Unrecognized optInitMethod specified.\n"
      "   Value specified    : %1$s\n"
      "   Allowed values     : WitHEUR_OPT_INIT_METHOD,\n"
      "                        WitACC_OPT_INIT_METHOD,\n"
      "                        WitSCHED_OPT_INIT_METHOD\n"
      "                        WitCRASH_OPT_INIT_METHOD");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "setWitAttrAttrMsg",
      460,
      info_,
      "%1$s changed from \"%2$s\" to \"%3$s\".");

   //---------------------------------------------------------------------------

   makeMsg (
      "writeMrpSolnMsg",
      461,
      info_,
      "Writing the comprehensive MRP solution.");

   currentMsg_->testable              (false);
   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "opnIdDdMsg",
      464,
      info_,
      "Operation \"%1$s\":");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "opnScalarsDdMsg",
      465,
      info_,
      "   Operation Index:       %1$8d\n"
      "   Two-Level Lot Sizes?      %2$5b\n"
      "   Execution Penalty:  %3$11.3f\n"
      "   Integer execVols?         %4$5b\n"
      "   PIP Enabled?              %5$5b\n"
      "   PIP Rank:                 %6$5d");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "fileHeadingDdMsg",
      468,
      info_,
      "Formatted Output of Input Data\n"
      "------------------------------");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "globalHeadingDdMsg",
      469,
      info_,
      "Global Data:\n"
      "------------");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "partHeadingDdMsg",
      470,
      info_,
      "Part Data:\n"
      "----------");

   currentMsg_->preceedingLineFeeds   (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "operationHeadingDdMsg",
      471,
      info_,
      "Operation Data:\n"
      "---------------");

   currentMsg_->preceedingLineFeeds   (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "demandHeadingDdMsg",
      472,
      info_,
      "Demand Data:\n"
      "------------");

   currentMsg_->preceedingLineFeeds   (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "subHeadingDdMsg",
      473,
      info_,
      "Substitute BOM Entry Data:\n"
      "--------------------------");

   currentMsg_->preceedingLineFeeds   (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "bomEntryHeadingDdMsg",
      474,
      info_,
      "BOM Entry Data:\n"
      "---------------");

   currentMsg_->preceedingLineFeeds   (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "readParamsMsg",
      475,
      info_,
      "Reading the control parameters.");

   currentMsg_->testable              (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "readDataMsg",
      476,
      info_,
      "Reading the input data.");

   currentMsg_->testable              (false);
   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "bopEntryHeadingDdMsg",
      478,
      info_,
      "BOP Entry Data:\n"
      "---------------");

   currentMsg_->preceedingLineFeeds   (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "bopEntryDdMsg",
      479,
      info_,
      "BOP Entry:\n"
      "\n"
      "   Producing Operation:  %1$12s\n"
      "   BOP Entry Number:         %2$8d\n"
      "   Produced Part:        %3$12s\n"
      "   Global BOP Entry Number:  %4$8d\n"
      "   Explosion Allowed?        %5$8b\n"
      "   Explosion Aversion:       %6$8.3f\n"
      "   Sel-Split for Multi-Exec? %7$8b\n");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "prodBopHeadingDdMsg",
      480,
      info_,
      "   Producing BOP Entries:\n"
      "      Producing Operation  BOP Entry #");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "prodBopDataDdMsg",
      481,
      info_,
      "             %1$12s  %2$-8d");

   //---------------------------------------------------------------------------

   makeMsg (
      "nonMaterialFmsg",
      482,
      fatal_,
      "Attempted to access a non-Material Part as a Material Part.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "negativeIndexFmsg",
      485,
      fatal_,
      "A Vector was accessed using a negative index.\n"
      "Index = %1$d");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "indexTooLargeFmsg",
      486,
      fatal_,
      "A Vector was accessed using an index above its upper limit.\n"
      "Index            = %1$d\n"
      "Max. valid index = %2$d");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "genericSmsg",
      491,
      severe_,
      "%1$s");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);
         ;
   //
   // The purpose of this message is to issue temporary severe error messages
   // that are intended to be removed before release.

   //---------------------------------------------------------------------------

   makeMsg (
      "materialDdMsg",
      493,
      info_,
      "Material Data for Part \"%1$s\":\n\n"
      "   Material Index:         %2$8d\n"
      "   Build NSTN?                %3$5b\n"
      "   Build ASAP?                %4$5b\n"
      "   Sel-Split for Build-Ahead? %5$5b\n"
      "   ASAP pegging order in PIP? %6$5b");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "myDemandsHeadingDdMsg",
      494,
      info_,
      "   Demands:");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "myDemandsDataDdMsg",
      495,
      info_,
      "      %1$s");

   //---------------------------------------------------------------------------

   makeMsg (
      "noneDdMsg",
      496,
      info_,
      "      None");

   //---------------------------------------------------------------------------

   makeMsg (
      "procBomHeadingDdMsg",
      497,
      info_,
      "   BOM Entries for this Operation:");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "procBomHeading2DdMsg",
      498,
      info_,
      "      Consumed Part  BOM Entry #");

   //---------------------------------------------------------------------------

   makeMsg (
      "procBomDataDdMsg",
      499,
      info_,
      "      %1$-12s      %2$8d");

   //---------------------------------------------------------------------------

   makeMsg (
      "procBopHeadingDdMsg",
      500,
      info_,
      "   BOP Entries for this Operation:");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "procBopHeading2DdMsg",
      501,
      info_,
      "      Produced Part  BOP Entry #");

   //---------------------------------------------------------------------------

   makeMsg (
      "procBopDataDdMsg",
      502,
      info_,
      "      %1$-12s      %2$8d");

   //---------------------------------------------------------------------------

   makeMsg (
      "bomSubHeadingDdMsg",
      503,
      info_,
      "   Substitutes for this BOM Entry:\n"
      "      Consumed Part    Sub #");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "bomSubDataDdMsg",
      504,
      info_,
      "      %1$-12s  %2$8d");

   //---------------------------------------------------------------------------

   makeMsg (
      "setWit34CompatibleCpuWmsg",
      505,
      warning_,
      "The value of the \"wit34Compatible\" attribute is being set "
      "when at least one operation exists. "
      "This may consume significant CPU time.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "forbiddenNullFmsg",
      508,
      fatal_,
      "A null pointer was used where it is not allowed.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "severeNotIDFmsg",
      509,
      fatal_,
      "A severe error has been detected, but could not be identified.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "dupOperationApMsg",
      510,
      severe_,
      "The operation \"%1$s\" can not be added because it already exists.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "offsetDdMsg",
      511,
      info_,
      "   Offsets:%1v$8.1f");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "selfConsistSmsg",
      512,
      severe_,
      "The following self-consistency condition was not met:\n\n"

      "   %1$s\n\n"
      
      "   %2$s[%3$d] = %4$d\n"
      "   %2$s[%5$d] = %6$d");

   currentMsg_->delayedSevere         (true);
   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "usageTime34DdMsg",
      516,
      info_,
      "   WIT 3.4 Usage Time:      %1$8.1f");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "operationNameAdMsg",
      519,
      info_,
      "Operation name is \"%1$s\".");

   //---------------------------------------------------------------------------

   makeMsg (
      "inputOperationNameSmsg",
      520,
      severe_,
      "   Operation name     : %1$s");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "undefinedOperationMsg",
      521,
      severe_,
      "The operation \"%1$s\" is not defined.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "operationNameMsg",
      522,
      info_,
      "Operation name is %1$s.");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "nSlbScreenedMsg",
      523,
      info_,
      "%1$d soft lower bound variables and constraints screened.");

   //---------------------------------------------------------------------------

   makeMsg (
      "bopAddMsg",
      524,
      info_,
      "A BOP entry for the production of part \"%1$s\" "
      "is being added to operation \"%2$s\"");

   //---------------------------------------------------------------------------

   makeMsg (
      "inputBopIndexSmsg",
      525,
      severe_,
      "   BOP entry index    : %1$d");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "inputProducedPartNameSmsg",
      526,
      severe_,
      "   Producing part     : %1$s");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "emptyBopSmsg",
      527,
      severe_,
      "Attempt to access a BOP entry of an operation whose Bill-Of-Products "
      "is empty.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "bopEntryIndexRangeSmsg",
      528,
      severe_,
      "BOP Entry index is out of range.\n"
      "   Value specified    : %1$d\n"
      "   Range              : %2$d <= value < %3$d");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "bopEntryIndexMsg",
      529,
      info_,
      "BOP entry index is %1$d.");

   //---------------------------------------------------------------------------

   makeMsg (
      "sortNodesMsg",
      530,
      info_,
      "Sorting the parts and operations in BOM order.");

   //---------------------------------------------------------------------------

   makeMsg (
      "bomCycleHeadingSmsg",
      531,
      severe_,
      "The following explodeable cycle was found "
      "in the complete BOM structure:");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (2);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "bomCycleNodesSmsg",
      532,
      severe_,
      "Part %1$s is produced by operation %2$s.\n"
      "Operation %2$s consumes part %3$s.");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "bomCycleSmsg",
      533,
      severe_,
      "Explodeable cycles in the complete BOM structure are not allowed.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "getting34AttributeWithCompatibilityMode",
      534,
      info_,
      "The function %1$s has been invoked to retrieve the value of "
      "WIT Version 3.4 %2$s attribute %3$s. "
      "This attribute has been superseded by %4$s attribute %5$s. "
      "Since wit34Compatible is TRUE, the value of the superseding attribute "
      "will be retrieved.");

   //---------------------------------------------------------------------------

   makeMsg (
      "getting34AttributeWithoutCompatibilityMode",
      535,
      severe_,
      "The function %1$s has been invoked to retrieve the value of "
      "WIT Version 3.4 %2$s attribute %3$s. "
      "This attribute has been superseded by %4$s attribute %5$s. "
      "Since wit34Compatible is FALSE, "
      "the WIT Version 3.4 %2$s attribute %3$s is not available.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "setting34AttributeWithCompatibilityMode",
      536,
      info_,
      "The function %1$s has been invoked to set the value of "
      "WIT Version 3.4 %2$s attribute %3$s. "
      "This attribute has been superseded by %4$s attribute %5$s. "
      "Since wit34Compatible is TRUE, "
      "the value of the superseding attribute will be set.");

   //---------------------------------------------------------------------------

   makeMsg (
      "setting34AttributeWithoutCompatibilityMode",
      537,
      severe_,
      "The function %1$s has been invoked to set the value of "
      "WIT Version 3.4 %2$s attribute %3$s. "
      "This attribute has been superseded by %4$s attribute %5$s. "
      "Since wit34Compatible is FALSE, "
      "the WIT Version 3.4 %2$s attribute %3$s is not available.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "compatible34WarningMsg",
      538,
      warning_,
      "wit34Compatible is TRUE. "
      "Your application should be updated to work with WIT Release 4.0 "
      "or higher.");

   //---------------------------------------------------------------------------

   makeMsg (
      "addProductPartWith34Compatability",
      539,
      info_,
      "The function %1$s has been invoked to add a part with category product. "
      "A WIT Version 3.4 product part has been superseded "
      "by a part with operation. "
      "Since wit34Compatible is TRUE, witAddPartWithOperation will be invoked.");

   //---------------------------------------------------------------------------

   makeMsg (
      "addRawPartWith34Compatability",
      540,
      info_,
      "The function %1$s has been invoked to add a part with category raw. "
      "This WIT Version 3.4 part category raw has been superseded "
      "with category material. "
      "Since wit34Compatible is TRUE, a material part will be added.");

   //---------------------------------------------------------------------------

   makeMsg (
      "minLotSizeTooLargeWmsg",
      541,
      warning_,
      "The minLotSize for operation %1$s in period %2$d consumes %3$.2f units "
      "of capacity part %4$s in period %5$d, when only %6$.2f units are "
      "available.\n"
      "This may cause heuristic implosion to give poor results.\n");

   currentMsg_->maxTimesIssued        (10);

   //---------------------------------------------------------------------------

   makeMsg (
      "getting34AttributeNBomEntriesForNonProduct",
      542,
      info_,
      "The attribute nBomEntries for the part \"%1$s\" with category %2$m has "
      "been requested.  Parts with this category do not have BOM entries. "
      "Since wit34Compatible is TRUE, zero will be returned. "
      "Part attribute nBomEntries has been superseded by Operation attribute "
      "nBomEntries.");

   //---------------------------------------------------------------------------

   makeMsg (
      "assertFailFmsg",
      543,
      fatal_,
      "Assertion failure:\n"
      "   Source File: %1$s\n"
      "   Line #:      %2$d\n"
      "   Assertion:   %3$s");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "hasSubsInEffectDdMsg",
      544,
      info_,
      "   Has Substitute Bom Entries In Effect?%1v$8b");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "mandECDdMsg",
      547,
      info_,
      "   Mandatory E/C? %1$5b");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "billEntryDdMsg",
      548,
      info_,
      "   Bill Entry Number:        %1$7d\n"
      "   Earliest Period:             %2$4d\n"
      "   Latest   Period:             %3$4d");

   //---------------------------------------------------------------------------

   makeMsg (
      "consEntryDdMsg",
      549,
      info_,
      "   Consumption Entry Number: %1$7d\n"
      "   Fallout Rate:            %2$8.3f\n"
      "   Exec Penalty:            %3$8.3f");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "expExecPeriodDdMsg",
      551,
      info_,
      "   Explosion Execution Periods:%1v$8d");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "belowHeadingDdMsg",
      552,
      info_,
      "   Below List:");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "belowDataDdMsg",
      553,
      info_,
      "      %1$-12s");

   //---------------------------------------------------------------------------

   makeMsg (
      "getting34AttributeWithCompatibilityCycleTime",
     554,
     info_,
      "The function %1$s has been invoked to get the value of "
      "WIT Version 3.4 Part attribute cycleTime. "
      "This attribute and BomEntry attribute usageTime have been superseded "
      "by BomEntry attribute offset. "
      "However, since wit34Compatable is TRUE, the value of "
      "a \"wit34Compatible-mode-only\" Operation attribute cycleTime "
      "will be retrieved.");

   //---------------------------------------------------------------------------

   makeMsg (
      "getting34AttributeWithoutCompatibilityCycleTime",
      555,
      severe_,
      "The function %1$s has been invoked to get the value of "
      "WIT Version 3.4 Part attribute cycleTime. "
      "This attribute and BomEntry attribute usageTime have been superseded "
      "by BomEntry attribute offset. "
      "Since wit34Compatible is FALSE, "
      "this attribute value can not be retrieved.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "setting34AttributeWithCompatibilityCycleTime",
      556,
      info_,
      "The function %1$s has been invoked to set the value of "
      "WIT Version 3.4 Part attribute cycleTime. "
      "This attribute and BomEntry attribute usageTime have been superseded "
      "by BomEntry attribute offset. "
      "However, since wit34Compatible is TRUE, "
      "a \"wit34Compatible-mode-only\" Operation attribute cycleTime "
      "is being set, "
      "and a corresponding adjustment is being made to the offset attribute of "
      "each BomEntry in the BOM of the Operation that produces "
      "the specified Part.");

   //---------------------------------------------------------------------------

   makeMsg (
      "setting34AttributeWithoutCompatibilityCycleTime",
      557,
      severe_,
      "The function %1$s has been invoked to set the value of "
      "WIT Version 3.4 Part attribute cycleTime. "
      "This attribute and BomEntry attribute usageTime has been superseded "
      "by BomEntry attribute offset. "
      "Since wit34Compatible is FALSE, this attribute value can not be set.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);
         ;



   makeMsg (
      "getting34AttributeWithCompatibilityUsageTime",
      558,
      info_,
      "The function %1$s has been invoked to get the value of "
      "WIT Version 3.4 BomEntry attribute usageTime. "
      "This attribute and Part attribute cycleTime have been superseded "
      "by BomEntry attribute offset. "
      "However, since wit34Compatible is TRUE, "
      "a corresponding value of usageTime will be computed and retrieved.");

   //---------------------------------------------------------------------------

   makeMsg (
      "getting34AttributeWithoutCompatibilityUsageTime",
      559,
      severe_,
      "The function %1$s has been invoked to get the value of "
      "WIT Version 3.4 BomEntry attribute usageTime. "
      "This attribute and Part attribute cycleTime have been superseded "
      "by BomEntry attribute offset. "
      "Since wit34Compatible is FALSE, "
      "this attribute value can not be retrieved.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "setting34AttributeWithCompatibilityUsageTime",
      560,
      info_,
      "The function %1$s has been invoked to set the value of "
      "WIT Version 3.4 BomEntry attribute usageTime. "
      "This attribute and Part attribute cycleTime have been superseded "
      "by BomEntry attribute offset. "
      "However, since wit34Compatible is TRUE, "
      "a corresponding value of offset will be computed and set.");

   //---------------------------------------------------------------------------

   makeMsg (
      "setting34AttributeWithoutCompatibilityUsageTime",
      561,
      severe_,
      "The function %1$s has been invoked to set the value of "
      "WIT Version 3.4 BomEntry attribute usageTime. "
      "This attribute and Part attribute cycleTime have been superseded "
      "by BomEntry attribute offset. "
      "Since wit34Compatible is FALSE, this attribute value can not be set.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "noPartsOrOpnsRequiredSmsg",
      562,
      severe_,
      "An attempt was made to set the value of attribute \"%1$s\" when "
      "one or more parts or operations exist.\n"
      "The number of parts is %2$d.\n"
      "The number of operations is %3$d.\n"
      "This attribute may only be set when there are no parts or operations.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "firstKeyWordWdMsg",
      572,
      info_,
      "%1$s");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "keyWordWdMsg",
      573,
      info_,
      " %1$s");

   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "firstInFileKeyWordWdMsg",
      574,
      info_,
      "%1$s");

   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "endCommandWdMsg",
      575,
      info_,
      ";");

   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "doubleWdMsg",
      576,
      info_,
      " %1$g");

   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "intWdMsg",
      577,
      info_,
      " %1$d");

   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "boolWdMsg",
      578,
      info_,
      " %1$b");

   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "stringWdMsg",
      579,
      info_,
      " \"%1$s\"");

   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "startVectorWdMsg",
      580,
      info_,
      " %1$s (");

   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "endVectorWdMsg",
      581,
      info_,
      ")");

   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "doubleSingleVectorWdMsg",
      582,
      info_,
      " single (%1$g)");

   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "doubleSparseVectorWdMsg",
      583,
      info_,
      "%1$s%2$d:%3$g");

   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "doubleDenseVectorWdMsg",
      584,
      info_,
      "%1$s%2$g");

   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "intSingleVectorWdMsg",
      585,
      info_,
      " single (%1$d)");

   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "intSparseVectorWdMsg",
      586,
      info_,
      "%1$s%2$d:%3$d");

   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "intDenseVectorWdMsg",
      587,
      info_,
      "%1$s%2$d");

   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "execBoundsHeadingDdMsg",
      588,
      info_,
      "   Execution Bounds:");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "stockBoundsHeadingDdMsg",
      589,
      info_,
      "   Stock Bounds:");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "cumShipBoundsHeadingDdMsg",
      590,
      info_,
      "   Cumulative Shipment Bounds:");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "paramReadErrorInvalidChoiceMain",
      591,
      severe_,
      "Input error in control parameter file \"%1$s\".\n"
      "\"%2$s\" parameter specified as \"%3$s\".\n"
      "Valid choices are:");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "paramReadErrorInvalidChoiceAlt",
      592,
      severe_,
      "   \"%1$s\"");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "paramReadErrorInvalidChoiceEnd",
      593,
      severe_,
      "");

   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "nonDevParamHeadingMsg",
      594,
      info_,
      "Control Parameter Input File: %1$s\n"
      "\n"
      "Control Parameter Settings\n"
      "==========================");

   currentMsg_->testable              (false);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "devParamHeadingMsg",
      595,
      info_,
      "Development Control Parameter Settings\n"
      "======================================");

   currentMsg_->testable              (false);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "stringParamMsg",
      596,
      info_,
      "%1$-15s: %2$-39s: %3$s");

   currentMsg_->testable              (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "intParamMsg",
      597,
      info_,
      "%1$-15s: %2$-39s: %3$d");

   currentMsg_->testable              (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "blankMsg",
      598,
      info_,
      "");

   currentMsg_->testable              (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "paramReadErrorReadProblemName",
      599,
      severe_,
      "Input error in control parameter file \"%1$s\".\n"
      "An error occurred while reading a control parameter name.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "paramReadErrorEofName",
      600,
      severe_,
      "Input error in control parameter file \"%1$s\".\n"
      "End-Of-File encountered while reading the value of control parameter "
      "\"%2$s\".");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "paramReadErrorReadProblemValue",
      601,
      severe_,
      "Input error in control parameter file \"%1$s\".\n"
      "An error occurred while reading the value of control parameter "
      "\"%2$s\".");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "dblParamMsg",
      602,
      info_,
      "%1$-15s: %2$-39s: %3$g");

   currentMsg_->testable              (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "negOperationCostWmsg",
      605,
      warning_,
      "Attribute \"%1$s\" of operation \"%2$s\" is being set to "
      "a negative value of %3$f in period %4$d.\n"
      "A negative value is permitted, "
      "but it may cause the objective to go to positive infinity.");

   currentMsg_->maxTimesIssued        (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "globalDataDdMsg",
      606,
      info_,
      "WIT 3.4 compatible mode?                     %01$5b\n"
      "Substitute offsets independent of BOM entry? %02$5b\n"
      "Execute an operation when its BOM is empty?  %03$5b\n"
      "Compute critical part list?                  %04$5b\n"
      "Pegged critical list mode?                   %05$5b\n"
      "Size of hash table:                      %06$9d\n"
      "Round up reqVols and focShortageVols?        %07$5b\n"
      "Output precision:                            %08$5d\n"
      "Use focus horizons in FSS?                   %09$5b\n"
      "Number of positive execution penalties:  %10$9d\n"
      "Preprocessing performed?                     %11$5b");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "preprocDataDdMsg",
      607,
      info_,
      "Data contains positive lot sizes?            %1$5b");

   //---------------------------------------------------------------------------

   makeMsg (
      "focusHorizonDdMsg",
      608,
      info_,
      "   Focus Horizon:          %1$5d");

   //---------------------------------------------------------------------------

   makeMsg (
      "noParamAddMsg",
      609,
      severe_,
      "No parameters have been defined for \"add %1$s\".");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "noParamSetMsg",
      610,
      severe_,
      "No parameters have been defined for \"set %1$s\".");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "lineNumberMsg",
      611,
      severe_,
      "Last token read: \"%1$s\" at line number %2$-d in file\n%3$s");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "noAttributeDefMsg",
      612,
      severe_,
      "Attribute \"%1$s\" is not defined for object type \"%2$s\".");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "tooManyArgsMsg",
      613,
      severe_,
      "Too many arguments specified for \"%1$s%2$s\".\n"
      "Exactly %3$-d %4$s required. ");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "vecValueNotIntMsg",
      614,
      severe_,
      "Vector value for attribute \"%1$s\" must be an integer.");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "booleanVectorMsg",
      615,
      severe_,
      "Boolean value in vector for attribute \"%1$s\" is not allowed.");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "stringVectorMsg",
      616,
      severe_,
      "String value in vector for attribute \"%1$s\" is not allowed.");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "tooManyValuesMsg",
      617,
      severe_,
      "Number of values in dense vector for attribute \"%1$s\""
      " must be equal to %2$-d.");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "vecTypeNotValidMsg",
      618,
      severe_,
      "A vector type is not valid for attribute \"%1$s\".");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "invalidPeriodMsg",
      619,
      severe_,
      "A period value in sparse vector for attribute \"%1$s\""
      " must be >= 0 and < %2$-d.");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "duplicatePeriodMsg",
      620,
      severe_,
      "Duplicate period specified in sparse vector for attribute \"%1$s\".");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "duplicateHardLBMsg",
      621,
      severe_,
      "HardLB bounds have already been specified for attribute \"%1$s\".");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "duplicateSoftLBMsg",
      622,
      severe_,
      "SoftLB bounds have already been specified for attribute \"%1$s\".");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "duplicateHardUBMsg",
      623,
      severe_,
      "HardUB bounds have already been specified  for attribute \"%1$s\".");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "unterminatedStringMsg",
      624,
      severe_,
      "Unterminated string.");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "invalidCharacterMsg",
      625,
      severe_,
      "Invalid character in input.");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->maxTimesIssued        (80);

   //---------------------------------------------------------------------------

   makeMsg (
      "invalidArgTypeMsg",
      626,
      severe_,
      "Argument %1$-d for \"%2$s %3$s\" %4$s.");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "noInputFileMsg",
      627,
      fatal_,
      "No input data file specified.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "incorrectNumberArgMsg",
      628,
      severe_,
      "\"%1$s%2$s\" must have %3$-d %4$s. "
      "%5$-d %6$s.");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "valuesNotEqualPeriodsMsg",
      630,
      severe_,
      "Number of values in dense vector for attribute \"%1$s\""
      " must be equal to nPeriods.\n"
      "# values = %2$d\n"
      "nPeriods = %3$d");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "parsingStopsMsg",
      631,
      severe_,
      "Parsing of data input stops.");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "effConsRateDdMsg",
      636,
      info_,
      "   Effective Consumption Rates:%1v$8.1f");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "bomConsumedPartCategorySmsg",
      638,
      severe_,
      "Attribute is only defined when the consumed part category is %2$s.\n"
      "   Part category      : %1$s");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "invalidAttributeMsg",
      639,
      severe_,
      "Attribute \"%1$s\" is only defined when the part category is \"%2$s\".");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "invalidValueTypeMsg",
      640,
      severe_,
      "The value of attribute \"%1$s\" %2$s.");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "noArgsMsg",
      641,
      severe_,
      "Expecting %1$-d %2$s after \"%3$s\" but found \"%4$s\".");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "syntaxErrorMsg",
      644,
      severe_,
      "Expecting %1$s but found \"%2$s\".");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "settingCycleTime34WithoutCompatibility",
      645,
      severe_,
      "The function %1$s has been invoked to set the value of "
      "Operation attribute cycleTime34. "
      "Since wit34Compatible is FALSE, this attribute value can not be set.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "bomEntryConsIndexRangeSmsg",
      647,
      severe_,
      "The consIndex for a consuming BOM entry is out of range.\n"
      "   Value specified    : %1$d\n"
      "   Range              : %2$d <= value < %3$d");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "subEntryConsIndexRangeSmsg",
      648,
      severe_,
      "The consIndex for a consuming substitute BOM entry is out of range.\n"
      "   Value specified    : %1$d\n"
      "   Range              : %2$d <= value < %3$d");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "bopEntryProdIndexRangeSmsg",
      649,
      severe_,
      "The prodIndex for a producing BOP entry is out of range.\n"
      "   Value specified    : %1$d\n"
      "   Range              : %2$d <= value < %3$d");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "witWriteProdSchedWithCompatibility",
      652,
      info_,
      "The function witWriteProdSched has been invoked to write an execution "
      "schedule. This function has been superseded by the function "
      "witWriteExecSched. Since wit34Compatible is TRUE, "
      "witWriteExecSched will be invoked.");

   //---------------------------------------------------------------------------

   makeMsg (
      "witWriteProdSchedWithoutCompatibility",
      653,
      severe_,
      "The function witWriteProdSched has been invoked to write an execution "
      "schedule. This function has been superseded by the function "
      "witWriteExecSched. Since wit34Compatible is FALSE, witWriteProdSched "
      "is not available.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "negConsRateMsg",
      654,
      info_,
      "The consRate attribute of BOM entry #%1$d of operation %2$s is being "
      "set to a negative value of %3$f in period %4$d. It is usually "
      "more natural to use a BOP entry with a positive productRate in place of "
      "a BOM entry with a negative consRate.");

   currentMsg_->issueDuringRD         (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);
   currentMsg_->maxTimesIssued        (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "attrValueNotFound",
      655,
      severe_,
      "Expecting a value of type %1$s for attribute \"%2$s\" "
      "but found \"%3$s\".");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "stackOverflowMsg",
      659,
      severe_,
      "Too many files have been opened by the \"read\" command.\n"
      "The maximum number is %1$-d.");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "inactiveHeurAllocSmsg",
      660,
      severe_,
      "Heuristic allocation must be active, when this function is called.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "incHeurDataMsg",
      661,
      info_,
      "Desired  increment to shipVol[%1$d] is %2$.3f.\n"
      "Achieved increment to shipVol[%1$d] is %3$.3f.");

   //---------------------------------------------------------------------------

   makeMsg (
      "invalidStringSmsg",
      663,
      severe_,
      "Expecting a \" or a \\ after a \\ but found \"%1$s\"");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "argRangeIntLeValLtIntSmsg",
      665,
      severe_,
      "Argument is out of range.\n"
      "   Value specified    : %1$d\n"
      "   Range              : %2$d <= value < %3$d");

   currentMsg_->idInputOnIssue        (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "argRangeValGeDblSmsg",
      666,
      severe_,
      "Argument is out of range.\n"
      "   Value specified    : %1$f\n"
      "   Range              : value >= %2$f");

   currentMsg_->idInputOnIssue        (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "heurAllActiveMsg",
      668,
      info_,
      "Heuristic allocation is now active.");

   //---------------------------------------------------------------------------

   makeMsg (
      "heurAllInactiveMsg",
      669,
      info_,
      "Heuristic allocation is now inactive.");

   //---------------------------------------------------------------------------

   makeMsg (
      "argRangeValGeIntSmsg",
      670,
      severe_,
      "Argument is out of range.\n"
      "   Value specified    : %1$d\n"
      "   Range              : value >= %2$d");

   currentMsg_->idInputOnIssue        (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "dupTargetSmsg",
      671,
      severe_,
      "Equitable allocation targets #%1$d and #%2$d are duplicates.\n"
      "   Demanded part name : %3$s\n"
      "   Demand name        : %4$s\n"
      "   shipPeriod         : %5$d");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "targetNumMsg",
      672,
      info_,
      "Equitable allocation target #%1$d:");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "inputArgumentSmsg",
      673,
      severe_,
      "   Argument           : %1$s");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "inputArgElSmsg",
      674,
      severe_,
      "   Argument           : %1$s[%2$d]");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "nullPointerSmsg",
      675,
      severe_,
      "A function argument was passed as a null pointer "
      "where this is not allowed.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "nullSetArgSmsg",
      676,
      severe_,
      "Attempt to set an attribute to a null pointer "
      "where this in not allowed.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "yieldRateDdMsg",
      679,
      info_,
      "   Yield Rates:%1v$8.2f");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "yieldRateRangeSmsg",
      680,
      severe_,
      "Attribute is out of range.\n"
      "   Value specified    : %1$f\n"
      "   Range              : 0.01 <= value <= 1.0, or value = 0.0");

   currentMsg_->delayedSevere         (true);
   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "getPre50AttrMsg",
      681,
      info_,
      "The function \"%1$s\" has been invoked to retrieve the value "
      "of the %2$s \"%3$s\" attribute. "
      "As of WIT release 5.0, this attribute has been replaced "
      "by the %2$s \"%4$s\" attribute. "
      "A value of \"%3$s\" equivalent to the current value of \"%4$s\" "
      "will be retrieved.");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "setPre50AttrMsg",
      682,
      info_,
      "The function \"%1$s\" has been invoked "
      "to set the value of the %2$s \"%3$s\" attribute. "
      "As of WIT release 5.0, this attribute has been replaced "
      "by the %2$s \"%4$s\" attribute. "
      "The \"%4$s\" attribute will be set to a value equivalent "
      "to the indicated value of \"%3$s\".");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "maxViolationMsg",
      684,
      info_,
      "Maximum constraint violation in the current implosion solution: %1$g.");

   //---------------------------------------------------------------------------

   makeMsg (
      "maxViolHeadingMsg",
      685,
      info_,
      "One or more constraint violations have been found with magnitude "
      "greater than %1$g (WIT's feasibility tolerance).\n"
      "The maximimum violation found so far is:"
      );

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "maxViolConstraintMsg",
      686,
      info_,
      "   Constraint:  %1$m");

   //---------------------------------------------------------------------------

   makeMsg (
      "maxViolObjectMsg",
      687,
      info_,
      "   %1$-10s   \"%2$s\"");

   //---------------------------------------------------------------------------

   makeMsg (
      "maxViolIndexMsg",
      688,
      info_,
      "   %1$-12s %2$d");

   //---------------------------------------------------------------------------

   makeMsg (
      "maxViolPeriodMsg",
      689,
      info_,
      "   Period:      %1$d");

   //---------------------------------------------------------------------------

   makeMsg (
      "expBopHeadDdMsg",
      690,
      info_,
      "   Explosion BOP Entries:\n"
      "      Period  Producing Operation  BOP Entry #");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "expBopDataDdMsg",
      691,
      info_,
      "      %1$6d         %2$12s  %3$-8d");

   //---------------------------------------------------------------------------

   makeMsg (
      "optAttDdMsg",
      692,
      info_,
      "CPLEX  Embedded?                             %1$5b\n"
      "Compute shadow prices?                       %2$5b\n"
      "Stay accelerated after opt implosion?        %3$5b\n"
      "Stay accelerated after an LB is softened?    %4$5b\n"
      "Opt implosion in MIP mode?                   %5$5b\n"
      "Multiple objectives mode?                    %6$5b\n"
      "Bounds objective weight:                 %7$9.1f\n"
      "Solver Log File Name:                   %8$10s");

   //---------------------------------------------------------------------------

   makeMsg (
      "permCommitFailedFmsg",
      694,
      fatal_,
      "HeurAtor::permCommit failed.\n"
      "   Part:      \"%1$s\"\n"
      "   Period:    %2$d\n"
      "   Qty:       %3$g");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "invalidBoolTempParSmsg",
      695,
      severe_,
      "Control parameter tempPar%1$d was used as a boolean, but its value is "
      "invalid for a boolean.\n"
      "Parameter value: \"%2$s\"\n"
      "Valid values: \"yes\", \"no\"");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "invalidIntTempParSmsg",
      696,
      severe_,
      "Control parameter tempPar%1$d was used as an integer, but its value is "
      "invalid for an integer.\n"
      "Parameter value: \"%2$s\"");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "invalidDoubleTempParSmsg",
      697,
      severe_,
      "Control parameter tempPar%1$d was used as a double, but its value is "
      "invalid for a double.\n"
      "Parameter value: \"%2$s\"");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "setSubOffsetSmsg",
      698,
      severe_,
      "Attempt to set a substitute BOM entry offset "
      "when independentOffsets is FALSE.");

   currentMsg_->delayedSevere         (true);
   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "invalidMsgIDFmsg",
      699,
      fatal_,
      "MsgFacility::operator () was called for a message ID "
      "that does not match any message in the facility.\n\n"
      "   Message ID: \"%1$s\"\n");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "invalidMsgFragIDFmsg",
      700,
      fatal_,
      "MsgFacility::operator [] was called for a message fragment ID "
      "that does not match any message fragment in the facility.\n\n"
      "   Message Fragment ID: \"%1$s\"\n");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "explainFatalMessageMsg",
      701,
      info_,
      "The above message indicates that there is a programming error in WIT.");

   currentMsg_->testable              (false);
   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "allIssuedMsg",
      702,
      info_,
      "All testable messages have been issued.");

   currentMsg_->testable              (false);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "nUnissuedMsg",
      703,
      info_,
      "The following %1$d testable messages were not issued:");

   currentMsg_->testable              (false);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "unissuedMsgMsg",
      704,
      info_,
      "   %1$4d  %2$s");

   currentMsg_->testable              (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "funcNotAvailSmsg",
      705,
      severe_,
      "Function \"%1$s\" is not available in this build of WIT.");

   //---------------------------------------------------------------------------

   makeMsg (
      "intLowEqUpEqSmsg",
      707,
      severe_,
      "Attribute is out of range.\n"
      "   Value specified    : %1$d\n"
      "   Range              : %2$d <= value <= %3$d");

   currentMsg_->delayedSevere         (true);
   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "postStateMsg",
      708,
      info_,
      "This WitRun is now in a postprocessed state.");

   //---------------------------------------------------------------------------

   makeMsg (
      "unpostStateMsg",
      709,
      info_,
      "This WitRun is now in an unpostprocessed state.");

   //---------------------------------------------------------------------------

   makeMsg (
      "accStateMsg",
      710,
      info_,
      "This WitRun is now in an accelerated state.");

   //---------------------------------------------------------------------------

   makeMsg (
      "unaccStateMsg",
      711,
      info_,
      "This WitRun is now in an unaccelerated state.");

   //---------------------------------------------------------------------------

   makeMsg (
      "crashMsg",
      712,
      info_,
      "Since the above message indicates a fatal error, and the "
      "\"crashOnFatal\" control parameter was set to \"yes\", WIT will now "
      "deliberately crash.");

   currentMsg_->testable              (false);
   currentMsg_->dupToStdout           (true);
   currentMsg_->issueDuringRD         (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "selForDelDdMsg",
      713,
      info_,
      "   This object has been selected for deletion.");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "objectTypeMsg",
      714,
      info_,
      "   Object Type:               %1$m");

   //---------------------------------------------------------------------------

   makeMsg (
      "partInstIDMsg",
      715,
      info_,
      "   Part Name:                 \"%1$s\"");

   //---------------------------------------------------------------------------

   makeMsg (
      "operationInstIDMsg",
      716,
      info_,
      "   Operation Name:            \"%1$s\"");

   //---------------------------------------------------------------------------

   makeMsg (
      "bomEntryInstIDMsg",
      717,
      info_,
      "   Consuming Operation Name:  \"%1$s\"\n"
      "   BOM Entry Index:           %2$d\n"
      "   Consumed Part Name:        \"%3$s\"");

   //---------------------------------------------------------------------------

   makeMsg (
      "subEntryInstIDMsg",
      718,
      info_,
      "   Consuming Operation Name:  \"%1$s\"\n"
      "   BOM Entry Index:           %2$d\n"
      "   Replaced Part Name:        \"%3$s\"\n"
      "   Substitute Index:          %4$d\n"
      "   Consumed Part Name:        \"%5$s\"");

   //---------------------------------------------------------------------------

   makeMsg (
      "bopEntryInstIDMsg",
      719,
      info_,
      "   Producing Operation Name:  \"%1$s\"\n"
      "   BOP Entry Index:           %2$d\n"
      "   Produced Part Name:        \"%3$s\"");

   //---------------------------------------------------------------------------

   makeMsg (
      "demandInstIDMsg",
      720,
      info_,
      "   Demanded Part Name:        \"%1$s\"\n"
      "   Demand Name:               \"%2$s\"");

   //---------------------------------------------------------------------------

   makeMsg (
      "purgeDataMsg",
      722,
      info_,
      "Object purge requested.\n"
      "Searching for objects to be deleted.");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "userSelForDelMsg",
      723,
      info_,
      "The user has selected the following object for deletion:");

   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "autoSelForDelMsg",
      724,
      info_,
      "WIT has selected the following object for deletion because "
      "at least one of its prerequisites has been selected for deletion:");

   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "selPrereqMsg",
      725,
      info_,
      "The prerequisite is:");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "shortDividerMsg",
      726,
      info_,
      "-----------------------------------------------------------------------");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "longDividerMsg",
      727,
      info_,
      "----------------------------------------"
      "----------------------------------------");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "noSelForDelsMsg",
      728,
      info_,
      "No objects have been selected for deletion.");

   //---------------------------------------------------------------------------

   makeMsg (
      "delCompsMsg",
      729,
      info_,
      "Deleting selected objects.\n\n"
      "   Number of objects selected by the user: %1$7d\n"
      "   Number of objects selected by WIT:      %2$7d\n"
      "   Number of objects selected total:       %3$7d");

   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "purgeCompleteMsg",
      730,
      info_,
      "Object purge complete.\n\n"
      "   Number of objects before purge: %1$7d\n"
      "   Number of objects deleted:      %2$7d\n"
      "   Number of objects after purge:  %3$7d");

   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "twmeMsg",
      732,
      info_,
      "The two-way multiple execution periods technique will be used.");

   //---------------------------------------------------------------------------

   makeMsg (
      "multiRouteMsg",
      735,
      info_,
      "The multiple routes technique will be used.");

   //---------------------------------------------------------------------------

   makeMsg (
      "multiSelMsg",
      736,
      info_,
      "The multiple selections technique will be used.");

   //---------------------------------------------------------------------------

   makeMsg (
      "partPreDdMsg",
      738,
      info_,
      "   Sorted Part Index: %1$12d\n"
      "   Explosion period gaps?    %2$5b");

   //---------------------------------------------------------------------------

   makeMsg (
      "multiExecMsg",
      739,
      info_,
      "The multiple execution periods technique will be used.");

   //---------------------------------------------------------------------------

   makeMsg (
      "penExecMsg",
      740,
      info_,
      "The penalized execution technique will be used.");

   //---------------------------------------------------------------------------

   makeMsg (
      "penExecWoMultiRouteWmsg",
      743,
      warning_,
      "The penExec attribute is TRUE, but the multiRoute attribute is FALSE.\n"
      "Penalized execution will not be performed.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "lmeWmsg",
      744,
      warning_,
      "The function \"%1$s\" has been invoked. This function accesses the "
      "obsolete global attribute \"localMultiExec\", which has been replaced "
      "by the global attribute \"multiExec\". For compatibility purposes, "
      "setting \"localMultiExec\" to TRUE causes heuristic implosion and "
      "allocation to use the (global) multiple execution periods technique, "
      "which is normally requested by setting \"multiExec\" to TRUE.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "lbaWmsg",
      745,
      warning_,
      "The function \"%1$s\" has been invoked. This function accesses the "
      "obsolete global attribute \"localBuildAhead\", which has been replaced "
      "by the part attribute \"buildNstn\". For compatibility purposes, "
      "setting \"localBuildAhead\" to TRUE causes heuristic implosion and "
      "allocation to use the NSTN build-ahead technique, "
      "which is normally requested by setting \"buildNstn\" to TRUE.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "nstnMsg",
      746,
      info_,
      "The NSTN build-ahead technique will be used.");

   //---------------------------------------------------------------------------

   makeMsg (
      "asapMsg",
      747,
      info_,
      "The ASAP build-ahead technique will be used.");

   //---------------------------------------------------------------------------

   makeMsg (
      "smallEffProdWmsg",
      748,
      warning_,

      "The expCutoff attribute was set to a smaller value than its default, "
      "and as a result, one or more BOP entries have been accepted for "
      "possible explosion that otherwise would have been rejected, due to "
      "their very small effective production rates "
      "(= yieldRate * productRate). "
      "The smallest effective production rate that was accepted and the "
      "corresponding BOP entry and execution period are given below:\n\n"

      "   expCutoff:              %1$g\n"
      "   Default expCutoff:      %2$g\n"
      "   Smallest Accepted Rate: %3$g\n"
      "   Operation:              \"%4$s\"\n"
      "   BOP Entry Index:        %5$d\n"
      "   Exec. Period:           %6$d\n\n"

      "This may cause numerical difficulties.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "rejBopEntWmsg",
      749,
      warning_,

      "One or more BOP entries have been rejected for possible explosion, "
      "because they had positive effective production rates "
      "(= yieldRate * productRate) that were below the value of expCutoff. The "
      "largest effective production rate that was rejected and the "
      "corresponding BOP entry and execution period are given below:\n\n"

      "   expCutoff:             %1$g\n"
      "   Largest Rejected Rate: %2$g\n"
      "   Operation:             \"%3$s\"\n"
      "   BOP Entry Index:       %4$d\n"
      "   Exec. Period:          %5$d");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "nMaxCommitsMsg",
      750,
      info_,
      "# Max commits:      %1$8d");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "nSelCommitsMsg",
      751,
      info_,
      "# Selected commits: %1$8d");

   //---------------------------------------------------------------------------

   makeMsg (
      "nCommsMsg",
      752,
      info_,
      "# Binary searches:  %1$8d\n"
      "# Temp-Commits:     %2$8d\n"
      "# Perm-Commits:     %3$8d\n"
      "#      Commits:     %4$8d");

   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "aveExecPenMsg",
      753,
      info_,
      "Estimated weighted average execution penalty: %1$.6f");

   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "wrongFuncForTwmeSmsg",
      754,
      severe_,
      "The function %1$s has been called when the twoWayMultiExec attribute is "
      "%2$b. This call is not allowed; the function %3$s should be called "
      "instead.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "incHeurMeDirMsg",
      755,
      info_,
      "Initial direction for multiple execution periods is %1$m.");

   //---------------------------------------------------------------------------

   makeMsg (
      "buildAheadUBDdMsg",
      757,
      info_,
      "   Build-Ahead Upper Bounds:%1v$8d");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "setVecAttrToScalMsg",
      758,
      info_,
      "The function \"%1$s\" has been invoked to set the value of the "
      "scalar %2$s attribute \"%3$s\", which has been replaced by the vector "
      "%2$s attribute \"%4$s\". The \"%4$s\" attribute will be set to the "
      "specified scalar value in all periods.");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "getVecAttrAsScalMsg",
      759,
      info_,
      "The function \"%1$s\" has been invoked to retrieve the value of the "
      "scalar %2$s attribute \"%3$s\", which has been replaced by the vector "
      "%2$s attribute \"%4$s\". The value of the \"%4$s\" attribute in "
      "period 0 will be retrieved.");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "shipLateUBDdMsg",
      760,
      info_,
      "   Ship-Late Upper Bounds:%1v$8d");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "oldSRMsg",
      761,
      info_,
      "The old stock reallocation technique will be performed.");

   //---------------------------------------------------------------------------

   makeMsg (
      "enfStockSLBsMsg",
      762,
      info_,
      "Enforcing stock soft lower bounds.");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "consRateDdMsg",
      764,
      info_,
      "   Consumption Rates:%1v$8.1f");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "productRateDdMsg",
      765,
      info_,
      "   Production Rates:%1v$8.2f");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "writeGlobalMsg",
      767,
      info_,
      "Writing the global input and preprocessing data.");

   currentMsg_->testable              (false);
   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "selSplitMsg",
      768,
      info_,
      "Selection splitting will be performed.");

   //---------------------------------------------------------------------------

   makeMsg (
      "critListSelSplitSmsg",
      770,
      severe_,
      "The critical parts list cannot be generated when selection splitting is "
      "being performed.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "incAllocMsg",
      771,
      info_,
      "IncHeurAlloc:\n"
      "   Part:      %1$s\n"
      "   Demand:    %2$s\n"
      "   Period:    %3$d\n"
      "   DesIncVol: %4$.0f\n"
      "   IncVol:    %5$.0f");

   currentMsg_->testable              (false);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "reqPerfPeggingSmsg",
      772,
      severe_,
      "The global \"perfPegging\" attribute must be TRUE when this function "
      "is called.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "concPeggingMsg",
      773,
      info_,
      "Concurrent pegging will be performed.");

   //---------------------------------------------------------------------------

   makeMsg (
      "shipPerMsg",
      774,
      info_,
      "Shipment Period is %1$d.");

   //---------------------------------------------------------------------------

   makeMsg (
      "globalComp1DdMsg",
      775,
      info_,
      "Use multiple execution periods?              %01$5b\n"
      "Use two-way multiple execution periods?      %02$5b\n"
      "Equitability:                             %00003$8d\n"
      "Forced multi-pass equitable allocation?      %04$5b\n"
      "User-specified heuristic starting solution?  %05$5b\n"
      "Modifiable heuristic allocation?             %06$5b\n"
      "Automatic priority?                          %07$5b\n"
      "Skip failed parts in heuristic?              %08$5b\n"
      "Multiple routes heuristic?                   %09$5b\n"
      "Penalized execution heuristic requested?     %10$5b\n"
      "Tie breaking proportionate routing?          %11$5b");

   //---------------------------------------------------------------------------

   makeMsg (
      "globalComp2DdMsg",
      776,
      info_,
      "Stock reallocation requested?                %1$5b\n"
      "Selection splitting requested?               %2$5b\n"
      "Selection recovery requested?                %3$5b\n"
      "Respect stock soft lower bounds?             %4$5b\n"
      "Give preference to higher stock SLBs?        %5$5b\n"
      "Perform concurrent pegging?                  %6$5b\n"
      "Use heuristic's shipment sequence for PIP?   %7$5b\n"
      "New PIP pegging order?                       %8$5b\n"
      "Truncate offsets?                            %9$5b\n"
      "Use high precision when writing input data? %10$6b\n"
      "NSTN residualVols?                          %11$6b\n"
      "Minimal excessVols?                         %12$6b\n"
      "Lot size tolerance:                       %0013$8g\n"
      "Explosion cutoff:                         %0014$8g");

   //---------------------------------------------------------------------------

   makeMsg (
      "stockReMsg",
      777,
      info_,
      "Stock reallocation will be performed.");

   //---------------------------------------------------------------------------

   makeMsg (
      "selSRMsg",
      778,
      info_,
      "Selective stock reallocation will be performed.");

   //---------------------------------------------------------------------------

   makeMsg (
      "bothTrueSmsg",
      780,
      severe_,
      "The global \"%1$s\" attribute cannot be set to TRUE, "
      "when the global \"%2$s\" attribute is TRUE.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "createWhileItrActSmsg",
      781,
      severe_,
      "Data objects cannot be created while object iteration is active.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "purgeWhileItrActSmsg",
      782,
      severe_,
      "Object purge is not allowed while object iteration is active.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "advancedToObjMsg",
      783,
      info_,
      "Object iteration has advanced to the following data object:");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "advancedToEndMsg",
      784,
      info_,
      "Object iteration has advanced past the last data object and has "
      "returned to an inactive state.");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "getObjWhileItrInactiveSmsg",
      785,
      severe_,
      "Object iteration's current location cannot be retrieved when object "
      "iteration is inactive.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "getObjObjTypeMismatchSmsg",
      786,
      severe_,
      "A retrieval of object iteration's current location was requested, "
      "but the object type requested does not match the object type at the "
      "current location.\n"
      "\n"
      "   Object Type Requested:           %1$m\n"
      "   Object Type at Current Location: %2$m");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "sglSrcMsg",
      787,
      info_,
      "The single-source technique will be used.");

   //---------------------------------------------------------------------------

   makeMsg (
      "sglSrcWoMultiRouteWmsg",
      788,
      warning_,
      "The singleSource attribute is TRUE for one or more parts or BOM "
      "entries, but the multiRoute attribute is FALSE.\n"
      "The single-source technique will not be used.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "sglSrcAndPenExecSmsg",
      789,
      severe_,
      "The single-source and penalized execution techniques cannot both be "
      "used in the same problem.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "sglSrcAndCritListSmsg",
      790,
      severe_,
      "The single-source and critical parts list techniques cannot both be "
      "used in the same problem.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "sglSrcAndBaByDemSmsg",
      791,
      severe_,
      "The single-source and build-ahead by demand techniques cannot both be "
      "used in the same problem.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "sglSrcCountsMsg",
      792,
      info_,
      "# Sgl-Src CMQ:             %1$5d\n"
      "# Sgl-Src CMQ-Fixed:       %2$5d\n"
      "# Full     Allocations:    %3$5d\n"
      "# Non-Full Allocations:    %4$5d\n"
      "# Sgl-Src Opps Considered: %5$5d\n"
      "# Sgl-Src Sels Discovered: %6$5d\n"
      "# Sgl-Src Sels Tested:     %7$5d\n"
      "# Sgl-Src Sels Accepted:   %8$5d\n"
      "# Sgl-Src Sels Rejected:   %9$5d");

   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "sglSrcAndEqAllSmsg",
      793,
      severe_,
      "The single-source technique cannot be used when equitability > 1.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "invalidAttrForMipModeSmsg",
      797,
      severe_,
      "The %1$s attribute must not be TRUE, when mipMode is TRUE.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "lpMipDimMsg",
      798,
      info_,
      "%1$m Problem Dimensions:\n"
      "   Number of Variables:    %2$8d\n"
      "   Number of Constraints:  %3$8d\n"
      "   Number of Coefficients: %4$8d");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "nIntVarsMsg",
      799,
      info_,
      "   Number of Integer Vars: %1$8d");

   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "lotSize2ThreshDdMsg",
      802,
      info_,
      "   Level 2 Lot Size Thresholds:%1v$8.2f");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "minLotSize2DdMsg",
      803,
      info_,
      "   Level 2 Minimum Lot Sizes:%1v$8.2f");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "incLotSize2DdMsg",
      804,
      info_,
      "   Level 2 Incremental Lot Sizes:%1v$8.2f");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "incLotSizeLT1Smsg",
      805,
      severe_,
      "incLotSize must be >= 1.0, when twoLevelLotSizes is TRUE\n"
      "\n"
      "   Operation:  \"%1$s\"\n"
      "   Period:     %2$d\n"
      "   incLotSize: %3$.3f");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "incLotSize2LT1Smsg",
      806,
      severe_,
      "incLotSize2 must be >= 1.0, when twoLevelLotSizes is TRUE\n"
      "\n"
      "   Operation:   \"%1$s\"\n"
      "   Period:      %2$d\n"
      "   incLotSize2: %3$.3f");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "userHeurStartMsg",
      808,
      info_,
      "The user-specified schedules will be used as the initial solution.");

   //---------------------------------------------------------------------------

   makeMsg (
      "uhsNonExecConViolSmsg",
      809,
      severe_,
      "The user-specified heuristic starting solution violates the following "
      "constraint:\n"
      "\n"
      "   execVol == 0.0, when the operation is not executable.\n"
      "\n"
      "   Operation: \"%1$s\"\n"
      "   Period:    %2$d\n"
      "   ExecVol:   %3$g");

   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "uhsConViolIntroWmsg",
      810,
      warning_,
      "WIT has detected a constraint violation in the user-specified heuristic "
      "starting solution This may cause similar or different constraint "
      "violations in the solution computed by the heuristic. The violated "
      "constraint was as follows:");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "uhsSubConViolWmsg",
      811,
      warning_,
      "   sum of subVols <= execVol\n"
      "\n"
      "   Max Violation:   %1$g\n"
      "   Operation:       \"%2$s\"\n"
      "   Bom Entry Index: %3$d\n"
      "   Period:          %4$d\n"
      "   ExecVol:         %5$g\n"
      "   Sum of SubVols:  %6$g");

   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "uhsCumDemandConViolWmsg",
      812,
      warning_,
      "   cum. shipVol <= cum. demandVol\n"
      "\n"
      "   Max Violation:  %1$g\n"
      "   Part:           \"%2$s\"\n"
      "   Demand:         \"%3$s\"\n"
      "   Period:         %4$d\n"
      "   Cum. ShipVol:   %5$g\n"
      "   Cum. DemandVol: %6$g");

   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "uhsExecHUBConViolWmsg",
      813,
      warning_,
      "   execVol <= execBounds.hardUB\n"
      "\n"
      "   Max Violation: %1$g\n"
      "   Operation:     \"%2$s\"\n"
      "   Period:        %3$d\n"
      "   ExecVol:       %4$g\n"
      "   HardUB:        %5$g");

   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "uhsLotSizeConViolWmsg",
      814,
      warning_,
      "   execVol must be lot size feasible.\n"
      "\n"
      "   Max Violation: %1$g\n"
      "   Operation:     \"%2$s\"\n"
      "   Period:        %3$d\n"
      "   ExecVol:       %4$g");

   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "uhsNNScrapConViolWmsg",
      815,
      warning_,
      "   scrapVol >= 0.0\n"
      "\n"
      "   Max Violation: %1$g\n"
      "   Part:          \"%2$s\"\n"
      "   Period:        %3$d\n"
      "   ScrapVol:      %4$g");

   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "netExecPenaltySmsg",
      816,
      severe_,
      "A BOM entry was found such that netAllowed is TRUE for at least "
      "one of its substitutes, and execPenalty > 0.0 for the BOM entry itself "
      "or at least one of its substitutes.\n"
      "\n"
      "   Operation: \"%1$s\"\n"
      "   BOM Entry  #%2$d");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "routingShareRangeSmsg",
      817,
      severe_,
      "Attribute is out of range.\n"
      "   Value specified    : %1$f\n"
      "   Range              : value >= 1.0");

   currentMsg_->delayedSevere         (true);
   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "propRtgMsg",
      818,
      info_,
      "The proportionate routing technique will be used.");

   //---------------------------------------------------------------------------

   makeMsg (
      "lexUnboundedSmsg",
      825,
      severe_,
      "Due to one or more negative cost attributes in the objective, \"%1$s\", "
      "the implosion problem has no optimal solution, "
      "because its objective function goes to positive infinity.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "routingShareDdMsg",
      828,
      info_,
      "   Routing Share Vector:%1v$8.1f");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "msktempSmsg",
      829,
      severe_,
      "Error from operating system function msktemp.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "pipMsg",
      830,
      info_,
      "Building a post-implosion pegging.");

   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "pipNegSubConsRateSmsg",
      831,
      severe_,
      "Post-implosion pegging with is not allowed if there are any substitute "
      "BOM entries with negative consRates.\n\n"
      "   Operation:        \"%1$s\"\n"
      "   BOM Entry Index:  %2$d\n"
      "   Substitute Index: %3$d\n"
      "   Period:           %4$d\n"
      "   ConsRate:         %5$.3f");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "pipMaxInfeasMsg",
      832,
      info_,
      "Maximum infeasibility encountered by post-implosion pegging: %1$g.");

   //---------------------------------------------------------------------------

   makeMsg (
      "pipMaxInfDataMsg",
      833,
      info_,
      "Occurred at:\n"
      "   Inf Part: %1$s\n"
      "   Inf Per:  %2$d");

   currentMsg_->testable              (false);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "unpostPipSmsg",
      834,
      severe_,
      "The WitRun must be in a postprocessed state when post-implosion pegging "
      "is invoked.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "pipSeqReducWmsg",
      835,
      warning_,
      "Post-implosion pegging has reduced the incShipVol for one or more "
      "triples in the shipment sequence in order to avoid exceeding the "
      "shipVol of the demand. The maximum reduction was as follows:\n\n"

      "   Demanded Part: %1$s\n"
      "   Demand:        %2$s\n"
      "   Ship Period:   %3$d\n"
      "   Reduction:     %4$g");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "needPipExistsSmsg",
      836,
      severe_,
      "A function that requires pipExists to be TRUE was called when pipExists "
      "was FALSE.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "incShipVolMsg",
      837,
      info_,
      "IncShipVol is %1$.0f.");

   //---------------------------------------------------------------------------

   makeMsg (
      "pipSeqHeadingDdMsg",
      838,
      info_,
      "Shipment Sequence for Post-Implosion Pegging:\n"
      "---------------------------------------------");

   currentMsg_->preceedingLineFeeds   (2);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "EmptyPipSeqDdMsg",
      839,
      info_,
      "   The shipment sequence is empty.");

   //---------------------------------------------------------------------------

   makeMsg (
      "pipShipTripDdMsg",
      840,
      info_,
      "   Part \"%1$s\", Demand \"%2$s\", Ship Period %3$d, IncShipVol %4$.3f");

   //---------------------------------------------------------------------------

   makeMsg (
      "pipSeqPreCommentWdMsg",
      841,
      info_,
      "//---------------------------------------------------\n"
      "// The following lines set the Post-Implosion Pegging\n"
      "// Shipment Sequence, using an undocumented format.\n"
      "//---------------------------------------------------");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "pipSeqPostCommentWdMsg",
      842,
      info_,
      "//------------------------------\n"
      "// End of PIP Shipment Sequence.\n"
      "//------------------------------");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "doubleHpWdMsg",
      843,
      info_,
      " %1$.14g");

   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "doubleSingleVectorHpWdMsg",
      844,
      info_,
      " single (%1$.14g)");

   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "doubleSparseVectorHpWdMsg",
      845,
      info_,
      "%1$s%2$d:%3$.14g");

   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "doubleDenseVectorHpWdMsg",
      846,
      info_,
      "%1$s%2$.14g");

   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "unknownExcFmsg",
      849,
      fatal_,
      "An exception of unknown type was thrown.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "highPriPropRtgMsg",
      850,
      info_,
      "Proportionate routing will override the execution penalties.");

   //---------------------------------------------------------------------------

   makeMsg (
      "tieBreakPropRtMsg",
      851,
      info_,
      "Proportionate routing will break ties in the execution penalties.");

   //---------------------------------------------------------------------------

   makeMsg (
      "pgdCritListSelSplitSmsg",
      852,
      severe_,
      "Pegged critical list mode and selection splitting cannot be used in the "
      "same problem.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "pgdCritListSglSrcSmsg",
      853,
      severe_,
      "Pegged critical list mode and single-source cannot be used in the same "
      "problem.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "pgdCritListMsg",
      854,
      info_,
      "The pegged critical list will be generated.");

   //---------------------------------------------------------------------------

   makeMsg (
      "propRtgDdMsg",
      855,
      info_,
      "   Proportionate Routing Vector:%1v$8b");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "boolSingleVectorWdMsg",
      856,
      info_,
      " single (%1$b)");

   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "boolSparseVectorWdMsg",
      857,
      info_,
      "%1$s%2$d:%3$b");

   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "boolDenseVectorWdMsg",
      858,
      info_,
      "%1$s%2$b");

   currentMsg_->trailingLineFeeds     (0);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "getAttrNewNameMsg",
      860,
      info_,
      "The function \"%1$s\" has been invoked to retrieve the value of the "
      "%2$s attribute \"%3$s\", which has been replaced by the attribute "
      "\"%4$s\". The value of the \"%4$s\" attribute will be retrieved.");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "setAttrNewNameMsg",
      861,
      info_,
      "The function \"%1$s\" has been invoked to set the value of the "
      "%2$s attribute \"%3$s\", which has been replaced by the attribute "
      "\"%4$s\". The \"%4$s\" attribute will be set to the specified value.");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "returnOnErrorSmsg",
      862,
      severe_,
      "WIT has encountered an error condition and "
      "is now returning execution to the application program.\n"
      "The return code is %1$d.");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "throwWitExcSmsg",
      863,
      severe_,
      "WIT has encountered an error condition "
      "and is now throwing an exception of type WitErrorExc.\n"
      "The return code is %1$d.");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "setMsg2ndBoolAttrMsg",
      864,
      info_,
      "The %1$s attribute for message number %2$d has been set to FALSE, "
      "because the %3$s attribute for this message has been set to TRUE.");

   currentMsg_->testable              (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "setMsgType2ndBoolAttrMsg",
      865,
      info_,
      "The %1$s attribute for %2$m messages has been set to FALSE, "
      "because the %3$s attribute for these messages has been set to TRUE.");

   //---------------------------------------------------------------------------

   makeMsg (
      "uninitWitRun2Smsg",
      866,
      severe_,
      "The function %1$s was called with an uninitialized WitRun "
      "as the %2$s argument.");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "stdExcFmsg",
      867,
      fatal_,
      "A standard library exception of type %1$s was thrown.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "overlapStandAloneSmsg",
      868,
      severe_,
      "An overlapping call to witExecStandAlone was detected.");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "discardMemoryMsg",
      869,
      info_,
      "%1$4dMB of memory have been discarded.");

   currentMsg_->testable              (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "invIssueByExcFmsg",
      870,
      fatal_,
      "The WIT internal function WitMsgFacility::issueByException was called "
      "with a non-aborting message, which is not allowed.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "overlapReadDataSmsg",
      872,
      severe_,
      "witReadData has been called while a previous call to witReadData was "
      "still in progress. "
      "Overlapping calls to witReadData are not allowed.");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "postErrApiCallSameWRSmsg",
      873,
      severe_,
      "A function was called with a WitRun that previously was passed "
      "to a function that either returned with return code >= WitSEVERE_RC "
      "or threw an exception of type WitErrorExc.\n"
      "\n"
      "   Function: %1$s");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "newRetNullFmsg",
      874,
      fatal_,
      "The \"new\" operator returned NULL instead of throwing a "
      "std:bad_alloc exception, when it was unable to allocate the "
      "requested memory.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "postErrReadDataSmsg",
      877,
      severe_,
      "witReadData was called when a previous call to witReadData "
      "had returned with return code >= 3.\n");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "standAloneAfterStopSmsg",
      879,
      severe_,
      "witExecStandAlone was called after either the stopRunning attribute "
      "or the throwErrorExc attribute was set for some message.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "pipSizesMsg",
      883,
      info_,
      "Max # PIP Req Specs Active: %1$d\n"
      "Total # PIP Req Specs:      %2$d");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "attNyaSmsg",
      884,
      severe_,
      "The attribute \"%1$s\" is not yet available.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "totPegTripsMsg",
      885,
      info_,
      "Total # of pegging triples: %1$d");

   //---------------------------------------------------------------------------

   makeMsg (
      "oldPipPggOrderMsg",
      886,
      info_,
      "The old PIP pegging order will be used.");

   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "extOptInactiveSmsg",
      890,
      severe_,
      "The function %1$s was called while external optimizing implosion was "
      "not active.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "extOptActiveSmsg",
      891,
      severe_,
      "The function %1$s was called while external optimizing implosion was "
      "active.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "attNotAvailSmsg",
      892,
      severe_,
      "The %1$s \"%2$s\" attribute is not available in this build of WIT.");

   //---------------------------------------------------------------------------

   makeMsg (
      "searchIncSelSplitSmsg",
      893,
      severe_,
      "The searchInc attribute must be equal to 1.0 for all demands, "
      "when selection splitting is performed.\n"
      "\n"
      "   Part:      %1$s\n"
      "   Demand:    %2$s\n"
      "   SearchInc: %3$5.3f");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "startExtOptMsg",
      894,
      info_,
      "Initiating external optimizing implosion.");

   //---------------------------------------------------------------------------

   makeMsg (
      "finishExtOptMsg",
      895,
      info_,
      "Concluding external optimizing implosion.");

   //---------------------------------------------------------------------------

   makeMsg (
      "invalidAttrForStartExtSmsg",
      896,
      severe_,
      "The function witStartExtOpt has been called while the %1$s attribute is "
      "TRUE.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "periodMsg",
      897,
      info_,
      "Period is %1$d.");

   //---------------------------------------------------------------------------

   makeMsg (
      "nonnegConstraintViolMsg",
      898,
      info_,
      "   Constraint:  %1$s >= 0");

   //---------------------------------------------------------------------------

   makeMsg (
      "intConstraintViolMsg",
      899,
      info_,
      "   Constraint:  %1$s must be an integer.");

   currentMsg_->testable              (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "infeasExtOptWmsg",
      903,
      warning_,
      "The solution provided for external optimizing implosion is infeasible.\n"
      "\n"
      "Maximum constraint violation: %1$g");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "shutDownExtOptMsg",
      904,
      info_,
      "Shutting down external optimizing implosion.");

   //---------------------------------------------------------------------------

   makeMsg (
      "extOptActiveWR2Smsg",
      905,
      severe_,
      "The function %1$s was called while external optimizing implosion "
      "was active for the %2$s argument.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "finishExtOptWoSolnSmsg",
      906,
      severe_,
      "A call to witFinishExtOpt must be preceeded by a call to either "
      "witSetExtOptSoln or witSetExtOptSolnDbl.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "intDevCmdMsg",
      907,
      info_,
      "The following string will be interpreted as a development command:\n"
      "\n"
      "   \"%1$s\"");

   currentMsg_->testable              (false);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "invalidDevCmdSmsg",
      908,
      severe_,
      "The string passed to witInterpDevCmd "
      "was not a valid development command.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "funcNyaSmsg",
      909,
      severe_,
      "The function %1$s is not yet available.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "copyObjectToSelfSmsg",
      910,
      severe_,
      "An input data copying function was called with the same object in "
      "the same WitRun used as both the original and duplicate object.\n"
      "\n"
      "   Object Type        : %1$m");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->idInputOnIssue        (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "copyPartCatMismatchSmsg",
      912,
      severe_,
      "The parts passed to witCopyPartData have unmatching partCategories.\n"
      "\n"
      "   Duplicate Part:     %1$s\n"
      "             Category: %2$m\n"
      "   Original  Part:     %3$s\n"
      "             Category: %4$m");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "dupObjectMsg",
      913,
      info_,
      "Duplicate %1$m:");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "origObjectMsg",
      914,
      info_,
      "Original %1$m:");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "sameWitRunMsg",
      915,
      info_,
      "The original and duplicate WitRuns are the same WitRun.");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "distinctWitRunsMsg",
      916,
      info_,
      "The original and duplicate WitRuns are distinct.");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "copyObjDiffNPeriodsSmsg",
      917,
      severe_,
      "%1$s was called with WitRuns that have different values of nPeriods.\n"
      "\n"
      "   nPeriods for  dupWitRun: %2$d\n"
      "   nPeriods for origWitRun: %3$d");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "copyObjDiffBoolAttSmsg",
      918,
      severe_,
      "%1$s was called with WitRuns that have different values of %2$s.\n"
      "\n"
      "   %2$s for  dupWitRun: %3$b\n"
      "   %2$s for origWitRun: %4$b");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "cumShipRewardWoShipLateSmsg",
      919,
      severe_,
      "A non-zero value of cumShipReward was specified for a demand with "
      "shipLateAllowed == FALSE.\n"
      "\n"
      "   Demanded Part Name: %1$s\n"
      "   Demand Name:        %2$s");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "cumShipSLBWoShipLateSmsg",
      920,
      severe_,
      "A non-zero value of cumShipBounds.softLB was specified "
      "for a demand with shipLateAllowed == FALSE.\n"
      "\n"
      "   Demanded Part Name: %1$s\n"
      "   Demand Name:        %2$s");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "cumShipHUBWoShipLateSmsg",
      921,
      severe_,
      "A finite value of cumShipBounds.hardUB was specified "
      "for a demand with shipLateAllowed == FALSE.\n"
      "\n"
      "   Demanded Part Name: %1$s\n"
      "   Demand Name:        %2$s");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "disScrapConstraintViolMsg",
      923,
      info_,
      "   Constraint:  scrapVol = 0");

   currentMsg_->testable              (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "intDevCmdWoDevModeSmsg",
      924,
      severe_,
      "witInterpretDevCmd is not available in this build of WIT.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "asapPipOrderOpoSmsg",
      925,
      severe_,
      "A part was found with asapPipOrder = TRUE "
      "in a problem with newPipPggOrder = FALSE.\n"
      "\n"
      "   Part: \"%1$s\"");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "stochImpPreCommentWdMsg",
      926,
      info_,
      "//---------------------------\n"
      "// Stochastic Implosion Data\n"
      "//---------------------------");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "stochImpPostCommentWdMsg",
      927,
      info_,
      "//----------------------------------\n"
      "// End of Stochastic Implosion Data\n"
      "//----------------------------------");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "enterStochModeMsg",
      928,
      info_,
      "Entering stochastic mode.");

   currentMsg_->issueDuringRD         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "leaveStochModeMsg",
      929,
      info_,
      "Leaving stochastic mode.");

   currentMsg_->issueDuringRD         (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "invBoolAttStochModeSmsg",
      930,
      severe_,
      "The value of the global attribute \"%1$s\" must be %2$b "
      "when entering stochastic mode.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "addStochModeSmsg",
      931,
      severe_,
      "Adding a new object is not allowed in stochastic mode.\n"
      "\n"
      "   Object Type: %1$m");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "funcInStochModeSmsg",
      932,
      severe_,
      "The function %1$s must not be invoked in stochastic mode.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "setObjStochModeSmsg",
      933,
      severe_,
      "The attribute \"%1$s\" for objects of type \"%2$m\" must not be set "
      "in stochastic mode.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "setGlobStochModeSmsg",
      934,
      severe_,
      "The global attribute \"%1$s\" must not be set in stochastic mode.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "stochImploderDdMsg",
      935,
      info_,
      "Current scenario index:                      %1$5d");

   //---------------------------------------------------------------------------

   makeMsg (
      "globNeedsStochModeSmsg",
      936,
      severe_,
      "The global attribute \"%1$s\" is only available in stochastic mode.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "dblLowEqUpEqImmSmsg",
      937,
      severe_,
      "Attribute is out of range.\n"
      "   Value specified    : %1$f\n"
      "   Range              : %2$f <= value <= %3$f");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "scenarioHeadingWdMsg",
      938,
      info_,
      "//-----------------------\n"
      "// Data for Scenario #%1$d\n"
      "//-----------------------");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "finalScenIdxWdMsg",
      939,
      info_,
      "//----------------------------------\n"
      "// Final Setting of CurrentScenario\n"
      "//----------------------------------");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "scenarioDdMsg",
      940,
      info_,
      "Data for Scenario #%1$d:\n"
      "---------------------\n"
      "\n"
      "Probability:          %2$5.3f");

   currentMsg_->preceedingLineFeeds   (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "stochImpMsg",
      942,
      info_,
      "Performing stochastic implosion.");

   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "funcNeedsStochModeSmsg",
      943,
      severe_,
      "The function %1$s may only be invoked in stochastic mode.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "stochImpWoStochModeSmsg",
      944,
      severe_,
      "Stochastic implosion may only be performed in stochastic mode.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "enterStochSolnModeMsg",
      945,
      info_,
      "Entering stochastic solution mode.");

   currentMsg_->dupToStdout           (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "leaveStochSolnModeMsg",
      946,
      info_,
      "Leaving stochastic solution mode.");

   currentMsg_->dupToStdout           (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "funcWStochSolnSmsg",
      947,
      severe_,
      "The function %1$s must not be invoked in stochastic solution mode.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "funcWoStochSolnSmsg",
      948,
      severe_,
      "The function %1$s may only be invoked in stochastic solution mode.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "setObjStochSolnSmsg",
      949,
      severe_,
      "The attribute \"%1$s\" for objects of type \"%2$m\" must not be set in "
      "stochastic solution mode.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "setGlobStochSolnSmsg",
      950,
      severe_,
      "The global attribute \"%1$s\" must not be set in stochastic solution "
      "mode.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "funcInStochModeWR2Smsg",
      952,
      severe_,
      "The function \"%1$s\" was called while the WitRun referenced by the "
      "\"%2$s\" argument was in stochastic mode.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "actionInStochModeSmsg",
      953,
      severe_,
      "Stochastic mode must not be used when the value of the \"action\" "
      "control parameter is \"%1$s\".");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "writeAttrData",
      954,
      info_,
      "Writing the attribute data");

   currentMsg_->testable              (false);
   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "stageHeadingDdMsg",
      955,
      info_,
      "Stage Data for Stochastic Implosion:\n"
      "------------------------------------");

   currentMsg_->preceedingLineFeeds   (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "stochMgrDdMsg",
      957,
      info_,
      "Stochastic mode?                             %1$5b\n"
      "Stage by object?                             %2$5b\n"
      "Number of scenarios:                         %3$5d");

   //---------------------------------------------------------------------------

   makeMsg (
      "objNeedsStochModeSmsg",
      958,
      severe_,
      "The attribute \"%1$s\" for objects of type \"%2$m\" is only available "
      "in stochastic mode.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "totScenProbNeq1Smsg",
      959,
      severe_,
      "The sum of the scenario probabilities must be 1 "
      "when stochastic implosion is invoked.\n"
      "\n"
      "   Total Scenario Probability: %1$.4f");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "bomEntStageConflictSmsg",
      960,
      severe_,
      "A conflict in the objectStage attributes was found at a BOM entry:\n"
      "The operation is in stage 1, while the part is in stage 0.\n"
      "\n"
      "   Consuming Operation: %1$s\n"
      "   BOM Entry Index:     %2$d\n"
      "   Consumed  Part:      %3$s");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "subStageConflictSmsg",
      961,
      severe_,
      "A conflict in the objectStage attributes was found "
      "at a substitute BOM entry:\n"
      "The operation is in stage 1, while the part is in stage 0.\n"
      "\n"
      "   Consuming  Operation: %1$s\n"
      "   BOM Entry  Index:     %2$d\n"
      "   Substitute Index:     %3$d\n"
      "   Consumed   Part:      %4$s");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "bopEntStageConflictSmsg",
      962,
      severe_,
      "A conflict in the objectStage attributes was found at a BOP entry:\n"
      "The operation is in stage 1, while the part is in stage 0.\n"
      "\n"
      "   Producing Operation: %1$s\n"
      "   BOM Entry Index:     %2$d\n"
      "   Produced  Part:      %3$s");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "noPartsStochModeSmsg",
      963,
      severe_,
      "The problem must contain at least one part when "
      "entering stochastic mode.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "undiffScenSpecWdMsg",
      964,
      info_,
      "//-----------------------------------------\n"
      "// Undifferentiated Scenario-Specific Data\n"
      "//-----------------------------------------");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->displayExternalNumber (false);
   currentMsg_->allowedToChange       (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "stage0ObjAttMisMatchSmsg",
      965,
      severe_,
      "An object in stage 0 was found that has a scenario-specific attribute "
      "with a different value in two different scenarios.");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "stage0AttMisMatchPartSmsg",
      966,
      severe_,
      "   Object Type:    Part\n"
      "   Part Name:      \"%1$s\"");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "stage0AttMisMatchDemandSmsg",
      967,
      severe_,
      "   Object Type:    Demand\n"
      "   Part   Name:    \"%1$s\"\n"
      "   Demand Name:    \"%2$s\"");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "stage0AttMisMatchOpnSmsg",
      968,
      severe_,
      "   Object Type:    Operation\n"
      "   Operation Name: \"%1$s\"");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "stage0AttMisMatchSubSmsg",
      969,
      severe_,
      "   Object Type:    Substitute BOM Entry\n"
      "   Operation Name: \"%1$s\"\n"
      "   BOM Entry  Idx: %2$d\n"
      "   Substitute Idx: %3$d");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "stage0AttMisMatchBoundSetSmsg",
      970,
      severe_,
      "   Bound Set:      %1$s");

   currentMsg_->delayedSevere         (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "stage0AttMisMatchTailSmsg",
      971,
      severe_,
      "   Attribute:      %1$s\n"
      "   Period:         %2$d\n"
      "   Scenario #:     0\n"
      "      Value:       %3$f\n"
      "   Scenario #:     %4$d\n"
      "      Value:       %5$f");

   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "objectStageWoStageByObjectSmsg",
      973,
      severe_,
      "The attribute \"objectStage\" for objects of type \"%1$m\" "
      "is not available when stageByObject is FALSE.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "periodStageWStageByObjectSmsg",
      974,
      severe_,
      "The global attribute \"periodStage\" "
      "is not available when stageByObject is TRUE.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "perStageConflictSmsg",
      975,
      severe_,
      "A conflict in the periodStage attribute was found:\n"
      "\n"
      "    periodStage[%1$2d] = 1,\n"
      "but periodStage[%2$2d] = 0.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "posBomOffsetInSBPSmsg",
      977,
      severe_,
      "A BOM entry was found with a positive offset "
      "when WIT was entering stochastic mode "
      "with stageByObject = FALSE.\n"
      "\n"
      "   Consuming Operation: %1$s\n"
      "   BOM Entry Index:     %2$d\n"
      "   Consumed  Part:      %3$s\n"
      "   Period:              %4$d\n"
      "   Offset:              %5$.0f");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "posSubOffsetInSBPSmsg",
      978,
      severe_,
      "A substitute BOM entry was found with a positive offset "
      "when WIT was entering stochastic mode "
      "with stageByObject = FALSE.\n"
      "\n"
      "   Consuming  Operation: %1$s\n"
      "   BOM Entry  Index:     %2$d\n"
      "   Substitute Index:     %3$d\n"
      "   Consumed   Part:      %4$s\n"
      "   Period:               %5$d\n"
      "   Offset:               %6$.0f");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "posBopOffsetInSBPSmsg",
      979,
      severe_,
      "A BOP entry was found with a positive offset "
      "when WIT was entering stochastic mode "
      "with stageByObject = FALSE.\n"
      "\n"
      "   Producing Operation: %1$s\n"
      "   BOP Entry Index:     %2$d\n"
      "   Produced  Part:      %3$s\n"
      "   Period:              %4$d\n"
      "   Offset:              %5$.0f");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "stage0PerAttMisMatchSmsg",
      980,
      severe_,
      "A scenario-specific attribute was found whose value in a stage 0 period "
      "is different in two different scenarios.");

   currentMsg_->delayedSevere         (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "periodStageHeadingDdMsg",
      981,
      info_,
      "Period  Stage");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "periodStageDdMsg",
      982,
      info_,
      "%1$6d  %2$5d");

   //---------------------------------------------------------------------------

   makeMsg (
      "partStageHeadingDdMsg",
      983,
      info_,
      "Part          Stage");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "opnStageHeadingDdMsg",
      984,
      info_,
      "Operation     Stage");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "objectStageDdMsg",
      985,
      info_,
      "%1$-12s  %2$5d");

   //---------------------------------------------------------------------------

   makeMsg (
      "setNodeNameToDupSmsg",
      986,
      severe_,
      "The \"newName\" argument specified in a call to %1$s "
      "matches the name of an existing %2$m.\n"
      "\n"
      "   newName            : %3$s");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "setDemandNameToDupSmsg",
      987,
      severe_,
      "The \"newName\" argument specified in a call to witSetDemandDemandName "
      "matches the name of an existing demand for the same part.\n"
      "\n"
      "   newName            : %1$s");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "setWit34CompatibleNotAllowedSmsg",
      988,
      severe_,
      "The \"wit34Compatible\" attribute cannot be set in this application "
      "program.\n");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "setObsAttFromFileWmsg",
      989,
      warning_,
      "A command to set the value of the \"%1$s\" attribute was found in an "
      "input data file. The \"unitCost\", \"grossRev\", and "
      "\"periodsPerYear\" attributes no longer exist in WIT. All commands to "
      "set them from an input data file will be ignored.");

   currentMsg_->testable              (false);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->maxTimesIssued        (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "maxViolViolMsg",
      990,
      info_,
      "   Violation:   %1$g");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "objHeadingDdMsg",
      991,
      info_,
      "Objectives:");

   currentMsg_->preceedingLineFeeds   (1);

   //-------------------------------------------------------------+-------------

   makeMsg (
      "objectiveDdMsg",
      992,
      info_,
      "   #%1$-2d \"%2$s\"");

   //---------------------------------------------------------------------------

   makeMsg (
      "multiObjAttDdMsg",
      993,
      info_,
      "Objective list specification:\n"
      "   \"%1$s\"\n"
      "\n"
      "Current objective:\n"
      "   \"%2$s\"\n"
      "\n"
      "Multi-objective optimality tolerance:        %3$g");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "setCurObjToNonMatchNameSmsg",
      994,
      severe_,
      "The global attribute \"currentObjective\" must be set to the name of an "
      "existing objective.\n"
      "\n"
      "   String: \"%1$s\"");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "globNeedsMultiObjModeSmsg",
      995,
      severe_,
      "The global attribute \"%1$s\" is only available in multiple objectives "
      "mode.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "stochModeInMultiObjSmsg",
      996,
      severe_,
      "The global attribute \"stochMode\" must not be set to TRUE when WIT is "
      "in multiple objectives mode.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "dupObjSeqNoSmsg",
      997,
      severe_,
      "Two objectives with the same objectiveSeqNo were found during "
      "optimizing implosion with multiple objectives.\n "
      "\n"
      "   Objective:     \"%1$s\"\n"
      "   Objective:     \"%2$s\"\n"
      "   objectiveSeqNo: %3$d");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "startExtOptMultiObjSmsg",
      998,
      severe_,
      "The function \"witStartExtOpt\" must not be called when WIT is "
      "in multiple objectives mode.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "globAttMultiObjOptImpSmsg",
      999,
      severe_,
      "The global attribute \"%1$s\" must not be TRUE, when optimizing "
      "implosion is invoked in multiple objectives mode.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "objNameDdMsg",
      1000,
      info_,
      "   Objective \"%1$s\":");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (0);

   //---------------------------------------------------------------------------

   makeMsg (
      "boundsValueMsg",
      1001,
      info_,
      "Total Soft Lower Bounds Violation: %1$.3f");

   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "multiObjValueMsg",
      1002,
      info_,
      "Objective \"%1$s\":\n"
      "   Objective Function Value: %2$.3f");

   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "lexOptMsg",
      1003,
      info_,
      "Lexicographic optimization will be performed.");

   currentMsg_->dupToStdout           (true);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "optLexObjElemMsg",
      1004,
      info_,
      "Optimizing objective \"%1$s\".");

   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------

   makeMsg (
      "vbarInObjNameSmsg",
      1005,
      severe_,
      "The name of an objective must not contain the \"|\" (vertical bar) "
      "character.\n"
      "\n"
      "   Objective Name: \"%1$s\"");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "dupObjNameSmsg",
      1006,
      severe_,
      "An attempt was made to set the objectiveList to a list of names that "
      "includes the same name more than once.\n"
      "\n"
      "   Duplicate Name: \"%1$s\"");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "intArgLowSmsg",
      1007,
      severe_,
      "Argument is out of range.\n"
      "\n"
      "   Argument:        %1$s\n"
      "   Value specified: %2$d\n"
      "   Range:           Value >= %3$d");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "nullVecElemSmsg",
      1008,
      severe_,
      "An element of a vector-valued argument was a null pointer "
      "where this is not allowed.\n"
      "\n"
      "   Argument: %1$s\n"
      "   Index:    %2$d");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "funcInMultiObjModeWR2Smsg",
      1010,
      severe_,
      "The function \"%1$s\" was called while the WitRun referenced by the "
      "\"%2$s\" argument was in multiple objectives mode.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "lexObjElemCpuTimeMsg",
      1011,
      info_,
      "CPU Time: %1$.2f seconds.");

   currentMsg_->testable              (false);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "setBoolToInvalidSmsg",
      1012,
      severe_,
      "The attribute \"%1$s\" cannot be set to %2$b.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "selectionRecoveryMsg",
      1013,
      info_,
      "The selection recovery capability will be used.");

   //---------------------------------------------------------------------------

   makeMsg (
      "leadTimeUBDdMsg",
      1014,
      info_,
      "   Lead Time Upper Bounds:%1v$8d");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "leadTimeBoundsMsg",
      1015,
      info_,
      "The lead time bounds capability will be used.");

   //---------------------------------------------------------------------------

   makeMsg (
      "boolAttReqForLtbSmsg",
      1016,
      severe_,
      "The \"%1$s\" attribute must be %2$b when the lead time bounds "
      "capability is being used.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "infeasSmsg",
      1018,
      severe_,
      "The solver has determined that the optimization problem is "
      "infeasible.\n");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "unboundedOrInfeasSmsg",
      1019,
      severe_,
      "The solver has determined that the optimization problem has no optimal "
      "solution, either because its objective function goes to positive "
      "infinity or because it is infeasible.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "optProbAbandonedSmsg",
      1020,
      severe_,
      "The solver was not able to find an optimal solution to the optimization "
      "problem due to insurmountable numerical difficulties.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "noOptReasonUnknownSmsg",
      1021,
      severe_,
      "The solver did not find an optimal solution to the optimization "
      "problem. The reason was not determined.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "boolAttInvForModHeurSmsg",
      1024,
      severe_,
      "The \"%1$s\" attribute must be %2$b during modifiable heuristic "
      "allocation.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "modHeurNonExecViolSmsg",
      1025,
      severe_,
      "The implosion solution specified during modifiable heuristic "
      "allocation violates the following constraint:\n"
      "\n"
      "   ExecVol == 0.0, when the operation is not executable.\n"
      "\n"
      "   Operation: %1$s\n"
      "   Period:    %2$d\n"
      "   ExecVol:   %3$g");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "modHeurExecHUBViolSmsg",
      1026,
      severe_,
      "The implosion solution specified during modifiable heuristic "
      "allocation violates the following constraint:\n"
      "\n"
      "   ExecVol <= ExecBounds.HardUB\n"
      "\n"
      "   Operation: %1$s\n"
      "   Period:    %2$d\n"
      "   ExecVol:   %3$g\n"
      "   HardUB:    %4$g\n"
      "   Violation: %5$g");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "modHeurLotSizeViolSmsg",
      1027,
      severe_,
      "The implosion solution specified during modifiable heuristic "
      "allocation violates the following constraint:\n"
      "\n"
      "   ExecVol must be lot size feasible.\n"
      "\n"
      "   Operation: %1$s\n"
      "   Period:    %2$d\n"
      "   ExecVol:   %3$g\n"
      "   Violation: %4$g");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "modHeurCumShipViolSmsg",
      1028,
      severe_,
      "The implosion solution specified during modifiable heuristic "
      "allocation violates the following constraint:\n"
      "\n"
      "   Cum. ShipVol <= Cum. DemandVol\n"
      "\n"
      "   Part:           \"%1$s\"\n"
      "   Demand:         \"%2$s\"\n"
      "   Period:         %3$d\n"
      "   Cum. ShipVol:   %4$g\n"
      "   Cum. DemandVol: %5$g\n"
      "   Violation:      %6$g");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "modHeurSubViolSmsg",
      1029,
      severe_,
      "The implosion solution specified during modifiable heuristic "
      "allocation violates the following constraint:\n"
      "\n"
      "   Sum of SubVols <= ExecVol\n"
      "\n"
      "   Operation:       \"%1$s\"\n"
      "   Bom Entry Index: %2$d\n"
      "   Period:          %3$d\n"
      "   ExecVol:         %4$g\n"
      "   Sum of SubVols:  %5$g\n"
      "   Violation:       %6$g");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "modHeurScrapViolSmsg",
      1030,
      severe_,
      "The implosion solution specified during modifiable heuristic "
      "allocation violates the following constraint:\n"
      "\n"
      "   ScrapVol >= 0\n"
      "\n"
      "   Part:     \"%1$s\"\n"
      "   Period:   %2$d\n"
      "   ScrapVol: %3$g");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "nonZeroCplexErrCodeSmsg",
      1031,
      severe_,
      "A CPLEX function has returned with a non-zero error code:\n"
      "\n"
      "   %1$s"
      "   Function:          %2$s");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "unexpCpxStatSmsg",
      1033,
      severe_,
      "An unexpected solution status code was returned by the CPLEX function "
      "CPXgetstat.\n"
      "\n"
      "   Solution Status Code: %1$d\n"
      "   Solution Status:      \"%2$s\"");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "lpSolveInfoMsg",
      1034,
      info_,
      "# simplex iterations: %1$d\n"
      "# phase 1 iterations: %2$d");

   currentMsg_->testable              (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "cplexNeededSmsg",
      1035,
      severe_,
      "Optimizing implosion and stochastic implosion cannot be invoked, "
      "because CPLEX was not embedded into this build of WIT.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "mipSolveInfoMsg",
      1037,
      info_,
      "# simplex iterations:     %1$d\n"
      "# nodes:                  %2$d\n"
      "Relative optimality gap:  %3$g%%\n"
      "Objective function bound: %4$g");

   currentMsg_->testable              (false);
   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "cpxParSpecAttsDdMsg",
      1038,
      info_,
      "CPLEX Parameter Spec Name: %1$23s\n"
      "CPLEX Parameter Spec Integer Value:      %2$9d\n"
      "CPLEX Parameter Spec Double  Value:      %3$9g");

   //---------------------------------------------------------------------------

   makeMsg (
      "cpxParSpecsHeadDdMsg",
      1039,
      info_,
      "CPLEX Parameter Specifications:\n"
      "\n"
      "   Name              Type        Value");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "intCpxParSpecDdMsg",
      1040,
      info_,
      "   %1$-17s Integer %2$9d");

   //---------------------------------------------------------------------------

   makeMsg (
      "dblCpxParSpecDdMsg",
      1041,
      info_,
      "   %1$-17s Double  %2$9.4g");

   //---------------------------------------------------------------------------

   makeMsg (
      "badCpxParamNameSmsg",
      1042,
      severe_,
      "A CPLEX parameter specification was given whose name (when prefixed "
      "with \"CPX_PARAM_\") does not match the name of any CPLEX parameter.\n"
      "\n"
      "CPLEX Parameter Specification Name: %1$s");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "wrongCpxParamTypeSmsg",
      1043,
      severe_,
      "A CPLEX parameter specification was given whose type does not match "
      "the type of the corresponding CPLEX parameter.\n"
      "\n"
      "   CPLEX Parameter Specification Name: %1$s\n"
      "   CPLEX Parameter Specification Type: %2$s\n"
      "   CPLEX Parameter Number:             %3$d");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "intCpxParSetMsg",
      1044,
      info_,
      "CPLEX parameter CPX_PARAM_%1$s has been set to %2$d.");

   currentMsg_->testable              (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "dblCpxParSetMsg",
      1045,
      info_,
      "CPLEX parameter CPX_PARAM_%1$s has been set to %2$g.");

   currentMsg_->testable              (false);

   //---------------------------------------------------------------------------

   makeMsg (
      "noParamCpxParamSpecSmsg",
      1046,
      severe_,
      "A CPLEX parameter specification cannot have the name: \"NO_PARAM\".");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "addIntCpxParSpecMsg",
      1047,
      info_,
      "CPLEX parameter specification added:\n"
      "\n"
      "   Name:  %1$s\n"
      "   Type:  Integer\n"
      "   Value: %2$d");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "addDblCpxParSpecMsg",
      1048,
      info_,
      "CPLEX parameter specification added:\n"
      "\n"
      "   Name:  %1$s\n"
      "   Type:  Double\n"
      "   Value: %2$g");

   currentMsg_->preceedingLineFeeds   (1);

   //---------------------------------------------------------------------------

   makeMsg (
      "inFeasLpCpxStatSmsg",
      1049,
      severe_,
      "The solution status code returned by CPLEX indicates that the LP solve "
      "routine terminated early. The resulting LP solution is not known to be "
      "primal feasible.\n"
      "\n"
      "   Solution Status Code: %1$d\n"
      "   Solution Status:      \"%2$s\"");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "nonOptLpCpxStatSmsg",
      1050,
      severe_,
      "The solution status code returned by CPLEX indicates that the LP solve "
      "routine terminated early and did not find an optimal solution "
      "to the LP problem. Optimality is required in this case.\n"
      "\n"
      "   Solution Status Code: %1$d\n"
      "   Solution Status:      \"%2$s\"");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "nonOptButFeasLpCpxStatWmsg",
      1051,
      warning_,
      "The solution status code returned by CPLEX indicates that the LP solve "
      "routine terminated early. Nevertheless, it did find a primal feasible "
      "solution and WIT will use it to compute the implosion solution.\n"
      "\n"
      "   Solution Status Code: %1$d\n"
      "   Solution Status:      \"%2$s\"");

   currentMsg_->testable              (false);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "inFeasMipCpxStatSmsg",
      1052,
      severe_,
      "The solution status code returned by CPLEX indicates that the MIP solve "
      "routine terminated early and did not find an integer feasible solution "
      "to the MIP problem.\n"
      "\n"
      "   Solution Status Code: %1$d\n"
      "   Solution Status:      \"%2$s\"");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "nonOptMipCpxStatSmsg",
      1053,
      severe_,
      "The solution status code returned by CPLEX indicates that the MIP solve "
      "routine terminated early and did not find an optimal solution "
      "to the MIP problem. Optimality is required in this case.\n"
      "\n"
      "   Solution Status Code: %1$d\n"
      "   Solution Status:      \"%2$s\"");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "nonOptButFeasMipCpxStatWmsg",
      1054,
      warning_,
      "The solution status code returned by CPLEX indicates that the MIP solve "
      "routine terminated early, but it did find an integer feasible solution "
      "to the MIP problem. WIT will use it to compute the implosion solution.\n"
      "\n"
      "   Solution Status Code: %1$d\n"
      "   Solution Status:      \"%2$s\"");

   currentMsg_->testable              (false);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "mipModeNoIntVarsSmsg",
      1055,
      severe_,
      "Optimizing implosion was invoked on a MIP problem "
      "that contains no integer variables.");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "nonZeroOpenCplexErrCodeSmsg",
      1056,
      severe_,
      "The CPLEX function CPXopenCPLEX has returned with a non-zero error "
      "code:\n"
      "\n"
      "   CPLEX Error  %1$d");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "execPerMsg",
      1057,
      info_,
      "Execution Period is %1$d.");

   //---------------------------------------------------------------------------

   makeMsg (
      "pipNotEnabledSmsg",
      1058,
      severe_,
      "A PIP to Operations retrieval function was called on an operation for "
      "which pipEnabled was FALSE.");

   currentMsg_->idInputOnIssue        (true);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "pipEnabledOpoSmsg",
      1059,
      severe_,
      "An operation was found with pipEnabled = TRUE "
      "in a problem with newPipPggOrder = FALSE.\n"
      "\n"
      "   Operation: \"%1$s\"");

   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "nPeriodsNotSetWmsg",
      1060,
      warning_,
      "A part or an operation is being added when the nPeriods attribute has "
      "not yet been explicitly set. The default value of nPeriods = %1$d will "
      "be used.");

   currentMsg_->testable              (false);
   currentMsg_->preceedingLineFeeds   (1);
   currentMsg_->trailingLineFeeds     (2);

   //---------------------------------------------------------------------------

   makeMsg (
      "cplexVersionMsg",
      1061,
      info_,
      "CPLEX Version: \"%1$s\".");

   currentMsg_->dupToStdout           (true);

   //---------------------------------------------------------------------------
   }
