#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include "lib.h"

int main(void)
{
    char name[10] = "bob";
    person_t* person = create(name, 5);
    print(person);
    free(person);
    return 0;
}