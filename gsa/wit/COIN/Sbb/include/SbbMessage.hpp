// Copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef SbbMessage_H
#define SbbMessage_H

#if defined(_MSC_VER)
// Turn off compiler warning about long names
#  pragma warning(disable:4786)
#endif

/** This deals with Sbb messages (as against Clp messages etc).
    CoinMessageHandler.hpp is the general part of message handling.
    All it has are enum's for the various messages.
    SbbMessage.cpp has text in various languages.

    It is trivial to use the .hpp and .cpp file as a basis for
    messages for other components.
 */

#include "CoinMessageHandler.hpp"
enum SBB_Message
{
  SBB_END_GOOD,
  SBB_MAXNODES,
  SBB_MAXTIME,
  SBB_MAXSOLS,
  SBB_SOLUTION,
  SBB_END,
  SBB_INFEAS,
  SBB_STRONG,
  SBB_SOLINDIVIDUAL,
  SBB_INTEGERINCREMENT,
  SBB_STATUS,
  SBB_GAP,
  SBB_ROUNDING,
  SBB_ROOT,
  SBB_GENERATOR,
  SBB_BRANCH,
  SBB_STRONGSOL,
  SBB_NOINT,
  SBB_VUB_PASS,
  SBB_VUB_END,
  SBB_DUMMY_END
};

class SbbMessage : public CoinMessages {

public:

  /**@name Constructors etc */
  //@{
  /** Constructor */
  SbbMessage(Language language=us_en);
  //@}

};

#endif
