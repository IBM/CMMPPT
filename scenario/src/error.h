#ifndef LGFRERRORH
#define LGFRERRORH
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

class LgFrError
{
public:
  void
    setError(int e);
  
  // Constructor
  LgFrError();
  LgFrError(int e);

  // Destructor
  ~LgFrError();

  // Equality and inequality
  int operator==(const LgFrError&) const;
  int operator!=(const LgFrError&) const;  

  // Assignment
  LgFrError& operator=(const LgFrError&);
    
private:
  int errorNum_;
};
#endif
