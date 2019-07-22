// RW2STL -- inserted:
// End of RW2STL -- inserted includes.
// Utilities for objects that use hashing.
// This file is meant to be included in .C files.

const
float
goodHashTableFullness (.8f);	// ratio between # of entries and # of buckets
				// that typically gives good performance

static
inline
int
goodSize (const size_t n)
{
  return (int)(   (size_tMax(n,1) / goodHashTableFullness)  +  1   );
}
