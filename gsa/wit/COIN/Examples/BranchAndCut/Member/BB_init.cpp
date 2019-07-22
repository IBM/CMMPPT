// Copyright (C) 2001, International Business Machines
// Corporation and others.  All Rights Reserved.

#include "BB_init.hpp"
#include "BB_lp.hpp"
#include "BB_tm.hpp"
#include "BB_lp_param.hpp"
#include "BB_tm_param.hpp"

//#############################################################################

USER_initialize * BCP_user_init()
{
   return new BB_init;
}

BCP_lp_user *
BB_init::lp_init(BCP_lp_prob& p)
{
   return new BB_lp;
}

BCP_tm_user *
BB_init::tm_init(BCP_tm_prob& p,
		 const int argnum, const char * const * arglist)
{
   BB_tm* tm = new BB_tm;
   tm->readInput(arglist[1]);

   return tm;
}
