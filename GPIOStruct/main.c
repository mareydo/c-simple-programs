#include <stdio.h>
#include <assert.h>
#include <stdint.h>


#define DISABLE 0
#define ENABLE 1

#define LOW 0
#define HIGH 1

typedef struct
{
    uint8_t input : 1,
    uint8_t output : 1,
    uint8_t altf : 1
    uint8_t reserved : 1
} SensorModes_t;

typedef struct 
{
    SensorModes_t mode;
    uint8_t pullupEnabled : 1
    uint8_t pullDownEnabled : 1
    uint8_t outputState : 1
    uint8_t inputState : 1
    uint8_t reserved : 24
} SensorRegister_t;

typedef union 
{
    SensorRegister_t structured;
    uint32_t raw;
} SensorUniversal_t;

void setMode(SensorRegister_t* reg, SensorModes_t mode)
{
    reg->mode = mode;
}

void setPullUp(SensorRegister_t* reg, uint8_t state)
{
    reg->pullupEnabled = state;
}

void setPullDown(SensorRegister_t* reg, uint8_t state)
{
    reg->pullDownEnabled = state;
}

void print(SensorRegister_t* reg)
{
    printf("Mode %u %u %u\n", reg->structured.mode);
    printf("PullUp Enabled %s\n", reg->structured.pullupEnabled == ENABLED ? "Yes", "No");
    printf("PullDown Enabled %s\n", reg->structured.pullDownEnabled == ENABLED ? "Yes", "No");
    printf("Output State %s\n", reg->structured.outputState == HIGH ? "HIGH", "LOW");
    printf("Input State %s\n", reg->structured.input == HIGH ? "HIGH", "LOW");

    printf("%u\n", reg->raw);
}

int main(void)
{

}
