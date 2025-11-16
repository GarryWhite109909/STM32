#include "main.h"
#include "delay.h"

// // ==================== 引脚定义 ====================
// #define LEFT_A_PIN GPIO_Pin_7 // PD7 --> A-1A (左轮 IA)
// #define LEFT_A_PORT GPIOD
// #define LEFT_B_PIN GPIO_Pin_4 // PA4 --> A-1B (左轮 IB)
// #define LEFT_B_PORT GPIOA

// #define RIGHT_A_PIN GPIO_Pin_6 // PD6 --> B-1A (右轮 IA)
// #define RIGHT_A_PORT GPIOD
// #define RIGHT_B_PIN GPIO_Pin_7 // PB7 --> B-1B (右轮 IB)
// #define RIGHT_B_PORT GPIOB

// // 红外探头引脚
// #define LEFT_SENSOR_PIN GPIO_Pin_15 // PG15 (DCMI RESET)
// #define LEFT_SENSOR_PORT GPIOG
// #define RIGHT_SENSOR_PIN GPIO_Pin_6 // PB6 (DCMI D5)
// #define RIGHT_SENSOR_PORT GPIOB

// #define KEY1_PIN GPIO_Pin_0
// #define KEY1_PORT GPIOA
// #define KEY2_PIN GPIO_Pin_2
// #define KEY2_PORT GPIOE
// #define KEY3_PIN GPIO_Pin_3
// #define KEY3_PORT GPIOE
// #define KEY4_PIN GPIO_Pin_4
// #define KEY4_PORT GPIOE

// // ==================== 小车动作函数 ====================
// void Car_Go(void) {
//   GPIO_SetBits(LEFT_A_PORT, LEFT_A_PIN);
//   GPIO_ResetBits(LEFT_B_PORT, LEFT_B_PIN);
//   GPIO_SetBits(RIGHT_A_PORT, RIGHT_A_PIN);
//   GPIO_ResetBits(RIGHT_B_PORT, RIGHT_B_PIN);
// }

// void Car_Back(void) {
//   GPIO_ResetBits(LEFT_A_PORT, LEFT_A_PIN);
//   GPIO_SetBits(LEFT_B_PORT, LEFT_B_PIN);
//   GPIO_ResetBits(RIGHT_A_PORT, RIGHT_A_PIN);
//   GPIO_SetBits(RIGHT_B_PORT, RIGHT_B_PIN);
// }

// void Car_Stop(void) {
//   GPIO_ResetBits(LEFT_A_PORT, LEFT_A_PIN);
//   GPIO_ResetBits(LEFT_B_PORT, LEFT_B_PIN);
//   GPIO_ResetBits(RIGHT_A_PORT, RIGHT_A_PIN);
//   GPIO_ResetBits(RIGHT_B_PORT, RIGHT_B_PIN);
// }

// void Car_TurnLeft(void) {
//   GPIO_ResetBits(LEFT_A_PORT, LEFT_A_PIN);
//   GPIO_SetBits(LEFT_B_PORT, LEFT_B_PIN);
//   GPIO_SetBits(RIGHT_A_PORT, RIGHT_A_PIN);
//   GPIO_ResetBits(RIGHT_B_PORT, RIGHT_B_PIN);
// }

// void Car_TurnRight(void) {
//   GPIO_SetBits(LEFT_A_PORT, LEFT_A_PIN);
//   GPIO_ResetBits(LEFT_B_PORT, LEFT_B_PIN);
//   GPIO_ResetBits(RIGHT_A_PORT, RIGHT_A_PIN);
//   GPIO_SetBits(RIGHT_B_PORT, RIGHT_B_PIN);
// }

// // 增强左转函数，原地左转
// void Car_SharpTurnLeft(void) {
//   // 左轮停止，右轮正转（原地左转）
//   GPIO_ResetBits(LEFT_A_PORT, LEFT_A_PIN);
//   GPIO_SetBits(LEFT_B_PORT, LEFT_B_PIN);
//   GPIO_SetBits(RIGHT_A_PORT, RIGHT_A_PIN);
//   GPIO_ResetBits(RIGHT_B_PORT, RIGHT_B_PIN);
// }

// // 增强右转函数，原地右转
// void Car_SharpTurnRight(void) {
//   // 左轮正转，右轮停止（原地右转）
//   GPIO_SetBits(LEFT_A_PORT, LEFT_A_PIN);
//   GPIO_ResetBits(LEFT_B_PORT, LEFT_B_PIN);
//   GPIO_ResetBits(RIGHT_A_PORT, RIGHT_A_PIN);
//   GPIO_SetBits(RIGHT_B_PORT, RIGHT_B_PIN);
// }

