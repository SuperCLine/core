#include "stdafx.h"
#include "core_util.h"
#include "sys/stat.h"


__BEGIN_NAMESPACE

float32 core_util::PI = 3.1415926535898f;

bool core_util::is_zero(float32 val, float32 epsl /*= 0.000001f*/)
{
	return app_fabs(val) < epsl;
}

bool core_util::is_equal(float32 v1, float32 v2)
{
	return is_zero(v1 - v2);
}

float32 core_util::deg_to_rad(float32 degree)
{
	return PI * degree / 180;
}

float32 core_util::rad_to_deg(float32 rad)
{
	return 180 * rad / PI;
}

void core_util::to_lower(ustring& str)
{
	std::transform(
		str.begin(),
		str.end(),
		str.begin(),
		app_tolower);
}

ustring core_util::to_string(bool val, bool yesNo /*= false*/)
{
	if (val)
	{
		return yesNo ? "yes" : "true";
	}
	else
	{
		return yesNo ? "no" : "false";
	}
}

ustring core_util::to_string(int32 val, uint16 width /*= 0*/, char fill /*= ' '*/, std::ios::fmtflags flags /*= std::ios::fmtflags(0)*/)
{
	ustringstream stream;
	stream.width(width);
	stream.fill(fill);
	if (flags)
		stream.setf(flags);
	stream << val;
	return stream.str();
}

ustring core_util::to_string(uint32 val, uint16 width /*= 0*/, char fill /*= ' '*/, std::ios::fmtflags flags /*= std::ios::fmtflags(0)*/)
{
	ustringstream stream;
	stream.width(width);
	stream.fill(fill);
	if (flags)
		stream.setf(flags);
	stream << val;
	return stream.str();
}

ustring core_util::to_string(float32 val, uint16 precision /*= 6*/, uint16 width /*= 0*/, char fill /*= ' '*/, std::ios::fmtflags flags /*= std::ios::fmtflags(0)*/)
{
	ustringstream stream;
	stream.precision(precision);
	stream.width(width);
	stream.fill(fill);
	if (flags)
		stream.setf(flags);
	stream << val;
	return stream.str();
}

ustring core_util::to_string(float64 val, uint16 precision /*= 6*/, uint16 width /*= 0*/, char fill /*= ' '*/, std::ios::fmtflags flags /*= std::ios::fmtflags(0)*/)
{
	ustringstream stream;
	stream.precision(precision);
	stream.width(width);
	stream.fill(fill);
	if (flags)
		stream.setf(flags);
	stream << val;
	return stream.str();
}

bool core_util::parse_bool(const ustring& val, bool default_val /*= 0*/)
{
	if ((starts_with(val, _T("true")) || starts_with(val, _T("yes")) || starts_with(val, _T("1"))))
		return true;
	else if ((starts_with(val, _T("false")) || starts_with(val, _T("no")) || starts_with(val, _T("0"))))
		return false;
	else
		return default_val;
}

float32 core_util::parse_float32(const ustring& val, float32 default_val /*= 0*/)
{
	ustringstream str(val);
	float ret = default_val;
	if (!(str >> ret))
		return default_val;

	return ret;
}

uint32 core_util::parse_uint32(const ustring& val, uint32 default_val /*= 0*/)
{
	ustringstream str(val);
	unsigned int ret = default_val;
	if (!(str >> ret))
		return default_val;

	return ret;
}

int32 core_util::parse_int32(const ustring& val, int32 default_val /*= 0*/)
{
	ustringstream str(val);
	int ret = default_val;
	if (!(str >> ret))
		return default_val;

	return ret;
}

bool core_util::starts_with(const ustring& str, const ustring& pattern, bool lowerCase /*= true*/)
{
	size_t thisLen = str.length();
	size_t patternLen = pattern.length();
	if (thisLen < patternLen || patternLen == 0)
		return false;

	ustring startOfThis = str.substr(0, patternLen);
	if (lowerCase)
		to_lower(startOfThis);

	return (startOfThis == pattern);
}

