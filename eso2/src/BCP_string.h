// Copyright (C) 2000, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef _BCP_STRING_H
#define _BCP_STRING_H

// This file is fully docified.

#include <cstring>
#include <string>

//#include "BCP_os.hpp"
#define BCP_DEFAULT_NAMESPACE

/** This class is a very simple impelementation of a constant length string.
    Using it one can avoid some memory errors related to using functions
    operating on C style strings. */
class BCP_string {
public:
   /* Return the length of the string. */
   int length() const         { return _len; }
   int size()   const         { return length(); }
   /* Return a pointer to the data stored in the string. I.e., return a C
      style string. */
   const char * c_str() const { return _data; }
private:
   /* the length of the string */
   int    _len;
   /* the data in the string */
   char * _data;
public:
   /* The default constructor creates an empty sting. */
   BCP_string() : _len(0), _data(0) {};
   /* Create a <code>BCP_string</code> from a C style string. */
   BCP_string(const char * str) {
      BCP_DEFAULT_NAMESPACE;
      _len = strlen(str);
      _data = new char[_len+1];
      memcpy(_data, str, _len);
      _data[_len] = 0;
   }
   /* Make a copy of the argument string. */
   BCP_string(const BCP_string& str) {
      BCP_DEFAULT_NAMESPACE;
      _len = str.length();
      _data = new char[_len+1];
      memcpy(_data, str.c_str(), _len);
      _data[_len] = 0;
   }
   
   /* This methods append str to *this. */
   BCP_string& append(const BCP_string& str) {
      BCP_DEFAULT_NAMESPACE;
      int newLen = _len + str.length();
      char * newData = new char[newLen+1];
      memcpy(newData, _data, _len);
      memcpy(&(newData[_len]), str.c_str(), str.length());
      newData[newLen] = 0;
      _len = newLen;
      delete [] _data;
      _data = newData;
      return *this;
   }

   /* Searches for str as a substring in *this,. */
   int find(const BCP_string& str) const {
      BCP_DEFAULT_NAMESPACE;
#if 1
      std::string thisString(c_str());
      int retVal = thisString.find(str.c_str());
      if ( retVal == std::string::npos ) retVal = npos;
      return retVal;
#else
      for ( int i=0; i<=length()-str.length(); i++ ) {
        if ( strncmp(c_str(),str.c_str(),str.length()) == 0 ) return i;
      }
      return npos;
#endif
   }


   /* returns a substring of *this. */
   BCP_string substr(int start=0, int len=npos) const {   
      BCP_DEFAULT_NAMESPACE;
      // create substring removing and leading characters.
      BCP_string retVal( &(this->c_str()[start]) ); 
      // now remove any trailing characters
      if ( len < retVal.length() ) {
        retVal._len = len;
        retVal._data[len] = 0;
      }
      return retVal;
   }


   static int npos;

   /* Delete the data members. */
   ~BCP_string() {
      delete[] _data;
   }
   /* This methods replaces the current <code>BCP_string</code> with one
      create from the first <code>len</code> bytes in <code>source</code>. */
   BCP_string& assign(const char * source, const int len) {
      BCP_DEFAULT_NAMESPACE;
      delete[] _data;
      _len = len;
      _data = new char[_len+1];
      memcpy(_data, source, _len);
      _data[_len] = 0;
      return *this;
   }
   /* replace the current <code>BCP_string</code> with a copy of the argument
    */ 
   BCP_string& operator= (const BCP_string& str) {
      return assign(str.c_str(), str.length());
   }
   /* replace the current <code>BCP_string</code> with a copy of the argument
      C style string. */ 
   BCP_string& operator= (const char * str) {
      BCP_DEFAULT_NAMESPACE;
      return assign(str, strlen(str));
   }

   /* Return i1'th character of string */
   char operator[](int i1) const {
      return c_str()[i1];
   }

};

/** Equality tester for a <code>BCP_string</code> and a C style string. */
inline bool operator==(const BCP_string& s0, const char* s1) {
   BCP_DEFAULT_NAMESPACE;
   if (s0.c_str() == 0)
      return s1 == 0;
   return s1 == 0 ? false : (strcmp(s0.c_str(), s1) == 0);
}

/** Equality tester for a C style string and a <code>BCP_string</code>. */
inline bool
operator==(const BCP_string& s0, const BCP_string& s1) {
   return s0 == s1.c_str();
}

/** Equality tester for two <code>BCP_string</code>s. */
inline bool
operator==(const char* s0, const BCP_string& s1) {
   return s1 == s0;
}


/** Not equal tester for a <code>BCP_string</code> and a C style string. */
inline bool operator!=(const BCP_string& s0, const char* s1) {
   BCP_DEFAULT_NAMESPACE;
   return !operator==(s0,s1);
}

/** Not equal tester for a C style string and a <code>BCP_string</code>. */
inline bool
operator!=(const BCP_string& s0, const BCP_string& s1) {
   return !operator==(s0,s1);
}

/** Not equal tester for two <code>BCP_string</code>s. */
inline bool
operator!=(const char* s0, const BCP_string& s1) {
   return !operator==(s0,s1);
}


/** Operator< needed so <code>BCP_string</code> can be key in std::map. */
inline bool
operator<(const BCP_string& s0, const BCP_string& s1) {
   return strcmp(s0.c_str(),s1.c_str()) < 0;
}


/** Concatenation <code>BCP_string</code>s. */
inline BCP_string
operator+(const BCP_string& s0, const BCP_string& s1) {
  BCP_string retVal(s0);
  retVal.append(s1);
  return retVal;
}

#endif
