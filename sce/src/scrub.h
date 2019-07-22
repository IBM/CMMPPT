// File: scrub.h

#ifndef LGFRSCESCRUBBER_H
#define LGFRSCESCRUBBER_H

// RW2STL - inserted:
#include <string>
// End of RW2STL - inserted includes.

#include <stdlib.h>
#include <iostream>

class LgFrSceScrubber
{
public:
  LgFrSceScrubber(const std::string& scrubDir,
                  bool scrubMode = false,
                  bool scrubOnly= false)
    : scrubDir_(scrubDir),
      scrubMode_(scrubMode),
      scrubOnly_(scrubOnly)
  {
    scrubDir_.append("/");
  }

  LgFrSceScrubber()
    :scrubDir_(""),
      scrubMode_(false),
      scrubOnly_(false)
  {
  }
  
  virtual ~LgFrSceScrubber(){}
  LgFrSceScrubber(const LgFrSceScrubber& source);
  LgFrSceScrubber& operator=(const LgFrSceScrubber& source);
  const std::string& scrubDir(void) const { return scrubDir_;}
  void scrubDir(const std::string& dir) { scrubDir_ = dir;}
  bool scrubMode(void) { return scrubMode_; }
  void scrubMode(bool mode) { scrubMode_ = mode;}
  const bool scrubOnly(void) const { return scrubOnly_;}
  void scrubOnly(const bool scrubOnly) { scrubOnly_ = scrubOnly;}

private:

  std::string scrubDir_;
  bool scrubOnly_;
  bool scrubMode_;
  
};

#endif
