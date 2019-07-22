// Copyright (C) 2001, International Business Machines
// Corporation and others.  All Rights Reserved.
#include "BB_lp_param.hpp"
#include "BCP_parameters.hpp"

using std::make_pair;

template <>
void BCP_parameter_set<BB_lp_par>::create_keyword_list() {
   // Create the list of keywords for parameter file reading
   //--------------------------------------------------------------------------
   // CharPar
   keys.push_back(make_pair(BCP_string("BB_KnapsackCover"),
			    BCP_parameter(BCP_CharPar,
					  KnapsackCover)));
   keys.push_back(make_pair(BCP_string("BB_LiftAndProject"),
			    BCP_parameter(BCP_CharPar,
					  LiftAndProject)));
   keys.push_back(make_pair(BCP_string("BB_SimpleRounding"),
			    BCP_parameter(BCP_CharPar,
					  SimpleRounding)));
   keys.push_back(make_pair(BCP_string("BB_OddHole"),
			    BCP_parameter(BCP_CharPar,
					  OddHole)));
//    keys.push_back(make_pair(BCP_string("BB_"),
// 			    BCP_parameter(BCP_CharPar,
// 					  )));

   //--------------------------------------------------------------------------
   // IntPar
//    keys.push_back(make_pair(BCP_string("BB_"),
// 			    BCP_parameter(BCP_IntPar,
// 					  )));
   //--------------------------------------------------------------------------
   // DoublePar
//    keys.push_back(make_pair(BCP_string("BB_"),
// 			    BCP_parameter(BCP_DoublePar,
// 					  )));

   //--------------------------------------------------------------------------
   // StringPar
//    keys.push_back(make_pair(BCP_string("BB_"),
// 			    BCP_parameter(BCP_StringPar,
// 					  )));
   //--------------------------------------------------------------------------
   // StringArrayPar
//    keys.push_back(make_pair(BCP_string("BB_"),
// 			    BCP_parameter(BCP_StringArrayPar,
// 					  )));
}

//#############################################################################

template <>
void BCP_parameter_set<BB_lp_par>::set_default_entries(){
   // set_entry(parname, parvalue);
   //--------------------------------------------------------------------------
   // CharPar
   set_entry(KnapsackCover, true);
   set_entry(LiftAndProject, true);
   set_entry(SimpleRounding, true);
   set_entry(OddHole, true);
   //--------------------------------------------------------------------------
   // IntPar
   //--------------------------------------------------------------------------
   // DoublePar
   //--------------------------------------------------------------------------
   // StringPar
   //--------------------------------------------------------------------------
   // StringArrayPar
}
