//------------------------------------------------------------------------------
// WIT-J C++ Header File VectorAtts.h.
//
// Contains the declarations of the following class templates:
//
//      ProbVectorAtt <Elem>
//      PartVectorAtt <Elem>
//    DemandVectorAtt <Elem>
//       OpnVectorAtt <Elem>
//    BomEntVectorAtt <Elem>
//       SubVectorAtt <Elem>
//    BopEntVectorAtt <Elem>
//------------------------------------------------------------------------------

#ifndef VectorAtts_h
#define VectorAtts_h

#include <Att.h>

#include <wit.h>

//------------------------------------------------------------------------------
// Class template ProbVectorAtt <Elem>
//
// Represents an attribute of type time-vector of Elems associated with the
// whole Problem.
//
// Class Hierarchy:
//
// JavaAccObj
//    Att
//       ProbVectorAtt <Elem>
//
// The implementation is in VectorAtts.C.
//------------------------------------------------------------------------------

namespace WitJ
{
template <typename Elem>
      class ProbVectorAtt: public Att
   {
   //---------------------------------------------------------------------------
   // Local typedefs.
   //
   // SetFuncPtr:
   //    A pointer to a WIT API function that sets      the value of an
   //    attribute of type time-vector of Elems associated with the whole
   //    Problem.
   //
   // GetFuncPtr:
   //    A pointer to a WIT API function that retrieves the value of an
   //    attribute of type time-vector of Elems associated with the whole
   //    Problem.
   //---------------------------------------------------------------------------

   typedef witReturnCode (* SetFuncPtr) (WitRun * const, const Elem * const);
   typedef witReturnCode (* GetFuncPtr) (WitRun * const,       Elem * *);

   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit ProbVectorAtt (
         AttBldr *    theAttBldr,
         const char * theName,
         GetFuncPtr   theGetFuncPtr,
         SetFuncPtr   theSetFuncPtr = NULL);
      //
      // Constructor.

   virtual ~ProbVectorAtt ();
      //
      // Destructor.

   virtual void set              (Component *, const Elem *);
   virtual void get              (Component *, ArrayJOR <Elem> &);
   virtual bool isModifiable     ();
   virtual bool valueTypeMatches (const std::string &);
      //
      // Overrides from class Att.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   ProbVectorAtt              (const ProbVectorAtt &);
   ProbVectorAtt & operator = (const ProbVectorAtt &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   const GetFuncPtr myGetFuncPtr_;
      //
      // A pointer to the WIT API function that retrieves the value of the
      // attribute represented by this ProbVectorAtt.

   const SetFuncPtr mySetFuncPtr_;
      //
      // A pointer to the WIT API function that sets      the value of the
      // attribute represented by this ProbVectorAtt.
      // NULL, if this ProbVectorAtt represents an output attribute.
   };
};
//
// End of namespace WitJ.

//------------------------------------------------------------------------------
// Class template PartVectorAtt <Elem>
//
// Represents an attribute of type time-vector of Elems associated with a Part.
//
// Class Hierarchy:
//
// JavaAccObj
//    Att
//       PartVectorAtt <Elem>
//
// The implementation is in VectorAtts.C.
//------------------------------------------------------------------------------

namespace WitJ
{
template <typename Elem>
      class PartVectorAtt: public Att
   {
   //---------------------------------------------------------------------------
   // Local typedefs.
   //
   // SetFuncPtr:
   //    A pointer to a WIT API function that sets      the value of an
   //    attribute of type time-vector of Elems associated with a Part.
   //
   // GetFuncPtr:
   //    A pointer to a WIT API function that retrieves the value of an
   //    attribute of type time-vector of Elems associated with a Part.
   //---------------------------------------------------------------------------

   public:

   typedef
      witReturnCode (* SetFuncPtr) (
         WitRun * const,
         const char * const,
         const Elem * const);

   typedef
      witReturnCode (* GetFuncPtr) (
         WitRun * const,
         const char * const,
         Elem * *);

   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit PartVectorAtt (
         AttBldr *    theAttBldr,
         const char * theName,
         GetFuncPtr   theGetFuncPtr,
         SetFuncPtr   theSetFuncPtr = NULL);
      //
      // Constructor.

