# include <iostream>
# include <fstream>
# include <string>
# include <sstream>
# include "Dictionary.h"
# include "EndWordObject.h"
# include "ValueObject.h"
# include <vector>
# include <codecvt>
# include <algorithm>
# include <windows.h> 
# include <locale>


# void searchword(Dictionary head[], std::string word, std::vector<std::string> & ArrErrors);
# int insertword(Dictionary head[], std::vector<std::string>& ArrErrors);
# std::wstring stringToWstring(const std::string& str);
# std::string wstringToString(const std::wstring& wstr);

# int main()
# {
  
#   std::vector<std::string> ArrErrors = {"correct"};
#   Dictionary head[27] = {};
  
#   SetConsoleOutputCP(CP_UTF8);
  
#    insertword(head , ArrErrors);
#    searchword(head,"מקלדת",ArrErrors);

#   return 0;
# }




# int insertword(Dictionary head[], std::vector<std::string> & ArrErrors)
# {
#     std::ifstream inFile("words.txt", std::ios::binary);
#     if (!inFile.is_open()) 
#     {
#         std::cout << "Error: Could not open file." << std::endl;
#         return 1;
#     }

#     std::string line;
#     while (std::getline(inFile, line)) 
#     {
#         if (!line.empty())
#         {
#             std::stringstream ss(line);
#             std::string word;
#             Dictionary* nextDict = head;
#             int index = 0;
            
#             ss >> word; 
            
#             // הדפסה 1: המילה הנוכחית (המילה הראשונה בשורה)
#             std::cout << "Word: " << word << std::endl;
            
#             ss >> std::ws;
#             bool ifCorrect = ss.eof();

#             // הדפסה 2: האם המילה תקינה או שגויה
#             if (ifCorrect) {
#                 std::cout << "Status: Correct" << std::endl;
#             } else {
#                 std::cout << "Status: Wrong" << std::endl;
#             }

#             for (int i = 0; i < (int)word.length(); i += 2) 
#             {
#                 std::string thisLetter = word.substr(i, 2);
#                 unsigned char sbyte = word[i + 1];
#                 index = sbyte - 144;
#                 bool isLast = (i + 2 >= (int)word.length());

#                 if (index >= 0 && index <= 26) 
#                 {
#                     nextDict[index].letter = thisLetter;
#                     if (nextDict[index].valueObject == nullptr) 
#                     {
#                         nextDict[index].valueObject = new ValueObject();
#                     }

#                     if (isLast && ifCorrect) 
#                     {
#                         EndWordObject endWordObj;
#                         endWordObj.IndexArrErrors = 0;
#                         nextDict[index].valueObject->endWordObjects.push_back(endWordObj);
#                     }
                    
#                     if (!isLast)
#                     {
#                         if (nextDict[index].valueObject->ArrNextLetters == nullptr) 
#                         {
#                             nextDict[index].valueObject->ArrNextLetters = new Dictionary[27]();
#                         }
#                         nextDict = nextDict[index].valueObject->ArrNextLetters;
#                     }
#                 }
#             }

#             if (!ifCorrect) 
#             {
#                 std::string correctW;
#                 std::string CurrentError;
#                 ss >> correctW; 
                
#                 std::string wordPart;
#                 while (ss >> wordPart) 
#                 {
#                     CurrentError += (CurrentError.empty() ? "" : " ") + wordPart;
#                 }

#                 // הדפסה 3: פירוט השגיאה (אם המילה שגויה)
#                 std::cout << "Error Detail: " << CurrentError << std::endl;

#                 EndWordObject endWordObjL;
#                 endWordObjL.CorrectWord = stringToWstring(correctW);
                
#                 int i = 0;
#                 while (i < (int)ArrErrors.size() && ArrErrors[i] != CurrentError) 
#                 {
#                     i++;
#                 }
                
#                 if (i == (int)ArrErrors.size() && !CurrentError.empty()) 
#                 {
#                     ArrErrors.push_back(CurrentError);
#                 }
                
