#include "I2C.h"


void I2C_INIT(void)
{
    //使能端口B硬件时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

    //配置I2C SCL与SDA相关引脚模式
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;         //开漏模式
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;     //IO速度为由高电平跳变至低电平响应的时间
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;       
    GPIO_Init(GPIOB,&GPIO_InitStructure);


    //只要是输出状态，必须得有初始电平
    //根据I2C模式默认开始SCL与SDA为高电平
    SCL_W = 1;
    SDA_W = 1;
}

void SDA_Pin_Mode(GPIOMode_TypeDef Pin_Mode)
{
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode  = Pin_Mode;   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;         //开漏模式
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;     //IO速度为由高电平跳变至低电平响应的时间
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;       
    GPIO_Init(GPIOB,&GPIO_InitStructure);

}

void I2C_Start(void)
{
    //保证SDA引脚为输出模式
    SDA_Pin_Mode(GPIO_Mode_OUT);

    SCL_W = 1;
    SDA_W = 1;
    delay_xus(5);

    SDA_W = 0;
    delay_xus(5);

    SCL_W = 0;
    delay_xus(5);

}

void I2C_Stop(void)
{
    //保证SDA引脚为输出模式
    SDA_Pin_Mode(GPIO_Mode_OUT);

    SCL_W = 1;
    SDA_W = 0;
    delay_xus(5);

    SDA_W = 1;
    delay_xus(5);

}

void I2C_Send_Byte(uint8_t byte)
{
    int32_t i;
    //保证SDA引脚为输出模式
    SDA_Pin_Mode(GPIO_Mode_OUT);

    SCL_W = 0;
    SDA_W = 0;
    delay_xus(5);

    //8bit发送
    for(i = 7;i >= 0;i--)
    {
        if(byte & (1<<i))
            SDA_W = 1;

        delay_xus(5);

        SCL_W = 1;
        delay_xus(5);

        SCL_W = 0;
        delay_xus(5);

    }

}

uint8_t I2C_Recv_Byte(void)
{
    uint8_t d = 0;
    int32_t i;

    //保证SDA引脚为输入模式
    SDA_Pin_Mode(GPIO_Mode_IN);

    /*8bit读取*/
    for(i = 7;i >= 0;i--)
    {
        SCL_W = 1;
        delay_xus(5);

        if(SDA_R)
            d |= 1<<i;
        
        //继续保持占用总线
        SCL_W = 0;
        delay_xus(5);

    }

    return d;
}

/*I2C等待ACK从机响应*/
uint8_t I2C_Wait_ACK(void)
{

    uint8_t ack;
    //保证SDA引脚为输入模式
    SDA_Pin_Mode(GPIO_Mode_IN);

    //紧接着第九个时钟周期,将SCL拉高
    SCL_W = 1;
    delay_xus(5);

    if(SDA_R)   //读取SDA是否有应答信号，有应答是低电平,无应答则反之
        ack = 1;

    SCL_W = 0;  //继续保持占用总线
    delay_xus(5);

    return ack;

}

int32_t AT24C02_Write(uint8_t addr,uint8_t *buf,uint8_t len)
{

    uint8_t ack;
    uint8_t *p = buf;

    //发送起始信号
    I2C_Start();

    //进行设备寻址，写访问地址0xA0
    I2C_Send_Byte(0xA0);
    
    //等待应答处理
    ack = I2C_Wait_ACK();
    if(ack)
    {
        printf("device address error\r\n");
        return -1;
    }

    //发送要放EEPROM数据的起始地址
    I2C_Send_Byte(addr);

    ack = I2C_Wait_ACK();
    if(ack)
    {
        printf("word address error\r\n");
        return -2;
    }
    //连续发送数据
    while(len--)
    {
        I2C_Send_Byte(*p++);

        ack = I2C_Wait_ACK();

        if(ack)
        {
            printf("write data error\r\n");
            return -3;
        }
    }

    I2C_Stop();
    printf("write data success\r\n");
    return 0;
}

void I2C_ACK(uint8_t ack)
{
    //保证SDA引脚为输出模式
    SDA_Pin_Mode(GPIO_Mode_OUT);

    SCL_W = 0;
    SDA_W = 0;
    delay_xus(5);

    if(ack)
        SDA_W = 1;

    delay_xus(5);

    SCL_W = 1;
    delay_xus(5);

    SCL_W = 0;
    delay_xus(5);
}

int32_t AT24C02_Read(uint8_t addr,uint8_t *buf,uint8_t len)
{
    uint8_t ack;
    uint8_t *p = buf;

    //发送起始信号
    I2C_Start();

    //进行设备寻址，写访问地址为0xA0
    I2C_Send_Byte(0xA0);

    ack = I2C_Wait_ACK();
    if(ack)
    {
        printf("device address error\r\n");
        return -1;

    }

    //发送要访问EEPROM数据的起始地址
    I2C_Send_Byte(addr);

    ack = I2C_Wait_ACK();
    if(ack)
    {
        printf("word address error\r\n");
        return -2;
    }

    //发送起始信号，用于切换访问方向，从写访问切换读访问
    I2C_Start();

    //进行设备寻址，写访问地址为0XA1,1010为固定发送MSB的高4位
    //1为最后读写为使得R读有效需使为高电平
    I2C_Send_Byte(0xA1);
    ack = I2C_Wait_ACK();
    if(ack)
    {
        printf("device address error\r\n");
        return -3;
    }

    //连续读取数据
    len = len-1;
    
    while(len--)
    {
        //读取数据
        *p = I2C_Recv_Byte();

        //发送应答信号
        I2C_ACK(0);

        p++;
    }

    //读取最后一个字节
    *p = I2C_Recv_Byte();

    //发送无应答信号
    I2C_ACK(1);

    I2C_Stop();

    return 0;
}