   virtual ~PartVectorAtt ();
      //
      // Destructor.

   virtual void set              (Component *, const Elem *);
   virtual void get              (Component *, ArrayJOR <Elem> &);
   virtual bool isModifiable     ();
   virtual bool valueTypeMatches (const std::string &);
      //
      // Overrides from class Att.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   PartVectorAtt              (const PartVectorAtt &);
   PartVectorAtt & operator = (const PartVectorAtt &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   const GetFuncPtr myGetFuncPtr_;
      //
      // A pointer to the WIT API function that retrieves the value of the
      // attribute represented by this PartVectorAtt.

   const SetFuncPtr mySetFuncPtr_;
      //
      // A pointer to the WIT API function that sets      the value of the
      // attribute represented by this PartVectorAtt.
      // NULL, if this PartVectorAtt represents an output attribute.
   };
};
//
// End of namespace WitJ.

//------------------------------------------------------------------------------
// Class template DemandVectorAtt <Elem>
//
// Represents an attribute of type time-vector of Elems associated with a
// Demand.
//
// Class Hierarchy:
//
// JavaAccObj
//    Att
//       DemandVectorAtt <Elem>
//
// The implementation is in VectorAtts.C.
//------------------------------------------------------------------------------

namespace WitJ
{
template <typename Elem>
      class DemandVectorAtt: public Att
   {
   //---------------------------------------------------------------------------
   // Local typedefs.
   //
   // SetFuncPtr:
   //    A pointer to a WIT API function that sets      the value of an
   //    attribute of type time-vector of Elems associated with a Demand.
   //
   // GetFuncPtr:
   //    A pointer to a WIT API function that retrieves the value of an
   //    attribute of type time-vector of Elems associated with a Demand.
   //---------------------------------------------------------------------------

   typedef
      witReturnCode (* SetFuncPtr) (
         WitRun * const,
         const char * const,
         const char * const,
         const Elem * const);

   typedef
      witReturnCode (* GetFuncPtr) (
         WitRun * const,
         const char * const,
         const char * const,
         Elem * *);

   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit DemandVectorAtt (
         AttBldr *    theAttBldr,
         const char * theName,
         GetFuncPtr   theGetFuncPtr,
         SetFuncPtr   theSetFuncPtr = NULL);
      //
      // Constructor.

   virtual ~DemandVectorAtt ();
      //
      // Destructor.

   virtual void set              (Component *, const Elem *);
   virtual void get              (Component *, ArrayJOR <Elem> &);
   virtual bool isModifiable     ();
   virtual bool valueTypeMatches (const std::string &);
      //
      // Overrides from class Att.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   DemandVectorAtt              (const DemandVectorAtt &);
   DemandVectorAtt & operator = (const DemandVectorAtt &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   const GetFuncPtr myGetFuncPtr_;
      //
      // A pointer to the WIT API function that retrieves the value of the
      // attribute represented by this DemandVectorAtt.

   const SetFuncPtr mySetFuncPtr_;
      //
      // A pointer to the WIT API function that sets      the value of the
      // attribute represented by this DemandVectorAtt.
      // NULL, if this DemandVectorAtt represents an output attribute.
   };
};
//
// End of namespace WitJ.

//------------------------------------------------------------------------------
// Class template OpnVectorAtt <Elem>
//
// Represents an attribute of type time-vector of Elems associated with an
// Operation.
//
// Class Hierarchy:
//
// JavaAccObj
//    Att
//       OpnVectorAtt <Elem>
//
// The implementation is in VectorAtts.C.
//------------------------------------------------------------------------------

namespace WitJ
{
template <typename Elem>
      class OpnVectorAtt: public Att
   {
   //---------------------------------------------------------------------------
   // Local typedefs.
   //
   // SetFuncPtr:
   //    A pointer to a WIT API function that sets      the value of an
   //    attribute of type time-vector of Elems associated with an Operation.
   //
   // GetFuncPtr:
   //    A pointer to a WIT API function that retrieves the value of an
   //    attribute of type time-vector of Elems associated with an Operation.
   //---------------------------------------------------------------------------

