// #include "Dictionary.h"
// #include "EndWordObject.h"
// #include "ValueObject.h"
// #include <iostream>
// #include <fcntl.h> 
// #include <io.h>    
// #include <stdio.h> 
// #include <fstream>   // ספרייה לקריאת קבצים
// #include <string>    // ספרייה למחרוזות
// #include <vector>    // ספרייה למערך דינמי
// #include <locale>


// int mainy()
// {
// _setmode(_fileno(stdout), _O_U16TEXT);
// _setmode(_fileno(stdin), _O_U16TEXT);

// Dictionary Head[27]; //מערך ראש של  במבנה המילים 
// std::wstring word; //משתנה לקליטת המילה הנוכחית מהקובץ
// std::wstring correctword; //משתנה לקליטת המילה התקנית מהקובץ כאשר מדובר במילה שגויה להכנסה
// std::wstring error; //משתנה לקליטת השגיאה מהקובץ



// /////


// _setmode(_fileno(stdout), _O_U16TEXT);
// _setmode(_fileno(stdin), _O_U16TEXT);

// Dictionary Head[27];

// std::vector<std::wstring> wordsArray;
// std::wifstream inputFile("words.txt");
// if (!inputFile.is_open()) {
//     std::wcerr << L"Error: Could not open the file!" << std::endl;
//     return 1;}
// else 
//     std::wcout << L"File opened successfully!" << std::endl;

// std::wstring tempWord;
//     while (std::getline(inputFile, tempWord))
//     {
//         wordsArray.push_back(tempWord); // הוספת המילה לסוף המערך
//         if (!tempWord.empty() && tempWord[0] == 0xFEFF) tempWord.erase(0, 1);
//     }
// inputFile.close();
// std::wcout << L"Read " << wordsArray.size() << L" words from file:" << std::endl;
//     for (int i = 0; i < wordsArray.size(); i++) {
//         std::wcout << L"Word [" << i << L"]: " << wordsArray[i] << std::endl;
//     }


// for (int i=0; i<27; i++)
// {
//  Head[i].letter= (wchar_t)(1488 + i);
// }

// for (int i = 0; i < 27; i++)
// {
//     //wchar_t currentLetter = head[i].letter;
//    // int unicodeValue = int(head[i].letter);
//     //std::wcout << L"Letter: " << currentLetter << L" | Code: " << unicodeValue << std::endl;
// }
// return 0;
// }


