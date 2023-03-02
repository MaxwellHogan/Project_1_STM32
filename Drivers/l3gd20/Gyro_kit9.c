#include "Gyro_kit9.h"
#include "l3gd20.h"


/* Read/Write command */
#define READWRITE_CMD              ((uint8_t)0x80) 
/* Multiple byte read/write command */ 
#define MULTIPLEBYTE_CMD           ((uint8_t)0x40)
/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                 ((uint8_t)0x00)

/**
  * @brief  Writes one byte to the GYROSCOPE.
  * @param  pBuffer pointer to the buffer  containing the data to be written to the GYROSCOPE.
  * @param  WriteAddr GYROSCOPE's internal address to write to.
  * @param  NumByteToWrite Number of bytes to write.
  * @retval None
  */
void GYRO_IO_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  /* Configure the MS bit: 
       - When 0, the address will remain unchanged in multiple read/write commands.
       - When 1, the address will be auto incremented in multiple read/write commands.
  */
  if(NumByteToWrite > 0x01)
  {
    WriteAddr |= (uint8_t)MULTIPLEBYTE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  GYRO_CS_LOW();
  
  /* Send the Address of the indexed register */
  SPI1_WriteRead(WriteAddr);
  
  /* Send the data that will be written into the device (MSB First) */
  while(NumByteToWrite >= 0x01)
  {
    SPI1_WriteRead(*pBuffer);
    NumByteToWrite--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  GYRO_CS_HIGH();
}

/**
  * @brief  Reads a block of data from the GYROSCOPE.
  * @param  pBuffer pointer to the buffer that receives the data read from the GYROSCOPE.
  * @param  ReadAddr GYROSCOPE's internal address to read from.
  * @param  NumByteToRead number of bytes to read from the GYROSCOPE.
  * @retval None
  */
void GYRO_IO_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{  
  if(NumByteToRead > 0x01)
  {
    ReadAddr |= (uint8_t)(READWRITE_CMD | MULTIPLEBYTE_CMD);
  }
  else
  {
    ReadAddr |= (uint8_t)READWRITE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  GYRO_CS_LOW();
  
  /* Send the Address of the indexed register */
  SPI1_WriteRead(ReadAddr);
  
  /* Receive the data that will be read from the device (MSB First) */
  while(NumByteToRead > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock to GYROSCOPE (Slave device) */
    *pBuffer = SPI1_WriteRead(DUMMY_BYTE);
    NumByteToRead--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  GYRO_CS_HIGH();
}  

/**
  * @brief  Set GYROSCOPE Initialization.
  * @retval GYRO_OK if no problem during initialization
  */
uint8_t GYRO_Init(void)
{  
  uint8_t ret = GYRO_ERROR;
  uint16_t ctrl = 0x0000;
  
//  /* Configure Mems : data rate, power mode, full scale and axes */
//  L3GD20_InitStructure.Power_Mode = L3GD20_MODE_ACTIVE;
//  L3GD20_InitStructure.Output_DataRate = L3GD20_OUTPUT_DATARATE_1;
//  L3GD20_InitStructure.Axes_Enable = L3GD20_AXES_ENABLE;
//  L3GD20_InitStructure.Band_Width = L3GD20_BANDWIDTH_4;
//  L3GD20_InitStructure.BlockData_Update = L3GD20_BlockDataUpdate_Continous;
//  L3GD20_InitStructure.Endianness = L3GD20_BLE_LSB;
//  L3GD20_InitStructure.Full_Scale = L3GD20_FULLSCALE_500; 
//  
  /* Configure MEMS: data rate, power mode, full scale and axes */
  ctrl = (uint16_t) (L3GD20_MODE_ACTIVE | L3GD20_OUTPUT_DATARATE_1 | \
    L3GD20_AXES_ENABLE | L3GD20_BANDWIDTH_4);
  
  ctrl |= (uint16_t) ((L3GD20_BlockDataUpdate_Continous | L3GD20_BLE_LSB | \
		  L3GD20_FULLSCALE_500) << 8);
  
  /* L3gd20 Init */	 
  L3GD20_Init(ctrl);
  
  ctrl = (uint8_t) ((L3GD20_HPM_NORMAL_MODE_RES | L3GD20_HPFCF_0));		
  
  L3GD20_FilterConfig(ctrl) ;
  
  L3GD20_FilterCmd(L3GD20_HIGHPASSFILTER_ENABLE);
  
//  L3GD20_DisableIT(L3GD20_INT2);
//  L3GD20_EnableIT(L3GD20_INT2);
  
  ret = GYRO_OK;
  
  return ret;
}
