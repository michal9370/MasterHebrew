#include <string>
#include <vector>


size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
void extractPartsOfSpeech(const std::string& html, std::vector<std::string>& results) ;
void pos(std::string word);

