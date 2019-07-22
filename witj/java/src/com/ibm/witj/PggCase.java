
//==============================================================================
// WIT-J Java Source File PggCase.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Enum PggCase
//
// "Pegging Case"
// Identifies a case for pegging.
// Specifies:
//    The recipient class (optionally)
//    The pegging attribute
//    The form of pegging: Post-Implosion Pegging vs. Concurrent Pegging
//==============================================================================

enum PggCase implements CaseToken
{
//==============================================================================
// Enum Constants
//==============================================================================

   DEM_CONS_PIP,
   DEM_CO_EXEC_PIP,
   DEM_EXEC_PIP,
   DEM_PROD_PIP,
   DEM_SIDE_PIP,
   DEM_SUB_PIP,
   DEM_SUPPLY_PIP,
   DEM_EXEC_CP,
   DEM_SUB_CP,
   OPN_CONS_PIP,
   OPN_CO_EXEC_PIP,
   OPN_EXEC_PIP,
   OPN_PROD_PIP,
   OPN_SIDE_PIP,
   OPN_SUB_PIP,
   OPN_SUPPLY_PIP,
   ;

//==============================================================================
}
