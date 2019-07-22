
//==============================================================================
// WIT-J Java Source File Attribute.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;
import static com.ibm.witj.CompClassID.*;
import static com.ibm.witj.Attribute.DefaultCase.*;
import static com.ibm.witj.AttValType.*;
import static com.ibm.witj.PreservedObject.*;

import       java.util.*;

//==============================================================================
// Generic Class Attribute <V>
//
// "Attribute (Value Type)"
//
// Represents the set of all WIT attributes that have a given name.
// Each Attribute has a value type, the type of value that it stores.
// The instances of this class are immutable.
//==============================================================================

public final class Attribute <V>
{
//==============================================================================
// Static initialization block
//==============================================================================

   static
      {
      initAttributes =
         new ArrayList <Attribute <?>> (200);

      materialPartWitJNames =
         newHashSet (
            "ASAP_PIP_ORDER",
            "BUILD_AHEAD_U_B",
            "BUILD_ASAP",
            "BUILD_NSTN",
            "STOCK_BOUNDS",
            "STOCK_COST",
            "STOCK_VOL");

      stochModeWitJNames =
         newHashSet (
            "CURRENT_SCENARIO",
            "OBJECT_STAGE",
            "PERIOD_STAGE",
            "PROBABILITY",
            "STOCH_SOLN_MODE");

      multiObjModeWitJNames =
         newHashSet (
            "CURRENT_OBJECTIVE",
            "MULTI_OBJ_TOL",
            "OBJECTIVE_LIST_SPEC",
            "OBJECTIVE_SEQ_NO");
      }

//==============================================================================
// Attribute fields
//
// The following fields store all of WIT-J's Attributes.
//==============================================================================

   public static final
   Attribute <Boolean>    ACCELERATED =
      newAtt (BOOL,      "ACCELERATED",         UNMODIFIABLE, PROBLEM);

   public static final
   Attribute <Boolean>    ACC_AFTER_OPT_IMP =
      newAtt (BOOL,      "ACC_AFTER_OPT_IMP",         false,  PROBLEM);

   public static final
   Attribute <Boolean>    ACC_AFTER_SOFT_L_B =
      newAtt (BOOL,      "ACC_AFTER_SOFT_L_B",         false, PROBLEM);

   public static final
   Attribute <Boolean>    ASAP_PIP_ORDER =
      newAtt (BOOL,      "ASAP_PIP_ORDER",             false, PART);

   public static final
   Attribute <Boolean>    AUTO_PRIORITY =
      newAtt (BOOL,      "AUTO_PRIORITY",              false, PROBLEM);

   public static final
   Attribute <Integer>    BOM_INDEX =
      newAtt (INT,       "BOM_INDEX",           UNMODIFIABLE, BOM_ENTRY);

   public static final
   Attribute <Integer>    BOP_INDEX =
      newAtt (INT,       "BOP_INDEX",           UNMODIFIABLE, BOP_ENTRY);

   public static final
   Attribute <Boolean>    BOUNDED_LEAD_TIMES =
      newAtt (BOOL,      "BOUNDED_LEAD_TIMES",         false, PART);

   public static final
   Attribute <Double>     BOUNDS_VALUE =
      newAtt (DBL,       "BOUNDS_VALUE",        UNMODIFIABLE, PROBLEM);

   public static final
   Attribute <int[]>      BUILD_AHEAD_U_B =
      newAtt (INT_VEC,   "BUILD_AHEAD_U_B",      LAST_PERIOD, PART);

   public static final
   Attribute <Boolean>    BUILD_ASAP =
      newAtt (BOOL,      "BUILD_ASAP",                 false, PART);

   public static final
   Attribute <Boolean>    BUILD_NSTN =
      newAtt (BOOL,      "BUILD_NSTN",                 false, PART);

   public static final
   Attribute <Boolean>    COMPUTE_CRITICAL_LIST =
      newAtt (BOOL,      "COMPUTE_CRITICAL_LIST",      false, PROBLEM);

   public static final
   Attribute <Boolean>    COMP_PRICES =
      newAtt (BOOL,      "COMP_PRICES",                false, PROBLEM);

   public static final
   Attribute <double[]>   CONS_RATE =
      newAtt (DBL_VEC,   "CONS_RATE",                    1.0, BOM_ENTRY,
                                                              SUBSTITUTE);
   public static final
   Attribute <double[]>   CONS_VOL =
      newAtt (DBL_VEC,   "CONS_VOL",            UNMODIFIABLE, PART);

   public static final
   Attribute <Boolean>    CPLEX_EMBEDDED =
      newAtt (BOOL,      "CPLEX_EMBEDDED",      UNMODIFIABLE, PROBLEM);

   public static final
   Attribute <Double>     CPLEX_MIP_BOUND =
      newAtt (DBL,       "CPLEX_MIP_BOUND",     UNMODIFIABLE, PROBLEM);

   public static final
   Attribute <Double>     CPLEX_MIP_REL_GAP =
      newAtt (DBL,       "CPLEX_MIP_REL_GAP",   UNMODIFIABLE, PROBLEM);

   public static final
   Attribute <Double>     CPLEX_PAR_SPEC_DBL_VAL =
      newAtt (DBL,       "CPLEX_PAR_SPEC_DBL_VAL",       0.0, PROBLEM);

   public static final
   Attribute <Integer>    CPLEX_PAR_SPEC_INT_VAL =
      newAtt (INT,       "CPLEX_PAR_SPEC_INT_VAL",         0, PROBLEM);

   public static final
   Attribute <String>     CPLEX_PAR_SPEC_NAME =
      newAtt (STRING,    "CPLEX_PAR_SPEC_NAME",   "NO_PARAM", PROBLEM);

   public static final
   Attribute <Integer>    CPLEX_STATUS_CODE =
      newAtt (INT,       "CPLEX_STATUS_CODE",   UNMODIFIABLE, PROBLEM);

   public static final
   Attribute <String>     CPLEX_STATUS_TEXT =
      newAtt (STRING,    "CPLEX_STATUS_TEXT",   UNMODIFIABLE, PROBLEM);

   public static final
   Attribute <BoundSet>   CUM_SHIP_BOUNDS =
      newAtt (BOUND_SET, "CUM_SHIP_BOUNDS",           BOUNDS, DEMAND);

