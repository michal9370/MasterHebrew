#include <Syntax.h>
#include <curl/curl.h>
#include <iostream>
#include <limits>

// פונקציית עזר לקבלת נתונים מהאינטרנט
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

#include <regex>

// פונקציית עזר להסרת ניקוד מהמילה שחזרה מהאתר
std::string removeNiqqud(std::string text) {
    // טווח התווים של הניקוד בעברית ב-Unicode הוא U+0591 עד U+05C7
    std::regex niqqud_regex("[\u0591-\u05C7]");
    return std::regex_replace(text, niqqud_regex, "");
}

void extractPartsOfSpeech(const std::string& html, const std::string& originalWord, std::vector<std::string>& results) {
    results.clear();
    
    std::string lemmaTag = "<span class=\"menukad\">";
    std::string posTag = "חלק דיבר: </span>";
    
    size_t currentPos = 0;

    while ((currentPos = html.find(lemmaTag, currentPos)) != std::string::npos) {
        // 1. חילוץ המילה המנוקדת
        currentPos += lemmaTag.length();
        size_t endLemma = html.find("</span>", currentPos);
        if (endLemma == std::string::npos) break;
        
        std::string foundWordWithNiqqud = html.substr(currentPos, endLemma - currentPos);
        std::string cleanFoundWord = removeNiqqud(foundWordWithNiqqud);

        // 2. מציאת חלק הדיבר הקרוב ביותר אחרי המילה הזו
        size_t nextPosStart = html.find(posTag, endLemma);
        if (nextPosStart != std::string::npos) {
            size_t startContent = nextPosStart + posTag.length();
            size_t endContent = html.find("<", startContent);
            std::string partOfSpeech = html.substr(startContent, endContent - startContent);

            // 3. השוואה: האם המילה שמצאנו (ללא ניקוד) תואמת למילה שחיפשנו?
            if (cleanFoundWord == originalWord) {
                // ניקוי רווחים
                partOfSpeech.erase(0, partOfSpeech.find_first_not_of(" \t\n\r"));
                partOfSpeech.erase(partOfSpeech.find_last_not_of(" \t\n\r") + 1);
                results.push_back(partOfSpeech);
            }
        }
        currentPos = endLemma;
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
            extractPartsOfSpeech(readBuffer, parts);

            // הדפסת חלקי הדיבר בלבד
            for (const auto& p : parts) {
                std::cout << p << std::endl;
            }
        }

        curl_free(encodedWord);
        curl_easy_cleanup(curl);
    }
}
