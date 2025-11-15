#include "main.h"
#include "delay.h"

// ==================== 引脚定义 ====================
// 电机控制引脚（根据L9110S驱动模块）
#define LEFT_A_PIN      GPIO_Pin_9      // 左电机A相 - PC9 (A-1A)
#define LEFT_A_PORT     GPIOC
#define LEFT_B_PIN      GPIO_Pin_10     // 左电机B相 - PA10 (A-1B)
#define LEFT_B_PORT     GPIOA

#define RIGHT_A_PIN     GPIO_Pin_9      // 右电机A相 - PB9 (B-1A)
#define RIGHT_A_PORT    GPIOB
#define RIGHT_B_PIN     GPIO_Pin_6      // 右电机B相 - PB6 (B-1B)
#define RIGHT_B_PORT    GPIOB

// 按键引脚（上拉输入，按下为低电平）
#define KEY1_PIN        GPIO_Pin_0      // S1 - PA0
#define KEY1_PORT       GPIOA
#define KEY2_PIN        GPIO_Pin_2      // S2 - PE2
#define KEY2_PORT       GPIOE
#define KEY3_PIN        GPIO_Pin_3      // S3 - PE3
#define KEY3_PORT       GPIOE
#define KEY4_PIN        GPIO_Pin_4      // S4 - PE4
#define KEY4_PORT       GPIOE

// ==================== 函数声明 ====================
void Car_Init(void);        // 小车初始化（电机引脚）
void Key_Init(void);        // 按键初始化
void Car_Go(void);          // 前进
void Car_Back(void);        // 后退
void Car_Stop(void);        // 停止
void Car_TurnLeft(void);    // 左转
void Car_TurnRight(void);   // 右转

// ==================== 小车初始化 ====================
void Car_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // 使能GPIO时钟（A、B、C端口）
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    
    // 配置电机引脚为推挽输出
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    
    // 左电机A相 (PC9)
    GPIO_InitStruct.GPIO_Pin = LEFT_A_PIN;
    GPIO_Init(LEFT_A_PORT, &GPIO_InitStruct);
    
    // 左电机B相 (PA10)
    GPIO_InitStruct.GPIO_Pin = LEFT_B_PIN;
    GPIO_Init(LEFT_B_PORT, &GPIO_InitStruct);
    
    // 右电机A相 (PB9)
    GPIO_InitStruct.GPIO_Pin = RIGHT_A_PIN;
    GPIO_Init(RIGHT_A_PORT, &GPIO_InitStruct);
    
    // 右电机B相 (PB6)
    GPIO_InitStruct.GPIO_Pin = RIGHT_B_PIN;
    GPIO_Init(RIGHT_B_PORT, &GPIO_InitStruct);
    
    // 初始状态：停止
    Car_Stop();
}

// ==================== 按键初始化 ====================
void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // 使能GPIO时钟（A、E端口）
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    
    // 配置按键引脚为上拉输入
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;  // 上拉，默认高电平
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    
    // S1 (PA0)
    GPIO_InitStruct.GPIO_Pin = KEY1_PIN;
    GPIO_Init(KEY1_PORT, &GPIO_InitStruct);
    
    // S2 (PE2)
    GPIO_InitStruct.GPIO_Pin = KEY2_PIN;
    GPIO_Init(KEY2_PORT, &GPIO_InitStruct);
    
    // S3 (PE3)
    GPIO_InitStruct.GPIO_Pin = KEY3_PIN;
    GPIO_Init(KEY3_PORT, &GPIO_InitStruct);
    
    // S4 (PE4)
    GPIO_InitStruct.GPIO_Pin = KEY4_PIN;
    GPIO_Init(KEY4_PORT, &GPIO_InitStruct);
}

// ==================== 运动控制函数 ====================

// 前进：左右电机都正转
void Car_Go(void)
{
    // 根据用户描述，按S1时左轮停止，右轮继续左转，说明当前前进逻辑有问题
    // 尝试反转控制逻辑
    GPIO_ResetBits(LEFT_A_PORT, LEFT_A_PIN);
    GPIO_SetBits(LEFT_B_PORT, LEFT_B_PIN);
    
    GPIO_ResetBits(RIGHT_A_PORT, RIGHT_A_PIN);
    GPIO_SetBits(RIGHT_B_PORT, RIGHT_B_PIN);
}

// 后退：左右电机都反转
void Car_Back(void)
{
    // 根据用户描述，按S2时左轮继续左转，右轮停止，说明当前后退逻辑有问题
    // 尝试反转控制逻辑
    GPIO_SetBits(LEFT_A_PORT, LEFT_A_PIN);
    GPIO_ResetBits(LEFT_B_PORT, LEFT_B_PIN);
    
    GPIO_SetBits(RIGHT_A_PORT, RIGHT_A_PIN);
    GPIO_ResetBits(RIGHT_B_PORT, RIGHT_B_PIN);
}

