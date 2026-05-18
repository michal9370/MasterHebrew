cmake_minimum_required(VERSION 3.10)
project(MasterHebrew)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. פקודה שמחפשת את ספריית curl במערכת
find_package(CURL REQUIRED)

# 2. הגדרת קבצי ה-CPP של הפרויקט
add_executable(MasterHebrew 
    Main.cpp 
    Syntax.cpp 
    Dictionary.cpp
    EndWordObject.cpp  
ValueObject.cpp     
Lexical.cpp

)

# 3. קישור קבצי ה-Header של curl לפרויקט
target_include_directories(MasterHebrew PRIVATE ${CURL_INCLUDE_DIRS})

# 4. קישור קובץ הספרייה הבינארי (.lib / .a) לפרויקט
target_link_libraries(MasterHebrew PRIVATE ${CURL_LIBRARIES})

