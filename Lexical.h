#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Dictionary.h"
#include "EndWordObject.h"
#include "ValueObject.h"
#include <vector>
#include <codecvt>
#include <algorithm>
#include <windows.h> 
#include <locale>


void searchword(Dictionary head[], std::string word, std::vector<std::string> & ArrErrors);
int insertword(Dictionary head[], std::vector<std::string>& ArrErrors);
std::wstring stringToWstring(const std::string& str);
std::string wstringToString(const std::wstring& wstr);