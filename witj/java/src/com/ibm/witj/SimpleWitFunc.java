
//==============================================================================
// WIT-J Java Source File SimpleWitFunc.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Enum SimpleWitFunc
//
// "Simple WIT Function"
// Identifies a WIT API function whose only argument is a WitRun *.
//==============================================================================

enum SimpleWitFunc implements CaseToken
{
//==============================================================================
// Enum Constants
//==============================================================================

   BuildPip,
   ClearCplexParSpecs,
   ClearPegging,
   ClearPipSeq,
   ClearStochSoln,
   EvalObjectives,
   FinishExtOpt,
   FinishHeurAlloc,
   GeneratePriorities,
   HeurImplode,
   Mrp,
   OptImplode,
   Preprocess,
   Postprocess,
   PurgeData,
   ShutDownExtOpt,
   ShutDownHeurAlloc,
   StartExtOpt,
   StartHeurAlloc,
   StochImplode,
   ;

//==============================================================================
}

