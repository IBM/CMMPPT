#ifndef ESO2stringStringIntIntTuple_h
#define ESO2stringStringIntIntTuple_h

#include <string>



class ESO2stringStringIntIntTuple  {
public:

  void setString1(const std::string & s ) { string1_ = s; };
  std::string getString1()const { return string1_; };

  void setString2(const std::string & s ) { string2_ = s; };
  std::string getString2()const { return string2_; };

  void setInt1(int i ) { int1_ = i; };
  int getInt1()const { return int1_; };

  void setInt2(int i ) { int2_ = i; };
  int getInt2()const { return int2_; };

  // default constructor
  ESO2stringStringIntIntTuple():string1_(),string2_(),int1_(0),int2_(0){};

  // alternate constructor
  ESO2stringStringIntIntTuple(
    const std::string & s1,
    const std::string & s2,
    int i1,
    int i2)
    :string1_(s1),string2_(s2),int1_(i1),int2_(i2)
  {};


  // destructor
  virtual ~ESO2stringStringIntIntTuple(){};

  // copy constructor
  ESO2stringStringIntIntTuple( const ESO2stringStringIntIntTuple& source )
    :
  string1_(source.string1_),
    string2_(source.string2_),
    int1_(source.int1_),
    int2_(source.int2_)
  {};

  // assignment operator
  ESO2stringStringIntIntTuple&
    operator=(const ESO2stringStringIntIntTuple& rhs)
  {  
    if (this != &rhs) {       
      string1_ = rhs.string1_;
      string2_ = rhs.string1_;
      int1_ = rhs.int1_;
      int2_ = rhs.int2_;
    }
    return *this;
  }

  // Self-test
  //static void test();

private:


  std::string string1_;
  std::string string2_;
  int int1_;
  int int2_;
};


class ESO2stringStringIntIntTupleCompareFunctionObject {

public:
  bool operator()(const ESO2stringStringIntIntTuple & a, const ESO2stringStringIntIntTuple & b) const
  {
    bool retVal=false;

    const std::string & as1 = a.getString1();
    const std::string & as2 = a.getString2();
    int ai1 = a.getInt1();
    int ai2 = a.getInt2();

    const std::string & bs1 = b.getString1();
    const std::string & bs2 = b.getString2();
    int bi1 = b.getInt1();
    int bi2 = b.getInt2();

    if( as1 < bs1 )retVal=true;
    else if ( as1==bs1 && as2<bs2 ) retVal=true;
    else if ( as1==bs1 && as2==bs2 && ai1<bi1 ) retVal=true;
    else if ( as1==bs1 && as2==bs2 && ai1==bi1 && ai2<bi2 ) retVal=true;

    return retVal;

  }
};


#endif
