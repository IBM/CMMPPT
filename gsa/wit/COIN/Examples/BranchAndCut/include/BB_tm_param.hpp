// Copyright (C) 2001, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef _BB_TM_PARAM_H
#define _BB_TM_PARAM_H

struct BB_tm_par {
   enum chr_params{
      AllCore,
      TestSolutions,
      //
      end_of_chr_params
   };
   enum int_params{
      // the dummy is needed so the allocation won't try for 0 entries
      int_dummy,
      //
      end_of_int_params
   };
   enum dbl_params{
      dbl_dummy,
      //
      end_of_dbl_params
   };
   enum str_params{
      InputFile,
      //
      end_of_str_params
   };
   enum str_array_params{
      str_array_dummy,
      //
      end_of_str_array_params
   };
};

#endif
