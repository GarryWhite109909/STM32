#include "main.h"
#include "delay.h"
typedef enum {
    MODE_REMOTE,   // 遥控模式
    MODE_FOLLOW    // 循迹模式
} CarMode;

volatile CarMode current_mode = MODE_REMOTE; // 默认遥控
// ==================== 引脚定义 ====================
#define LEFT_A_PIN GPIO_Pin_7 // PD7 --> A-1A (左轮 IA)
#define LEFT_A_PORT GPIOD
#define LEFT_B_PIN GPIO_Pin_4 // PA4 --> A-1B (左轮 IB)
#define LEFT_B_PORT GPIOA

#define RIGHT_A_PIN GPIO_Pin_6 // PD6 --> B-1A (右轮 IA)
#define RIGHT_A_PORT GPIOD
#define RIGHT_B_PIN GPIO_Pin_7 // PB7 --> B-1B (右轮 IB)
#define RIGHT_B_PORT GPIOB

// 蜂鸣器控制引脚
#define BEEP_PORT       GPIOF
#define BEEP_PIN        GPIO_Pin_8

// 红外探头引脚
#define LEFT_SENSOR_PIN GPIO_Pin_15 // PG15 (DCMI RESET)
#define LEFT_SENSOR_PORT GPIOG
#define RIGHT_SENSOR_PIN GPIO_Pin_6 // PB6 (DCMI D5)
#define RIGHT_SENSOR_PORT GPIOB

// ==================== 小车动作函数 ====================
void Car_Go(void) {
  GPIO_SetBits(LEFT_A_PORT, LEFT_A_PIN);
  GPIO_ResetBits(LEFT_B_PORT, LEFT_B_PIN);
  GPIO_SetBits(RIGHT_A_PORT, RIGHT_A_PIN);
  GPIO_ResetBits(RIGHT_B_PORT, RIGHT_B_PIN);
}

void Car_Back(void) {
  GPIO_ResetBits(LEFT_A_PORT, LEFT_A_PIN);
  GPIO_SetBits(LEFT_B_PORT, LEFT_B_PIN);
  GPIO_ResetBits(RIGHT_A_PORT, RIGHT_A_PIN);
  GPIO_SetBits(RIGHT_B_PORT, RIGHT_B_PIN);
}

void Car_Stop(void) {
  GPIO_ResetBits(LEFT_A_PORT, LEFT_A_PIN);
  GPIO_ResetBits(LEFT_B_PORT, LEFT_B_PIN);
  GPIO_ResetBits(RIGHT_A_PORT, RIGHT_A_PIN);
  GPIO_ResetBits(RIGHT_B_PORT, RIGHT_B_PIN);
}

void Car_TurnLeft(void) {
  GPIO_ResetBits(LEFT_A_PORT, LEFT_A_PIN);
  GPIO_SetBits(LEFT_B_PORT, LEFT_B_PIN);
  GPIO_SetBits(RIGHT_A_PORT, RIGHT_A_PIN);
  GPIO_ResetBits(RIGHT_B_PORT, RIGHT_B_PIN);
}

void Car_TurnRight(void) {
  GPIO_SetBits(LEFT_A_PORT, LEFT_A_PIN);
  GPIO_ResetBits(LEFT_B_PORT, LEFT_B_PIN);
  GPIO_ResetBits(RIGHT_A_PORT, RIGHT_A_PIN);
  GPIO_SetBits(RIGHT_B_PORT, RIGHT_B_PIN);
}

// 增强左转函数，原地左转
void Car_SharpTurnLeft(void) {
    // 左轮停止（A=0, B=0），右轮正转（A=1, B=0）
    GPIO_ResetBits(LEFT_A_PORT, LEFT_A_PIN);
    GPIO_ResetBits(LEFT_B_PORT, LEFT_B_PIN);  
    GPIO_SetBits(RIGHT_A_PORT, RIGHT_A_PIN);
    GPIO_ResetBits(RIGHT_B_PORT, RIGHT_B_PIN);
}

// 增强右转函数，原地右转
void Car_SharpTurnRight(void) {
    // 左轮正转（A=1, B=0），右轮停止（A=0, B=0）
    GPIO_SetBits(LEFT_A_PORT, LEFT_A_PIN);
    GPIO_ResetBits(LEFT_B_PORT, LEFT_B_PIN);
    GPIO_ResetBits(RIGHT_A_PORT, RIGHT_A_PIN);
    GPIO_ResetBits(RIGHT_B_PORT, RIGHT_B_PIN); 
}

// 蜂鸣器控制函数
void Beep_On(void) {
    GPIO_SetBits(BEEP_PORT, BEEP_PIN);
}

void Beep_Off(void) {
    GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
}