   public static final
   Attribute <double[]>   CUM_SHIP_REWARD =
      newAtt (DBL_VEC,   "CUM_SHIP_REWARD",              0.0, DEMAND);

   public static final
   Attribute <String>     CURRENT_OBJECTIVE =
      newAtt (STRING,    "CURRENT_OBJECTIVE",     NO_DEFAULT, PROBLEM);

   public static final
   Attribute <Integer>    CURRENT_SCENARIO =
      newAtt (INT,       "CURRENT_SCENARIO",               0, PROBLEM);

   public static final
   Attribute <String>     DEMAND_NAME =
      newAtt (STRING,    "DEMAND_NAME",           NO_DEFAULT, DEMAND);

   public static final
   Attribute <double[]>   DEMAND_VOL =
      newAtt (DBL_VEC,   "DEMAND_VOL",                   0.0, DEMAND);

   public static final
   Attribute <Integer>    EARLIEST_PERIOD =
      newAtt (INT,       "EARLIEST_PERIOD",                0, BOM_ENTRY,
                                                              SUBSTITUTE,
                                                              BOP_ENTRY);

   public static final
   Attribute <Integer>    EQUITABILITY =
      newAtt (INT,       "EQUITABILITY",                   1, PROBLEM);

   public static final
   Attribute <double[]>   EXCESS_VOL =
      newAtt (DBL_VEC,   "EXCESS_VOL",          UNMODIFIABLE, PART);

   public static final
   Attribute <boolean[]>  EXECUTABLE =
      newAtt (BOOL_VEC,  "EXECUTABLE",          UNMODIFIABLE, OPERATION);

   public static final
   Attribute <BoundSet>   EXEC_BOUNDS =
      newAtt (BOUND_SET, "EXEC_BOUNDS",               BOUNDS, OPERATION);

   public static final
   Attribute <double[]>   EXEC_COST =
      newAtt (DBL_VEC,   "EXEC_COST",                    0.0, OPERATION);

   public static final
   Attribute <Boolean>    EXEC_EMPTY_BOM =
      newAtt (BOOL,      "EXEC_EMPTY_BOM",              true, PROBLEM);

   public static final
   Attribute <Double>     EXEC_PENALTY =
      newAtt (DBL,       "EXEC_PENALTY",                 0.0, OPERATION,
                                                              BOM_ENTRY,
                                                              SUBSTITUTE);

   public static final
   Attribute <double[]>   EXEC_VOL =
      newAtt (DBL_VEC,   "EXEC_VOL",                     0.0, OPERATION);

   public static final
   Attribute <Boolean>    EXP_ALLOWED =
      newAtt (BOOL,      "EXP_ALLOWED",                 true, SUBSTITUTE,
                                                              BOP_ENTRY);

   public static final
   Attribute <Double>     EXP_AVERSION =
      newAtt (DBL,       "EXP_AVERSION",                 0.0, BOP_ENTRY);

   public static final
   Attribute <Double>     EXP_CUTOFF =
      newAtt (DBL,       "EXP_CUTOFF",                  0.01, PROBLEM);

   public static final
   Attribute <Double>     EXP_NET_AVERSION =
      newAtt (DBL,       "EXP_NET_AVERSION",             0.0, SUBSTITUTE);

   public static final
   Attribute <Boolean>    EXT_OPT_ACTIVE =
      newAtt (BOOL,      "EXT_OPT_ACTIVE",      UNMODIFIABLE, PROBLEM);

   public static final
   Attribute <Double>     FALLOUT_RATE =
      newAtt (DBL,       "FALLOUT_RATE",                 0.0, BOM_ENTRY,
                                                              SUBSTITUTE);

   public static final
   Attribute <Boolean>    FEASIBLE =
      newAtt (BOOL,      "FEASIBLE",            UNMODIFIABLE, PROBLEM);

   public static final
   Attribute <Integer>    FOCUS_HORIZON =
      newAtt (INT,       "FOCUS_HORIZON",                 -1, DEMAND);

   public static final
   Attribute <double[]>   FOCUS_SHORTAGE_VOL =
      newAtt (DBL_VEC,   "FOCUS_SHORTAGE_VOL",  UNMODIFIABLE, PART);

   public static final
   Attribute <Boolean>    FORCED_MULTI_EQ =
      newAtt (BOOL,      "FORCED_MULTI_EQ",            false, PROBLEM);

   public static final
   Attribute <double[]>   FSS_EXEC_VOL =
      newAtt (DBL_VEC,   "FSS_EXEC_VOL",        UNMODIFIABLE, OPERATION);

   public static final
   Attribute <double[]>   FSS_SHIP_VOL =
      newAtt (DBL_VEC,   "FSS_SHIP_VOL",                 0.0, DEMAND);

   public static final
   Attribute <double[]>   FSS_SUB_VOL =
      newAtt (DBL_VEC,   "FSS_SUB_VOL",         UNMODIFIABLE, SUBSTITUTE);

   public static final
   Attribute <Boolean>    HEUR_ALLOC_ACTIVE =
      newAtt (BOOL,      "HEUR_ALLOC_ACTIVE",   UNMODIFIABLE, PROBLEM);

   public static final
   Attribute <Boolean>    HIGH_PRECISION_W_D =
      newAtt (BOOL,      "HIGH_PRECISION_W_D",         false, PROBLEM);

   public static final
   Attribute <int[]>      IMPACT_PERIOD =
      newAtt (INT_VEC,   "IMPACT_PERIOD",       UNMODIFIABLE, BOM_ENTRY,
                                                              SUBSTITUTE,
                                                              BOP_ENTRY);

   public static final
   Attribute <double[]>   INC_LOT_SIZE =
      newAtt (DBL_VEC,   "INC_LOT_SIZE",                 0.0, OPERATION);

   public static final
   Attribute <double[]>   INC_LOT_SIZE2 =
      newAtt (DBL_VEC,   "INC_LOT_SIZE2",                0.0, OPERATION);

   public static final
   Attribute <Boolean>    INDEPENDENT_OFFSETS =
      newAtt (BOOL,      "INDEPENDENT_OFFSETS",        false, PROBLEM);

