// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <limits.h>

#include <scenario/src/model.h>
#include <scenario/src/lastPrbS.h>
#include <scenario/src/zeroTvFS.h>

// This file contains the implementation of 
// class LgFrSubscription 

// Initialize class high water mark
unsigned long LgFrSubscription::highWaterMark_ = 0;


//--------------------------------------------------------------------------
//
// Implementation of LgFrSubscription model methods
//
//--------------------------------------------------------------------------

// addDependent - Adds the object pointed to by m to the list of
// dependents of self.
void
LgFrSubscription::addDependent(
   LgFrModelClient * mc )
{
   modelClientList_.push_back( mc );
   mc->addDependence(*this);
}

// addDependents
void
LgFrSubscription::addDependents(
   const LgFrModelClientList & modelClientList )
{
  for( LgFrModelClientList::const_iterator it = modelClientList.begin();
        it != modelClientList.end();
        it++ )
    addDependent( *it );
}



// removeDependent - Removes the object pointed to by m to the list of
// dependents of self.
void
LgFrSubscription::removeDependent(
   LgFrModelClient * mc )
{
  for( LgFrModelClientList::iterator it = modelClientList_.begin();
        it != modelClientList_.end();
        it++ )
  {
    if( *it == mc )  
    {
        modelClientList_.erase( it );
        mc->removeDependence(*this);
        break;
    }
  }

//   assert( modelClientList_.size() >= 0 );
//   modelClientList_.remove( mc );
//   mc->removeDependence(*this);
}

void
LgFrSubscription::removeDependentFromHeterogeneousList(
   LgFrModelClient * mc )
{
    removeDependent( mc ); // removeDependent now handles different object types

}

// removeDependents
void
LgFrSubscription::removeDependents(
   const LgFrModelClientList &  modelClientList )
{
  for( LgFrModelClientList::const_iterator it = modelClientList.begin();
        it != modelClientList.end();
        it++ )
    removeDependent( *it );
}


// numberDependents - Returns number of subscribers to this object
size_t
LgFrSubscription::numberDependents( )
const
{
   return modelClientList_.size();
}

// Get list of dependents
const LgFrModelClientList &
LgFrSubscription::dependents()
const
{
  return modelClientList_;
}

// externalChange - Inform all dependent objects that this object has changed.
// This is done by invoking the dependent objects updateFrom method.
void
LgFrSubscription::externalChange(
   void * pData )
{
  incrementHighWaterMark();
  waterMark_ = highWaterMark();
  numberOfTimesVisited_ = waterMark_;

  for( LgFrModelClientList::const_iterator it = modelClientList_.begin();
        it != modelClientList_.end();
        it++ )
  {
      LgFrModelClient* mc = *it;
      mc->bringUpToDateAndCheckIfObjectHasChanged(waterMark_, this, pData);
  }

}


void
LgFrSubscription::changed(
   void * pData )
{
//  cout <<  std::endl << "in LgFrSubscript::changed for object " << (unsigned long)this <<  std::endl;
//  cout << "this object has " << modelClientList_.size() << " children" <<  std::endl;
  for( LgFrModelClientList::const_iterator it = modelClientList_.begin();
        it != modelClientList_.end();
        it++ )
  {
      LgFrModelClient* mc = *it;
      mc->bringUpToDateAndCheckIfObjectHasChanged(waterMark_, this, pData);
  }
//  LgFrModelClient* mc;
//  RWOrderedIterator next(modelClientList_);
//  while ((mc = (LgFrModelClient*)next())!=0)
//    mc->bringUpToDateAndCheckIfObjectHasChanged(waterMark_, this, pData);
}

// updateFrom - do local update and send changed() to all of the
//  objects that subscribe to you
void
LgFrSubscription::updateFrom (
   LgFrModel * p,
   void * d)
{
  // Do a local update if neccessary
  localUpdate(p, d);
  // Inform all objects dependent on this object that there has been
  // a change in the data
  
  changed( 0 );
}


void
LgFrSubscription::localUpdate (
   LgFrModel * /* p */,
   void *      /* d */ )
{
  // the user should overload this method if anything of significance is being
  //   done
}



// Get instance water mark
unsigned long
LgFrSubscription::waterMark() const
{
   return waterMark_;
}

// Set instance water mark
void
LgFrSubscription::waterMark(unsigned long waterMark)
{
   waterMark_ = waterMark;
}

// Get class high water mark
unsigned long
LgFrSubscription::highWaterMark()
{
   return highWaterMark_;
}

// increment class high water mark
void
LgFrSubscription::incrementHighWaterMark()
{
   if( highWaterMark_ == ULONG_MAX ) {
       std::cerr <<"LgFrSubscription::incrementHighWaterMark()" << std::endl
            <<"  highWaterMark is at maximum allowed value" << std::endl;
       abort();
   }
   highWaterMark_++;
}

