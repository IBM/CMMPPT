//------------------------------------------------------------------------------
// WIT-J C++ Header File Att.h.
//
// Contains the declaration of class Att.
//------------------------------------------------------------------------------

#ifndef Att_h
#define Att_h

#include <JavaAccObj.h>

#include     <string>

//------------------------------------------------------------------------------
// Class Att
//
// Represents an attribute of an Component in an implosion problem.
//
// Class Hierarchy:
//
// JavaAccObj
//    Att
//       ProbScalarAtt    <Value>
//       GlobalScalarAtt  <Value>
//       PartScalarAtt    <Value>
//       DemandScalarAtt  <Value>
//       OpnScalarAtt     <Value>
//       BomScalarAtt     <Value>
//       SubScalarAtt     <Value>
//       BopScalarAtt     <Value>
//       ProbScalarAttString
//       ProbVectorAtt    <Elem>
//       PartVectorAtt    <Elem>
//       DemandVectorAtt  <Elem>
//       OpnVectorAtt     <Elem>
//       BomVectorAtt     <Elem>
//       SubVectorAtt     <Elem>
//       BopVectorAtt     <Elem>
//------------------------------------------------------------------------------

namespace WitJ
{
class Att: public JavaAccObj
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   virtual ~Att ();
      //
      // Destructor.

   virtual Att * asAtt ();
      //
      // Override from class JavaAccObj.

   //---------------------------------------------------------------------------
   // set (Component *, ...) functions
   //
   // For each of these functions, if this Att represents an input attribute of
   // the type that matches the second argument and that's associated with the
   // Component class of the Component argument, the function sets the value of
   // the attribute for the Component to the value of the second argument.
   // Otherwise, the function throws an assertion exception.
   //---------------------------------------------------------------------------

   virtual void set (Component *, int);
   virtual void set (Component *, double);
   virtual void set (Component *, const char *);
   virtual void set (Component *, const int *);
   virtual void set (Component *, const double *);

   //---------------------------------------------------------------------------

   template <typename Elem>
         void setVectorToScalar (Component * theComp, Elem theScalar);
      //
      // If this Att represents an input attribute of type vector of Elem and
      // that's associated with the Component class of the Component argument,
      // this function sets the value of the attribute for the Component to
      // theScalar in all periods.
      // Otherwise, this function throws an assertion exception.
      // Note: During the execution of this function, the CTVecSupply for the
      // Problem is temporarily put into a reserved state.

   //---------------------------------------------------------------------------
   // get (Component *, ...) functions
   //
   // For each of these functions, if this Att represents an attribute of the
   // type that matches the second argument and that's associated with the
   // Component class of the Component argument, the function sets the value of
   // the second argument to the value of the attribute for the Component.
   //---------------------------------------------------------------------------

   virtual void get (Component *, int &);
   virtual void get (Component *, double &);
   virtual void get (Component *, ArrayJOR <int> &);
   virtual void get (Component *, ArrayJOR <double> &);
   virtual void get (Component *, StringJOR &);

   //---------------------------------------------------------------------------

   virtual bool isModifiable () = 0;
      //
      // Returns true, iff this Att supports a "set" member function.

   virtual bool valueTypeMatches (const std::string & theTypeName) = 0;
      //
      // Returns true, iff theTypeName indicates a java type that matches the
      // value type of this Att.

   //---------------------------------------------------------------------------
   // Accessors.
   //---------------------------------------------------------------------------

   inline const std::string & getAttName ()
      {
      return myAttName_;
      }

   inline const std::string & getCompClassName ()
      {
      return myCompClassName_;
      }

   //---------------------------------------------------------------------------
   // Protected member functions.
   //---------------------------------------------------------------------------

   protected:

   explicit Att (
         AttBldr *    theAttBldr,
         const char * theAttName,
         const char * theCompClassName);
      //
      // Constructor.
      // The first character of theAttName must be an upper case letter.
      // myAttName_ is set to theAttName, with the first character converted
      // to lower case.

   template <typename Value>
         static bool scalarTypeMatches (const std::string & theTypeName);
      //
      // Returns true, iff theTypeName indicates a java scalar type that matches
      // Value.

   template <typename Elem>
         static bool vectorTypeMatches (const std::string & theTypeName);
      //
      // Returns true, iff theTypeName indicates a java array type whose element
      // type matches Elem.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   Att              (const Att &);
   Att & operator = (const Att &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   const std::string myAttName_;
      //
      // The name of the attribute represented by this Att.

   const std::string myCompClassName_;
      //
      // The name of the Component class to which the represented attribute
      // applies.
   };
};
//
// End of namespace WitJ.

#endif
