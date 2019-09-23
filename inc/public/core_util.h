/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core\inc\public\core_util.h
| AUTHOR     : CLine
| PURPOSE    :
|
| SPEC       :
|
| MODIFICATION HISTORY
|
| Ver      Date            By              Details
| -----    -----------    -------------   ----------------------
| 1.0      2019-9-20      CLine           Created
|
+-----------------------------------------------------------------------------*/


#ifndef _CORE_UTIL_H_
#define _CORE_UTIL_H_


__BEGIN_NAMESPACE

//util
template<class T> inline void util_swap(T& a, T& b) { T t = a; a = b; b = t; }
template<class T> inline T util_min(T a, T b) { return a < b ? a : b; }
template<class T> inline T util_max(T a, T b) { return a > b ? a : b; }
template<class T> inline T util_abs(T a) { return a < 0 ? -a : a; }
template<class T> inline T util_sqr(T a) { return a * a; }
template<class T> inline T util_clamp(T v, T mn, T mx) { return v < mn ? mn : (v > mx ? mx : v); }


class CORE_API core_util
{
public:
	static float32 PI;

	static bool is_zero(float32 val, float32 epsl = 0.000001f);
	static bool is_equal(float32 v1, float32 v2);

	static float32 deg_to_rad(float32 degree);
	static float32 rad_to_deg(float32 rad);

	static void  to_lower(ustring& str);

	static ustring to_string(bool val, bool yesNo = false);
	static ustring to_string(int32 val, uint16 width = 0, char fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));
	static ustring to_string(uint32 val, uint16 width = 0, char fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));
	static ustring to_string(float32 val, uint16 precision = 6, uint16 width = 0, char fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));
	static ustring to_string(float64 val, uint16 precision = 6, uint16 width = 0, char fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));
	
	static bool  parse_bool(const ustring& val, bool default_val = 0);
	static float32 parse_float32(const ustring& val, float32 default_val = 0);
	static uint32 parse_uint32(const ustring& val, uint32 default_val = 0);
	static int32 parse_int32(const ustring& val, int32 default_val = 0);

	static bool  starts_with(const ustring& str, const ustring& pattern, bool lowerCase = true);

	static void trim_right(ustring& str, const char* p);
	static void trim_left(ustring& str, const char* p);
	static void trim(ustring& str, const char* p = " \t\n\r");
	static void cut_right(ustring& str, uint32 n);
	static void cut_left(ustring& str, uint32 n);
	static void replace(ustring& str, const ustring& oriStr, const ustring& newStr);
	static void replace(ustring& str, char oric, char newc);
	static bool compare_nocase(const ustring& l, const ustring& r);

	static uvector<ustring> split(const char* str, const char* seperator);

	static ustring get_file_path(const char* path);
	static ustring get_file_fullname(const char* path);
	static ustring get_file_name(const char* path);
	static ustring get_file_basename(const char* name, char sep = '_');
	static ustring get_file_extension(const char* path);
	static bool file_exist(const char* file);

	static int32 random_int32(int32 max);
	static float32 random_float32(void);
};


__END_NAMESPACE


#endif //_CORE_UTIL_H_