// Use the high water mark to update all objects that I depend on
//  (i.e. all objects that I subscribe to)
int
LgFrSubscription::bringUpToDateAndCheckIfObjectHasChanged(unsigned long highWaterMark,
                                  LgFrModel* model,
                                  void* pData)
{
  // smart algorithm (recursive)
  // visit object(s) I subscribe to
  //    numberOfTimesVisited_ = highWaterMark
  // if any objects I subscribe to have changed (waterMark_ == highWaterMark)  {
  //    updateFrom
  //    return 1;
  // }
  // else  {
  //    return 0;
  // }
  

//  cout <<  std::endl << "object address is " << (unsigned long)this <<  std::endl;
//  cout << "high water mark is " << highWaterMark << " and local water mark is " << waterMark_ <<  std::endl;
//  cout << "this object has " << modelList_.size() << " parents and " << modelClientList_.size() << " children" <<  std::endl;

  // if this object subscribes to no one we don't have to do anything
  int changed = 0;
  if ( (modelList_.size() != 0) && (highWaterMark > numberOfTimesVisited_) )  
  {
    int hasObjectSubscribedToChanged = 0;
    for( LgFrModelClientList::const_iterator it = modelList_.begin();
          it != modelList_.end();
          it++ )
    {
        LgFrModelClient* mc = *it;
        if( mc->bringUpToDateAndCheckIfObjectHasChanged(highWaterMark, model, pData) )
          hasObjectSubscribedToChanged++;
    }

    //RWOrderedIterator next(modelList_);
    //while ((mc = (LgFrModel*)next())!=0) 
    //  if (mc->bringUpToDateAndCheckIfObjectHasChanged(highWaterMark, model, pData))
    //    hasObjectSubscribedToChanged++;
    
    numberOfTimesVisited_ = highWaterMark;
    if ( (waterMark_ < highWaterMark) && hasObjectSubscribedToChanged ) 
    {
      waterMark_ = highWaterMark;
      updateFrom(model, pData);
      changed = 1;
    }
  }
  else  {
    // check for the case where the object is the one that originated the
    //   external change or where the object has already been visited.
    if (waterMark_ == highWaterMark)
      changed = 1;
  }
  return changed;
}

//--------------------------------------------------------------------------
//
// Implementation of LgFrSubscription cannonical methods
//
//--------------------------------------------------------------------------


// Assignment
LgFrSubscription&
LgFrSubscription::operator=
(const LgFrSubscription& rhs )
{

  // This could be tricky. Needs to be looked into since RWModel
  // does not have an assignment operator.

  if (this != &rhs) {           // Check for assignment to self
  }
  return *this;

}


// Default constructor
LgFrSubscription::LgFrSubscription(unsigned long waterMark)
:  
   waterMark_(waterMark),
   numberOfTimesVisited_(waterMark),
   modelList_(),
   modelClientList_()
    
{
  // nothing to do
}


//Copy constructor
LgFrSubscription::LgFrSubscription 
   (const LgFrSubscription& rhs)
:  
   waterMark_(rhs.waterMark_),
   numberOfTimesVisited_(rhs.numberOfTimesVisited_),
   modelList_(),
   modelClientList_()
{
  // nothing to do
}

// Destructor
LgFrSubscription::~LgFrSubscription()
{
  assert( numberDependents() == 0 );
}



//--------------------------------------------------------------------------
//
// Implementation of LgFrSubscription modelClient methods
//
//--------------------------------------------------------------------------


// deleteAndRemoveDependence - Removes the dependence this object 
// has on object m. If m has no other subscribers, then it is deleted.
void
LgFrSubscription::deleteAndRemoveDependence(
   LgFrModel * m )
{
   // Remove this objects subscription to m.
   m->removeDependent( this );

   // Is anything else subscribing to m?
   if ( m->numberDependents() == 0 ) {

      // Nothing else is dependent on m, so it no longer has a reason to live.
      delete m;

   }
   
}

// insert model into list of the modelClient depenencies
void
LgFrSubscription::addDependence(
   LgFrModel & m )
{
   modelList_.push_back( &m );
}

// remove model from list of the modelClient depenencies
void
LgFrSubscription::removeDependence(
   const LgFrModel & m )
{
  for( LgFrModelClientList::iterator it = modelList_.begin();
        it != modelList_.end();
        it++ )
  {
    if( *it == &m )  
    {
        modelList_.erase( it );
        break;
    }
  }
}


//--------------------------------------------------------------------------
//
// Implementation of of Test methods
//
//--------------------------------------------------------------------------


#ifdef NDEBUG
#undef NDEBUG
#endif

