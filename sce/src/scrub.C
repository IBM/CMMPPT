#include <sce/src/scrub.h>

LgFrSceScrubber::LgFrSceScrubber(const LgFrSceScrubber& source)
  {
    scrubMode_ = source.scrubMode_;
    scrubDir_ = source.scrubDir_;
    scrubOnly_ = source.scrubOnly_;
  }


LgFrSceScrubber& LgFrSceScrubber::operator= (const LgFrSceScrubber& source)
  {
    if (this != &source)
      {
       scrubMode_ = source.scrubMode_;
       scrubDir_ = source.scrubDir_;
       scrubOnly_ = source.scrubOnly_;
      }
    return *this;
  }
