#ifndef LIB_H
#define LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct person_s person_t; 
person_t* create(const char*, const int);
void print(person_t*);

#endif