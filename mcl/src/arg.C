//==============================================================================
// Message Class Library
//
// (C) Copyright IBM Corp. 1997, 2005  All Rights Reserved
//==============================================================================

//-----------------------------------------------------------------------------
// File arg.C
//
// Contains the declaration and implementation of the following classes:
//
// ArgValueType
//
// Contains the implementation of the following classes:
//
// Arg
//    BoolArg
//    IntArg
//    FloatArg
//    DoubleArg
//    StringArg
//    MsgFragArg
//    VectorArg
//       BoolVectorArg
//       IntVectorArg
//       FloatVectorArg
//       DoubleVectorArg
//       StringVectorArg
//
// ArgList
//-----------------------------------------------------------------------------

#include <mcl/src/arg.h>
#include <mcl/src/facility.h>
#include <mcl/src/msg.h>

#include <vector>
#include <iostream>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//-----------------------------------------------------------------------------
// class ArgValueType.
//
// Responsible for some of the behavior of an Arg associated with the type
// of its value, where "value" refers to the scalar value associated with a
// scalar Arg or the scalar value of a single element of a VectorArg.
//-----------------------------------------------------------------------------

class MclArgValueType
   {
   public:

      //-----------------------------------------------------------------------
      // Access functions
      //-----------------------------------------------------------------------

      inline const std::string & name () const
         {
         return name_;
         }

      inline const std::string & validFlags () const
         {
         return validFlags_;
         }

      inline const std::string & testFormat () const
         {
         return testFormat_;
         }

      //-----------------------------------------------------------------------
      // Each of these functions returns the ArgValueType for the
      // appropriate type.
      //-----------------------------------------------------------------------

      inline static const MclArgValueType & boolType ()
         {
         reqSetUpDone ();

         return * boolTypePtr_;
         }

      inline static const MclArgValueType & intType ()
         {
         reqSetUpDone ();

         return * intTypePtr_;
         }

      inline static const MclArgValueType & floatType ()
         {
         reqSetUpDone ();

         return * floatTypePtr_;
         }

      inline static const MclArgValueType & doubleType ()
         {
         reqSetUpDone ();

         return * doubleTypePtr_;
         }

      inline static const MclArgValueType & stringType ()
         {
         reqSetUpDone ();

         return * stringTypePtr_;
         }

      inline static const MclArgValueType & msgFragType ()
         {
         reqSetUpDone ();

         return * msgFragTypePtr_;
         }

      //-----------------------------------------------------------------------

      static void setUpClass ();
         //
         // Does initial set up of this class the first time it is called
         // during the program process.

   private:

      MclArgValueType (
            const char * nameVal,
            const char * validFlagsVal,
            const char * testFormatVal);
         //
         // Constructor
         //
         // name_       <-- nameVal
         // validFlags_ <-- validFlagsVal
         // testFormat_ <-- testFormatVal

      ~MclArgValueType ();
         //
         // Destructor
         // It is not implemented, because the instances of this class are never
         // destructed.

      MclArgValueType             (const MclArgValueType &);
      MclArgValueType & operator= (const MclArgValueType &);
         //
         // Copy ctor and assignment are declared private and not implemented
         // so as to prevent inadvertant copying and assignment.

      inline static void reqSetUpDone ()
         {
         if (! setUpDone_)
            setUpNotDone ();
         }

      static void setUpNotDone ();
         //
         // Issues a client error indicating that a MclArg class constructor
         // was called when the Facility has not yet been called.

      //-----------------------------------------------------------------------
      // Data.
      //-----------------------------------------------------------------------

      const std::string name_;
         //
         // Text identifying this ArgValueType.

      const std::string validFlags_;
         //
         // The list of valid output format flags for Args that have this
         // ArgValueType.

      const std::string testFormat_;
         //
         // The format to be used for testing purposes.

      static const MclArgValueType *    boolTypePtr_;
      static const MclArgValueType *     intTypePtr_;
      static const MclArgValueType *   floatTypePtr_;
      static const MclArgValueType *  doubleTypePtr_;
      static const MclArgValueType *  stringTypePtr_;
      static const MclArgValueType * msgFragTypePtr_;
         //
         // Type behavior for Args whose values are ints, floats, strings, or
         // MsgFrags.
         // These are the only instances of this class that ever exist.
         // They are constructed by the first call to setUpClass () and never
         // destructed.

      static bool setUpDone_;
         //
         // True, iff setUpClass () has been called at least once during the
         // program process.
   };

