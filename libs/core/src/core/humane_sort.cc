#include "core/humane_sort.h"

// from https://github.com/vivianliang/humane-sort
// but adopted to take sane arguments and compile without warnings


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


namespace ride
{

int humane_strcmp(const char* str1, const char* str2)
{
    unsigned long long num1;
    unsigned long long num2;
    char *next_char1;
    char *next_char2;

    while (1){
        if (*str1 == *str2 && (!isdigit(*str1))){
            if (*str1 == '\0'){	// The strings are equal
                return 0;
            } else {
                str1++;
                str2++;
            }
        } else{
            if (*str1 == '\0') return -1;
            if (*str2 == '\0') return 1;

            // All symbols should go before alphanumeric characters
            if (!isalnum(*str1) && isalnum(*str2)) return -1;
            if (isalnum(*str1) && !isalnum(*str2)) return 1;

            // Compare numbers
            if (isdigit(*str1) && isdigit(*str2)){

                num1 = strtoull (str1, &next_char1, 10);
                num2 = strtoull (str2, &next_char2, 10);

                if (num1 < num2){
                    return -1;
                } else if (num1 > num2){
                    return 1;
                } else{
                    // When numbers are equal, priority goes to most leading zeroes
                    if ((next_char1 - str1) > (next_char2 - str2)){
                        return -1;
                    } else if((next_char2 - str2) > (next_char1 - str1)){
                        return 1;
                    // The two numbers are completely equal (no leading zeroes).
                    // Continue traversing the string.
                    } else {
                        str1 = next_char1;
                        str2 = next_char2;
                    }
                }
            // Compare ascii values for non-digits
            } else {
                
                // Ignore case
                char tmp1, tmp2;
                if (isupper(*str1)){ 
                    tmp1 = static_cast<char>(tolower(*str1));
                } else {
                    tmp1 = *str1;
                }
                if (isupper(*str2)){ 
                    tmp2 = static_cast<char>(tolower(*str2));
                } else {
                    tmp2 = *str2;
                }

                if (tmp1 < tmp2){
                    return -1;
                } else if(tmp1 > tmp2){
                    return 1;
                } else {
                    str1++;
                    str2++;
                }
            }
        }
    }
    // We should never get here
}


}

