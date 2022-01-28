#include "SI24R1.h"

uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {
0x0A,0x01,0x07,0x0E,0x01};  // ����һ����̬���͵�ַ


static uint8_t SPI_RW(uint8_t byte) {
    uint8_t bit_re;
    HAL_SPI_TransmitReceive(&hspi2, (uint8_t *) &byte, (uint8_t *) &bit_re, 1, 0xff);
    return (bit_re);
}


/********************************************************
�������ܣ�SI24R1���ų�ʼ��                
��ڲ�������
����  ֵ����
*********************************************************/
void SI24R1_Init(void) {
    HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_SET);
//    SCK = 0;                                                    //SPIʱ��������
//    CSN = 1;
//    CE = 0;
//    IRQ = 1;
}


/********************************************************
�������ܣ�д�Ĵ�����ֵ�����ֽڣ�                
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��NRF_WRITE_REG��reg��
					value:�Ĵ�����ֵ
����  ֵ��״̬�Ĵ�����ֵ
*********************************************************/
uint8_t SI24R1_Write_Reg(uint8_t reg, uint8_t value) {
    uint8_t status;

    HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_RESET);
    status = SPI_RW(reg);
    SPI_RW(value);
    HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_SET);

    return (status);
}


/********************************************************
�������ܣ�д�Ĵ�����ֵ�����ֽڣ�                  
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��NRF_WRITE_REG��reg��
					pBuf:д�����׵�ַ
					bytes:д�����ֽ���
����  ֵ��״̬�Ĵ�����ֵ
*********************************************************/
uint8_t SI24R1_Write_Buf(uint8_t reg, const uint8_t *pBuf, uint8_t bytes) {
    uint8_t status, byte_ctr;

    HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_RESET);
    status = SPI_RW(reg);
    for (byte_ctr = 0; byte_ctr < bytes; byte_ctr++)
        SPI_RW(*pBuf++);
    HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_SET);

    return (status);
}


/********************************************************
�������ܣ���ȡ�Ĵ�����ֵ�����ֽڣ�                  
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��NRF_READ_REG��reg��
����  ֵ���Ĵ���ֵ
*********************************************************/
uint8_t SI24R1_Read_Reg(uint8_t reg) {
    uint8_t value;

    HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_RESET);
    SPI_RW(reg);
    value = SPI_RW(0);
    HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_SET);

    return (value);
}


/********************************************************
�������ܣ���ȡ�Ĵ�����ֵ�����ֽڣ�                  
��ڲ�����reg:�Ĵ���ӳ���ַ��NRF_READ_REG��reg��
					pBuf:���ջ��������׵�ַ
					bytes:��ȡ�ֽ���
����  ֵ��״̬�Ĵ�����ֵ
*********************************************************/
uint8_t SI24R1_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes) {
    uint8_t status, byte_ctr;

    HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_RESET);
    status = SPI_RW(reg);
    for (byte_ctr = 0; byte_ctr < bytes; byte_ctr++)
        pBuf[byte_ctr] = SPI_RW(0);                   //��ȡ���ݣ����ֽ���ǰ
    HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_SET);

    return (status);
}


/********************************************************
�������ܣ�SI24R1����ģʽ��ʼ��                      
��ڲ�������
����  ֵ����
*********************************************************/
void SI24R1_RX_Mode(void) {

    HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_RESET);
    SI24R1_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);    // �����豸����ͨ��0ʹ�úͷ����豸��ͬ�ķ��͵�ַ
    SI24R1_Write_Reg(NRF_WRITE_REG + EN_AA, 0x01);                                    // ʹ�ܽ���ͨ��0�Զ�Ӧ��
    SI24R1_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);                                // ʹ�ܽ���ͨ��0
    SI24R1_Write_Reg(NRF_WRITE_REG + RF_CH, 40);                                        // ѡ����Ƶͨ��0x40
    SI24R1_Write_Reg(NRF_WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);                        // ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ��
    SI24R1_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f);                                    // ���ݴ�����2Mbps�����书��7dBm
    SI24R1_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f);                                    // CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
    SI24R1_Write_Reg(NRF_WRITE_REG + STATUS, 0xff);                                                //������е��жϱ�־λ
    HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_SET);                                                                               // ����CE���������豸
}


