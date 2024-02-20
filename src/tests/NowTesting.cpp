#include <optional>
#include "../../libs/ClastBC/module.h"


std::pair<std::string, lfloat> getTextFromNumberForAxis(size_t index, size_t amount, lfloat minValue, lfloat maxValue, size_t lengthNumber = 3)  
{
    assert(index < amount && index >= 0);
    size_t length = 0;
    lfloat value = minValue + std::round(index * (maxValue - minValue) / (amount - 1.f));
    lfloat buff = value;        
    while((value /= 10) >= 1) 
    {
        length++; 
    }
    value *= 10;

    if(length < lengthNumber) // If a number correct return it
    {
        return {std::to_string(value) ,value};
    }


    return {std::to_string(value).substr(0, lengthNumber + 1) + "*10^" + std::to_string(length), buff}; // "LengthNumber + 1" == String have dot.  

}



#include <iostream>

int main()
{
    std::string otv = getTextFromNumberForAxis(0, 10, 10000, 100000).first;
    if((otv) != "1.00*10^4")
    {
        std::cout << otv << "\n";
        return EXIT_FAILURE;
    }
    otv = getTextFromNumberForAxis(9, 10, 10000, 100000).first;
    if((otv) != "1.00*10^5")
    {
        std::cout << otv << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}