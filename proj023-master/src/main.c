#define BUTTON_BLINK

#include <stdbool.h> // booleans, i.e. true and false
#include <stdio.h>   // sprintf() function
#include <stdlib.h>  // srand() and random() functions

#include "ece198.h"
#include <LiquidCrystal.h>

int main(void)
{
    HAL_Init(); // initialize the Hardware Abstraction Layer

    // Peripherals (including GPIOs) are disabled by default to save power, so we
    // use the Reset and Clock Control registers to enable the GPIO peripherals we're using.

    __HAL_RCC_GPIOA_CLK_ENABLE(); // enable port A (for the on-board LED, for example)
    __HAL_RCC_GPIOB_CLK_ENABLE(); // enable port B (for the rotary encoder inputs, for example)
    __HAL_RCC_GPIOC_CLK_ENABLE(); // enable port C (for the on-board blue pushbutton, for example)

    // initialize the pins to be input, output, alternate function, etc
    SerialSetup(9600);
    InitializePin(GPIOA, GPIO_PIN_9 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_10, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); // initialize the pin that the on-board LED is on
    InitializePin(GPIOB, GPIO_PIN_6, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);
    // note: the on-board pushbutton is fine with default values (input, and no pull-up resistor required since there's one on the board)
    LiquidCrystal(GPIOB, GPIO_PIN_8, GPIO_PIN_9 , GPIO_PIN_10, GPIO_PIN_3,GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6);
    setCursor(0,0);
    print("Welcome");//start
    HAL_Delay(2500);
    clear();
    print("Question 1");//question
    HAL_Delay(1000);
    clear();
    print("... -.- -.-- / -.-.");//question1
    setCursor(0,1);
    print("--- .-.. --- .-.");//question1 part 2
    HAL_Delay(10000);
    clear();
    print("..--..");// question1 part 3
    HAL_Delay(10000);
    clear();


    char answer[] = {'-','.','.','.','.','-','.','.','.','.','-','.'};//answer in a char
    char user_input[16];// character for user input

    for (int i= 0; i<16; i++){
        user_input[i]= ' ';
    }

    int index = 0;

    while (true){
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)){//yellow button
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);//GREEN LED

            HAL_Delay(1000);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
            char buff[100];
            user_input[index] = '.';
            if (index !=15){
                index++;
            }
            clear();
            sprintf(buff, "%d", user_input);
            SerialPuts(user_input);
            SerialPuts("\n");
            setCursor(0,0);
            print(user_input);
        }
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)){//red button
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);//RED LED

            HAL_Delay(1000);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
            char buff[100];
            user_input[index] = '-';
            if (index !=15){
                index++;
            }
            sprintf(buff, "%d", user_input);
            SerialPuts(user_input);
            SerialPuts("\n");
            setCursor(0,0);
            print(user_input);
        }
        
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)){//backspace button, black
            
            char buff[100];
            HAL_Delay(1000);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
            user_input[index]=NULL;
            if (index !=15){
            index--;
            }
            sprintf(buff, "%d", user_input);
            SerialPuts(user_input);
            SerialPuts("\n");
            setCursor(0,0);
            print(user_input);
        } 

        if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8)){//enter button, far right
            bool correct = true;
        
            for (int i=0; i<index; i++){
                if (user_input[i] != answer[i]){
                    correct = false;
                    break;
                }
            }
            if (correct){// if comparison is correct print you win
                clear();
                print("you win");
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

                HAL_Delay(100);
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

            } else {//else print you lose
                clear();
                print("you lose");
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

                HAL_Delay(100);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
            }
        }
    }
    



        


    return 0;

}


void SysTick_Handler(void)
{
    HAL_IncTick(); // tell HAL that a new tick has happened
    // we can do other things in here too if we need to, but be careful
}
