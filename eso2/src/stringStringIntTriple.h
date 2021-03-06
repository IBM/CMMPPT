#ifndef ESO2stringStringIntTriple_h
#define ESO2stringStringIntTriple_h

#include <string>



class ESO2stringStringIntTriple  {
public:

  void setString1(const std::string & s ) { string1_ = s; };
  std::string getString1()const { return string1_; };

  void setString2(const std::string & s ) { string2_ = s; };
  std::string getString2()const { return string2_; };

  void setInt(int i ) { int_ = i; };
  int getInt()const { return int_; };

  // default constructor
  ESO2stringStringIntTriple():string1_(),string2_(),int_(0){};

  // alternate constructor
  ESO2stringStringIntTriple(
    const std::string & s1,
    const std::string & s2,
    int i)
    :string1_(s1),string2_(s2),int_(i)
  {};


  // destructor
  virtual ~ESO2stringStringIntTriple(){};

  // copy constructor
  ESO2stringStringIntTriple( const ESO2stringStringIntTriple& source )
    :
  string1_(source.string1_),
    string2_(source.string2_),
    int_(source.int_)
  {};

  // assignment operator
  ESO2stringStringIntTriple&
    operator=(const ESO2stringStringIntTriple& rhs)
  {  
    if (this != &rhs) {       
      string1_ = rhs.string1_;
      string2_ = rhs.string1_;
      int_ = rhs.int_;
    }
    return *this;
  }

  // Self-test
  //static void test();

private:


  std::string string1_;
  std::string string2_;
  int int_;
};


class ESO2stringStringIntTripleCompareFunctionObject {

public:
  bool operator()(const ESO2stringStringIntTriple & a, const ESO2stringStringIntTriple & b) const
  {
    bool retVal=false;

    const std::string & as1 = a.getString1();
    const std::string & as2 = a.getString2();
    int ai = a.getInt();

    const std::string & bs1 = b.getString1();
    const std::string & bs2 = b.getString2();
    int bi = b.getInt();

    if( as1 < bs1 )retVal=true;
    else if ( as1==bs1 && as2<bs2 ) retVal=true;
    else if ( as1==bs1 && as2==bs2 && ai<bi ) retVal=true;

    return retVal;

  }
};


#endif