// 停止：全部置低电平
void Car_Stop(void)
{
    GPIO_ResetBits(LEFT_A_PORT, LEFT_A_PIN);
    GPIO_ResetBits(LEFT_B_PORT, LEFT_B_PIN);
    GPIO_ResetBits(RIGHT_A_PORT, RIGHT_A_PIN);
    GPIO_ResetBits(RIGHT_B_PORT, RIGHT_B_PIN);
}

// 左转：左反转，右正转（差速转向）
void Car_TurnLeft(void)
{
    // 根据用户描述，按S3没反应，说明当前左转逻辑有问题
    // 尝试新的控制逻辑
    GPIO_ResetBits(LEFT_A_PORT, LEFT_A_PIN);
    GPIO_SetBits(LEFT_B_PORT, LEFT_B_PIN);
    
    GPIO_SetBits(RIGHT_A_PORT, RIGHT_A_PIN);
    GPIO_ResetBits(RIGHT_B_PORT, RIGHT_B_PIN);
}

// 右转：左正转，右反转（差速转向）
void Car_TurnRight(void)
{
    // 根据用户描述，按S4时两轮都停止，说明当前右转逻辑有问题
    // 尝试新的控制逻辑
    GPIO_SetBits(LEFT_A_PORT, LEFT_A_PIN);
    GPIO_ResetBits(LEFT_B_PORT, LEFT_B_PIN);
    
    GPIO_ResetBits(RIGHT_A_PORT, RIGHT_A_PIN);
    GPIO_SetBits(RIGHT_B_PORT, RIGHT_B_PIN);
}
// ==================== 主函数 ====================
int main(void)
{
    // 1. 初始化小车和按键
    Car_Init();
    Key_Init();
    // ===== 调试：用LED0(PF9)指示程序运行状态 =====
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    GPIO_InitTypeDef led;
    led.GPIO_Pin = GPIO_Pin_9;
    led.GPIO_Mode = GPIO_Mode_OUT;
    led.GPIO_Speed = GPIO_Speed_50MHz;
    led.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOF, &led);
    GPIO_SetBits(GPIOF, GPIO_Pin_9);  // 初始熄灭

    // 2. 默认停止状态
    // 强制停止（确保电平正确）
    Car_Stop();
    
    // 确保所有电机引脚都处于低电平
    GPIO_ResetBits(LEFT_A_PORT, LEFT_A_PIN);
    GPIO_ResetBits(LEFT_B_PORT, LEFT_B_PIN);
    GPIO_ResetBits(RIGHT_A_PORT, RIGHT_A_PIN);
    GPIO_ResetBits(RIGHT_B_PORT, RIGHT_B_PIN);
    
    GPIO_ResetBits(GPIOF, GPIO_Pin_9);  // 点亮LED表示初始化完成
    
    // 延时2秒，观察电机是否转动
    Mdelay_Lib(2000);
    GPIO_SetBits(GPIOF, GPIO_Pin_9);  // 熄灭LED，进入主循环
    
    // 3. 主循环：检测按键并控制小车
    while(1)
    {
        // 检测S1是否按下（前进）
        if(GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN) == 0)
        {
            Car_Go();
            // 等待按键释放，添加延时防止抖动
            Mdelay_Lib(20);
            while(GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN) == 0); 
            Mdelay_Lib(20);
        }
        // 检测S2是否按下（后退）
        else if(GPIO_ReadInputDataBit(KEY2_PORT, KEY2_PIN) == 0)
        {
            Car_Back();
            Mdelay_Lib(20);
            while(GPIO_ReadInputDataBit(KEY2_PORT, KEY2_PIN) == 0);
            Mdelay_Lib(20);
        }
        // 检测S3是否按下（左转）
        else if(GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN) == 0)
        {
            Car_TurnLeft();
            Mdelay_Lib(20);
            while(GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN) == 0);
            Mdelay_Lib(20);
        }
        // 检测S4是否按下（右转）
        else if(GPIO_ReadInputDataBit(KEY4_PORT, KEY4_PIN) == 0)
        {
            Car_TurnRight();
            Mdelay_Lib(20);
            while(GPIO_ReadInputDataBit(KEY4_PORT, KEY4_PIN) == 0);
            Mdelay_Lib(20);
        }
        // 无按键按下，停止
        else
        {
            Car_Stop();
        }
    }
}