#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <codecvt>
#include <algorithm>
#include <windows.h> 
#include <locale>

#include "Dictionary.h"
#include "EndWordObject.h"
#include "ValueObject.h"

#include "Syntax.h"
#include "Lexical.h"

int main()
{
  
  std::vector<std::string> ArrErrors = {"correct"};
  Dictionary head[27] = {};
  
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
  
   insertword(head , ArrErrors);
   searchword(head,"מקלדת",ArrErrors);
   std::string testWord = "שלי"; 
   pos(testWord);

   return 0;
}








