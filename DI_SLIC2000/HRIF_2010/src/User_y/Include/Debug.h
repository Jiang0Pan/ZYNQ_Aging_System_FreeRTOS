




#ifndef _DEBUG_H
#define _DEBUG_H


#include "GlobalRef.h"
#include "eeprom.h"
#include "fpga.h"


s32 Debug_MV_Vector(int argc, char * argv[]);

     s32 ReadEEPROM(int argc, char * argv[]);

    s32 WriteEEPROM(int argc, char * argv[]);

       s32 WriteReg(int argc, char * argv[]);

        s32 ReadReg(int argc, char * argv[]);

       s32 WriteDDR(int argc, char * argv[]);

        s32 ReadDDR(int argc, char * argv[]);

     s32 ReadSystem(int argc, char * argv[]);

     s32 Setready(int argc, char * argv[]);

     s32 Trigger(int argc, char * argv[]);

     int WriteIO(int argc, char * argv[]);

     int DebugEcho(int argc, char * argv[]);

     int ReadDebug(int argc, char * argv[]);

     int WriteDebug(int argc, char * argv[]);

     int Debug_SetDDR(int argc, char * argv[]);

     int Debug_FillDDR(int argc, char * argv[]);

     int Debug_Switch(int argc, char * argv[]);

     int Debug_Max528(int argc, char * argv[]);

     int Debug_DriverLevel(int argc, char * argv[]);

     int Debug_SMbus(int argc, char * argv[]);

     int Debug_Dpm(int argc, char * argv[]);

     int Debug_DAC(int argc, char * argv[]);

     int Debug_Max9972(int argc, char * argv[]);

     int SetIp(int argc, char * argv[]);

     int reboot(int argc, char * argv[]);

     int printdps(int argc, char * argv[]);

     int Debug_brebootfpga(int argc, char * argv[]);

     int Debug_List(int argc, char * argv[]);

     int reg_test(int argc, char * argv[]);

     int Debug_fpga(int argc, char * argv[]);

     int Insert_Status(int argc, char * argv[]);

     int Dps_Fpga(int argc, char * argv[]);

     int ADC_Set_Voltage(int argc, char * argv[]);

     int Isl28023_Set_Voltage(int argc, char * argv[]);

     int Isl8272_Read_Reg(int argc, char * argv[]);
     
     int Dps_Disbale(int argc, char * argv[]);

     int Get_Dps_Voltage(int argc, char * argv[]);

     int Get_Dps_Current(int argc, char * argv[]);

     int IIC_Address_Test(int argc, char * argv[]);

     int DPSA_Enable_Disable(int argc, char * argv[]);
#endif






