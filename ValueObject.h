#pragma once // מונע טעינה כפולה של הקובץ
#include "EndWordObject.h"
#include "ValueObject.h"
#include <vector>


class Dictionary;

class ValueObject
{
    public:
    std::vector<EndWordObject> endWordObjects;
    Dictionary* ArrNextLetters;

    ValueObject();
};
