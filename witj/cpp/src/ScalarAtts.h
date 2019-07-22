//------------------------------------------------------------------------------
// WIT-J C++ Header File ScalarAtts.h.
//
// Contains the declarations of the following class templates:
//
//      ProbScalarAtt <Value>
//      PartScalarAtt <Value>
//    DemandScalarAtt <Value>
//       OpnScalarAtt <Value>
//    BomEntScalarAtt <Value>
//       SubScalarAtt <Value>
//    BopEntScalarAtt <Value>
//
// Contains the declaration of the following class:
//
//      ProbScalarAttString
//------------------------------------------------------------------------------

#ifndef ScalarAtts_h
#define ScalarAtts_h

#include <Att.h>

#include <wit.h>

//------------------------------------------------------------------------------
// Class template ProbScalarAtt <Value>
//
// Represents an attribute of scalar type Value associated with the whole
// Problem.
//
// Class Hierarchy:
//
// JavaAccObj
//    Att
//       ProbScalarAtt <Value>
//
// The implementation is in ScalarAtts.C.
//------------------------------------------------------------------------------

namespace WitJ
{
template <typename Value>
      class ProbScalarAtt: public Att
   {
   //---------------------------------------------------------------------------
   // Local typedefs.
   //
   // SetFuncPtr:
   //    A pointer to a WIT API function that sets      the value of an
   //    attribute of scalar type Value associated with the whole Problem
   //
   // GetFuncPtr:
   //    A pointer to a WIT API function that retrieves the value of an
   //    attribute of scalar type Value associated with the whole Problem.
   //---------------------------------------------------------------------------

   typedef witReturnCode (* SetFuncPtr) (WitRun * const, const Value);
   typedef witReturnCode (* GetFuncPtr) (WitRun * const,       Value *);

   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit ProbScalarAtt (
         AttBldr *    theAttBldr,
         const char * theName,
         GetFuncPtr   theGetFuncPtr,
         SetFuncPtr   theSetFuncPtr = NULL);
      //
      // Constructor.

   virtual ~ProbScalarAtt ();
      //
      // Destructor.

   virtual void set              (Component *, Value);
   virtual void get              (Component *, Value &);
   virtual bool isModifiable     ();
   virtual bool valueTypeMatches (const std::string &);
      //
      // Overrides from class Att.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   ProbScalarAtt              (const ProbScalarAtt &);
   ProbScalarAtt & operator = (const ProbScalarAtt &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   const GetFuncPtr myGetFuncPtr_;
      //
      // A pointer to the WIT API function that retrieves the value of the
      // attribute represented by this ProbScalarAtt.

   const SetFuncPtr mySetFuncPtr_;
      //
      // A pointer to the WIT API function that sets      the value of the
      // attribute represented by this ProbScalarAtt.
      // NULL, if this ProbScalarAtt represents an output attribute.
   };
};
//
// End of namespace WitJ.

//------------------------------------------------------------------------------
// Class template PartScalarAtt <Value>
//
// Represents an attribute of scalar type Value associated with a Part.
//
// Class Hierarchy:
//
// JavaAccObj
//    Att
//       PartScalarAtt <Value>
//
// The implementation is in ScalarAtts.C.
//------------------------------------------------------------------------------

namespace WitJ
{
template <typename Value>
      class PartScalarAtt: public Att
   {
   //---------------------------------------------------------------------------
   // Local typedefs.
   //
   // SetFuncPtr:
   //    A pointer to a WIT API function that sets      the value of an
   //    attribute of scalar type Value associated with a Part.
   //
   // GetFuncPtr:
   //    A pointer to a WIT API function that retrieves the value of an
   //    attribute of scalar type Value associated with a Part.
   //---------------------------------------------------------------------------

   typedef
      witReturnCode (* SetFuncPtr) (
         WitRun * const,
         const char * const,
         const Value);

   typedef
      witReturnCode (* GetFuncPtr) (
         WitRun * const,
         const char * const,
         Value *);

   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit PartScalarAtt (
         AttBldr *    theAttBldr,
         const char * theName,
         GetFuncPtr   theGetFuncPtr,
         SetFuncPtr   theSetFuncPtr = NULL);
      //
      // Constructor.

   virtual ~PartScalarAtt ();
      //
      // Destructor.