//---------------------------------------------------------------------------
// Implementation of class MclArgValueType
//---------------------------------------------------------------------------

const MclArgValueType * MclArgValueType::   boolTypePtr_ = NULL;
const MclArgValueType * MclArgValueType::    intTypePtr_ = NULL;
const MclArgValueType * MclArgValueType::  floatTypePtr_ = NULL;
const MclArgValueType * MclArgValueType:: doubleTypePtr_ = NULL;
const MclArgValueType * MclArgValueType:: stringTypePtr_ = NULL;
const MclArgValueType * MclArgValueType::msgFragTypePtr_ = NULL;

bool                    MclArgValueType::     setUpDone_ = false;

//-----------------------------------------------------------------------------

void MclArgValueType::setUpClass ()
   {
   if (setUpDone_)
      return;

      boolTypePtr_ = new MclArgValueType ("bool",               "b",  "%b");
       intTypePtr_ = new MclArgValueType ("int",                "db", "%d");
     floatTypePtr_ = new MclArgValueType ("float",              "fg", "%g");
    doubleTypePtr_ = new MclArgValueType ("double",             "fg", "%.14g");
    stringTypePtr_ = new MclArgValueType ("const char *",       "s",  "%s");
   msgFragTypePtr_ = new MclArgValueType ("const MclMsgFrag *", "ms", "%m");

   setUpDone_      = true;
   }

//-----------------------------------------------------------------------------

MclArgValueType::MclArgValueType (
         const char * nameVal,
         const char * validFlagsVal,
         const char * testFormatVal):

      name_       (nameVal),
      validFlags_ (validFlagsVal),
      testFormat_ (testFormatVal)
   {
   }

//-----------------------------------------------------------------------------

void MclArgValueType::setUpNotDone ()
   {
   std::cerr <<
      "\n\n"
      "MCL CLIENT ERROR:\n\n"
      "   A constructor for an MclArg class was called when \n"
      "   the MclFacility constructor had not yet been called.\n";

   MclFacility::abortMcl ();
   }

//-----------------------------------------------------------------------------
//
// MclArg Implementation
//
//-----------------------------------------------------------------------------

MclArg::MclArg (const MclArgValueType & theValueType):
   myValueType_ (theValueType)
{
}

MclArg::~MclArg ()
   {
   }

void MclArg::format (
      std::string &       formattedArg,
      const std::string & theFormat,
      MclBoolean          reqVector,
      int                 argNum,
      const MclMsg *      theMsg)
      const
   {
   char theFlag;

   if (isaVector () != reqVector)
      vectornessError (reqVector, argNum, theMsg);

   theFlag = formatFlag (theFormat);

   if (myValueType_.validFlags ().find (theFlag) == std::string::npos)
      flagError (theFlag, argNum, theMsg);

   formatValidArg (formattedArg, theFormat, theMsg);
   }

void MclArg::getValueText (std::string & valueText, const MclMsg * theMsg) const
   {
   formatValidArg (valueText, myValueType_.testFormat (), theMsg);
   }

void MclArg::setUpClass ()
   {
   MclArgValueType::setUpClass ();
   }

MclArg::MclArg (const MclArg & theArg):
      myValueType_ (theArg.myValueType_)
   {
   }

void MclArg::getTypeName (std::string & theName) const
   {
   theName = myValueType_.name ();
   }

MclBoolean MclArg::isaVector () const
   {
   return FALSE;
   }