// // ==================== GPIO 初始化 ====================
// void car_gpio_init(void) {
//   GPIO_InitTypeDef g;
//   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |
//                              RCC_AHB1Periph_GPIOD,
//                          ENABLE);

//   g.GPIO_Mode = GPIO_Mode_OUT;
//   g.GPIO_OType = GPIO_OType_PP;
//   g.GPIO_Speed = GPIO_Speed_50MHz;
//   g.GPIO_PuPd = GPIO_PuPd_NOPULL;

//   g.GPIO_Pin = LEFT_A_PIN;
//   GPIO_Init(LEFT_A_PORT, &g);
//   g.GPIO_Pin = LEFT_B_PIN;
//   GPIO_Init(LEFT_B_PORT, &g);
//   g.GPIO_Pin = RIGHT_A_PIN;
//   GPIO_Init(RIGHT_A_PORT, &g);
//   g.GPIO_Pin = RIGHT_B_PIN;
//   GPIO_Init(RIGHT_B_PORT, &g);

//   GPIO_SetBits(LEFT_A_PORT, LEFT_A_PIN);
//   GPIO_SetBits(LEFT_B_PORT, LEFT_B_PIN);
//   GPIO_SetBits(RIGHT_A_PORT, RIGHT_A_PIN);
//   GPIO_SetBits(RIGHT_B_PORT, RIGHT_B_PIN);
//   Mdelay_Lib(50);
//   Car_Stop();
//   Mdelay_Lib(20);
// }

// void sensor_gpio_init(void) {
//   GPIO_InitTypeDef g;
//   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOB,
//                          ENABLE); // 使能GPIOG和GPIOB

//   g.GPIO_Mode = GPIO_Mode_IN;
//   g.GPIO_PuPd = GPIO_PuPd_NOPULL; // 浮空输入 (红外模块内部有上拉)
//   g.GPIO_Speed = GPIO_Speed_50MHz;

//   g.GPIO_Pin = LEFT_SENSOR_PIN;
//   GPIO_Init(LEFT_SENSOR_PORT, &g);

//   g.GPIO_Pin = RIGHT_SENSOR_PIN;
//   GPIO_Init(RIGHT_SENSOR_PORT, &g);
// }

// void key_gpio_init(void) {
//   GPIO_InitTypeDef g;
//   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);

//   g.GPIO_Mode = GPIO_Mode_IN;
//   g.GPIO_PuPd = GPIO_PuPd_UP;
//   g.GPIO_Speed = GPIO_Speed_50MHz;

//   g.GPIO_Pin = KEY1_PIN;
//   GPIO_Init(KEY1_PORT, &g);
//   g.GPIO_Pin = KEY2_PIN;
//   GPIO_Init(KEY2_PORT, &g);
//   g.GPIO_Pin = KEY3_PIN;
//   GPIO_Init(KEY3_PORT, &g);
//   g.GPIO_Pin = KEY4_PIN;
//   GPIO_Init(KEY4_PORT, &g);
// }

// // ==================== 主函数 ====================
// int main() {
//   car_gpio_init();
//   sensor_gpio_init(); // 初始化红外探头
//   key_gpio_init();

//   // 添加初始化延时，确保系统稳定
//   Mdelay_Lib(100);

//   while (1) {
//     // 检查按键控制
//     if (GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN) == 0) {
//       Car_Go();
//       while (GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN) == 0)
//         ;
//       Car_Stop();
//     } else if (GPIO_ReadInputDataBit(KEY2_PORT, KEY2_PIN) == 0) {
//       Car_Back();
//       while (GPIO_ReadInputDataBit(KEY2_PORT, KEY2_PIN) == 0)
//         ;
//       Car_Stop();
//     } else if (GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN) == 0) {
//       Car_TurnLeft();
//       while (GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN) == 0)
//         ;
//       Car_Stop();
//     } else if (GPIO_ReadInputDataBit(KEY4_PORT, KEY4_PIN) == 0) {
//       Car_TurnRight();
//       while (GPIO_ReadInputDataBit(KEY4_PORT, KEY4_PIN) == 0)
//         ;
//       Car_Stop();
//     }
//     // 没有按键按下 -> 执行循迹逻辑
//     else {
//       uint8_t left = GPIO_ReadInputDataBit(LEFT_SENSOR_PORT, LEFT_SENSOR_PIN);
//       uint8_t right =
//           GPIO_ReadInputDataBit(RIGHT_SENSOR_PORT, RIGHT_SENSOR_PIN);

