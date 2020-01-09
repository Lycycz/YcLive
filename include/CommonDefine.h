#pragma once

#define SAFE_DELETE(p)          if(nullptr != (p)) {delete (p); (p) = nullptr;}
#define SAFE_DELETE_ARRAY(p)    if(nullptr != (p)) {delete[] (p); (p) = nullptr;}
typedef unsigned short     ushort;
typedef unsigned char      uchar;
typedef unsigned int       uint;
typedef unsigned long      ulong;
typedef long long          long64;
typedef unsigned long long ulong64;