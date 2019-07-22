#ifdef COIN_USE_DYLP
#ifndef OsiDylpMessages_H
#define OsiDylpMessages_H

/*! \legal
  Copyright (C) 2003.
  Lou Hafer, International Business Machines Corporation and others. All
  Rights Reserved.
*/

/*
  sccs: %W%     %G%
  cvs: $Id: OsiDylpMessages.hpp,v 1.1 2003/05/14 22:40:31 lou-oss Exp $
*/


#include "CoinMessageHandler.hpp"

typedef enum { ODSI_TEST_MSG,
	       ODSI_MPSFILEIO,
	       ODSI_UNSUPFORCEDO,
	       ODSI_IGNORED,
	       ODSI_DUMMY_END } OsiDylpMessageID_enum ;


#endif /* OsiDylpMessages_H */
#endif /* COIN_USE_DYLP */
