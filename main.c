#include <stdint.h>
#include <stm32f4xx_hal.h>
#include <stdio.h>
TIM_HandleTypeDef htim4;
uint16_t led[7]={GPIO_PIN_2,GPIO_PIN_6,GPIO_PIN_7,GPIO_PIN_8,GPIO_PIN_9,GPIO_PIN_10,GPIO_PIN_11};
uint16_t block[4]={GPIO_PIN_10,GPIO_PIN_12,GPIO_PIN_14,GPIO_PIN_15};
char data[128];
int i= 0;
char x;
int flag = 0;
int new = 0;

struct GpsDATA {
	char date[9];
	char time[9];
	char lat[11];
	char ns;
	char lon[12];
	char ew;
};
struct GpsDATA gps;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
int _write(int file, char *ptr, int len) {
	//HAL_UART_Transmit(&huart3, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    for (int i = 0; i < len; i++) {
        ITM_SendChar(ptr[i]);
    }
    return len;
}
int HH;
int MM;
//configure all led segment pin {PC2[a],PC6[b],PC7[c],PC8[d],PC9[e],PC10[f],PC11[g]}as output pin
void init_led_segment_pins(void){
	GPIO_InitTypeDef GPIO_INIT={0};
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_INIT.Pin = GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
	GPIO_INIT.Mode =GPIO_MODE_OUTPUT_PP;
	GPIO_INIT.Speed=GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC,&GPIO_INIT);

}
// configure all led blocks pin {PE10[d1],PE12[d2],PE14[d3],PE15[d4]} as output pin
void init_led_block_pins(void){
	GPIO_InitTypeDef GPIO_INIT={0};
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_INIT.Pin = GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_INIT.Mode =GPIO_MODE_OUTPUT_PP;
	GPIO_INIT.Speed=GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE,&GPIO_INIT);

}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void convert_utc_to_ist(char *time) {
    if (!time || strlen(time) < 6) return;

    // Parse hours, minutes, seconds
    int hh = (time[0] - '0') * 10 + (time[1] - '0');
    int mm = (time[2] - '0') * 10 + (time[3] - '0');
    int ss = (time[4] - '0') * 10 + (time[5] - '0');

    // Add 5 hours 30 minutes
    mm += 30;
    hh += 5;

    if (mm >= 60) {
        mm -= 60;
        hh += 1;
    }

    if (hh >= 24) {
        hh -= 24;
    }

    // Format in-place as HH:MM:SS
    sprintf(time, "%02d:%02d:%02d", hh, mm, ss);
}


void init_count(int x)
{
	if(x==0){
		for(int i=0;i<7;i++)
		{
			if(i==6)
			{
			 HAL_GPIO_WritePin( GPIOC,led[i], GPIO_PIN_RESET);
			}
			else
			{
			HAL_GPIO_WritePin( GPIOC,led[i], GPIO_PIN_SET);
			}
		}
	}
	if(x==1){
		for(int i=0;i<7;i++)
				{
			            if(i==1||i==2)
						{
						 HAL_GPIO_WritePin( GPIOC,led[i], GPIO_PIN_SET);
						}
						else
						{
						HAL_GPIO_WritePin( GPIOC,led[i], GPIO_PIN_RESET);
						}
				}
	}
	if(x==2){
		for(int i=0;i<7;i++)
				{
			            if(i==6||i==4||i==3||i==1||i==0)
						{
						 HAL_GPIO_WritePin( GPIOC,led[i], GPIO_PIN_SET);
						}
						else
						{
						HAL_GPIO_WritePin( GPIOC,led[i], GPIO_PIN_RESET);
						}
				}
	}
	if(x==3){
		for(int i=0;i<7;i++)
				{
			            if(i==6||i==3||i==2||i==1||i==0)
						{
						 HAL_GPIO_WritePin( GPIOC,led[i], GPIO_PIN_SET);
						}
						else
						{
						HAL_GPIO_WritePin( GPIOC,led[i], GPIO_PIN_RESET);
						}
				}
	}
	if(x==4){
		for(int i=0;i<7;i++)
				{
			            if(i==6||i==5||i==1||i==2)
						{
						 HAL_GPIO_WritePin( GPIOC,led[i], GPIO_PIN_SET);
						}
						else
						{
						HAL_GPIO_WritePin( GPIOC,led[i], GPIO_PIN_RESET);
						}
				}
	}
	if(x==5){
		for(int i=0;i<7;i++)
				{
			            if(i==6||i==5||i==3||i==2||i==0)
						{
						 HAL_GPIO_WritePin( GPIOC,led[i], GPIO_PIN_SET);
						}
						else
						{
						HAL_GPIO_WritePin( GPIOC,led[i], GPIO_PIN_RESET);
						}
				}
	}
	if(x==6){
		for(int i=0;i<7;i++)
				{
			           if(i==1)
						{
						 HAL_GPIO_WritePin( GPIOC,led[i], GPIO_PIN_RESET);
						}
						else
						{
						HAL_GPIO_WritePin( GPIOC,led[i], GPIO_PIN_SET);
						}
				}
	}
	if(x==7){
		for(int i=0;i<7;i++)
				{
			            if(i==2||i==1||i==0)
						{
						 HAL_GPIO_WritePin( GPIOC,led[i], GPIO_PIN_SET);
						}
						else
						{
						HAL_GPIO_WritePin( GPIOC,led[i], GPIO_PIN_RESET);
						}
				}
	}
	if(x==8){
		for(int i=0;i<7;i++)
				{

						 HAL_GPIO_WritePin( GPIOC,led[i], GPIO_PIN_SET);


				}
	}
	if(x==9){
		for(int i=0;i<7;i++)
				{
			           if(i==4)
						{
						 HAL_GPIO_WritePin( GPIOC,led[i], GPIO_PIN_RESET);
						}
						else
						{
						HAL_GPIO_WritePin( GPIOC,led[i], GPIO_PIN_SET);
						}
				}
	}
}

