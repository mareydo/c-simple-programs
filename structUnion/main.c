#include <stdio.h>
#include <assert.h>
#include <stdint.h>

typedef struct 
{
    uint8_t battery_low   : 1;
    uint8_t signal_loss   : 1;
    uint8_t sensor_error  : 1;
    uint8_t reserved      : 5;
} StatusFlags_t;

typedef struct
{
    uint8_t id;
    StatusFlags_t flag;
    int16_t data;
} SensorData_t;

typedef union 
{
    SensorData_t structured;
    uint32_t raw;
} SensorUniversal_t;


void print(SensorUniversal_t* sensor)
{
    uint8_t id = sensor->structured.id;
    StatusFlags_t flag = sensor->structured.flag;
    int16_t data = sensor->structured.data;

    uint32_t raw = sensor->raw;

    printf("Id: %u\n", id);
    printf("Flags: %u,%u,%u\n", flag.battery_low, flag.signal_loss, flag.sensor_error);
    printf("Data: %d\n", data);
    printf("Raw: %x\n", raw);
}

int main(void)
{
    SensorUniversal_t s;
    s.raw = 0x120501F4;
    print(&s);

    SensorUniversal_t s2;
    s2.structured.id = 1;
    s2.structured.flag.battery_low = 1;
    s2.structured.flag.signal_loss = 0;
    s2.structured.flag.sensor_error = 1;
    s2.structured.data = -10;
    print(&s2);

    printf("%u %u\n",sizeof(s), sizeof(s.structured.flag));
}