void MclArg::formatValidData (
      std::string &       formattedData,
      const std::string & theFormat,
      int                 theData,
      const MclFacility & theFacility)
   {
   if (formatFlag (theFormat) == 'b')
      {
      std::string stringFormat = theFormat;

      formatFlag (stringFormat) = 's';

      formatValidData (
         formattedData,
         stringFormat,
         theFacility.booleanText (theData));
      }
   else
      {
        char buffer[200];
   
        sprintf (buffer, theFormat.c_str(), theData);

        formattedData = buffer;
      }
   }

void MclArg::formatValidData (
      std::string &       formattedData,
      const std::string & theFormat,
      float               theData)
   {
   char buffer[100];

   sprintf (buffer, theFormat.c_str (), theData);

   formattedData = buffer;
   }

void MclArg::formatValidData (
      std::string &       formattedData,
      const std::string & theFormat,
      double              theData)
   {
   char buffer[100];

   sprintf (buffer, theFormat.c_str (), theData);

   formattedData = buffer;
   }

void MclArg::formatValidData (
      std::string &       formattedData,
      const std::string & theFormat,
      const char *        theData)
   {
     int stringLength;
     stringLength = strlen(theData);
     int width = getWidth(theFormat);
     if (stringLength < width)
       stringLength = width;
     char * buffer = new char[stringLength + 1];
     sprintf (buffer, theFormat.c_str(), theData);
     formattedData = buffer;
     delete [] buffer;
   }

char & MclArg::formatFlag (std::string & theFormat)
   {
   return theFormat[theFormat.length () - 1];
   }

char MclArg::formatFlag (const std::string & theFormat)
   {
   return theFormat[theFormat.length () - 1];
   }

void MclArg::vectornessError (
      MclBoolean     reqVector,
      int            argNum,
      const MclMsg * theMsg)
      const
   {
   std::string typeName;
   std::string valueText;

   getTypeName  (typeName);
   getValueText (valueText, theMsg);

   std::cerr <<
      "\n\n"
      "MCL CLIENT ERROR:\n\n"
      "   A " << (reqVector? "non-": "") <<
         "vector was passed as the argument corresponding to\n"
      "   a " << (reqVector? "": "non-") <<
         "vector argument given in the format string of a message.\n\n"
      "   Message ID:              \"" << theMsg->id () << "\"\n"
      "   Argument #:              "   << argNum        << "\n"
      "   Type of argument passed: "   << typeName.c_str()  << "\n"
      "   Argument value:          "   << valueText.c_str()     << "\n";

   MclFacility::abortMcl ();
   }

void MclArg::flagError (
      char           formatFlag,
      int            argNum,
      const MclMsg * theMsg)
      const
   {
   std::string validFlags = myValueType_.validFlags ();
   size_t    theIndex;
   std::string typeName;
   std::string valueText;

   getTypeName  (typeName);
   getValueText (valueText, theMsg);

   std::cerr <<
      "\n\n"
      "MCL CLIENT ERROR:\n\n"
      "   The type of an argument passed to a message does not match\n"
      "   the format flag given in the format string.\n\n"
      "   Message ID:              \"" << theMsg->id () << "\"\n"
      "   Argument #:              "   << argNum        << "\n"
      "   Format flag:             '"  << formatFlag    << "'\n"
      "   Type of argument passed: "   << typeName.c_str() << "\n"
      "   Argument value:          "   << valueText.c_str()     << "\n"
      "   The valid format flags for this type of argument are:";

   for (theIndex = 0; theIndex < validFlags.length (); ++ theIndex) 
      std::cerr << " '"  << validFlags.substr (theIndex, 1).c_str()  << "'";

   std::cerr << "\n";

   MclFacility::abortMcl ();
   }