   public static final
   Attribute <Boolean>    INT_EXEC_VOLS =
      newAtt (BOOL,      "INT_EXEC_VOLS",              false, OPERATION);

   public static final
   Attribute <Boolean>    INT_SHIP_VOLS =
      newAtt (BOOL,      "INT_SHIP_VOLS",              false, DEMAND);

   public static final
   Attribute <Boolean>    INT_SUB_VOLS =
      newAtt (BOOL,      "INT_SUB_VOLS",               false, SUBSTITUTE);

   public static final
   Attribute <Integer>    LATEST_PERIOD =
      newAtt (INT,       "LATEST_PERIOD",        LAST_PERIOD, BOM_ENTRY,
                                                              SUBSTITUTE,
                                                              BOP_ENTRY);

   public static final
   Attribute <int[]>      LEAD_TIME_U_B =
      newAtt (INT_VEC,   "LEAD_TIME_U_B",        LAST_PERIOD, DEMAND);

   public static final
   Attribute <double[]>   LOT_SIZE2_THRESH =
      newAtt (DBL_VEC,   "LOT_SIZE2_THRESH",             0.0, OPERATION);

   public static final
   Attribute <Double>     LOT_SIZE_TOL =
      newAtt (DBL,       "LOT_SIZE_TOL",             0.00001, PROBLEM);

   public static final
   Attribute <Boolean>    MAND_E_C =
      newAtt (BOOL,      "MAND_E_C",                   false, BOM_ENTRY);

   public static final
   Attribute <Boolean>    MINIMAL_EXCESS =
      newAtt (BOOL,      "MINIMAL_EXCESS",             false, PROBLEM);

   public static final
   Attribute <double[]>   MIN_LOT_SIZE =
      newAtt (DBL_VEC,   "MIN_LOT_SIZE",                 0.0, OPERATION);

   public static final
   Attribute <double[]>   MIN_LOT_SIZE2 =
      newAtt (DBL_VEC,   "MIN_LOT_SIZE2",                0.0, OPERATION);

   public static final
   Attribute <Boolean>    MIP_MODE =
      newAtt (BOOL,      "MIP_MODE",                   false, PROBLEM);

   public static final
   Attribute <Boolean>    MOD_HEUR_ALLOC =
      newAtt (BOOL,      "MOD_HEUR_ALLOC",             false, PROBLEM);

   public static final
   Attribute <double[]>   MRP_CONS_VOL =
      newAtt (DBL_VEC,   "MRP_CONS_VOL",        UNMODIFIABLE, PART);

   public static final
   Attribute <double[]>   MRP_EXCESS_VOL =
      newAtt (DBL_VEC,   "MRP_EXCESS_VOL",      UNMODIFIABLE, PART);

   public static final
   Attribute <double[]>   MRP_EXEC_VOL =
      newAtt (DBL_VEC,   "MRP_EXEC_VOL",        UNMODIFIABLE, OPERATION);

   public static final
   Attribute <Boolean>    MRP_NET_ALLOWED =
      newAtt (BOOL,      "MRP_NET_ALLOWED",            false, SUBSTITUTE);

   public static final
   Attribute <double[]>   MRP_RESIDUAL_VOL =
      newAtt (DBL_VEC,   "MRP_RESIDUAL_VOL",    UNMODIFIABLE, PART);

   public static final
   Attribute <double[]>   MRP_SUB_VOL =
      newAtt (DBL_VEC,   "MRP_SUB_VOL",         UNMODIFIABLE, SUBSTITUTE);

   public static final
   Attribute <Boolean>    MULTI_EXEC =
      newAtt (BOOL,      "MULTI_EXEC",                 false, PROBLEM);

   public static final
   Attribute <Boolean>    MULTI_OBJ_MODE =
      newAtt (BOOL,      "MULTI_OBJ_MODE",             false, PROBLEM);

   public static final
   Attribute <Double>     MULTI_OBJ_TOL =
      newAtt (DBL,       "MULTI_OBJ_TOL",              1.e-7, PROBLEM);

   public static final
   Attribute <Boolean>    MULTI_ROUTE =
      newAtt (BOOL,      "MULTI_ROUTE",                false, PROBLEM);

   public static final
   Attribute <Boolean>    NET_ALLOWED =
      newAtt (BOOL,      "NET_ALLOWED",                 true, SUBSTITUTE);

   public static final
   Attribute <Boolean>    NSTN_RESIDUAL =
      newAtt (BOOL,      "NSTN_RESIDUAL",              false, PROBLEM);

   public static final
   Attribute <Integer>    N_PERIODS =
      newAtt (INT,       "N_PERIODS",                     26, PROBLEM);

   public static final
   Attribute <Integer>    N_SCENARIOS =
      newAtt (INT,       "N_SCENARIOS",                    1, PROBLEM);

   public static final
   Attribute <String>     OBJECTIVE_LIST_SPEC =
      newAtt (STRING,    "OBJECTIVE_LIST_SPEC",    "Default", PROBLEM);

   public static final
   Attribute <Integer>    OBJECTIVE_SEQ_NO =
      newAtt (INT,       "OBJECTIVE_SEQ_NO",      NO_DEFAULT, PROBLEM);

   public static final
   Attribute <Integer>    OBJECT_STAGE =
      newAtt (INT,       "OBJECT_STAGE",                   0, PART,
                                                              OPERATION);
   public static final
   Attribute <Double>     OBJ_VALUE =
      newAtt (DBL,       "OBJ_VALUE",           UNMODIFIABLE, PROBLEM);

   public static final
   Attribute <double[]>   OFFSET =
      newAtt (DBL_VEC,   "OFFSET",                       0.0, BOM_ENTRY,
                                                              SUBSTITUTE,
                                                              BOP_ENTRY);

   public static final
   Attribute <String>     OPERATION_NAME =
      newAtt (STRING,    "OPERATION_NAME",        NO_DEFAULT, OPERATION);

   public static final
   Attribute <String>     PART_NAME =
      newAtt (STRING,    "PART_NAME",             NO_DEFAULT, PART);

   public static final
   Attribute <Boolean>    PEN_EXEC =
      newAtt (BOOL,      "PEN_EXEC",                   false, PROBLEM);

   public static final
   Attribute <Boolean>    PERF_PEGGING =
      newAtt (BOOL,      "PERF_PEGGING",               false, PROBLEM);

