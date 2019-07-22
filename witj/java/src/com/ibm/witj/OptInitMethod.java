
//==============================================================================
// WIT-J Java Source File OptInitMethod.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Enum OptInitMethod
//
// Identifies one of the methods by which the initial solution for optimizing
// implosion is to be computed.
//==============================================================================

public enum OptInitMethod implements CaseToken
{
   HEURISTIC,
   ACCELERATED,
   SCHEDULE,
   CRASH,
}
