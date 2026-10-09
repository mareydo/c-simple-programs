#include "lib.h"

struct person_s
{
    char name[10];
    int age;
};

person_t* create(const char* name, const int age)
{
    person_t* retval = (person_t*)malloc(sizeof(person_t));
    strncpy(retval->name, name, 10);
    retval->age = age;
    return retval;
}

void print(person_t* val)
{
    printf("name: %s\n", val->name);
    printf("age: %d\n", val->age);
}