   public static final
   Attribute <int[]>      PERIOD_STAGE =
      newAtt (INT_VEC,   "PERIOD_STAGE",                   0, PROBLEM);

   public static final
   Attribute <Boolean>    PGD_CRIT_LIST_MODE =
      newAtt (BOOL,      "PGD_CRIT_LIST_MODE",         false, PROBLEM);

   public static final
   Attribute <Boolean>    PIP_ENABLED =
      newAtt (BOOL,      "PIP_ENABLED",                false, OPERATION);

   public static final
   Attribute <Boolean>    PIP_EXISTS =
      newAtt (BOOL,      "PIP_EXISTS",          UNMODIFIABLE, PROBLEM);

   public static final
   Attribute <Integer>    PIP_RANK =
      newAtt (INT,       "PIP_RANK",                       0, OPERATION);

   public static final
   Attribute <Boolean>    PIP_SEQ_FROM_HEUR =
      newAtt (BOOL,      "PIP_SEQ_FROM_HEUR",          false, PROBLEM);

   public static final
   Attribute <Boolean>    POSTPROCESSED =
      newAtt (BOOL,      "POSTPROCESSED",       UNMODIFIABLE, PROBLEM);

   public static final
   Attribute <Boolean>    PREF_HIGH_STOCK_S_L_BS =
      newAtt (BOOL,      "PREF_HIGH_STOCK_S_L_BS",     false, PROBLEM);

   public static final
   Attribute <Boolean>    PREPROCESSED =
      newAtt (BOOL,      "PREPROCESSED",        UNMODIFIABLE, PROBLEM);

   public static final
   Attribute <int[]>      PRIORITY =
      newAtt (INT_VEC,   "PRIORITY",                       0, DEMAND);

   public static final
   Attribute <Double>     PROBABILITY =
      newAtt (DBL,       "PROBABILITY",                  1.0, PROBLEM);

   public static final
   Attribute <double[]>   PRODUCT_RATE =
      newAtt (DBL_VEC,   "PRODUCT_RATE",                 1.0, BOP_ENTRY);

   public static final
   Attribute <double[]>   PROD_VOL =
      newAtt (DBL_VEC,   "PROD_VOL",            UNMODIFIABLE, PART);

   public static final
   Attribute <boolean[]>  PROP_RTG =
      newAtt (BOOL_VEC,  "PROP_RTG",                   false, PART,
                                                              BOM_ENTRY);

   public static final
   Attribute <double[]>   REQ_VOL =
      newAtt (DBL_VEC,   "REQ_VOL",             UNMODIFIABLE, PART);

   public static final
   Attribute <double[]>   RESIDUAL_VOL =
      newAtt (DBL_VEC,   "RESIDUAL_VOL",        UNMODIFIABLE, PART);

   public static final
   Attribute <Boolean>    RESPECT_STOCK_S_L_BS =
      newAtt (BOOL,      "RESPECT_STOCK_S_L_BS",       false, PROBLEM);

   public static final
   Attribute <Boolean>    ROUND_REQ_VOLS =
      newAtt (BOOL,      "ROUND_REQ_VOLS",             false, PROBLEM);

   public static final
   Attribute <double[]>   ROUTING_SHARE =
      newAtt (DBL_VEC,   "ROUTING_SHARE",                1.0, BOM_ENTRY,
                                                              SUBSTITUTE,
                                                              BOP_ENTRY);

   public static final
   Attribute <Boolean>    SCRAP_ALLOWED =
      newAtt (BOOL,      "SCRAP_ALLOWED",               true, PART);

   public static final
   Attribute <double[]>   SCRAP_COST =
      newAtt (DBL_VEC,   "SCRAP_COST",                   0.0, PART);

   public static final
   Attribute <double[]>   SCRAP_VOL =
      newAtt (DBL_VEC,   "SCRAP_VOL",           UNMODIFIABLE, PART);

   public static final
   Attribute <Double>     SEARCH_INC =
      newAtt (DBL,       "SEARCH_INC",                   1.0, DEMAND);

   public static final
   Attribute <Boolean>    SELECTION_RECOVERY =
      newAtt (BOOL,      "SELECTION_RECOVERY",         false, PROBLEM);

   public static final
   Attribute <Boolean>    SEL_FOR_DEL =
      newAtt (BOOL,      "SEL_FOR_DEL",                false, PART,
                                                              DEMAND,
                                                              OPERATION,
                                                              BOM_ENTRY,
                                                              SUBSTITUTE,
                                                              BOP_ENTRY);

   public static final
   Attribute <Boolean>    SEL_SPLIT =
      newAtt (BOOL,      "SEL_SPLIT",                  false, PROBLEM);

   public static final
   Attribute <double[]>   SHADOW_PRICE =
      newAtt (DBL_VEC,   "SHADOW_PRICE",        UNMODIFIABLE, PART);

   public static final
   Attribute <Boolean>    SHIP_LATE_ALLOWED =
      newAtt (BOOL,      "SHIP_LATE_ALLOWED",           true, DEMAND);

   public static final
   Attribute <int[]>      SHIP_LATE_U_B =
      newAtt (INT_VEC,   "SHIP_LATE_U_B",        LAST_PERIOD, DEMAND);

   public static final
   Attribute <double[]>   SHIP_REWARD =
      newAtt (DBL_VEC,   "SHIP_REWARD",                  0.0, DEMAND);

   public static final
   Attribute <double[]>   SHIP_VOL =
      newAtt (DBL_VEC,   "SHIP_VOL",                     0.0, DEMAND);

   public static final
   Attribute <Boolean>    SINGLE_SOURCE =
      newAtt (BOOL,      "SINGLE_SOURCE",              false, PART,
                                                              BOM_ENTRY);

   public static final
   Attribute <Boolean>    SKIP_FAILURES =
      newAtt (BOOL,      "SKIP_FAILURES",               true, PROBLEM);

   public static final
   Attribute <String>     SOLVER_LOG_FILE_NAME =
      newAtt (STRING,    "SOLVER_LOG_FILE_NAME","solver.log", PROBLEM);

   public static final
   Attribute <Boolean>    STAGE_BY_OBJECT =
      newAtt (BOOL,      "STAGE_BY_OBJECT",            false, PROBLEM);

