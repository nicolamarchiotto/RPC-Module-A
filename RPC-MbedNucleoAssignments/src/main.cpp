#include "mbed.h"

#define TIM_USR TIM2
#define TIM_USR_IRQ TIM2_IRQn

DigitalOut myled(LED1);
AnalogIn input(A0);
Serial serial(USBTX, USBRX, 9600);
Ticker flipper;

TIM_HandleTypeDef mTimUserHandle;
volatile char flag_time = 0;

extern "C" void M_TIM_USR_Handler(void)
{
    if (__HAL_TIM_GET_FLAG(&mTimUserHandle, TIM_FLAG_UPDATE) == SET)
    {
        __HAL_TIM_CLEAR_FLAG(&mTimUserHandle, TIM_FLAG_UPDATE);
        flag_time = 1;
    }
}

void hw1()
{
    float value;
    float interval;
    while (1)
    {
        value = input.read();
        serial.printf("myled = %f \n\r", value);
        if (value <= 0.1)
            interval = 0.1;
        else
            interval = value;

        wait(interval);
        myled = !myled;
    }
}

void printPotentiometer()
{
    serial.printf("myled = %f \n\r", input.read());
}

void hw2()
{
    float value;
    float interval;

    flipper.attach(&printPotentiometer, 0.2);
    while (1)
    {
        value = input.read();

        if (value <= 0.1)
            interval = 0.1;
        else
            interval = value;

        wait(interval);
        myled = !myled;
    }
}

void hw3()
{
    __HAL_RCC_TIM2_CLK_ENABLE();

    // frequency of 1 Hz = 1 sec
    mTimUserHandle.Instance = TIM_USR;
    mTimUserHandle.Init.Prescaler = 9999;
    mTimUserHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
    mTimUserHandle.Init.Period = 9999;
    mTimUserHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&mTimUserHandle);
    HAL_TIM_Base_Start_IT(&mTimUserHandle);

    NVIC_SetVector(TIM2_IRQn, (uint32_t)M_TIM_USR_Handler);
    NVIC_EnableIRQ(TIM2_IRQn);

    // Blink LED
    while (1)
    {
        serial.printf("myled = %f \n\r", input.read());

        if (flag_time == 1)
        {
            flag_time = 0;
            myled = !myled;
            if (flag_time == 1)
                serial.printf("More than 1 second has passed");
        }
    }
}

int main()
{
    hw2();
}