   virtual void set              (Component *, Value);
   virtual void get              (Component *, Value &);
   virtual bool isModifiable     ();
   virtual bool valueTypeMatches (const std::string &);
      //
      // Overrides from class Att.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   PartScalarAtt              (const PartScalarAtt &);
   PartScalarAtt & operator = (const PartScalarAtt &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   const GetFuncPtr myGetFuncPtr_;
      //
      // A pointer to the WIT API function that retrieves the value of the
      // attribute represented by this PartScalarAtt.

   const SetFuncPtr mySetFuncPtr_;
      //
      // A pointer to the WIT API function that sets      the value of the
      // attribute represented by this PartScalarAtt.
      // NULL, if this PartScalarAtt represents an output attribute.
   };
};
//
// End of namespace WitJ.

//------------------------------------------------------------------------------
// Class template DemandScalarAtt <Value>
//
// Represents an attribute of scalar type Value associated with a Demand.
//
// Class Hierarchy:
//
// JavaAccObj
//    Att
//       DemandScalarAtt <Value>
//
// The implementation is in ScalarAtts.C.
//------------------------------------------------------------------------------

namespace WitJ
{
template <typename Value>
      class DemandScalarAtt: public Att
   {
   //---------------------------------------------------------------------------
   // Local typedefs.
   //
   // SetFuncPtr:
   //    A pointer to a WIT API function that sets      the value of an
   //    attribute of scalar type Value associated with a Demand.
   //
   // GetFuncPtr:
   //    A pointer to a WIT API function that retrieves the value of an
   //    attribute of scalar type Value associated with a Demand.
   //---------------------------------------------------------------------------

   typedef
      witReturnCode (* SetFuncPtr) (
         WitRun * const,
         const char * const,
         const char * const,
         const Value);

   typedef
      witReturnCode (* GetFuncPtr) (
         WitRun * const,
         const char * const,
         const char * const,
         Value *);

   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit DemandScalarAtt (
         AttBldr *    theAttBldr,
         const char * theName,
         GetFuncPtr   theGetFuncPtr,
         SetFuncPtr   theSetFuncPtr = NULL);
      //
      // Constructor.

   virtual ~DemandScalarAtt ();
      //
      // Destructor.

   virtual void set              (Component *, Value);
   virtual void get              (Component *, Value &);
   virtual bool isModifiable     ();
   virtual bool valueTypeMatches (const std::string &);
      //
      // Overrides from class Att.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   DemandScalarAtt              (const DemandScalarAtt &);
   DemandScalarAtt & operator = (const DemandScalarAtt &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   const GetFuncPtr myGetFuncPtr_;
      //
      // A pointer to the WIT API function that retrieves the value of the
      // attribute represented by this DemandScalarAtt.

   const SetFuncPtr mySetFuncPtr_;
      //
      // A pointer to the WIT API function that sets      the value of the
      // attribute represented by this DemandScalarAtt.
      // NULL, if this DemandScalarAtt represents an output attribute.
   };
};
//
// End of namespace WitJ.

//------------------------------------------------------------------------------
// Class template OpnScalarAtt <Value>
//
// Represents an attribute of scalar type Value associated with an Operation.
//
// Class Hierarchy:
//
// JavaAccObj
//    Att
//       OpnScalarAtt <Value>
//
// The implementation is in ScalarAtts.C.
//------------------------------------------------------------------------------

namespace WitJ
{
template <typename Value>
      class OpnScalarAtt: public Att
   {
   //---------------------------------------------------------------------------
   // Local typedefs.
   //
   // SetFuncPtr:
   //    A pointer to a WIT API function that sets      the value of an
   //    attribute of scalar type Value associated with an Operation.
   //
   // GetFuncPtr:
   //    A pointer to a WIT API function that retrieves the value of an
   //    attribute of scalar type Value associated with an Operation.
   //---------------------------------------------------------------------------

   typedef
      witReturnCode (* SetFuncPtr) (
         WitRun * const,
         const char * const,
         const Value);

   typedef
      witReturnCode (* GetFuncPtr) (
         WitRun * const,
         const char * const,
         Value *);

   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit OpnScalarAtt (
         AttBldr *    theAttBldr,
         const char * theName,
         GetFuncPtr   theGetFuncPtr,
         SetFuncPtr   theSetFuncPtr = NULL);
      //
      // Constructor.

   virtual ~OpnScalarAtt ();
      //
      // Destructor.