   public static final
   Attribute <Boolean>    STOCH_MODE =
      newAtt (BOOL,      "STOCH_MODE",                 false, PROBLEM);

   public static final
   Attribute <Boolean>    STOCH_SOLN_MODE =
      newAtt (BOOL,      "STOCH_SOLN_MODE",     UNMODIFIABLE, PROBLEM);

   public static final
   Attribute <BoundSet>   STOCK_BOUNDS =
      newAtt (BOUND_SET, "STOCK_BOUNDS",              BOUNDS, PART);

   public static final
   Attribute <double[]>   STOCK_COST =
      newAtt (DBL_VEC,   "STOCK_COST",                   0.0, PART);

   public static final
   Attribute <Boolean>    STOCK_REALLOCATION =
      newAtt (BOOL,      "STOCK_REALLOCATION",         false, PROBLEM);

   public static final
   Attribute <double[]>   STOCK_VOL =
      newAtt (DBL_VEC,   "STOCK_VOL",           UNMODIFIABLE, PART);

   public static final
   Attribute <double[]>   SUB_COST =
      newAtt (DBL_VEC,   "SUB_COST",                     0.0, SUBSTITUTE);

   public static final
   Attribute <Integer>    SUB_INDEX =
      newAtt (INT,       "SUB_INDEX",           UNMODIFIABLE, SUBSTITUTE);

   public static final
   Attribute <double[]>   SUB_VOL =
      newAtt (DBL_VEC,   "SUB_VOL",                      0.0, SUBSTITUTE);

   public static final
   Attribute <double[]>   SUPPLY_VOL =
      newAtt (DBL_VEC,   "SUPPLY_VOL",                   0.0, PART);

   public static final
   Attribute <Boolean>    TIE_BREAK_PROP_RT =
      newAtt (BOOL,      "TIE_BREAK_PROP_RT",          false, PROBLEM);

   public static final
   Attribute <String>     TITLE =
      newAtt (STRING,    "TITLE",                 "Untitled", PROBLEM);

   public static final
   Attribute <Boolean>    TRUNC_OFFSETS =
      newAtt (BOOL,      "TRUNC_OFFSETS",              false, PROBLEM);

   public static final
   Attribute <Boolean>    TWO_LEVEL_LOT_SIZES =
      newAtt (BOOL,      "TWO_LEVEL_LOT_SIZES",        false, OPERATION);

   public static final
   Attribute <Boolean>    TWO_WAY_MULTI_EXEC =
      newAtt (BOOL,      "TWO_WAY_MULTI_EXEC",         false, PROBLEM);

   public static final
   Attribute <Boolean>    USER_HEUR_START =
      newAtt (BOOL,      "USER_HEUR_START",            false, PROBLEM);

   public static final
   Attribute <Boolean>    USE_FOCUS_HORIZONS =
      newAtt (BOOL,      "USE_FOCUS_HORIZONS",          true, PROBLEM);

   public static final
   Attribute <Double>     WBOUNDS =
      newAtt (DBL,       "WBOUNDS",                  10000.0, PROBLEM);

   public static final
   Attribute <double[]>   YIELD_RATE =
      newAtt (DBL_VEC,   "YIELD_RATE",                   1.0, OPERATION);

//==============================================================================
// Public Methods
//==============================================================================

//------------------------------------------------------------------------------
// getAttributes
//
// Return an unmodifiable List of all Attributes in WIT-J.
//------------------------------------------------------------------------------