   typedef
      witReturnCode (* SetFuncPtr) (
         WitRun * const,
         const char * const,
         const Elem * const);

   typedef
      witReturnCode (* GetFuncPtr) (
         WitRun * const,
         const char * const,
         Elem * *);

   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit OpnVectorAtt (
         AttBldr *    theAttBldr,
         const char * theName,
         GetFuncPtr   theGetFuncPtr,
         SetFuncPtr   theSetFuncPtr = NULL);
      //
      // Constructor.

   virtual ~OpnVectorAtt ();
      //
      // Destructor.

   virtual void set              (Component *, const Elem *);
   virtual void get              (Component *, ArrayJOR <Elem> &);
   virtual bool isModifiable     ();
   virtual bool valueTypeMatches (const std::string &);
      //
      // Overrides from class Att.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   OpnVectorAtt              (const OpnVectorAtt &);
   OpnVectorAtt & operator = (const OpnVectorAtt &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   const GetFuncPtr myGetFuncPtr_;
      //
      // A pointer to the WIT API function that retrieves the value of the
      // attribute represented by this OpnVectorAtt.

   const SetFuncPtr mySetFuncPtr_;
      //
      // A pointer to the WIT API function that sets      the value of the
      // attribute represented by this OpnVectorAtt.
      // NULL, if this OpnVectorAtt represents an output attribute.
   };
};
//
// End of namespace WitJ.

//------------------------------------------------------------------------------
// Class template BomEntVectorAtt <Elem>
//
// Represents an attribute of type time-vector of Elems associated with a
// BomEntry.
//
// Class Hierarchy:
//
// JavaAccObj
//    Att
//       BomEntVectorAtt <Elem>
//
// The implementation is in VectorAtts.C.
//------------------------------------------------------------------------------

namespace WitJ
{
template <typename Elem>
      class BomEntVectorAtt: public Att
   {
   //---------------------------------------------------------------------------
   // Local typedefs.
   //
   // SetFuncPtr:
   //    A pointer to a WIT API function that sets      the value of an
   //    attribute of type time-vector of Elems associated with a BomEntry.
   //
   // GetFuncPtr:
   //    A pointer to a WIT API function that retrieves the value of an
   //    attribute of type time-vector of Elems associated with a BomEntry.
   //---------------------------------------------------------------------------

   typedef
      witReturnCode (* SetFuncPtr) (
         WitRun * const,
         const char * const,
         const int,
         const Elem * const);

   typedef
      witReturnCode (* GetFuncPtr) (
         WitRun * const,
         const char * const,
         const int,
         Elem * *);

   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit BomEntVectorAtt (
         AttBldr *    theAttBldr,
         const char * theName,
         GetFuncPtr   theGetFuncPtr,
         SetFuncPtr   theSetFuncPtr = NULL);
      //
      // Constructor.

   virtual ~BomEntVectorAtt ();
      //
      // Destructor.

   virtual void set              (Component *, const Elem *);
   virtual void get              (Component *, ArrayJOR <Elem> &);
   virtual bool isModifiable     ();
   virtual bool valueTypeMatches (const std::string &);
      //
      // Overrides from class Att.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   BomEntVectorAtt              (const BomEntVectorAtt &);
   BomEntVectorAtt & operator = (const BomEntVectorAtt &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   const GetFuncPtr myGetFuncPtr_;
      //
      // A pointer to the WIT API function that retrieves the value of the
      // attribute represented by this BomEntVectorAtt.

   const SetFuncPtr mySetFuncPtr_;
      //
      // A pointer to the WIT API function that sets      the value of the
      // attribute represented by this BomEntVectorAtt.
      // NULL, if this BomEntVectorAtt represents an output attribute.
   };
};
//
// End of namespace WitJ.

//------------------------------------------------------------------------------
// Class template SubVectorAtt <Elem>
//
// Represents an attribute of type time-vector of Elems associated with a
// Substitute.
//
// Class Hierarchy:
//
// JavaAccObj
//    Att
//       SubVectorAtt <Elem>
//
// The implementation is in VectorAtts.C.
//------------------------------------------------------------------------------

namespace WitJ
{
template <typename Elem>
      class SubVectorAtt: public Att
   {
   //---------------------------------------------------------------------------
   // Local typedefs.
   //
   // SetFuncPtr:
   //    A pointer to a WIT API function that sets      the value of an
   //    attribute of type time-vector of Elems associated with a Substitute.
   //
   // GetFuncPtr:
   //    A pointer to a WIT API function that retrieves the value of an
   //    attribute of type time-vector of Elems associated with a Substitute.
   //---------------------------------------------------------------------------

