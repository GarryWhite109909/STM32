#include "main.h"
#include "delay.h"

// ==================== 引脚定义 ====================
#define LEFT_A_PIN      GPIO_Pin_7   // PD7 --> A-1A (左轮 IA)
#define LEFT_A_PORT     GPIOD
#define LEFT_B_PIN      GPIO_Pin_4   // PA4 --> A-1B (左轮 IB)
#define LEFT_B_PORT     GPIOA

#define RIGHT_A_PIN     GPIO_Pin_6   // PD6 --> B-1A (右轮 IA)
#define RIGHT_A_PORT    GPIOD
#define RIGHT_B_PIN     GPIO_Pin_7   // PB7 --> B-1B (右轮 IB)
#define RIGHT_B_PORT    GPIOB


#define KEY1_PIN        GPIO_Pin_0
#define KEY1_PORT       GPIOA
#define KEY2_PIN        GPIO_Pin_2
#define KEY2_PORT       GPIOE
#define KEY3_PIN        GPIO_Pin_3
#define KEY3_PORT       GPIOE
#define KEY4_PIN        GPIO_Pin_4
#define KEY4_PORT       GPIOE


// ==================== 小车动作函数 ====================
void Car_Go(void) {
    // 左轮正转：IA=1, IB=0
    GPIO_SetBits(LEFT_A_PORT, LEFT_A_PIN);
    GPIO_ResetBits(LEFT_B_PORT, LEFT_B_PIN);
    // 右轮正转：IA=1, IB=0
    GPIO_SetBits(RIGHT_A_PORT, RIGHT_A_PIN);
    GPIO_ResetBits(RIGHT_B_PORT, RIGHT_B_PIN);
}

void Car_Back(void) {
    // 左轮反转：IA=0, IB=1
    GPIO_ResetBits(LEFT_A_PORT, LEFT_A_PIN);
    GPIO_SetBits(LEFT_B_PORT, LEFT_B_PIN);
    // 右轮反转：IA=0, IB=1
    GPIO_ResetBits(RIGHT_A_PORT, RIGHT_A_PIN);
    GPIO_SetBits(RIGHT_B_PORT, RIGHT_B_PIN);
}

void Car_Stop(void) {
    // 全部置0
    GPIO_ResetBits(LEFT_A_PORT, LEFT_A_PIN);
    GPIO_ResetBits(LEFT_B_PORT, LEFT_B_PIN);
    GPIO_ResetBits(RIGHT_A_PORT, RIGHT_A_PIN);
    GPIO_ResetBits(RIGHT_B_PORT, RIGHT_B_PIN);
}

void Car_TurnLeft(void) {
    // 左轮反转，右轮正转
    GPIO_ResetBits(LEFT_A_PORT, LEFT_A_PIN);
    GPIO_SetBits(LEFT_B_PORT, LEFT_B_PIN);
    GPIO_SetBits(RIGHT_A_PORT, RIGHT_A_PIN);
    GPIO_ResetBits(RIGHT_B_PORT, RIGHT_B_PIN);
}

void Car_TurnRight(void) {
    // 左轮正转，右轮反转
    GPIO_SetBits(LEFT_A_PORT, LEFT_A_PIN);
    GPIO_ResetBits(LEFT_B_PORT, LEFT_B_PIN);
    GPIO_ResetBits(RIGHT_A_PORT, RIGHT_A_PIN);
    GPIO_SetBits(RIGHT_B_PORT, RIGHT_B_PIN);
}

// ==================== GPIO 初始化 ====================
void car_gpio_init(void) {
    GPIO_InitTypeDef g;

    // 使能用到的 GPIO 时钟：A, B, D
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA |
                           RCC_AHB1Periph_GPIOB |
                           RCC_AHB1Periph_GPIOD, ENABLE);

    // 配置电机引脚为推挽输出，无上下拉
    g.GPIO_Mode = GPIO_Mode_OUT;
    g.GPIO_OType = GPIO_OType_PP;
    g.GPIO_Speed = GPIO_Speed_50MHz;
    g.GPIO_PuPd = GPIO_PuPd_NOPULL;  // 推荐：输出不用上拉

    g.GPIO_Pin = LEFT_A_PIN;
    GPIO_Init(LEFT_A_PORT, &g);

    g.GPIO_Pin = LEFT_B_PIN;
    GPIO_Init(LEFT_B_PORT, &g);

    g.GPIO_Pin = RIGHT_A_PIN;
    GPIO_Init(RIGHT_A_PORT, &g);

    g.GPIO_Pin = RIGHT_B_PIN;
    GPIO_Init(RIGHT_B_PORT, &g);

    // 初始化：先全高 50ms，再停止
    GPIO_SetBits(LEFT_A_PORT, LEFT_A_PIN);
    GPIO_SetBits(LEFT_B_PORT, LEFT_B_PIN);
    GPIO_SetBits(RIGHT_A_PORT, RIGHT_A_PIN);
    GPIO_SetBits(RIGHT_B_PORT, RIGHT_B_PIN);
    Mdelay_Lib(50);

    Car_Stop();
    Mdelay_Lib(20);
}

// 按键初始化
void key_gpio_init(void) {
    GPIO_InitTypeDef g;  

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);

    g.GPIO_Mode = GPIO_Mode_IN;
    g.GPIO_PuPd = GPIO_PuPd_UP;
    g.GPIO_Speed = GPIO_Speed_50MHz;

    g.GPIO_Pin = KEY1_PIN; GPIO_Init(KEY1_PORT, &g);
    g.GPIO_Pin = KEY2_PIN; GPIO_Init(KEY2_PORT, &g);
    g.GPIO_Pin = KEY3_PIN; GPIO_Init(KEY3_PORT, &g);
    g.GPIO_Pin = KEY4_PIN; GPIO_Init(KEY4_PORT, &g);
}

// ==================== 主函数 ====================
int main()
{
    car_gpio_init();
    key_gpio_init();
    
    while(1)
    {
        // S1按下：前进
        if(GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN) == 0)
        {
            Car_Go();
            while(GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN) == 0);  // 等待松开
            Car_Stop();
        }
        // S2按下：后退
        else if(GPIO_ReadInputDataBit(KEY2_PORT, KEY2_PIN) == 0)
        {
            Car_Back();
            while(GPIO_ReadInputDataBit(KEY2_PORT, KEY2_PIN) == 0);
            Car_Stop();
        }
        // S3按下：左转
        else if(GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN) == 0)
        {
            Car_TurnLeft();
            while(GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN) == 0);
            Car_Stop();
        }
        // S4按下：右转
        else if(GPIO_ReadInputDataBit(KEY4_PORT, KEY4_PIN) == 0)
        {
            Car_TurnRight();
            while(GPIO_ReadInputDataBit(KEY4_PORT, KEY4_PIN) == 0);
            Car_Stop();
        }
        
        // 无按键按下：保持停止（已在Car_Stop()状态）
    }
}