//final counting function
void loop(int H,int M) {
	init_count((H/10)%10);
    HAL_GPIO_WritePin(GPIOE, block[0], GPIO_PIN_RESET);
    HAL_Delay(2);
    HAL_GPIO_WritePin(GPIOE, block[0], GPIO_PIN_SET);

    init_count(H%10);
    HAL_GPIO_WritePin(GPIOE, block[1], GPIO_PIN_RESET);
    HAL_Delay(2);
    HAL_GPIO_WritePin(GPIOE, block[1], GPIO_PIN_SET);

    init_count((M/10)%10);
    HAL_GPIO_WritePin(GPIOE, block[2], GPIO_PIN_RESET);
    HAL_Delay(2);
     HAL_GPIO_WritePin(GPIOE, block[2], GPIO_PIN_SET);

    init_count(M%10);
    HAL_GPIO_WritePin(GPIOE, block[3], GPIO_PIN_RESET);
    HAL_Delay(2);
   HAL_GPIO_WritePin(GPIOE, block[3], GPIO_PIN_SET);
}
void extraction() {
    int j = 0, field = 0, l = 0;


    gps.time[0] = gps.date[0] = gps.lat[0] = gps.lon[0] = '\0';
    gps.ns = gps.ew = '\0';

    while (data[j] != '\n' && j < 127) {
        if (data[j] == ',') {
            field++;
            j++;
            continue;
        }

        switch (field) {
            case 1:
                l = 0;
                while (data[j] != '.' && data[j] != ',' && l < 6) {
                gps.time[l++] = data[j++];
                }
                gps.time[l] = '\0';
                convert_utc_to_ist(gps.time);
                while (data[j] != ',' && j < 127) j++;
                break;


            case 3:
                l = 0;
                while (data[j] != ',' && l < 10) {
                    gps.lat[l++] = data[j++];
                }
                gps.lat[l] = '\0';
                break;

            case 4:
                gps.ns = data[j++];
                break;

            case 5:
                l = 0;
                while (data[j] != ',' && l < 11) {
                    gps.lon[l++] = data[j++];
                }
                gps.lon[l] = '\0';
                break;

            case 6:
                gps.ew = data[j++];
                break;

            case 9:
                l = 0;
                while (data[j] != ',' && l < 9) {
                    gps.date[l++] = data[j++];
                    gps.date[l++] =data[j++];
                    gps.date[l++] ='/';
                }
                gps.date[8] = '\0';
                break;

            default:
                while (data[j] != ',' && data[j] != '\n') j++;
                break;
        }
    }

    new = 1;
}
char t;
void init_uart() {
    GPIO_InitTypeDef GPIO_INIT = {0};

    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_USART3_CLK_ENABLE();

    // USART2: PD5 (TX), PD6 (RX)
    GPIO_INIT.Pin = GPIO_PIN_5 | GPIO_PIN_6;
    GPIO_INIT.Mode = GPIO_MODE_AF_PP;
    GPIO_INIT.Pull = GPIO_NOPULL;
    GPIO_INIT.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_INIT.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOD, &GPIO_INIT);

    // USART3: PD8 (TX), PD9 (RX)
    GPIO_INIT.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_INIT.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_INIT);

    huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    huart3.Instance = USART3;
    huart3.Init = huart2.Init;

    HAL_UART_Init(&huart2);
    HAL_UART_Init(&huart3);

    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);

    HAL_UART_Receive_IT(&huart2, (uint8_t *)&x, 1);
}


int main(void) {
    HAL_Init();
    init_led_segment_pins();
    init_led_block_pins();
    init_uart();

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);

    while (1) {
        loop(HH, MM);
    }
}

void TIM4_IRQHandler(void)
{

    HAL_TIM_IRQHandler(&htim4);

}
void USART2_IRQHandler(){
	HAL_UART_IRQHandler(&huart2);
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {

    	HAL_UART_Receive_IT(&huart2, (uint8_t *)&x, 1);
        if (x == 'C' && t=='M') {
            i = 0;
            flag = 1;
        }

        if (x == '\n' && flag == 1) {
            flag = 0;
            data[i++] = x;
            data[i] = '\0';
            extraction();
        }

        if (flag && i < 127) {
            data[i++] = x;
        }
         t=x;
       // printf("%c", x);

        if (new) {

        	    int newHH = (gps.time[0] - '0') * 10 + (gps.time[1] - '0');
        	    int newMM = (gps.time[3] - '0') * 10 + (gps.time[4] - '0');
        	    if (HH != newHH || MM != newMM) {
        	        HH = newHH;
        	        MM = newMM;
        	    }


            new = 0;
            printf("\r\n--- GPS Data ---\r\n");
            printf("Time:   %s\r\n", gps.time);
            printf("Date:   %s\r\n", gps.date);
            printf("Lat:    %s %c\r\n", gps.lat, gps.ns);
            printf("Lon:    %s %c\r\n", gps.lon, gps.ew);
            printf("----------------\r\n");
        }



    }
}


void SysTick_Handler(){
	HAL_IncTick();
}

