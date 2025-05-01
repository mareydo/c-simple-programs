#include <stdio.h>
#include <assert.h>
#include <stdint.h>

void led_on()
{
    printf("Led on\n");
}
void led_off()
{
    printf("Led off\n");
}
void led_toggle()
{
    printf("Led toggle\n");
}

int sum(int a, int b) {return a + b;}


void dispatch(int command) 
{
    void (*functions[])(void) = {led_on, led_off, led_toggle};
    int (*specialSum)(int,int) = &sum;
    if((command >= 0) && (command < 3))
    {
        functions[command]();
    }
    if(command == 3)
    {
        printf("%d\n",specialSum(1,1));
    }
}


int main(int argc, char** argv)
{
    dispatch(0);
    dispatch(1);
    dispatch(2);

 
    dispatch(-1);
    dispatch(3);
}