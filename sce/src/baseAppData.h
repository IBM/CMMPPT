// file: baseAppData.h
// author: tom

// Base class for sce app data classes


#ifndef  LGFRSCEBASEAPPDATA_H
#define  LGFRSCEBASEAPPDATA_H

#include <stdlib.h>
#include <sce/src/sceDefin.h>
#include <assert.h>

class LgFrSceAppData
{
public:

virtual
LgFrSceAppData *
clone() const = 0;


  
// default constructor.
LgFrSceAppData(); 


// copy constructor
LgFrSceAppData(const LgFrSceAppData& source);

// assignment operator
LgFrSceAppData&
operator=(const LgFrSceAppData& rhs);

// destructor
virtual
~LgFrSceAppData();

};


#endif