   typedef
      witReturnCode (* SetFuncPtr) (
         WitRun * const,
         const char * const,
         const int,
         const int,
         const Elem * const);

   typedef
      witReturnCode (* GetFuncPtr) (
         WitRun * const,
         const char * const,
         const int,
         const int,
         Elem * *);

   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit SubVectorAtt (
         AttBldr *    theAttBldr,
         const char * theName,
         GetFuncPtr   theGetFuncPtr,
         SetFuncPtr   theSetFuncPtr = NULL);
      //
      // Constructor.

   virtual ~SubVectorAtt ();
      //
      // Destructor.

   virtual void set              (Component *, const Elem *);
   virtual void get              (Component *, ArrayJOR <Elem> &);
   virtual bool isModifiable     ();
   virtual bool valueTypeMatches (const std::string &);
      //
      // Overrides from class Att.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   SubVectorAtt              (const SubVectorAtt &);
   SubVectorAtt & operator = (const SubVectorAtt &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   const GetFuncPtr myGetFuncPtr_;
      //
      // A pointer to the WIT API function that retrieves the value of the
      // attribute represented by this SubVectorAtt.

   const SetFuncPtr mySetFuncPtr_;
      //
      // A pointer to the WIT API function that sets      the value of the
      // attribute represented by this SubVectorAtt.
      // NULL, if this SubVectorAtt represents an output attribute.
   };
};
//
// End of namespace WitJ.

//------------------------------------------------------------------------------
// Class template BopEntVectorAtt <Elem>
//
// Represents an attribute of type time-vector of Elems associated with a
// BopEntry.
//
// Class Hierarchy:
//
// JavaAccObj
//    Att
//       BopEntVectorAtt <Elem>
//
// The implementation is in VectorAtts.C.
//------------------------------------------------------------------------------

namespace WitJ
{
template <typename Elem>
      class BopEntVectorAtt: public Att
   {
   //---------------------------------------------------------------------------
   // Local typedefs.
   //
   // SetFuncPtr:
   //    A pointer to a WIT API function that sets      the value of an
   //    attribute of type time-vector of Elems associated with a BopEntry.
   //
   // GetFuncPtr:
   //    A pointer to a WIT API function that retrieves the value of an
   //    attribute of type time-vector of Elems associated with a BopEntry.
   //---------------------------------------------------------------------------

   typedef
      witReturnCode (* SetFuncPtr) (
         WitRun * const,
         const char * const,
         const int,
         const Elem * const);

   typedef
      witReturnCode (* GetFuncPtr) (
         WitRun * const,
         const char * const,
         const int,
         Elem * *);

   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit BopEntVectorAtt (
         AttBldr *    theAttBldr,
         const char * theName,
         GetFuncPtr   theGetFuncPtr,
         SetFuncPtr   theSetFuncPtr = NULL);
      //
      // Constructor.

   virtual ~BopEntVectorAtt ();
      //
      // Destructor.

   virtual void set              (Component *, const Elem *);
   virtual void get              (Component *, ArrayJOR <Elem> &);
   virtual bool isModifiable     ();
   virtual bool valueTypeMatches (const std::string &);
      //
      // Overrides from class Att.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   BopEntVectorAtt              (const BopEntVectorAtt &);
   BopEntVectorAtt & operator = (const BopEntVectorAtt &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   const GetFuncPtr myGetFuncPtr_;
      //
      // A pointer to the WIT API function that retrieves the value of the
      // attribute represented by this BopEntVectorAtt.

   const SetFuncPtr mySetFuncPtr_;
      //
      // A pointer to the WIT API function that sets      the value of the
      // attribute represented by this BopEntVectorAtt.
      // NULL, if this BopEntVectorAtt represents an output attribute.
   };
};
//
// End of namespace WitJ.

#endif