int MclArg::getWidth(const std::string & format)
{
	size_t i = format.find('%');
	if( i < format.length() )
	{
		std::string widthStr;
		if( format[++i] == '-' )
			i++; // if a '-' was following the %, skip it;
		while( isdigit( format[i] ) )
			widthStr += format[i++];
		int length = atoi(widthStr.c_str());
		return length;
	}
	return -1;
  
}

//-----------------------------------------------------------------------------
//
// MclBoolArg Implementation
//
//-----------------------------------------------------------------------------

MclBoolArg::MclBoolArg(bool theBool):
  MclArg(MclArgValueType::boolType ()),
  myBool_    (theBool)
{
}

MclBoolArg::MclBoolArg(const MclBoolArg&  theBoolArg):
  MclArg(theBoolArg),
  myBool_    (theBoolArg.myBool_)
{
}

void MclBoolArg::formatValidArg (
      std::string &       formattedArg,
      const std::string & theFormat,
      const MclMsg *      theMsg)
      const
   {
   formatValidData (formattedArg, theFormat, myBool_, theMsg->myFacility ());
   }

//-----------------------------------------------------------------------------
//
// MclIntArg Implementation
//
//-----------------------------------------------------------------------------

MclIntArg::MclIntArg(int i):
  MclArg(MclArgValueType::intType ()),
  i_    (i)
{
}

MclIntArg::MclIntArg(const MclIntArg&  i):
  MclArg(i),
  i_    (i.i_)
{
}

void MclIntArg::formatValidArg (
      std::string &       formattedArg,
      const std::string & theFormat,
      const MclMsg *      theMsg)
      const
   {
   formatValidData (formattedArg, theFormat, i_, theMsg->myFacility ());
   }

//-----------------------------------------------------------------------------
//
// MclFloatArg Implementation
//
//-----------------------------------------------------------------------------

MclFloatArg::MclFloatArg(float f):
  MclArg(MclArgValueType::floatType ()),
  f_    (f)
{
}

MclFloatArg::MclFloatArg(const MclFloatArg& f):
  MclArg(f),
  f_ (f.f_)
{
}

void MclFloatArg::formatValidArg (
      std::string &       formattedArg,
      const std::string & theFormat,
      const MclMsg *)
      const
   {
   formatValidData (formattedArg, theFormat, f_);
   }

//-----------------------------------------------------------------------------
//
// MclDoubleArg Implementation
//
//-----------------------------------------------------------------------------

MclDoubleArg::MclDoubleArg (double theDouble):

      MclArg    (MclArgValueType::doubleType ()),
      myDouble_ (theDouble)
   {
   }

//-----------------------------------------------------------------------------

MclDoubleArg::~MclDoubleArg ()
   {
   }

//-----------------------------------------------------------------------------

MclArg * MclDoubleArg::clone () const
   {
   return new MclDoubleArg (* this);
   }

//-----------------------------------------------------------------------------

MclDoubleArg::MclDoubleArg (const MclDoubleArg & theDblArg):

      MclArg    (theDblArg),
      myDouble_ (theDblArg.myDouble_)
   {
   }

//-----------------------------------------------------------------------------

void MclDoubleArg::formatValidArg (
      std::string &       formattedArg,
      const std::string & theFormat,
      const MclMsg *)
      const
   {
   formatValidData (formattedArg, theFormat, myDouble_);
   }

//-----------------------------------------------------------------------------
//
// MclStringArg Implementation
//
//-----------------------------------------------------------------------------

MclStringArg::MclStringArg(const char * s):
  MclArg(MclArgValueType::stringType ()),
   string_(s)
{
MclFacility::forbidNullArg (s, "s", "MclStringArg::MclStringArg");
}

MclArg* MclStringArg::clone()const
{
  return new MclStringArg(*this);
}

MclStringArg::~MclStringArg()
{
  // Nothing to do here
}

MclStringArg::MclStringArg(const MclStringArg & source)
  :MclArg(source),
   string_(source.string_)
{
  // Nothing to do here
}

void MclStringArg::formatValidArg (
      std::string &       formattedArg,
      const std::string & theFormat,
      const MclMsg *)
      const
   {
   formatValidData (formattedArg, theFormat, string_);
   }

