#include <mcl/src/mcl.h>
#include <sce/src/sceDefin.h>

//----------------------------------------------------------------------------
//  For those who are curious about what this file is for:
//
//  In actuality these enumerations are not neccessary by any means, in
//  fact, it might be a good idea to get rid of them, but it provides an
//  easy to read list of all the messages.
//
//----------------------------------------------------------------------------

// enumerations of "internal messages" and "message types"
enum messageType {
  info,
  warning,
  error,
  severe,
  fatal,
  numberMsgTypes
};

// actually this is obsolete, but we'll keep it around because we need the
// numberErrNum which is only defined if we keep this enumeration.
enum internalMsgNum {
  AlreadyDefined,
  BBCategoryAlreadyDefined,
  BBCategoryNotDefined,
  BadDate,
  BadBuildAheadCode,
  CalendarFormat,
  CalendarLateStartDate,
  CalendarNotUpToDate,
  CalendarPeriod,
  CalendarWorkUnits,
  CannotFindFeatureRel,
  CannotOpenFileForWrite,
  CannotSourceDemandRecord,
  CannotSourceFeature,
  CannotSourcePureOption,
  CollapsedEffectiveInterval,
  ConflictingBounds,
  ConflictingInterplantRecordsED,
  ConflictingInterplantRecordsUR,
  CustChoiceFeatRatioGreaterThanOne,
  DatalessMandatoryInputFile,
  DatedRecordBeforeHorizonStartIgnore,
  DatedRecordBeyondHorizonEndIgnore,
  DatedRecordBeforeHorizonStartAccept,
  DatedRecordBeyondHorizonEndIgnoreAccept,
  DuplicateAggRecord,
  DuplicatePCFPart,
  DuplicatePartPDFPairError,
  DuplicatePartPDFPairWarning,
  ExecCostInHeurMode,
  ExtraPCFPart,
  ExtraPCFPartBogon,
  ForcingSubNetAllowedFalse,
  GeoDemandNotSourced,
  IllegalCharacter,
  IllegalParameterEntry,
  IndDemandDoesNotExist,
  IndDemandedPartNotExist,
  InternalError1,
  InternalError2,
  InternalError3,
  InternalError4,
  InternalError5,
  InternalError6,
  InterplantOperationBomError,
  InterplantToMyselfWithoutMultiSourcing,
  InvalidCalendar,
  InvalidCalendarDate,
  InvalidDataType,
  InvalidDataWarning,
  InvalidDataWarningb,
  InvalidDate,
  InvalidPCFPart,
  JumperArcError,
  MandatoryParameterMissing,
  MinDemandExceedsDemand,
  MinIndDemVolIsNegative,
  MipModeAndCritListDontMix,
  MipModeAndShadowProcesDontMix,
  MiscountWarning,
  MissingBOMMatch,
  MissingBillRecord,
  MissingData,
  MissingDataWarning,
  MissingMfgOptionPart,
  MissingPDFSource,
  MissingParameter,
  MissingParameterRequested,
  MissingSourceAndPart,
  MultipleDemandVolRecs,        
  MultipleMinDemandVolRecs,        
  MultipleSourceAttempt,
  MwoPosAndNegNotSupported,
  OptEngineAndPriorities,
  OptEngineNotSupported,
  OptionAllZeroRatios,
  OptionAllZeroRatiosPO,
  OptionAlreadyDefined,
  OptionRatiosAlreadySpecified,
  OutOfRange,
  OutOfRangeFloatWithBound,
  OutOfRangeIntWithBound,
  OutOfRangeVectorElement,
  ParamOutOfRange,
  PartDoesNotExist,
  PartDoesNotExistb,
  PossibleInvalidParameter,
  QualifiedInputFileName,
  QualifiedOutputFileName,
  QualifiedOutputFilePrefix,
  ResetMinDemandVolRecs,
  RoutingShareOfZero,
  ScrubOutPutFileNameBad,
  SevereCannotOpenFile,
  UnknownMfgPartPDFPair,
  UnrecognizedOperationError,
  UnrecognizedOperationPdfError,
  UnrecognizedOperationPdfWarning,
  UnrecognizedOperationWarning,
  UnrecognizedPartError,
  UnrecognizedPartPdfError,
  UnrecognizedPartPdfWarning,
  UnrecognizedPartWarning,
  UnrecognizedSourceCode,
  UsingDefaultParameter,
  WarningCannotOpenFile,
  FssHorizon,
  InvalidPartFSS,
  NoDemandFSS,        
  numberErrMsgs,
  TestMsg,
  numberNonErrMsgs
};

// structure for storing messages for message facilities
struct messageStructure {
  char * internalMsgName;
  messageType msgType;
  unsigned int extNum;
  char * msgTemplate;
};

class SceMsg: public MclMsg
{
public:
  
   SceMsg (
     MclFacility &    theFacility,
     const char *     id,
     const MclLevel & theLevel,
     const char *     textVal,
     int              externalNumber)
     : MclMsg(theFacility,
              id,
              theLevel,
              textVal,
              externalNumber)
  {}

private:
  void postIssue ();
  
};