// test LgFrSubscription
void
LgFrSubscription::test()
{


  // Test default constructor
  LgFrZeroTimeVecFilterStrategy m;
  assert (m.dependents().size() == 0);
  assert (m.numberDependents() == 0);

  // Test copy constructor
  LgFrZeroTimeVecFilterStrategy mCopy(m);

  // Test assignment
  m = mCopy; 
  
  // Test addDependent
  LgFrZeroTimeVecFilterStrategy mc; 
  m.addDependent( &mc );
  assert (m.dependents().size() == 1);
  assert (m.numberDependents() == 1);
  m.addDependent( &mc );
  assert (m.dependents().size() == 2);
  assert (m.numberDependents() == 2);

  // Test changed
  m.externalChange( 0 );

  // Test remove dependents
  LgFrModelClientList mcl = m.dependents();
  m.removeDependents(mcl);
  assert (m.dependents().size() == 0);
  assert (m.numberDependents() == 0);

  // Test add dependents
  m.addDependents(mcl);
  assert (m.dependents().size() == 2);
  assert (m.numberDependents() == 2);

  // Test removeDependent
  m.removeDependent( &mc );
  assert (m.dependents().size() == 1);
  assert (m.numberDependents() == 1);
  m.removeDependent( &mc );
  assert (m.dependents().size() == 0);
  assert (m.numberDependents() == 0);


  {
  LgFrLastProblemSolved * m = new LgFrLastProblemSolved;

  // Test default constructor
  LgFrZeroTimeVecFilterStrategy mc;

  // Test copy constructor
  LgFrZeroTimeVecFilterStrategy mcCopy(mc); 

  // Test assignment
  mc = mcCopy; 
  
  // Test updateFrom by invoking model's changed method
  m->addDependent( &mc );
  m->externalChange( 0 );

  // test Remove dependence
  mc.deleteAndRemoveDependence( m );
  }

  
  // test out high water update code
  {

    //                 e
    //               /   \
    //              d     b
    //               \   / \
    //                 c    a
    //
    //  b subscribes to a and c
    //  e subscribes to b and d
    //  d subscribes to c
    //  we update a and want to see c, {b, d}, and e updated
    //  to prove this works, add some debug code in
    //    LgFrFilterStrategy::updateFrom
    
    
  LgFrZeroTimeVecFilterStrategy a;
  LgFrZeroTimeVecFilterStrategy b;
  LgFrZeroTimeVecFilterStrategy c;
  LgFrZeroTimeVecFilterStrategy d;
  LgFrZeroTimeVecFilterStrategy e;

/*  
  cout << "a is " << (unsigned long)&a <<  std::endl;
  cout << "b is " << (unsigned long)&b <<  std::endl;
  cout << "c is " << (unsigned long)&c <<  std::endl;
  cout << "d is " << (unsigned long)&d <<  std::endl;
  cout << "e is " << (unsigned long)&e <<  std::endl;
*/
  
  a.addDependent(&b);
  c.addDependent(&b);
  c.addDependent(&d);  
  b.addDependent(&e);
  d.addDependent(&e);
  
//  cout << "a changes (update path should be b->e)" <<  std::endl;  
  a.externalChange( 0 );
  
  // do an update at C
//  cout << "c changes (update path should be b->d->e)" <<  std::endl;    
  c.externalChange( 0 );

  // do an update at D
//  cout << "d changes (update path should be e)" <<  std::endl;      
  d.externalChange( 0 );
  
  b.removeDependent(&e);
  d.removeDependent(&e);
  c.removeDependent(&d);
  c.removeDependent(&b);
  a.removeDependent(&b);  
  }

  // test out high water update code
  {

    // to see this code really work uncomment the couts here
    //   and in LgFrFilterStrategy::localUpdate

    //                 e
    //                   \
    //              d     b
    //               \   / \
    //                 c    a
    //
    //  b subscribes to a and c
    //  e subscribes to b and d
    //  d subscribes to c
    //  we update a and want to see c, b, e and d updated
    //  to prove this works, add some debug code in    
    //    LgFrFilterStrategy::updateFrom
    
  LgFrZeroTimeVecFilterStrategy a;
  LgFrZeroTimeVecFilterStrategy b;
  LgFrZeroTimeVecFilterStrategy c;
  LgFrZeroTimeVecFilterStrategy d;
  LgFrZeroTimeVecFilterStrategy e;

/*
  cout << "a is " << (unsigned long)&a <<  std::endl;
  cout << "b is " << (unsigned long)&b <<  std::endl;
  cout << "c is " << (unsigned long)&c <<  std::endl;
  cout << "d is " << (unsigned long)&d <<  std::endl;
  cout << "e is " << (unsigned long)&e <<  std::endl;  
*/
  
  a.addDependent(&b);
  c.addDependent(&b);
  c.addDependent(&d);  
  b.addDependent(&e);
  
//  cout << "a changes (update path should be b->e)" <<  std::endl;      
  a.externalChange( 0 );

  b.removeDependent(&e);
  c.removeDependent(&d);
  c.removeDependent(&b);
  a.removeDependent(&b);  
  
  }
}