//-----------------------------------------------------------------------------
//
// MclMsgFragArg Implementation
//
//-----------------------------------------------------------------------------

MclMsgFragArg::MclMsgFragArg (const MclMsgFrag * theFrag):
      MclArg  (MclArgValueType::msgFragType ()),
      myFrag_ (theFrag)
   {
   MclFacility::forbidNullArg (
      theFrag,
      "theFrag",
      "MclMsgFragArg::MclMsgFragArg");
   }

//-----------------------------------------------------------------------------

MclMsgFragArg::~MclMsgFragArg ()
   {
   }

//-----------------------------------------------------------------------------

MclArg * MclMsgFragArg::clone () const
   {
   return new MclMsgFragArg (*this);
   }

//-----------------------------------------------------------------------------

MclMsgFragArg::MclMsgFragArg (const MclMsgFragArg & source):
      MclArg  (source),
      myFrag_ (source.myFrag_)
   {
   }

//-----------------------------------------------------------------------------

void MclMsgFragArg::formatValidArg (
      std::string &       formattedArg,
      const std::string & theFormat,
      const MclMsg *)
      const
   {
   std::string finalFormat = theFormat;

   if (formatFlag (finalFormat) == 'm')
       formatFlag (finalFormat) =  's';

   formatValidData (formattedArg, finalFormat, myFrag_->text ());
   }

//-----------------------------------------------------------------------------
//
// MclVectorArg Implementation
//
//-----------------------------------------------------------------------------

MclVectorArg::MclVectorArg (int l, const MclArgValueType & theValueType):

   MclArg (theValueType),
   length_(l)
{
  // do nothing
}

MclVectorArg::~MclVectorArg()
{
}

MclVectorArg::MclVectorArg(const MclVectorArg& v):
   MclArg(v),
   length_(v.length_)
{
  // do nothing
}

void MclVectorArg::getTypeName (std::string & theName) const
   {
   MclArg::getTypeName (theName);

   theName += " Vector";
   }

MclBoolean MclVectorArg::isaVector () const
   {
   return TRUE;
   }

void MclVectorArg::formatValidArg (
      std::string &       formattedArg,
      const std::string & theFormat,
      const MclMsg *      theMsg)
      const
   {
   std::string formattedElem;
   size_t       i;
   size_t       charsAvail;
   size_t       charsAvailPerLine;
   std::string newLineString ("\n");

   newLineString += std::string (theMsg->vectorIndent(), ' ');

   charsAvailPerLine =
      theMsg->myFacility ().lineLength () - theMsg->vectorIndent ();

   if (theMsg->displayExternalNumber ())
       charsAvailPerLine -= 9;

   formattedArg = "";

   charsAvail = 0;

   for (i = 0; i < length_; ++ i)
      {
      formatElem (formattedElem, theFormat, i, theMsg->myFacility ());

      if (charsAvail < formattedElem.length () + 1)
         {
         formattedArg += newLineString;
         charsAvail    = charsAvailPerLine;
         }
      else
         {
         formattedArg += " ";
         charsAvail   --;
         }

      formattedArg += formattedElem.c_str();
      charsAvail   -= formattedElem.length ();
      }
   }

//-----------------------------------------------------------------------------
//
// MclBoolVectorArg Implementation
//
//-----------------------------------------------------------------------------

MclBoolVectorArg::MclBoolVectorArg(const bool* v, int l):
   MclVectorArg(l, MclArgValueType::boolType ()),
   boolVec_     (v)
{
if (l > 0)
   MclFacility::forbidNullArg (v, "v", "MclBoolVectorArg::MclBoolVectorArg");
}

MclBoolVectorArg::~MclBoolVectorArg()
{
}


MclArg * MclBoolVectorArg::clone() const
{
  MclArg * newInst = new MclBoolVectorArg(*this);
  return newInst;
}

