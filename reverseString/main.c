#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>


size_t getLength(char* str)
{
    size_t len = 0;
    char* c = str;
    while(*c != '\0')
    {
        ++len;
        ++c;
    } 
    return len;
}

void swapLetters(char* a, char*b)
{
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

void reverseString(char *str)
{
    uint8_t swap = 1;
    size_t len = getLength(str);


    for(size_t i = 0, j = len-1; i < len / 2; ++i, --j)
    {
        swapLetters(&str[i], &str[j]);
    }

}

int main(int argc, char** argv) 
{
    char str1[] = "hello";
    reverseString(str1);
    assert(strcmp(str1, "olleh")==0);

    char str2[] = "C programming";
    reverseString(str2);
    assert(strcmp(str2, "gnimmargorp C")==0);

    return 0;
}