   virtual void set              (Component *, Value);
   virtual void get              (Component *, Value &);
   virtual bool isModifiable     ();
   virtual bool valueTypeMatches (const std::string &);
      //
      // Overrides from class Att.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   OpnScalarAtt              (const OpnScalarAtt &);
   OpnScalarAtt & operator = (const OpnScalarAtt &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   const GetFuncPtr myGetFuncPtr_;
      //
      // A pointer to the WIT API function that retrieves the value of the
      // attribute represented by this OpnScalarAtt.

   const SetFuncPtr mySetFuncPtr_;
      //
      // A pointer to the WIT API function that sets      the value of the
      // attribute represented by this OpnScalarAtt.
      // NULL, if this OpnScalarAtt represents an output attribute.
   };
};
//
// End of namespace WitJ.

//------------------------------------------------------------------------------
// Class template BomEntScalarAtt <Value>
//
// Represents an attribute of scalar type Value associated with a BopEntry.
//
// Class Hierarchy:
//
// JavaAccObj
//    Att
//       BomEntScalarAtt <Value>
//
// The implementation is in ScalarAtts.C.
//------------------------------------------------------------------------------

namespace WitJ
{
template <typename Value>
      class BomEntScalarAtt: public Att
   {
   //---------------------------------------------------------------------------
   // Local typedefs.
   //
   // SetFuncPtr:
   //    A pointer to a WIT API function that sets      the value of an
   //    attribute of scalar type Value associated with a BopEntry.
   //
   // GetFuncPtr:
   //    A pointer to a WIT API function that retrieves the value of an
   //    attribute of scalar type Value associated with a BopEntry.
   //---------------------------------------------------------------------------

   typedef
      witReturnCode (* SetFuncPtr) (
         WitRun * const,
         const char * const,
         const int,
         const Value);

   typedef
      witReturnCode (* GetFuncPtr) (
         WitRun * const,
         const char * const,
         const int,
         Value *);

   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit BomEntScalarAtt (
         AttBldr *    theAttBldr,
         const char * theName,
         GetFuncPtr   theGetFuncPtr,
         SetFuncPtr   theSetFuncPtr = NULL);
      //
      // Constructor.

   virtual ~BomEntScalarAtt ();
      //
      // Destructor.

   virtual void set              (Component *, Value);
   virtual void get              (Component *, Value &);
   virtual bool isModifiable     ();
   virtual bool valueTypeMatches (const std::string &);
      //
      // Overrides from class Att.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   BomEntScalarAtt              (const BomEntScalarAtt &);
   BomEntScalarAtt & operator = (const BomEntScalarAtt &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   const GetFuncPtr myGetFuncPtr_;
      //
      // A pointer to the WIT API function that retrieves the value of the
      // attribute represented by this BomEntScalarAtt.

   const SetFuncPtr mySetFuncPtr_;
      //
      // A pointer to the WIT API function that sets      the value of the
      // attribute represented by this BomEntScalarAtt.
      // NULL, if this BomEntScalarAtt represents an output attribute.
   };
};
//
// End of namespace WitJ.

//------------------------------------------------------------------------------
// Class template SubScalarAtt <Value>
//
// Represents an attribute of scalar type Value associated with a Substitute.
//
// Class Hierarchy:
//
// JavaAccObj
//    Att
//       SubScalarAtt <Value>
//
// The implementation is in ScalarAtts.C.
//------------------------------------------------------------------------------

namespace WitJ
{
template <typename Value>
      class SubScalarAtt: public Att
   {
   //---------------------------------------------------------------------------
   // Local typedefs.
   //
   // SetFuncPtr:
   //    A pointer to a WIT API function that sets      the value of an
   //    attribute of scalar type Value associated with a Substitute.
   //
   // GetFuncPtr:
   //    A pointer to a WIT API function that retrieves the value of an
   //    attribute of scalar type Value associated with a Substitute.
   //---------------------------------------------------------------------------

   typedef
      witReturnCode (* SetFuncPtr) (
         WitRun * const,
         const char * const,
         const int,
         const int,
         const Value);

   typedef
      witReturnCode (* GetFuncPtr) (
         WitRun * const,
         const char * const,
         const int,
         const int,
         Value *);

   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit SubScalarAtt (
         AttBldr *    theAttBldr,
         const char * theName,
         GetFuncPtr   theGetFuncPtr,
         SetFuncPtr   theSetFuncPtr = NULL);
      //
      // Constructor.

   virtual ~SubScalarAtt ();
      //
      // Destructor.

