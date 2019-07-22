// Copyright (C) 2001, International Business Machines
// Corporation and others.  All Rights Reserved.
#include "BB_tm_param.hpp"
#include "BCP_parameters.hpp"

using std::make_pair;

template <>
void BCP_parameter_set<BB_tm_par>::create_keyword_list() {
   // Create the list of keywords for parameter file reading
   //--------------------------------------------------------------------------
   // CharPar
   keys.push_back(make_pair(BCP_string("BB_TestSolutions"),
 			    BCP_parameter(BCP_CharPar,
 					  TestSolutions)));
   keys.push_back(make_pair(BCP_string("BB_AllCore"),
			    BCP_parameter(BCP_CharPar,
					  AllCore)));
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
   keys.push_back(make_pair(BCP_string("BB_InputFile"),
 			    BCP_parameter(BCP_StringPar,
 					  InputFile)));
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
void BCP_parameter_set<BB_tm_par>::set_default_entries(){
   // set_entry(parname, parvalue);
   //--------------------------------------------------------------------------
   // CharPar
   set_entry(AllCore, false);
   set_entry(TestSolutions, true);
   //--------------------------------------------------------------------------
   // IntPar
   //--------------------------------------------------------------------------
   // DoublePar
   //--------------------------------------------------------------------------
   // StringPar
   set_entry(InputFile, "");
   //--------------------------------------------------------------------------
   // StringArrayPar
}