// ==================== GPIO 初始化 ====================
void car_gpio_init(void) {
    GPIO_InitTypeDef g;
    // 使能所有用到的 GPIO 时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |
                           RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOF,
                           ENABLE);

    // === 电机引脚配置 ===
    g.GPIO_Mode = GPIO_Mode_OUT;
    g.GPIO_OType = GPIO_OType_PP;
    g.GPIO_Speed = GPIO_Speed_50MHz;
    g.GPIO_PuPd = GPIO_PuPd_NOPULL;

    g.GPIO_Pin = LEFT_A_PIN;
    GPIO_Init(LEFT_A_PORT, &g);
    g.GPIO_Pin = LEFT_B_PIN;
    GPIO_Init(LEFT_B_PORT, &g);
    g.GPIO_Pin = RIGHT_A_PIN;
    GPIO_Init(RIGHT_A_PORT, &g);
    g.GPIO_Pin = RIGHT_B_PIN;
    GPIO_Init(RIGHT_B_PORT, &g);

    // === 蜂鸣器引脚配置===
    g.GPIO_Pin = BEEP_PIN;
    g.GPIO_Mode = GPIO_Mode_OUT;
    g.GPIO_OType = GPIO_OType_PP;      // 推挽输出
    g.GPIO_Speed = GPIO_Speed_50MHz;
    g.GPIO_PuPd = GPIO_PuPd_NOPULL;    // 无上下拉
    GPIO_Init(BEEP_PORT, &g);          // 初始化 PF8

    // 安全初始化：停止电机 + 关闭蜂鸣器
    Car_Stop();
    Beep_Off(); // 确保上电静音
    Mdelay_Lib(10);
}
void sensor_gpio_init(void) {
  GPIO_InitTypeDef g;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOB,
                         ENABLE); // 使能GPIOG和GPIOB

  g.GPIO_Mode = GPIO_Mode_IN;
  g.GPIO_PuPd = GPIO_PuPd_NOPULL; // 浮空输入 (红外模块内部有上拉)
  g.GPIO_Speed = GPIO_Speed_50MHz;

  g.GPIO_Pin = LEFT_SENSOR_PIN;
  GPIO_Init(LEFT_SENSOR_PORT, &g);

  g.GPIO_Pin = RIGHT_SENSOR_PIN;
  GPIO_Init(RIGHT_SENSOR_PORT, &g);
}
// ==================== USART2 初始化 ====================
void usart2_init(void)
{
    // Step 1: 使能 GPIOA 时钟（USART2 使用 PA2/TX, PA3/RX）
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef g;
    
    // 配置 PA2 为复用推挽输出 (TX)
    g.GPIO_Pin = GPIO_Pin_2;
    g.GPIO_Mode = GPIO_Mode_AF;
    g.GPIO_Speed = GPIO_Speed_50MHz;
    g.GPIO_OType = GPIO_OType_PP;
    g.GPIO_PuPd = GPIO_PuPd_UP; // 可选上拉
    GPIO_Init(GPIOA, &g);

    // 配置 PA3 为复用浮空输入 (RX)
    g.GPIO_Pin = GPIO_Pin_3;
    g.GPIO_Mode = GPIO_Mode_AF;
    g.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &g);

    // Step 2: 配置 GPIO 复用为 USART2
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

    // Step 3: 使能 USART2 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    USART_InitTypeDef u;
    u.USART_BaudRate = 9600;                    // 波特率 9600
    u.USART_WordLength = USART_WordLength_8b;   // 8 数据位
    u.USART_StopBits = USART_StopBits_1;        // 1 停止位
    u.USART_Parity = USART_Parity_No;           // 无校验
    u.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // 收发模式
    u.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无流控
    USART_Init(USART2, &u);

    // Step 4: NVIC 配置
    NVIC_InitTypeDef n;
    n.NVIC_IRQChannel = USART2_IRQn;
    n.NVIC_IRQChannelPreemptionPriority = 2;
    n.NVIC_IRQChannelSubPriority = 2;
    n.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&n);

    // Step 5: 使能接收中断
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    // Step 6: 使能 USART2
    USART_Cmd(USART2, ENABLE);
}

// ==================== USART2 中断服务函数 ====================
void USART2_IRQHandler(void)
{
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        char recv_buf = USART_ReceiveData(USART2);

        if (current_mode == MODE_REMOTE)
        {
            // 遥控模式：响应 A~I
            if (recv_buf == 'A') {
                Car_Go();
            } else if (recv_buf == 'B') {
                Car_Back();
            } else if (recv_buf == 'C') {
                Car_TurnLeft();
            } else if (recv_buf == 'D') {
                Car_TurnRight();
            } else if (recv_buf == 'E') {
                Car_Stop();
            } else if (recv_buf == 'F') {
                Car_SharpTurnLeft();
            } else if (recv_buf == 'G') {
                Car_SharpTurnRight();
            } else if (recv_buf == 'H') {
                Beep_On(); // 打开蜂鸣器
            } else if (recv_buf == 'I') {
                Beep_Off(); // 关闭蜂鸣器
            } else if (recv_buf == 'S') {
                current_mode = MODE_FOLLOW; // 切换到循迹模式
                Car_Stop(); // 安全起见，先停止
            }
            // 可选：else if (recv_buf == 'R') { /* 已是遥控模式 */ }
        }
        else if (current_mode == MODE_FOLLOW)
        {
            // 循迹模式下，只响应 'R' 返回遥控
            if (recv_buf == 'R') {
                current_mode = MODE_REMOTE;
                Car_Stop();
            }
            // 忽略其他字符（如 A~E）
        }

        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}

// ==================== 主函数 ====================
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    car_gpio_init();      // 电机
    sensor_gpio_init();   // ← 新增：红外传感器
    usart2_init();        // 蓝牙串口

    while (1)
    {
        if (current_mode == MODE_FOLLOW)
        {
            // 执行循迹逻辑
            uint8_t left = GPIO_ReadInputDataBit(LEFT_SENSOR_PORT, LEFT_SENSOR_PIN);
            uint8_t right = GPIO_ReadInputDataBit(RIGHT_SENSOR_PORT, RIGHT_SENSOR_PIN);

            if (left == 0 && right == 0) {
                Car_Go();
            } else if (left == 1 && right == 1) {
                Car_Stop(); // 或继续前进，看赛道设计
            } else if (left == 1 && right == 0) {
                Car_SharpTurnLeft();   // 使用修正后的原地左转
            } else if (left == 0 && right == 1) {
                Car_SharpTurnRight();  // 使用修正后的原地右转
            }

            Mdelay_Lib(10); // 控制循迹频率
        }
        // 遥控模式下，什么都不做（由中断处理）
    }
}