MclBoolVectorArg::MclBoolVectorArg(const MclBoolVectorArg& theBoolVec):
   MclVectorArg (theBoolVec),
   boolVec_      (theBoolVec.boolVec_)
{
}

void MclBoolVectorArg::formatElem (
      std::string &       formattedElem,
      const std::string & theFormat,
      int                 theIndex,
      const MclFacility & theFacility)
      const
   {
   formatValidData (formattedElem, theFormat, boolVec_[theIndex], theFacility);
   }

//-----------------------------------------------------------------------------
//
// MclIntVectorArg Implementation
//
//-----------------------------------------------------------------------------

MclIntVectorArg::MclIntVectorArg(const int* v, int l):
   MclVectorArg(l, MclArgValueType::intType ()),
   intVec_     (v)
{
if (l > 0)
   MclFacility::forbidNullArg (v, "v", "MclIntVectorArg::MclIntVectorArg");
}

MclIntVectorArg::~MclIntVectorArg()
{
}


MclArg * MclIntVectorArg::clone() const
{
  MclArg * newInst = new MclIntVectorArg(*this);
  return newInst;
}

MclIntVectorArg::MclIntVectorArg(const MclIntVectorArg& iv):
   MclVectorArg (iv),
   intVec_      (iv.intVec_)
{
}

void MclIntVectorArg::formatElem (
      std::string &       formattedElem,
      const std::string & theFormat,
      int                 theIndex,
      const MclFacility & theFacility)
      const
   {
   formatValidData (formattedElem, theFormat, intVec_[theIndex], theFacility);
   }

//-----------------------------------------------------------------------------
//
// MclFloatVectorArg Implementation
//
//-----------------------------------------------------------------------------

MclFloatVectorArg::MclFloatVectorArg(const float* v, int l):
   MclVectorArg(l, MclArgValueType::floatType ()),
   floatVec_     (v)
{
if (l > 0)
   MclFacility::forbidNullArg (v, "v", "MclFloatVectorArg::MclFloatVectorArg");
}

MclFloatVectorArg::~MclFloatVectorArg()
{
}


MclArg * MclFloatVectorArg::clone() const
{
  MclArg * newInst = new MclFloatVectorArg(*this);
  return newInst;
}

MclFloatVectorArg::MclFloatVectorArg(const MclFloatVectorArg& iv):
   MclVectorArg (iv),
   floatVec_      (iv.floatVec_)
{
}

void MclFloatVectorArg::formatElem (
      std::string &       formattedElem,
      const std::string & theFormat,
      int                 theIndex,
      const MclFacility &)
      const
   {
   formatValidData (formattedElem, theFormat, floatVec_[theIndex]);
   }

//-----------------------------------------------------------------------------
//
// MclDoubleVectorArg Implementation
//
//-----------------------------------------------------------------------------

MclDoubleVectorArg::MclDoubleVectorArg (const double * theCVec, int lengthArg):

      MclVectorArg (lengthArg, MclArgValueType::doubleType ()),
      doubleVec_   (theCVec)
   {
   if (lengthArg > 0)
      MclFacility::forbidNullArg (
         theCVec,
         "theCVec",
         "MclDoubleVectorArg::MclDoubleVectorArg");
   }

//-----------------------------------------------------------------------------

MclDoubleVectorArg::~MclDoubleVectorArg ()
   {
   }

//-----------------------------------------------------------------------------

MclArg * MclDoubleVectorArg::clone () const
   {
   return (new MclDoubleVectorArg (* this));
   }

//-----------------------------------------------------------------------------

MclDoubleVectorArg::MclDoubleVectorArg (
         const MclDoubleVectorArg & theDblVecArg):

      MclVectorArg (theDblVecArg),
      doubleVec_   (theDblVecArg.doubleVec_)
   {
   }

//-----------------------------------------------------------------------------

