#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <codecvt>
#include <algorithm>
#include <windows.h> 
#include <locale>

#include "Lexical.h"


int insertword(Dictionary head[], std::vector<std::string> & ArrErrors)
{
    // פתיחת זרם קריאה מהקובץ במצב בינארי כדי לשמור על מבנה התווים המקורי
    std::ifstream inFile("../words.txt", std::ios::binary);
    if (!inFile.is_open()) 
    {
        std::cout << "Error: Could not open file." << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(inFile, line)) 
    {
        if (!line.empty())
        {
            std::stringstream ss(line);
            std::string word;
            Dictionary* nextDict = head;
            int index = 0;
            
             // קריאת המילה הראשונה בשורה
            ss >> word;
            
            // הדפסה 1: הצגת המילה שמעובדת כעת
            //std::cout << "Word: " << word << std::endl;
            
            // דילוג על רווחים מיותרים בשורה
            ss >> std::ws;

            // אם הגענו לסוף השורה, המילה תקינה (אין מילה תקינה בהמשך)
            bool ifCorrect = ss.eof();

            // הדפסה 2: האם המילה תקינה או שגויה
            if (ifCorrect) {
                //std::cout << "Status: Correct" << std::endl;
            } else {
                //std::cout << "Status: Wrong" << std::endl;
            }

            // UTF-8 לולאה למעבר על אותיות המילה הנוכחית. בדילוגים של 2 בתים עבור תווים בעברית בקידוד
            for (int i = 0; i < (int)word.length(); i += 2) 
            {
                std::string thisLetter = word.substr(i, 2);
                unsigned char sbyte = word[i + 1];
                index = sbyte - 144;

                // בדיקה אם זו האות האחרונה במילה
                bool isLast = (i + 2 >= (int)word.length()); 

                if (index >= 0 && index <= 26) 
                {
                    nextDict[index].letter = thisLetter;
                    if (nextDict[index].valueObject == nullptr) 
                    {
                        nextDict[index].valueObject = new ValueObject();
                    }

                    if (isLast && ifCorrect) 
                    {
                        EndWordObject endWordObj;
                        endWordObj.IndexArrErrors = 0;
                        nextDict[index].valueObject->endWordObjects.push_back(endWordObj);
                    }
                    
                    if (!isLast)
                    {
                        if (nextDict[index].valueObject->ArrNextLetters == nullptr) 
                        {
                            nextDict[index].valueObject->ArrNextLetters = new Dictionary[27]();
                        }
                        nextDict = nextDict[index].valueObject->ArrNextLetters;
                    }
                }
            }

            if (!ifCorrect) 
            {
                std::string correctW;
                std::string CurrentError;

                // קריאת המילה המתוקנת (המילה השנייה בשורה)
                ss >> correctW;  
                
                std::string wordPart;

                // קריאת שאר המילים בשורה המהוות את תיאור השגיאה
                while (ss >> wordPart) 
                {
                    CurrentError += (CurrentError.empty() ? "" : " ") + wordPart;
                }

                // הדפסה 3: פירוט השגיאה (אם המילה שגויה)
                //std::cout << "Error Detail: " << CurrentError << std::endl;

                EndWordObject endWordObjL;
                endWordObjL.CorrectWord = stringToWstring(correctW);
                
                // חיפוש האם תיאור השגיאה כבר קיים בוקטור השגיאות
                int i = 0;
                while (i < (int)ArrErrors.size() && ArrErrors[i] != CurrentError) 
                {
                    i++;
                }
                
                // אם השגיאה חדשה, נוסיף אותה למאגר השגיאות
                if (i == (int)ArrErrors.size() && !CurrentError.empty()) 
                {
                    ArrErrors.push_back(CurrentError);
                }
                
                endWordObjL.IndexArrErrors = (CurrentError.empty() ? 0 : i);
                nextDict[index].valueObject->endWordObjects.push_back(endWordObjL);
            }

            // הדפסה 5: הפרדה ויזואלית בקונסולה בין מילה למילה
            //std::cout << "-------------------" << std::endl;
        }
    }
    inFile.close();
    return 0;
}




std::wstring stringToWstring(const std::string& str)
 {
    if (str.empty()) return std::wstring();

    // 1. חישוב גודל המחרוזת הנדרש (בתווי wchat_t)
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
    
    // 2. יצירת מחרוזת wstring בגודל המתאים
    std::wstring wstr(size_needed, 0);
    
    // 3. ביצוע ההמרה בפועל
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
    
    return wstr;
}


std::string wstringToString(const std::wstring& wstr)
 {
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}


void searchword(Dictionary head[], std::string word, std::vector<std::string>& ArrErrors)
{
    //הדפסה 1: הדפסת המילה לחיפוש
    std::cout << "the word for search: " << word << std::endl;
    int index = 0;
    unsigned char sbyte;
    std::string thisLetter;
    Dictionary* nextDict = head;
    int i = 0;

    while (i < (int)word.length() && nextDict != nullptr)
    {
        thisLetter = word.substr(i, 2);
        sbyte = word[i + 1];
        index = sbyte - 144;

        if (index >= 0 && index <= 26 && nextDict[index].letter == thisLetter)
        {
            if (i + 2 < (int)word.length()) // אות אמצעית
            {
                
                if (nextDict[index].valueObject != nullptr && nextDict[index].valueObject->ArrNextLetters != nullptr)
                {
                    nextDict = nextDict[index].valueObject->ArrNextLetters;
                    i += 2; 
                }
                else
                {
                    // אם אין המשך לעץ באמצע המילה - המילה לא קיימת
                    std::cout << "There word does'nt exist" << std::endl;
                    nextDict = nullptr; // יגרום להפסקת הלולאה
                }
            }
            else // אות אחרונה
            {
                
                if (nextDict[index].valueObject == nullptr || nextDict[index].valueObject->endWordObjects.empty())
                {
                    // המילה קיימת בעץ אך לא מסומנת כסוף מילה
                    std::cout << "Not found" << std::endl;
                }
                else
                {
                    int size = nextDict[index].valueObject->endWordObjects.size();
                    
                    for (int j = 0; j < size; j++)
                    {
                        if (ArrErrors[nextDict[index].valueObject->endWordObjects[j].IndexArrErrors] == "correct")
                        {
                            std::cout << "There word is correct" << std::endl;
                        }
                        else
                        {
                            // הדפסת פרטי השגיאה/ות והמילה/ים המתוקנת/ות עבור מילה שגויה
                            std::cout << j << ":";
                            std::cout << "correct word: " << wstringToString(nextDict[index].valueObject->endWordObjects[j].CorrectWord) << std::endl;
                            std::cout << "wrong: " << ArrErrors[nextDict[index].valueObject->endWordObjects[j].IndexArrErrors] << std::endl;
                        }
                    }
                }
                nextDict = nullptr; // סימן לסיום החיפוש בהצלחה, יגרום להפסקת הלולאה
                i += 2;
            }
        }
        else
        {
            // אם האות לא נמצאה במיקום הנוכחי בעץ או שיש בה תו לא חוקי
            std::cout << "There word does'nt exist" << std::endl;
            nextDict = nullptr; // יגרום להפסקת הלולאה
        }
    }

    if (i == 0 && word.empty())
    {
        // המילה שהוזנה לחיפוש הייתה ריקה
        std::cout << "This word is empty" << std::endl;
    }
}