#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <type_traits>

using namespace std;
namespace
{
   typedef  const char cchar;
   typedef  const double cdouble;
   typedef  const int cint;

   typedef  unsigned int uint;
   typedef  const unsigned int cuint;

   typedef  char int8;
   typedef  const char cint8;

   typedef  unsigned char uint8;
   typedef  const unsigned char cuint8;

   typedef  short int16;
   typedef  const short cint16;

   typedef  unsigned short uint16;
   typedef  const unsigned short cuint16;

   typedef  int int32;
   typedef  const int cint32;

   typedef  unsigned int uint32;
   typedef  const unsigned int cuint32;

   typedef  long long  int64;
   typedef  const long long cint64;

   typedef  unsigned long long uint64;
   typedef  const unsigned long long cuint64;

   typedef  unsigned long ulong;
   typedef  const unsigned long culong;


   typedef  long long lli;
   typedef  unsigned long long ulli;
   typedef  const unsigned long long culli;

   typedef unsigned char uchar;
   typedef unsigned const char ucchar;
}

namespace concepts
{
   template <class T>
   concept bool const_convertible_t = is_convertible<T, const T>::value;

   template <class T>
   concept bool uchar_t = is_convertible<T, unsigned char*>::value || const_convertible_t<T>;

   template <class T>
   concept bool arr_t = std::is_array<T[]>::value;

   template <class T>
   concept bool char_t = arr_t<T> || uchar_t<T>;
}

constexpr unsigned short BUFSIZE = 512;

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#if __cplusplus > 201402L
template <typename T>
using  init_list = std::initializer_list<T>;
#endif // __cplusplus

#if __cplusplus
#if __GNUC__ == 6
#include <string>
//using string = std::__cxx11::string;
typedef std::basic_string<char> string;
typedef std::basic_string<unsigned char> ustring;
#endif // __GNUC__
#endif // __cplusplus

#if __cpp_fold_expressions >= 201603
/// sizeof... operator: Queries the number of elements in a parameter pack.
///
template <class ...T, class ...Names>
void types_size(Names&... names)
{
   std::string s = "";
   cchar* nms[] = { names...};
   for (uint i = 0; i < sizeof...(Names); ++i) ///
      s.append(nms[i]).append(": {}\n");
#ifdef FMT_API
   fmt::print(s, sizeof(T)...);
#endif // FMT_API
}

template <class T = void>
void type_size_usage()
{
   types_size<size_t, long, uint32, uint16, uint8>("size_t", "long", "uint32", "uint16", "uint8");
}
#endif // __cpp_fold_expressions

#endif//TYPEDEFS_H