   public static List <Attribute <?>> getAttributes ()
      {
      try
         {
         notAllowedWhenTerminated ("Attribute.getAttributes");

         return allAttributes;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getWitName ()
//
// Returns the WIT name for this Attribute.
//------------------------------------------------------------------------------

   public final String getWitName ()
      {
      try
         {
         notAllowedWhenTerminated ("Attribute.getWitName");

         return myWitName;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getValueType ()
//
// Returns the value type of this Attribute.
//------------------------------------------------------------------------------

   public Class <V> getValueType ()
      {
      try
         {
         notAllowedWhenTerminated ("Attribute.getValueType");

         return myAttValType.getValueType ();
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// appliesTo (theClass)
//
// Returns true, iff this Attribute applies to the Component class indicated by
// theClass (which must not be class Component).
//------------------------------------------------------------------------------

   public boolean appliesTo (Class <? extends Component> theClass)
      {
      try
         {
         CompClassID theCompClassID;

         notAllowedWhenTerminated ("Attribute.appliesTo");

         mustBeNonNull (theClass, "theClass", "Attribute <?>.appliesTo");

         if (theClass == Component.class)
            throw
               new StatusQuoException (
                  "   Attribute.appliesTo (theClass) was called\n"
                + "   where the theClass is Component.class.");

         theCompClassID = CompClassID.getInstanceFor (theClass);

         assert (theCompClassID != null);

         return appliesTo (theCompClassID);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// isModifiable ()
//
// Returns true, iff the corresponding set method(s) of class Component can
// validly be invoked for this Attribute.
//------------------------------------------------------------------------------

   public boolean isModifiable ()
      {
      try
         {
         notAllowedWhenTerminated ("Attribute.isModifiable");

         return modifiable;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// isValidFor (theComp)
//
// Returns true, iff this Attribute is valid for a "get" call to theComp in the
// present state.
//------------------------------------------------------------------------------
   
   public boolean isValidFor (Component theComp)
      {
      try
         {
         notAllowedWhenTerminated ("Attribute.isValidFor");

         if (! appliesTo (theComp.getCompClassID ()))
            return false;

         if (reqMaterialPart)
            if (theComp.asPart ().getCategory () != Part.Category.MATERIAL)
               return false;

         if (reqStochMode)
            if (! theComp.getProblem ().get (Attribute.STOCH_MODE))
               return false;

         if (reqMultiObjMode)
            if (! theComp.getProblem ().get (Attribute.MULTI_OBJ_MODE))
               return false;

         if (this == OBJECT_STAGE)
            if (! theComp.getProblem ().get (Attribute.STAGE_BY_OBJECT))
               return false;

         if (this == PERIOD_STAGE)
            if (  theComp.getProblem ().get (Attribute.STAGE_BY_OBJECT))
               return false;

         return true;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// hasDefaultValue ()
//
// Returns true, iff this Attribute has a default value.
//------------------------------------------------------------------------------

   public boolean hasDefaultValue ()
      {
      try
         {
         notAllowedWhenTerminated ("Attribute.hasDefaultValue");

         return haveDefaultValue;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getDefaultValue (theProblem)
//
// Returns the default value of this Attribute for Components of theProblem.
// The value type must not be BoundSet.
// This Attribute must have a default value.
//------------------------------------------------------------------------------

   public V getDefaultValue (Problem theProblem)
      {
      try
         {
         mustBeActive (theProblem, "theProblem", "Attribute.getDefaultValue");
         mustNotBeBSetAtt                       ("Attribute.getDefaultValue");
         mustHaveDefault                        ();

         if (myDefaultCase == LAST_PERIOD)
            return myAttValType.getLastPeriod (theProblem);

         else if (myAttValType.isArrayType ())
            return getDefaultArray (theProblem);

         else
            return mySpecDefValue;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getDefaultBoundSet (theProblem, hardLBArr, softLBArr, hardUBArr)
//
// Retrieves the default value of this Attribute <BoundSet> for Components in
// theProblem, storing it in the arrays.
// The value type must be BoundSet.
//------------------------------------------------------------------------------

   public void getDefaultBoundSet (
         Problem  theProblem,
         double[] hardLBArr,
         double[] softLBArr,
         double[] hardUBArr)
      {
      try
         {
         String methName;
         int    theIdx;

         methName = "Attribute.getDefaultBoundSet";

         mustBeBSetAtt         ();
         mustBeActive          (theProblem, "theProblem", methName);
         mustBeNonNull         (hardLBArr,  "hardLBArr",  methName);
         mustBeNonNull         (softLBArr,  "softLBArr",  methName);
         mustBeNonNull         (hardUBArr,  "hardUBArr",  methName);
         checkBoundArrayLength (theProblem,  hardLBArr,  "hardLBArr");
         checkBoundArrayLength (theProblem,  softLBArr,  "softLBArr");
         checkBoundArrayLength (theProblem,  hardUBArr,  "hardUBArr");

         for (theIdx = 0; theIdx < theProblem.getNPeriods (); theIdx++)
            {
            hardLBArr[theIdx] =  0.0;
            softLBArr[theIdx] =  0.0;
            hardUBArr[theIdx] = -1.0;
            }
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// asAttOfType (theValueType)
//
// Returns a view of this Attribute, safely converted to an Attribute <V2>,
// where V2 corresponds to theValueType.
// theValueType must match the value type of this Attribute.
//------------------------------------------------------------------------------

  @SuppressWarnings ("unchecked")

   public <V2> Attribute <V2> asAttOfType (Class <V2> theValueType)
      {
      try
         {
         notAllowedWhenTerminated ("Attribute.asAttOfType");

         if (theValueType != myAttValType.getValueType ())
            throw
               new StatusQuoException (
                  "   Attribute.asAttOfType (theValueType) was called, where\n"
                + "   theValueType does not match the value type of the "
                +    "Attribute.\n"
                + "\n"
                + "      Attribute:            " + this + "\n"
                + "      Attribute Value Type: " + myAttValType.getName () 
                                                 + "\n"
                + "      Requested Value Type: "
                +        theValueType.getSimpleName ());

         return (Attribute <V2>) this;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// toString ()
//
// Override from class Object.
//------------------------------------------------------------------------------

   public String toString ()
      {
      try
         {
         notAllowedWhenTerminated ("Attribute.toString");

         return myWitJName;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//==============================================================================
// Package-Internal Static Nested Enum DefaultCase
//
// Indicates which kind of default value an Attribute has.
//==============================================================================

   static enum DefaultCase
   {
      SPECIFIED,
      LAST_PERIOD,
      BOUNDS,
      NO_DEFAULT,
      UNMODIFIABLE,
   }

//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// makeOrdAttImp (theCoord)
//
// Creates a new OrdAttImp for this Attribute and theCoord.
// Not allowed for value type BoundSet.
//------------------------------------------------------------------------------

   void makeOrdAttImp (Coordinator theCoord)
      {
      myAttValType.makeOrdAttImp (this, theCoord);
      }

//------------------------------------------------------------------------------
// appliesTo (theCompClassID)
//
// Returns true, iff this Attribute applies to the Component class identified by
// theCompClassID.
//------------------------------------------------------------------------------

   boolean appliesTo (CompClassID theCompClassID)
      {
      return myApplicableSet.contains (theCompClassID);
      }

//------------------------------------------------------------------------------
// selfTest ()
//
// Does self-test for Attributes.
//------------------------------------------------------------------------------

   static void selfTest ()
      {
      checkAlphaOrder ();
      }

//------------------------------------------------------------------------------
// mustNotBeBSetAtt (fullMethodName)
//
// Verifies that the value type of this Attribute is not BoundSet.
//------------------------------------------------------------------------------

   void mustNotBeBSetAtt (String fullMethodName)
      {
      if (myAttValType == BOUND_SET)
         throw
            new StatusQuoException (
               "   " + fullMethodName + " was called with a BoundSet Attribute."
             + "\n\n"
             + "      Attribute: " + this);
      }

//------------------------------------------------------------------------------
// Accessors
//------------------------------------------------------------------------------

   AttValType <V> getAttValType ()
      {
      return myAttValType;
      }

   int getIndex ()
      {
      return myIndex;
      }

//==============================================================================
// Private Constructor
//==============================================================================

   private Attribute (
         String         theWitJName,
         AttValType <V> theAttValType,
         DefaultCase    theDefaultCase,
         V              theSpecDefValue,
         CompClassID[]  theCompClassIDArr)
      {
      if (theDefaultCase == SPECIFIED)
         assert (theSpecDefValue != null);
      else
         assert (theSpecDefValue == null);

      myWitJName       = theWitJName;
      myWitName        = makeWitName (theWitJName);
      myAttValType     = theAttValType;
      myDefaultCase    = theDefaultCase;

      haveDefaultValue =    (theDefaultCase == SPECIFIED)
                         || (theDefaultCase == LAST_PERIOD)
                         || (theDefaultCase == BOUNDS);

      mySpecDefValue   = theSpecDefValue;

      myApplicableSet  = newCompClassIDSet (theCompClassIDArr);

      myIndex          = initAttributes.size ();

      modifiable       = (theDefaultCase != UNMODIFIABLE);

      reqMaterialPart  = materialPartWitJNames.contains (theWitJName);
      reqStochMode     = stochModeWitJNames   .contains (theWitJName);
      reqMultiObjMode  = multiObjModeWitJNames.contains (theWitJName);

      initAttributes.add (this);
      }

//==============================================================================
// Private Methods
//==============================================================================

//------------------------------------------------------------------------------
// makeWitName (theWitJName)
//
// Computes and returns the WIT name that corresponds to theWitJName for an
// Attribute.
//------------------------------------------------------------------------------

   private static String makeWitName (String theWitJName)
      {
      StringBuilder theWitName;
      int           theIdx;
      char          theChar;

      theWitName = new StringBuilder (theWitJName.length ());

      for (theIdx = 0; theIdx < theWitJName.length (); theIdx ++)
         {
         theChar = theWitJName.charAt (theIdx);

         if (theChar == '_')
            continue;

         theChar = Character.toLowerCase (theChar);

         if (theIdx > 0)
            if (theWitJName.charAt (theIdx - 1) == '_')
               {
               theChar = Character.toUpperCase (theChar);
               }

         theWitName.append (theChar);
         }

      return theWitName.toString ();
      }

//------------------------------------------------------------------------------
// newAtt (theAttValType, theWitJName, theSpecDefValue, theCompClassIDs)
//
// Creates and returns a new Attribute <V>:
//    theAttValType   identifies the value type.
//    theWitJName     will be the name of the Attribute.
//    theSpecDefValue will be the specified default value of the Attribute.
//    theCompClassIDs lists the CompClassIDs of the Component classes to which
//                    the Attribute will apply.
//------------------------------------------------------------------------------

   private static <V> Attribute <V> newAtt (
         AttValType <V> theAttValType,
         String         theWitJName,
         V              theSpecDefValue,
         CompClassID... theCompClassIDs)
      {
      return
         new Attribute <V> (
            theWitJName,
            theAttValType,
            SPECIFIED,
            theSpecDefValue,
            theCompClassIDs);
      }

//------------------------------------------------------------------------------
// newAtt (...)
//
// Same as newAtt above, but for a specific array type, where theSpecDefValue is
// a scalar of the array type.
//------------------------------------------------------------------------------

   private static Attribute <boolean[]> newAtt (
         AttValType <boolean[]> theAttValType,
         String                 theWitJName,
         boolean                theSpecDefValue,
         CompClassID...         theCompClassIDs)
      {
      return
         new Attribute <boolean[]> (
            theWitJName,
            theAttValType,
            SPECIFIED,
            new boolean[]{theSpecDefValue},
            theCompClassIDs);
      }

   private static Attribute <int[]> newAtt (
         AttValType <int[]>  theAttValType,
         String              theWitJName,
         int                 theSpecDefValue,
         CompClassID...      theCompClassIDs)
      {
      return
         new Attribute <int[]> (
            theWitJName,
            theAttValType,
            SPECIFIED,
            new int[]{theSpecDefValue},
            theCompClassIDs);
      }

   private static Attribute <double[]> newAtt (
         AttValType <double[]> theAttValType,
         String                theWitJName,
         double                theSpecDefValue,
         CompClassID...        theCompClassIDs)
      {
      return
         new Attribute <double[]> (
            theWitJName,
            theAttValType,
            SPECIFIED,
            new double[]{theSpecDefValue},
            theCompClassIDs);
      }

//------------------------------------------------------------------------------
// newAtt (theAttValType, theWitJName, theDefaultCase, theCompClassIDs)
//
// Creates and returns a new Attribute <V>:
//    theAttValType   identifies the value type.
//    theWitJName     will be the name of the Attribute.
//    theDefaultCase  will be the DefaultCase for the Attribute, which must not
//                    be SPECIFIED.
//    theCompClassIDs lists the CompClassIDs of the Component classes to which
//                    the Attribute will apply.
//------------------------------------------------------------------------------

   private static <V> Attribute <V> newAtt (
         AttValType <V> theAttValType,
         String         theWitJName,
         DefaultCase    theDefaultCase,
         CompClassID... theCompClassIDs)
      {
      return
         new Attribute <V> (
            theWitJName,
            theAttValType,
            theDefaultCase,
            null,
            theCompClassIDs);
      }

//------------------------------------------------------------------------------
// newCompClassIDSet (theCompClassArr)
//
// Creates and returns an EnumSet whose contents are theCompClassArr.
//------------------------------------------------------------------------------

   private static EnumSet <CompClassID> newCompClassIDSet (
         CompClassID[] theCompClassArr)
      {
      EnumSet <CompClassID> theCompClassIDSet;

      theCompClassIDSet = EnumSet.noneOf (CompClassID.class);

      for (CompClassID theCompClassID: theCompClassArr)
         {
         theCompClassIDSet.add (theCompClassID);
         }

      return theCompClassIDSet;
      }

//------------------------------------------------------------------------------
// mustHaveDefault ()
//
// Verifies that this Attribute has a default value.
//------------------------------------------------------------------------------

   private void mustHaveDefault ()
      {
      if (! haveDefaultValue)
         throw
            new StatusQuoException (
               "   Attribute.getDefaultValue was called on an Attribute\n"
             + "   that does not have a default value.\n"
             + "\n"
             + "      Attribute: " + this);
      }

//------------------------------------------------------------------------------
// getDefaultArray (theProblem)
//
// Returns the default value of this Attribute for Components of theProblem.
// The value type must be an array type.
//------------------------------------------------------------------------------

   private V getDefaultArray (Problem theProblem)
      {
      V   theArray;
      int theIdx;

      assert (myAttValType.isArrayType ());

      theArray = myAttValType.newArray (theProblem.getNPeriods ());

      for (theIdx = 0; theIdx < theProblem.getNPeriods (); theIdx ++)
         {
         myAttValType.setElemToElem0 (theArray, theIdx, mySpecDefValue);
         }

      return theArray;
      }

//------------------------------------------------------------------------------
// mustBeBSetAtt ()
//
// Verifies that the value type of this Attribute is BoundSet.
//------------------------------------------------------------------------------

   private void mustBeBSetAtt ()
      {
      if (myAttValType != BOUND_SET)
         throw
            new StatusQuoException (
               "   Attribute.getDefaultBoundSet was called on a\n"
             + "   non-BoundSet Attribute.\n"
             + "\n"
             + "      Attribute: " + this);
      }

//------------------------------------------------------------------------------
// checkBoundArrayLength (theProblem, theBoundArr, methodName)
//
// Verifies that theBoundArr has a length that matches nPeriods for theProblem.
//------------------------------------------------------------------------------

   private void checkBoundArrayLength (
         Problem  theProblem,
         double[] theBoundArr,
         String   argName)
      {
      if (theBoundArr.length != theProblem.getNPeriods ())
         throw
            new StatusQuoException (
               "   Attribute.getDefaultBoundSet was called with an array\n"
             + "   whose length does not match nPeriods for the Problem.\n"
             + "\n"
             + "      Attribute:      " + this               + "\n"
             + "      Array Argument: " + argName            + "\n"
             + "      Array Length:   " + theBoundArr.length + "\n"
             + "      nPeriods:       " + theProblem.getNPeriods ());
      }

//------------------------------------------------------------------------------
// checkAlphaOrder ()
//
// Verifies that the Attributes are listed in alphabetical order.
//------------------------------------------------------------------------------

   private static void checkAlphaOrder ()
      {
      Attribute <?> prevAtt = null;

      println (
           "\n"
         + "Verifying that the Attributes are listed in alphabetical order.\n");

      for (Attribute <?> theAtt: allAttributes)
         {
         if (prevAtt != null)
            if (theAtt.myWitJName.compareTo (prevAtt.myWitJName) < 0)
               {
               println (
                  "The following two Attributes are listed in "
                + "non-alphabetical order:\n"
                + "\n"
                + "   " + prevAtt + ")\n"
                + "   " +  theAtt + ")\n");

               throw new AssertionError ();
               }

         prevAtt = theAtt;
         }

      println ("Verified: The Attributes are listed in alphabetical order.");
      }

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// initAttributes
//
// During static initialization, initAttributes is a modifiable ArrayList of the
// Attributes in WIT-J.
// After static initialization, initAttributes is a null pointer.
// (This is done to facilitate thread safety.)
//------------------------------------------------------------------------------

   private static ArrayList <Attribute <?>> initAttributes;

//------------------------------------------------------------------------------
// allAttributes
//
// After static initialization, allAttributes is an unmodifiable List of all of
// the Attributes in WIT-J.
//------------------------------------------------------------------------------

   private static final List <Attribute <?>> allAttributes;

//------------------------------------------------------------------------------
// myWitJName
//
// The WIT-J name of this Attribute.
// This is the name of the static field of class Attribute <V> whose value is a
// reference to this Attribute.
//------------------------------------------------------------------------------

   private final String myWitJName;

//------------------------------------------------------------------------------
// myWitName
//
// The name of the WIT attribute corresponding to this Attribute.
// This is the name of the attribute as given in the WIT Guide.
//------------------------------------------------------------------------------

   private final String myWitName;

//------------------------------------------------------------------------------
// myAttValType
//
// Represents the value type of this Attribute.
//------------------------------------------------------------------------------

   private final AttValType <V> myAttValType;

//------------------------------------------------------------------------------
// myDefaultCase
//
// Specifies which kind of default value this Attribute has, if any.
//------------------------------------------------------------------------------

   private final DefaultCase myDefaultCase;

//------------------------------------------------------------------------------
// haveDefaultValue
//
// True, iff this Attribute has a default value.
//------------------------------------------------------------------------------

   private final boolean haveDefaultValue;

//------------------------------------------------------------------------------
// mySpecDefValue
//
// If myDefaultCase == SPECIFIED, mySpecDefValue is the specified default value
//                                of this Attribute.
// If myDefaultCase != SPECIFIED, mySpecDefValue == null.
//------------------------------------------------------------------------------

   private final V mySpecDefValue;

//------------------------------------------------------------------------------
// myApplicableSet
//
// The set of CompClassID identifying the set of Component classes to which
// this Attribute applies.
//------------------------------------------------------------------------------

   private final EnumSet <CompClassID> myApplicableSet;

//------------------------------------------------------------------------------
// myIndex
//
// # Attributes constructed before this one.
//------------------------------------------------------------------------------

   private final int myIndex;

//------------------------------------------------------------------------------
// True, iff the corresponding set method(s) of class Component can validly be
// invoked with this Attribute.
//------------------------------------------------------------------------------

   private final boolean modifiable;

//------------------------------------------------------------------------------
// reqMaterialPart
//
// true, iff this Attribute applies only to Parts of Category MATERIAL.
//------------------------------------------------------------------------------

   private final boolean reqMaterialPart;

//------------------------------------------------------------------------------
// reqStochMode
//
// true, iff this Attribute is valid only in stochastic mode (stochMode == true)
//------------------------------------------------------------------------------

   private final boolean reqStochMode;

//------------------------------------------------------------------------------
// reqMultiObjMode
//
// true, iff this Attribute is valid only in multiple objectives mode
// (multiObjMode == true)
//------------------------------------------------------------------------------

   private final boolean reqMultiObjMode;

//------------------------------------------------------------------------------
// materialPartWitJNames
//
// A List of the WIT-J names of all Attributes that require a material Part.
//------------------------------------------------------------------------------

   private static final HashSet <String> materialPartWitJNames;

//------------------------------------------------------------------------------
// stochModeWitJNames
//
// A List of the WIT-J names of all Attributes that require stochastic mode.
//------------------------------------------------------------------------------

   private static final HashSet <String> stochModeWitJNames;

//------------------------------------------------------------------------------
// multiObjModeWitJNames
//
// A List of the WIT-J names of all Attributes that require multiple objectives
// mode.
//------------------------------------------------------------------------------

   private static final HashSet <String> multiObjModeWitJNames;

//==============================================================================
// Static initialization block
//==============================================================================

   static
      {
      initAttributes.trimToSize ();

      allAttributes  = Collections.unmodifiableList (initAttributes);

      initAttributes = null;
      }

//==============================================================================
}
