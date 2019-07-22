// Copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef SbbCountRowCut_H
#define SbbCountRowCut_H


class OsiCuts;
class OsiRowCut;
class SbbNodeInfo;

//#############################################################################
// This is just like OsiRowCut but has a count of number nodes using it
class SbbCountRowCut : public OsiRowCut {

public:
  
  /**@name Constructors and destructors */
  //@{

    /// Copy constructor from OsiRowCut
    SbbCountRowCut ( const OsiRowCut &);  

    /// Copy constructor from OsiRowCut and info
    SbbCountRowCut ( const OsiRowCut &, SbbNodeInfo *, int whichOne,
		     int whichGenerator=-1);  

    /// Default Constructor 
    SbbCountRowCut ();
  
    /// Destructor 
    virtual ~SbbCountRowCut ();
  //@}
  /// Increment number of references
  void increment(int change=1);

  /// Set information
  void setInfo(SbbNodeInfo *, int whichOne);

  /// Decrement number of references and return number left
  int decrement(int change=1);

  /// Number of other SbbNodeInfos pointing to this
  inline int numberPointingToThis()
  { return numberPointingToThis_;};
#ifdef CHECK_CUT_COUNTS
  // Just for printing sanity checks
  int tempNumber_;
#endif
  /// Which generator for cuts - as user order
   int whichCutGenerator() const
  { return whichCutGenerator_;};
private:
  /// Illegal Copy constructor 
  SbbCountRowCut(const SbbCountRowCut &);
  
  /// Illegal Assignment operator 
  SbbCountRowCut & operator=(const SbbCountRowCut& rhs);

  /// Backward pointer to owner
  SbbNodeInfo * owner_;
  
  /// Backward pointer to cut in owner
  int ownerCut_;
  
  /// Number of other SbbNodeInfos pointing to this
  int numberPointingToThis_;
  /// Which generator created this cut
  int whichCutGenerator_;

};

#endif
