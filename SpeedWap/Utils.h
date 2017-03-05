#ifndef __UTILS_H
#define __UTILS_H
#include <string>
class Utils
{
public:
	Utils(void);
	static std::string char2HexString(std::string& szBuffer);
	static std::string string2BCD(std::string& mobile, int& out_len);
	static void Dec2Hex(int val, char* result);
	~Utils(void);
};
#endif

