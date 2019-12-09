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
		[](uint8 c) -> uint8 { return (uint8)app_tolower(c); });
}

void core_util::to_upper(ustring& str)
{
	std::transform(
		str.begin(),
		str.end(),
		str.begin(),
		[](uint8 c) -> uint8 { return (uint8)app_toupper(c); });
}

ustring core_util::to_string(bool val, bool yes_no /*= false*/)
{
	if (val)
	{
		return yes_no ? "yes" : "true";
	}
	else
	{
		return yes_no ? "no" : "false";
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
	if ((starts_with(val, app_makestr("true")) || starts_with(val, app_makestr("yes")) || starts_with(val, app_makestr("1"))))
		return true;
	else if ((starts_with(val, app_makestr("false")) || starts_with(val, app_makestr("no")) || starts_with(val, app_makestr("0"))))
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

bool core_util::starts_with(const ustring& str, const ustring& pattern, bool lower_case /*= true*/)
{
	size_t thisLen = str.length();
	size_t patternLen = pattern.length();
	if (thisLen < patternLen || patternLen == 0)
		return false;

	ustring start_of = str.substr(0, patternLen);
	if (lower_case)
		to_lower(start_of);

	return (start_of == pattern);
}

void core_util::trim_right(ustring& str, const char* p)
{
	if (!p) return;

	size_t pos = str.find_last_of(p);

	while (pos != std::string::npos && pos + 1 == str.size())
	{
		str = str.substr(0, pos);
		pos = str.find_last_of(p);
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

void core_util::replace(ustring& str, const ustring& ori_str, const ustring& new_str)
{
	size_t index = str.find(ori_str);

	while (index != std::string::npos)
	{
		str.replace(str.begin() + index, str.begin() + index + ori_str.size(), new_str);

		index = str.find(ori_str, index + new_str.size());
	}
}

void core_util::replace(ustring& str, char oric, char newc)
{
	size_t index = str.find(oric);

	while (index != std::string::npos)
	{
		str.replace(str.begin() + index, str.begin() + index + 1, 1, newc);

		index = str.find(oric, index + 1);
	}
}

struct TComparePred //TO CLine: C++17 removed public std::binary_function<std::string::value_type, std::string::value_type, bool> 
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

uvector<ustring> core_util::split(const char* str, char delimiter)
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

	return tmp.substr(0, pos);
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

int32 core_util::random_int32(int32 min, int32 max)
{
	static short inc = 1;
	srand((uint32)core_time::get_calendar_time() + (inc++));
	return min + rand() % (max - min);
}

int32 core_util::random_int32(int32 max)
{
	return random_int32(0, max);
}

float32 core_util::random_float32(float32 min, float32 max)
{
	srand((uint32)core_time::get_unix_nano(core_time::now()));
	return min + (max - min) * rand() / ((float32)RAND_MAX);
}

float32 core_util::random_float32(void)
{
	return random_float32(0, 1);
}

__END_NAMESPACE