void core_util::trim_right(ustring& str, const char* p)
{
	if (!p) return;

	size_t nPos = str.find_last_of(p);

	while (nPos != std::string::npos && nPos + 1 == str.size())
	{
		str = str.substr(0, nPos);
		nPos = str.find_last_of(p);
	}
}

void core_util::trim_left(ustring& str, const char* p)
{
	if (!p) return;

	str = str.substr(str.find_first_not_of(p), str.size());
}

void core_util::trim(ustring& str, const char* p /*= " \t\n\r"*/)
{
	if (!p) return;

	trim_right(str, p);
	trim_left(str, p);
}

void core_util::cut_right(ustring& str, uint32 n)
{
	if (n >= str.size())
	{
		str.clear();
	}
	else
	{
		str.erase(str.size() - n, n);
	}
}

void core_util::cut_left(ustring& str, uint32 n)
{
	str.erase(0, n);
}

void core_util::replace(ustring& str, const ustring& oriStr, const ustring& newStr)
{
	size_t nIdx = str.find(oriStr);

	while (nIdx != std::string::npos)
	{
		str.replace(str.begin() + nIdx, str.begin() + nIdx + oriStr.size(), newStr);

		nIdx = str.find(oriStr, nIdx + newStr.size());
	}
}

void core_util::replace(ustring& str, char oric, char newc)
{
	size_t nIdx = str.find(oric);

	while (nIdx != std::string::npos)
	{
		str.replace(str.begin() + nIdx, str.begin() + nIdx + 1, 1, newc);

		nIdx = str.find(oric, nIdx + 1);
	}
}

struct TComparePred //C++17 removed: public std::binary_function<std::string::value_type, std::string::value_type, bool> 
{
	bool operator()(std::string::value_type val, std::string::value_type itr_val)
	{
		return app_toupper(val) == app_toupper(itr_val);
	}
};

bool core_util::compare_nocase(const ustring& l, const ustring& r)
{
	return l.size() == r.size() && std::equal(l.begin(), l.end(), r.begin(), TComparePred());
}

uvector<ustring> split(const char* str, char delimiter)
{
	uvector<ustring> tokens;
	std::string token;
	std::istringstream token_stream(str);
	while (std::getline(token_stream, token, delimiter))
	{
		tokens.push_back(token);
	}

	return tokens;
}

ustring core_util::get_file_path(const char* path)
{
	ustring tmp(path);
	ustring::size_type pos = tmp.find_last_of(app_dir_separator);

	return tmp.substr(0, pos + 1);
}

ustring core_util::get_file_fullname(const char* path)
{
	ustring tmp(path);
	ustring::size_type pos = tmp.find_last_of(app_dir_separator);

	return tmp.substr(pos + 1);
}

ustring core_util::get_file_name(const char* path)
{
	ustring tmp(path);
	ustring::size_type pos_separator = tmp.find_last_of(app_dir_separator);
	ustring::size_type pos_dot = tmp.find_last_of(app_dir_dot);

	return tmp.substr(pos_separator + 1, pos_dot - pos_separator - 1);
}

ustring core_util::get_file_basename(const char* name, char sep /*= '_'*/)
{
	ustring tmp(name);
	ustring::size_type pos = tmp.find_last_of(sep);

	return tmp.substr(0, pos + 1);
}

ustring core_util::get_file_extension(const char* path)
{
	ustring tmp(path);
	ustring::size_type pos = tmp.find_last_of(app_dir_dot);

	return tmp.substr(pos + 1);
}

bool core_util::file_exist(const char* file)
{
	struct stat buffer;
	return (stat(file, &buffer) == 0);
}

int32 core_util::random_int32(int32 max)
{
	srand((uint32)core_time::get_calendar_time());
	return rand() % max;
}

float32 core_util::random_float32(void)
{
	srand((uint32)core_time::get_calendar_time());
	return (float32)rand() / (float32)RAND_MAX;
}

__END_NAMESPACE
