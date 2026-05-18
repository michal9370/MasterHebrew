
#include "Syntax.h"
#include <curl/curl.h>
#include <iostream>
#include <limits>
#include <regex>

// פונקציית עזר לקבלת נתונים מהאינטרנט
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}



// פונקציית עזר להסרת ניקוד מהמילה שחזרה מהאתר
std::string removeNiqqud(std::string text) {
    // טווח התווים של הניקוד בעברית ב-Unicode הוא U+0591 עד U+05C7
    std::regex niqqud_regex("[\u0591-\u05C7]");
    return std::regex_replace(text, niqqud_regex, "");
}

void extractPartsOfSpeech(const std::string& html, const std::string& originalWord, std::vector<std::string>& results) {
    results.clear();
    
    // נחפש את תחילת הבלוק של כל תוצאת חיפוש באתר
    std::string resultBlockTag = "class=\"vf-search-result\"";
    std::string posTag = "חלק דיבר: </span>";
    
    size_t currentPos = 0;

    while ((currentPos = html.find(resultBlockTag, currentPos)) != std::string::npos) {
        
        // נמצא את גבולות הבלוק הנוכחי (למשל עד הבלוק הבא או סוף הדיב)
        size_t nextBlockPos = html.find(resultBlockTag, currentPos + resultBlockTag.length());
        size_t blockEnd = (nextBlockPos != std::string::npos) ? nextBlockPos : html.length();
        
        // נבודד את כל ה-HTML של כרטיס התוצאה הנוכחי
        std::string blockHtml = html.substr(currentPos, blockEnd - currentPos);
        
        // ננקה את כל תגיות ה-HTML מהבלוק כדי להישאר רק עם טקסט נקי
        std::string cleanText = "";
        bool inTag = false;
        for (char c : blockHtml) {
            if (c == '<') inTag = true;
            else if (c == '>') inTag = false;
            else if (!inTag) {
                // נסנן גם סימני קריאה (!) כדי ש"שלי!" יהפוך ל"שלי"
                if (c != '!') {
                    cleanText += c;
                }
            }
        }
        
        // נסיר ניקוד מהטקסט הנקי של הבלוק
        std::string cleanTextNoNiqqud = removeNiqqud(cleanText);
        
        // בדיקה: האם המילה שחיפשנו קיימת בתוך הטקסט של הבלוק הזה?
        if (cleanTextNoNiqqud.find(originalWord) != std::string::npos) {
            
            // אם כן, נחפש את חלק הדיבר בתוך ה-HTML של הבלוק הזה
            size_t posStart = blockHtml.find(posTag);
            if (posStart != std::string::npos) {
                size_t startContent = posStart + posTag.length();
                size_t endContent = blockHtml.find("<", startContent);
                
                if (endContent != std::string::npos) {
                    std::string partOfSpeech = blockHtml.substr(startContent, endContent - startContent);
                    
                    // ניקוי רווחים ושורות חדשות
                    partOfSpeech.erase(0, partOfSpeech.find_first_not_of(" \t\n\r"));
                    partOfSpeech.erase(partOfSpeech.find_last_not_of(" \t\n\r") + 1);
                    
                    // הוספה לתוצאות ללא כפילויות
                    if (std::find(results.begin(), results.end(), partOfSpeech) == results.end()) {
                        results.push_back(partOfSpeech);
                    }
                }
            }
        }
        
        // מתקדמים לבלוק הבא
        currentPos += resultBlockTag.length();
    }
}

void pos(std::string word) {
    // אין צורך ב-std::cin או std::getline כאן יותר

    CURL* curl = curl_easy_init();
    std::string readBuffer;

    if (curl) {
        // קידוד המילה לפורמט URL
        char* encodedWord = curl_easy_escape(curl, word.c_str(), (int)word.length());
        std::string url = "https://www.pealim.com/he/search/?q=" + std::string(encodedWord);
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        CURLcode res = curl_easy_perform(curl);
        
        if (res == CURLE_OK) {
            std::vector<std::string> parts;
            extractPartsOfSpeech(readBuffer, word, parts);

            // הדפסת חלקי הדיבר בלבד
            for (const auto& p : parts) {
                std::cout << p << std::endl;
            }
        }

        curl_free(encodedWord);
        curl_easy_cleanup(curl);
    }
}
