# ZYNQ_Aging_System_FreeRTOS

SOC：

    ZYNQ7Z015

OS:

    FreeRTOS
    
HRIF_2010:

    1. aging system
    2. peripherals:
       MIO
       EMIO
       IIC
       SPI
       UART
       AXI
       Ethernet
       QSPI
     3.function
       EMIO、MIO： system status display
       IIC: EEPROM board mangerment
            dps mode mangerment
            iic extern gpio
            iic switch
        spi: spi flash mangerment
        uart: log printf and system debug
        axi: pl communction and operation
        Ethernet: communction with pc
        qspi: qspi interface mangerment
     4. Task
        1. network physical thread
        2. network user layer thread
        3. system status thread
        4. shell thread
        
        
       
HRIF_2010_BSP:

    1. xilinx lib
  

fsbl:

    1. fsbl code

    
fsbl bsp:

    1. xilinx lib

pl008_chk201207_n_hw_platform_0:
    1. platform folder

pl008_chk201207_n.bit：
    1. pl bit file
   
pl008_chk201207_n.hdf:
    1. xilinux platform file


