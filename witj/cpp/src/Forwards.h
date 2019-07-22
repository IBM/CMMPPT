//------------------------------------------------------------------------------
// WIT-J C++ Header File Forwards.h.
//
// Contains the forward declarations used by WIT-J/C++.
// Contains the typedef declarations used by WIT-J/C++.
//------------------------------------------------------------------------------

#ifndef Forwards_h
#define Forwards_h

//------------------------------------------------------------------------------
// Forward class declarations.
//------------------------------------------------------------------------------

namespace WitJ
{
class Att;
class AttBldr;
class BomEntry;
class BopEntry;
class CTVecSupply;
class CaseToken;
class Component;
class ComponentJOR;
class Coordinator;
class DblALJOR;
class DblArrayJOR;
class Demand;
class DemandALJOR;
class IntALJOR;
class JavaAccObj;
class MessageMgr;
class Operation;
class Part;
class PeggingTripleALJOR;
class PggHandler;
class PreJavaException;
class Problem;
class StringALJOR;
class StringJOR;
class StringVec;
class Substitute;
}

//------------------------------------------------------------------------------
// Forward template declarations.
//------------------------------------------------------------------------------

namespace WitJ
{
template <typename E> class ArrayJOR;
template <typename C> class CompALJOR;
}

//------------------------------------------------------------------------------
// Typedefs.
//------------------------------------------------------------------------------

namespace WitJ
{
typedef int Boolean;
}

//------------------------------------------------------------------------------
// Typedef DemPggFunc
//
// A pointer to a Demand pegging retrieval function.
//------------------------------------------------------------------------------

namespace WitJ
{
typedef void (PggHandler::* DemPggFunc) (
   Demand *,
   int,
   PeggingTripleALJOR &);
}

//------------------------------------------------------------------------------
// Typedef OpnPggFunc
//
// A pointer to an Operation pegging retrieval function.
//------------------------------------------------------------------------------

namespace WitJ
{
typedef void (PggHandler::* OpnPggFunc) (
   Operation * ,
   int,
   PeggingTripleALJOR &);
}

//------------------------------------------------------------------------------
// Forward class declarations for WIT.
//------------------------------------------------------------------------------

class WitRun;
class WitErrorExc;

#endif
