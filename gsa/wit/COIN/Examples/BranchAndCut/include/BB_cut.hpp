// Copyright (C) 2001, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef _BB_CUT_H
#define _BB_CUT_H

#include "BCP_cut.hpp"
#include "BCP_mempool.hpp"
#include "OsiRowCut.hpp"

class BCP_buffer;

//#############################################################################

class BB_cut : public BCP_cut_algo, public OsiRowCut {
private:
   static BCP_MemPool memPool;
public:
   static inline void * operator new(size_t size) {
      return memPool.alloc(size);
   }
   static inline void operator delete(void *p, size_t size) {
      memPool.free(p, size);
   }

public:
   ~BB_cut() {}
   BB_cut(BCP_buffer& buf);
   BB_cut(const OsiRowCut& cut);
   void pack(BCP_buffer& buf) const;
};

//#############################################################################

static inline void
BB_pack_cut(const BCP_cut_algo* cut, BCP_buffer& buf)
{
   const BB_cut* bb_cut = dynamic_cast<const BB_cut*>(cut);
   if (!bb_cut)
      throw BCP_fatal_error("BB_lp::pack_cut_algo() : unknown cut type!\n");
   bb_cut->pack(buf);
}

#endif
