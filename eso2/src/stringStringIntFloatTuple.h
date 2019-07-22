#ifndef ESO2stringStringIntFloatTuple_h
#define ESO2stringStringIntFloatTuple_h

#include <string>
#include "CoinFloatEqual.h"


class ESO2stringStringIntFloatTuple  {
public:

  void setString1(const std::string & s ) { string1_ = s; };
  std::string getString1()const { return string1_; };

  void setString2(const std::string & s ) { string2_ = s; };
  std::string getString2()const { 
    return string2_; 
  };

  void setInt(int i ) { int_ = i; };
  int getInt()const { return int_; };

  void setFloat(float f ) { float_ = f; };
  float getFloat()const { return float_; };

  // default constructor
  ESO2stringStringIntFloatTuple():string1_(),string2_(),int_(0),float_(0.f){};

  // alternate constructor
  ESO2stringStringIntFloatTuple(
    const std::string & s1,
    const std::string & s2,
    int i,
    float f)
    :string1_(s1),string2_(s2),int_(i),float_(f)
  {};


  // destructor
  virtual ~ESO2stringStringIntFloatTuple(){};

  // copy constructor
  ESO2stringStringIntFloatTuple( const ESO2stringStringIntFloatTuple& source )
    :
  string1_(source.string1_),
    string2_(source.string2_),
    int_(source.int_),
    float_(source.float_)
  {};

  // assignment operator
  ESO2stringStringIntFloatTuple&
    operator=(const ESO2stringStringIntFloatTuple& rhs)
  {  
    if (this != &rhs) {       
      string1_ = rhs.string1_;
      string2_ = rhs.string1_;
      int_ = rhs.int_;
      float_=rhs.float_;
    }
    return *this;
  }

  bool operator==(const ESO2stringStringIntFloatTuple& rhs)
  {
    CoinRelFltEq eq;
    bool retVal=false;
    if( 
      getString1() == rhs.getString1() &&
      getString2() == rhs.getString2() &&
      getInt() == rhs.getInt() &&
      eq(getFloat(),rhs.getFloat() ) )
      retVal=true;
    return retVal;
  }

  // Self-test
  //static void test();

private:


  std::string string1_;
  std::string string2_;
  int int_;
  float float_;
};

class ESO2stringStringIntFloatTupleCompareFunctionObject {

public:
  bool operator()(const ESO2stringStringIntFloatTuple & a, const ESO2stringStringIntFloatTuple & b) const
  {
    bool retVal=false;

    const std::string & as1 = a.getString1();
    const std::string & as2 = a.getString2();
    int ai = a.getInt();
    float af = a.getFloat();

    const std::string & bs1 = b.getString1();
    const std::string & bs2 = b.getString2();
    int bi = b.getInt();
    float bf = b.getFloat();

    if( as1 < bs1 )retVal=true;
    else if ( as1==bs1 && as2<bs2 ) retVal=true;
    else if ( as1==bs1 && as2==bs2 && ai<bi ) retVal=true;
    else if ( as1==bs1 && as2==bs2 && ai==bi && af<bf ) retVal=true;

    return retVal;

  }
};



#endif