void MclDoubleVectorArg::formatElem (
      std::string &       formattedElem,
      const std::string & theFormat,
      int                 theIndex,
      const MclFacility &)
      const
   {
   formatValidData (formattedElem, theFormat, doubleVec_[theIndex]);
   }

//-----------------------------------------------------------------------------
//
// MclStringVectorArg Implementation
//
//-----------------------------------------------------------------------------

MclStringVectorArg::MclStringVectorArg(const char* const * v, int l):
      MclVectorArg(l, MclArgValueType::stringType ()),
      strVec_     (v)
   {
   int i;

   if (l > 0)
      MclFacility::forbidNullArg (
         v,
         "v",
         "MclStringVectorArg::MclStringVectorArg");

   for (i = 0; i < l; ++ i)
      MclFacility::forbidNullArg (
         v[i],
         "v[i]",
         "MclStringVectorArg::MclStringVectorArg");
   }

MclStringVectorArg::~MclStringVectorArg()
{
}


MclArg * MclStringVectorArg::clone() const
{
  MclArg * newInst = new MclStringVectorArg(*this);
  return newInst;
}

MclStringVectorArg::MclStringVectorArg(const MclStringVectorArg& sv):
   MclVectorArg (sv),
   strVec_      (sv.strVec_)
{
}

void MclStringVectorArg::formatElem (
      std::string &       formattedElem,
      const std::string & theFormat,
      int                 theIndex,
      const MclFacility &)
      const
   {
   formatValidData (formattedElem, theFormat, strVec_[theIndex]);
   }

//---------------------------------------------------------------------------
// Implementation of MclMakeVec functions.
//---------------------------------------------------------------------------

MclBoolVectorArg MclMakeVec (
      const bool * theBoolVector,
      int          length)
   {
   return MclBoolVectorArg (theBoolVector, length);
   }

MclIntVectorArg MclMakeVec (
      const int * theIntVector,
      int         length)
   {
   return MclIntVectorArg (theIntVector, length);
   }

MclFloatVectorArg MclMakeVec (
      const float * theFloatVector,
      int           length)
   {
   return MclFloatVectorArg (theFloatVector, length);
   }

MclStringVectorArg MclMakeVec (
      const char * const * theStringVector,
      int                  length)
   {
   return MclStringVectorArg (theStringVector, length);
   }

//---------------------------------------------------------------------------
//
// MclArgList Implementation
//
//---------------------------------------------------------------------------

// Default constructor
MclArgList::MclArgList()
{
  //Nothing to do here
}

// Destructor
MclArgList::~MclArgList()
{
  for ( size_t i=0; i<list_.size(); i++ ) delete list_[i];
}

// operator[]
const MclArg &
MclArgList::operator[]( size_t i )
const
{
  return *(list_[i-1]);
}

size_t MclArgList::length () const
   {
   return list_.size ();
   }

// operator <<

MclArgList & MclArgList::operator << (const MclArg & theArg)
   {
   MclArg * theCopy = theArg.clone ();

   list_.push_back (theCopy);

   return * this;
   }

MclArgList & MclArgList::operator << (bool theBool)
   {
   MclBoolArg theBoolArg (theBool);

   return (* this) << theBoolArg;
   }

MclArgList & MclArgList::operator << (int theInt)
   {
   MclIntArg theIntArg (theInt);

   return (* this) << theIntArg;
   }

MclArgList & MclArgList::operator << (float theFloat)
   {
   MclFloatArg theFloatArg (theFloat);

   return (* this) << theFloatArg;
   }

MclArgList & MclArgList::operator << (const char * theCstring)
   {
   MclStringArg theStringArg (theCstring);

   return (* this) << theStringArg;
   }

MclArgList & MclArgList::operator << (const MclMsgFrag * theFrag)
   {
   MclMsgFragArg theMsgFragArg (theFrag);

   return (* this) << theMsgFragArg;
   }

MclArgList & MclArgList::operator << (const std::string & theString)
   {
   MclStringArg theStringArg (theString.c_str ());

   return (* this) << theStringArg;
   }