/********************************************************
�������ܣ�SI24R1����ģʽ��ʼ��                      
��ڲ�������
����  ֵ����
*********************************************************/
void SI24R1_TX_Mode(void) {
    HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_RESET);
    SI24R1_Write_Buf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // д�뷢�͵�ַ
    SI24R1_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ

    SI24R1_Write_Reg(NRF_WRITE_REG + EN_AA, 0x01);                                                // ʹ�ܽ���ͨ��0�Զ�Ӧ��
    SI24R1_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);                                            // ʹ�ܽ���ͨ��0
    SI24R1_Write_Reg(NRF_WRITE_REG + SETUP_RETR,
                     0x0a);                                            // �Զ��ط���ʱ�ȴ�250us+86us���Զ��ط�10��
    SI24R1_Write_Reg(NRF_WRITE_REG + RF_CH, 40);                                                    // ѡ����Ƶͨ��0x40
    SI24R1_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f);                                                // ���ݴ�����2Mbps�����书��7dBm
    SI24R1_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);                                                // CRCʹ�ܣ�16λCRCУ�飬�ϵ�
    HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_SET);
}


/********************************************************
�������ܣ���ȡ��������                       
��ڲ�����rxbuf:�������ݴ���׵�ַ
����  ֵ��0:���յ�����
          1:û�н��յ�����
*********************************************************/
uint8_t SI24R1_RxPacket(uint8_t *rxbuf) {
    uint8_t state;
    state = SI24R1_Read_Reg(STATUS);                             //��ȡ״̬�Ĵ�����ֵ
    SI24R1_Write_Reg(NRF_WRITE_REG + STATUS, state);               //���RX_DS�жϱ�־

    if (state & RX_DR)                                                           //���յ�����
    {
        SI24R1_Read_Buf(RD_RX_PLOAD, rxbuf, TX_PLOAD_WIDTH);     //��ȡ����
        SI24R1_Write_Reg(FLUSH_RX, 0xff);                                  //���RX FIFO�Ĵ���
        return 0;
    }
    return 1;                                                   //û�յ��κ�����
}


/********************************************************
�������ܣ�����һ�����ݰ�                      
��ڲ�����txbuf:Ҫ���͵�����
����  ֵ��0x10:�ﵽ����ط�����������ʧ�� 
          0x20:���ͳɹ�            
          0xff:����ʧ��                  
*********************************************************/
uint8_t SI24R1_TxPacket(uint8_t *txbuf) {
    uint8_t state;
    HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_RESET);                                                                                                         //CE���ͣ�ʹ��SI24R1����
    SI24R1_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);        //д���ݵ�TX FIFO,32���ֽ�
    HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_SET);                                                                                                        //CE�øߣ�ʹ�ܷ���

    while (HAL_GPIO_ReadPin(IRQ_GPIO_Port,IRQ_Pin) == 1);                                                                                  //�ȴ��������
    state = SI24R1_Read_Reg(STATUS);                                              //��ȡ״̬�Ĵ�����ֵ
    SI24R1_Write_Reg(NRF_WRITE_REG + STATUS, state);                                //���TX_DS��MAX_RT�жϱ�־
    if (state & MAX_RT)                                                                                //�ﵽ����ط�����
    {
        SI24R1_Write_Reg(FLUSH_TX, 0xff);                                            //���TX FIFO�Ĵ���
        return MAX_RT;
    }
    if (state & TX_DS)                                                                                  //�������
    {
        return TX_DS;
    }
    return 0XFF;                                                                                          //����ʧ��
}