//       // 红外探头逻辑 (0=浅色/白色, 1=黑色)
//       if (left == 0 && right == 0) { // 两探头都在白色
//         Car_Go();
//         // Mdelay_Lib(10); // 短暂延时，确保前进指令执行
//       } else if (left == 1 && right == 1) { // 两探头都在黑色
//         Car_Stop();
//         // Mdelay_Lib(20); // 短暂延时，确保停止指令执行
//       } else if (left == 1 &&
//                  right == 0) { // 左探头在黑色，右探头在白色 -> 左转
//         // 使用增强版左转，提高左转灵敏度
//         Car_SharpTurnLeft();
//         // Mdelay_Lib(35); // 增加左转时间，与右转保持一致
//       } else if (left == 0 &&
//                  right == 1) { // 左探头在白色，右探头在黑色 -> 右转
//         // 使用增强版右转，提高右转灵敏度
//         Car_SharpTurnRight();
//         // Mdelay_Lib(35); // 增加右转时间，与左转保持一致
//       }
//       // 其他情况 (理论上不会出现) 保持停止
//     }
//   }
// }

// ==================== 引脚定义 ====================
// 按键
#define S1_PIN        GPIO_Pin_0
#define S1_PORT       GPIOA

// LED
#define D1_PIN        GPIO_Pin_9
#define D1_PORT       GPIOF
#define D2_PIN        GPIO_Pin_10
#define D2_PORT       GPIOF
#define D3_PIN        GPIO_Pin_13
#define D3_PORT       GPIOE
#define D4_PIN        GPIO_Pin_14
#define D4_PORT       GPIOE

// ==================== 函数声明 ====================
void led_gpio_init(void);
void s1_int_init(void);
void EXTI0_IRQHandler(void);

// ==================== LED 初始化 ====================
void led_gpio_init()
{
	// PF9 PF10 PE13 PE14
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_InitTypeDef g;
	g.GPIO_Mode = GPIO_Mode_OUT;
	g.GPIO_Speed = GPIO_Speed_50MHz;
	g.GPIO_OType = GPIO_OType_PP;

	 // D1 - PF9
    g.GPIO_Pin = D1_PIN;
    GPIO_Init(D1_PORT, &g);

    // D2 - PF10
    g.GPIO_Pin = D2_PIN;
    GPIO_Init(D2_PORT, &g);

    // D3 - PE13
    g.GPIO_Pin = D3_PIN;
    GPIO_Init(D3_PORT, &g);

    // D4 - PE14
    g.GPIO_Pin = D4_PIN;
    GPIO_Init(D4_PORT, &g);
	
  GPIO_ResetBits(D1_PORT, D1_PIN);
  GPIO_ResetBits(D2_PORT, D2_PIN);
  GPIO_ResetBits(D3_PORT, D3_PIN);
  GPIO_ResetBits(D4_PORT, D4_PIN);
}

void s1_int_init(void)
{
    // 1. GPIO 时钟 & 配置（输入模式无需 Speed）
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin = S1_PIN;
    gpio.GPIO_Mode = GPIO_Mode_IN;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;  // 上拉
    GPIO_Init(S1_PORT, &gpio);

    // 2. SYSCFG 映射
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0); // 注意：这里仍需用 EXTI_PinSource0

    // 3. EXTI 配置
    EXTI_InitTypeDef exti;
    exti.EXTI_Line = EXTI_Line0;           // 对应 Pin0
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);

    // 4. NVIC 配置
    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel = EXTI0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
}

void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line0); // 先清除中断标志

        GPIO_ToggleBits(D1_PORT, D1_PIN);   // 控制 D1 LED

        // 等待按键释放（防多次触发）
        while (GPIO_ReadInputDataBit(S1_PORT, S1_PIN) == RESET);

        // 软件消抖延时（使用你的延时函数）
        Mdelay_Lib(10);  // 延时 10ms
    }
}
// ==================== 主函数 ====================
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 全局中断分组，只调一次

    led_gpio_init();   // 你原有的 LED 初始化（已用宏）
    s1_int_init();     // 修改后的中断初始化

    while (1)
    {
        // 主循环空转，由中断驱动
    }
}