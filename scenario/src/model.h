#ifndef LGFR_SUBSCRIPTION_H
#define LGFR_SUBSCRIPTION_H
// RW2STL -- inserted:
#include <vector>
// End of RW2STL -- inserted includes.


// This file contains classe LgFrSubscription.
// This class is an enhancement to and combination of
// RWModel and LgFrModelClient.

// Forward references
class LgFrSubscription;

typedef LgFrSubscription LgFrModelClient;
typedef std::vector<LgFrModelClient*> LgFrModelClientList;

typedef LgFrSubscription LgFrModel;
typedef std::vector<LgFrModel*> LgFrModelList;


//--------------------------------------------------------------------------
//
// Implementation of LgFrSubscription
//
//--------------------------------------------------------------------------


class LgFrSubscription //: 
   //public SCECollectable
{

public:

  //-------------------------------------------------------------------
  //
  //  model methods 
  //
  //-------------------------------------------------------------------

  // addDependent - Adds the object pointed to by m to the list of
  // dependents of self.
  // Add mc as a client of this model.
  virtual
     void
        addDependent( 
           LgFrModelClient * mc ); 

  // Add zero or more dependents
  virtual
     void
        addDependents( 
           const LgFrModelClientList & modelClients ); 

  // removeDependent - Removes the object pointed to by m from the list of
  // dependents of self.
  // Remove mc as a client of this model.
  virtual
     void
        removeDependent( 
           LgFrModelClient * mc ); 

  virtual
     void
        removeDependentFromHeterogeneousList( 
           LgFrModelClient * mc ); 

  // Remove zero or more dependents
  virtual
     void
        removeDependents( 
           const LgFrModelClientList & modelClients ); 

  // numberDependents - Returns number of subscribers (clients) to this object
  virtual
     size_t
        numberDependents( )
           const;

  // Get current list of dependents
  virtual
     const LgFrModelClientList &
        dependents()
           const;

  // externalChanged - This method should be called when an object is changed
  // by an "external influence".  Many objects are changed when their updateFrom
  // methods are called.  This method should be called to tell an object it
  // has changed by some event that does not cause the object's updateFrom method
  // to be called.  This method informs all dependent objects that this object 
  // has changed.  This is done by invoking the dependent objects updateFrom method.
  virtual
     void
        externalChange(
           void * d );

  // changed - Inform all dependent objects that this object has changed.
  // This is done by invoking the dependent objects updateFrom method.
  virtual
     void
        changed(
           void * d );
  
  // Get class high water mark
  static
     unsigned long
        highWaterMark();

  // Get instance water mark
  unsigned long
     waterMark() const;


  // Set instance water mark - users should not use this
  void
     waterMark(unsigned long);


  // make all the objects I depend on as up to date as I am        
  virtual
     int
        bringUpToDateAndCheckIfObjectHasChanged(unsigned long highWaterMark,
                        LgFrModel* model, void* pData);
        
  //-------------------------------------------------------------------
  //
  //  model client methods 
  //
  //-------------------------------------------------------------------


  // removeDependence - Removes the dependence this object has on object m.
  // This object has a subscription to object m. This method removes the
  // subscription.
  // Additionally, this method deletes object m if nothing else is
  // dependent on it (ie. nothing else subscribes to it).
  // m must be allocated on the heap, since it may be deleted.
  virtual
     void
        deleteAndRemoveDependence(
           LgFrModel * m );

  // updateFrom - This method is invoked when the independent object
  // p has changed.
  virtual
     void
        updateFrom(
           LgFrModel * p,
                   void *d );


  virtual
     void
        localUpdate(
           LgFrModel * p,
                   void *d );
                 
  //-------------------------------------------------------------------
  //
  //  cannonical form methods 
  //
  //-------------------------------------------------------------------


  // Assignment 
  LgFrSubscription& 
    operator=(const LgFrSubscription& rhs);

  // Constructor.
   LgFrSubscription(unsigned long waterMark = 0);

  // Copy constructor
  LgFrSubscription (
     const LgFrSubscription& source);
  
  // destructor
  ~LgFrSubscription();

  // test this class
  static void test();

private:

  // global high water mark for all classes
  static unsigned long highWaterMark_;

  // high water mark for this object
  unsigned long waterMark_;   

  // used for marking object when it has been visited
  unsigned long numberOfTimesVisited_;
  
  // List of Model* that this object depends on
  LgFrModelList modelList_;

  // List of ModelClients* that depend on this object
  LgFrModelClientList modelClientList_;

  // increment global high water mark
  static
     void
        incrementHighWaterMark();
  
  // Insert "Subscribe to" model into the list of models this object
  // subcribes to.  
  virtual
     void
        addDependence( LgFrModel & );

  // Remove model from the list of models this object
  // subcribes to.
  virtual
     void
        removeDependence( const LgFrModel & );


};


#endif