#                 endWordObjL.IndexArrErrors = (CurrentError.empty() ? 0 : i);
#                 nextDict[index].valueObject->endWordObjects.push_back(endWordObjL);
#             }
#             std::cout << "-------------------" << std::endl;
#         }
#     }
#     inFile.close();
#     return 0;
# }




# std::wstring stringToWstring(const std::string& str)
#  {
#     if (str.empty()) return std::wstring();

#     // 1. חישוב גודל המחרוזת הנדרש (בתווי wchat_t)
#     int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
    
#     // 2. יצירת מחרוזת wstring בגודל המתאים
#     std::wstring wstr(size_needed, 0);
    
#     // 3. ביצוע ההמרה בפועל
#     MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
    
#     return wstr;
# }


# std::string wstringToString(const std::wstring& wstr)
#  {
#     if (wstr.empty()) return std::string();
#     int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
#     std::string strTo(size_needed, 0);
#     WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
#     return strTo;
# }


# void searchword(Dictionary head[], std::string word, std::vector<std::string>& ArrErrors)
# {
#     std::cout << "the word for search: " << word << std::endl;
#     int index = 0;
#     unsigned char sbyte;
#     std::string thisLetter;
#     Dictionary* nextDict = head;
#     int i = 0;

#     // הלולאה תרוץ כל עוד לא חרגנו מאורך המילה וכל עוד לא הגענו למבוי סתום (nextDict אינו nullptr)
#     while (i < (int)word.length() && nextDict != nullptr)
#     {
#         thisLetter = word.substr(i, 2);
#         //std::cout << "This letter that i'm searching now: " << thisLetter << std::endl;
#         sbyte = word[i + 1];
#         index = sbyte - 144;

#         if (index >= 0 && index <= 26 && nextDict[index].letter == thisLetter)
#         {
#             if (i + 2 < (int)word.length()) // אות אמצעית
#             {
#                 std::cout << "This letter in the middle: " << thisLetter << std::endl;
#                 if (nextDict[index].valueObject != nullptr && nextDict[index].valueObject->ArrNextLetters != nullptr)
#                 {
#                     nextDict = nextDict[index].valueObject->ArrNextLetters;
#                     i += 2; 
#                 }
#                 else
#                 {
#                     std::cout << "There word does'nt exist" << std::endl;
#                     nextDict = nullptr; // יגרום להפסקת הלולאה
#                 }
#             }
#             else // אות אחרונה
#             {
#                 std::cout << "This letter in the last: " << thisLetter << std::endl;
#                 if (nextDict[index].valueObject == nullptr || nextDict[index].valueObject->endWordObjects.empty())
#                 {
#                     std::cout << "Not found" << std::endl;
#                 }
#                 else
#                 {
#                     int size = nextDict[index].valueObject->endWordObjects.size();
                    
#                     for (int j = 0; j < size; j++)
#                     {
#                         if (ArrErrors[nextDict[index].valueObject->endWordObjects[j].IndexArrErrors] == "correct")
#                         {
#                             std::cout << "There word is correct" << std::endl;
#                         }
#                         else
#                         {
#                             std::cout << j << ":";
#                             std::cout << "correct word: " << wstringToString(nextDict[index].valueObject->endWordObjects[j].CorrectWord) << std::endl;
#                             std::cout << "wrong: " << ArrErrors[nextDict[index].valueObject->endWordObjects[j].IndexArrErrors] << std::endl;
#                         }
#                     }
#                 }
#                 nextDict = nullptr; // סימן לסיום החיפוש בהצלחה, יגרום להפסקת הלולאה
#                 i += 2;
#             }
#         }
#         else
#         {
#             std::cout << "There word does'nt exist" << std::endl;
#             nextDict = nullptr; // יגרום להפסקת הלולאה
#         }
#     }

#     if (i == 0 && word.empty())
#     {
#         std::cout << "This word is empty" << std::endl;
#     }
# }