   virtual void set              (Component *, Value);
   virtual void get              (Component *, Value &);
   virtual bool isModifiable     ();
   virtual bool valueTypeMatches (const std::string &);
      //
      // Overrides from class Att.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   SubScalarAtt              (const SubScalarAtt &);
   SubScalarAtt & operator = (const SubScalarAtt &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   const GetFuncPtr myGetFuncPtr_;
      //
      // A pointer to the WIT API function that retrieves the value of the
      // attribute represented by this SubScalarAtt.

   const SetFuncPtr mySetFuncPtr_;
      //
      // A pointer to the WIT API function that sets      the value of the
      // attribute represented by this SubScalarAtt.
      // NULL, if this SubScalarAtt represents an output attribute.
   };
};
//
// End of namespace WitJ.

//------------------------------------------------------------------------------
// Class template BopEntScalarAtt <Value>
//
// Represents an attribute of scalar type Value associated with a BopEntry.
//
// Class Hierarchy:
//
// JavaAccObj
//    Att
//       BopEntScalarAtt <Value>
//
// The implementation is in ScalarAtts.C.
//------------------------------------------------------------------------------

namespace WitJ
{
template <typename Value>
      class BopEntScalarAtt: public Att
   {
   //---------------------------------------------------------------------------
   // Local typedefs.
   //
   // SetFuncPtr:
   //    A pointer to a WIT API function that sets      the value of an
   //    attribute of scalar type Value associated with a BopEntry.
   //
   // GetFuncPtr:
   //    A pointer to a WIT API function that retrieves the value of an
   //    attribute of scalar type Value associated with a BopEntry.
   //---------------------------------------------------------------------------

   typedef
      witReturnCode (* SetFuncPtr) (
         WitRun * const,
         const char * const,
         const int,
         const Value);

   typedef
      witReturnCode (* GetFuncPtr) (
         WitRun * const,
         const char * const,
         const int,
         Value *);

   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit BopEntScalarAtt (
         AttBldr *    theAttBldr,
         const char * theName,
         GetFuncPtr   theGetFuncPtr,
         SetFuncPtr   theSetFuncPtr = NULL);
      //
      // Constructor.

   virtual ~BopEntScalarAtt ();
      //
      // Destructor.

   virtual void set              (Component *, Value);
   virtual void get              (Component *, Value &);
   virtual bool isModifiable     ();
   virtual bool valueTypeMatches (const std::string &);
      //
      // Overrides from class Att.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   BopEntScalarAtt              (const BopEntScalarAtt &);
   BopEntScalarAtt & operator = (const BopEntScalarAtt &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   const GetFuncPtr myGetFuncPtr_;
      //
      // A pointer to the WIT API function that retrieves the value of the
      // attribute represented by this BopEntScalarAtt.

   const SetFuncPtr mySetFuncPtr_;
      //
      // A pointer to the WIT API function that sets      the value of the
      // attribute represented by this BopEntScalarAtt.
      // NULL, if this BopEntScalarAtt represents an output attribute.
   };
};
//
// End of namespace WitJ.

//------------------------------------------------------------------------------
// Class template ProbScalarAttString
//
// Represents an string attribute associated with the whole Problem.
//
// Class Hierarchy:
//
// JavaAccObj
//    Att
//       ProbScalarAttString
//
// The implementation is in ScalarAtts.C.
//------------------------------------------------------------------------------

namespace WitJ
{
class ProbScalarAttString: public Att
   {
   //---------------------------------------------------------------------------
   // Local typedefs.
   //
   // SetFuncPtr:
   //    A pointer to a WIT API function that sets      the value of a string
   //    attribute associated with the whole Problem
   //
   // GetFuncPtr:
   //    A pointer to a WIT API function that retrieves the value of a string
   //    attribute associated with the whole Problem.
   //---------------------------------------------------------------------------

   typedef witReturnCode (* SetFuncPtr) (WitRun * const, const char * const);
   typedef witReturnCode (* GetFuncPtr) (WitRun * const,       char * *);

   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit ProbScalarAttString (
         AttBldr *    theAttBldr,
         const char * theName,
         GetFuncPtr   theGetFuncPtr,
         SetFuncPtr   theSetFuncPtr = NULL);
      //
      // Constructor.

   virtual ~ProbScalarAttString ();
      //
      // Destructor.

   virtual void set              (Component *, const char *);
   virtual void get              (Component *, StringJOR &);
   virtual bool isModifiable     ();
   virtual bool valueTypeMatches (const std::string &);
      //
      // Overrides from class Att.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   ProbScalarAttString              (const ProbScalarAttString &);
   ProbScalarAttString & operator = (const ProbScalarAttString &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   const GetFuncPtr myGetFuncPtr_;
      //
      // A pointer to the WIT API function that retrieves the value of the
      // attribute represented by this ProbScalarAttString.

   const SetFuncPtr mySetFuncPtr_;
      //
      // A pointer to the WIT API function that sets      the value of the
      // attribute represented by this ProbScalarAttString.
      // NULL, if this ProbScalarAttString represents an output attribute.
   };
};
//
// End of namespace WitJ.

#endif
