/**
 * @file shell_cmd_list.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief shell cmd list
 * @version 3.0.0
 * @date 2020-01-17
 * 
 * @copyright (c) 2020 Letter
 * 
 */

#include "shell.h"
#include "Debug.h"



extern int shellSetVar(char *name, int value);
extern void shellUp(Shell *shell);
extern void shellDown(Shell *shell);
extern void shellRight(Shell *shell);
extern void shellLeft(Shell *shell);
extern void shellTab(Shell *shell);
extern void shellBackspace(Shell *shell);
extern void shellDelete(Shell *shell);
extern void shellEnter(Shell *shell);
extern void shellHelp(int argc, char *argv[]);
extern void shellUsers(void);
extern void shellCmds(void);
extern void shellVars(void);
extern void shellKeys(void);
extern void shellClear(void);

SHELL_AGENCY_FUNC(shellRun, shellGetCurrent(), (const char *)p1);



const ShellCommand shellCommandList[] =
{
    {.attr.value=SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_USER),
    .data.user.name = SHELL_DEFAULT_USER,
    .data.user.password = SHELL_DEFAULT_USER_PASSWORD,
    .data.user.desc = "default user"},
    SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),
                   setVar, shellSetVar, set var),
    SHELL_KEY_ITEM(SHELL_CMD_PERMISSION(0), 0x1B5B4100, shellUp, up),
    SHELL_KEY_ITEM(SHELL_CMD_PERMISSION(0), 0x1B5B4200, shellDown, down),
    SHELL_KEY_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_ENABLE_UNCHECKED,
                   0x1B5B4300, shellRight, right),
    SHELL_KEY_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_ENABLE_UNCHECKED,
                   0x1B5B4400, shellLeft, left),
    SHELL_KEY_ITEM(SHELL_CMD_PERMISSION(0), 0x09000000, shellTab, tab),
    SHELL_KEY_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_ENABLE_UNCHECKED,
                   0x08000000, shellBackspace, backspace),
    SHELL_KEY_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_ENABLE_UNCHECKED,
                   0x7F000000, shellDelete, delete),
    SHELL_KEY_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_ENABLE_UNCHECKED,
                   0x1B5B337E, shellDelete, delete),
#if SHELL_ENTER_LF == 1
    SHELL_KEY_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_ENABLE_UNCHECKED,
                   0x0A000000, shellEnter, enter),
#endif
#if SHELL_ENTER_CR == 1
    SHELL_KEY_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_ENABLE_UNCHECKED,
                   0x0D000000, shellEnter, enter),
#endif
#if SHELL_ENTER_CRLF == 1
    SHELL_KEY_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_ENABLE_UNCHECKED,
                   0x0D0A0000, shellEnter, enter),
#endif
    SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
                   help, shellHelp, show command info\r\nhelp [cmd]),
    SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN,
                   users, shellUsers, list all user),
    SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN,
                   ls, shellCmds, list all cmd),
    SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN,
                   vars, shellVars, list all var),
    SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN,
                   keys, shellKeys, list all key),
    SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN,
                   clear, shellClear, clear console),
#if SHELL_EXEC_UNDEF_FUNC == 1
    SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
                   exec, shellExecute, execute function undefined),
#endif
	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
			    echo, DebugEcho, echo hellowd),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
			    move, Debug_MV_Vector, "move bid addr(hex) frm"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
			    read, ReadDebug, "read bid obj addr(hex) size"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
			 	write, WriteDebug, "write bid obj addr(hex) msg"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
			   	setddr, Debug_SetDDR, "setddr addr(hex) frm end"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
		       	switch, Debug_Switch, "switch 12v/io (pin) on/off"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
				setready, Setready, "setready bid addr(hex) frm"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
				trigger, Trigger, "trigger/trigger level"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
				fill, Debug_FillDDR, "fill addr flag length"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
				max528, Debug_Max528, "max528 bid cs dacid Votage"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
				driver, Debug_DriverLevel, "driver bid obj Votage0 Votage1"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
				smbus, Debug_SMbus, "smbus r/w slaveaddr(hex) code size"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
				dac, Debug_DAC, "dac r/w ndac Votage"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
				max9972, Debug_Max9972, "max9972 bid r/w cs data"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
				ip, SetIp, "ip set/get num"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
				dpsinfo, printdps, "dpsinfo"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
				rebootfpga, Debug_brebootfpga, "rebootfpga bid"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
				list, Debug_List, "list set num addr(hex) len repeat/list w num/list start/list stop"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
				dpm, Debug_Dpm, "dpm adc n"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
				regtest, reg_test, "regtest bid reg time"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
				fpga, Debug_fpga, "fpga start/stop/pause/reset"),



	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
				insert, Insert_Status, "Insert status data"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
				dps_fpga, Dps_Fpga, "Write/Read fpga control dps"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
				set_ad5694, ADC_Set_Voltage, "Set AD5694 Voltage"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
			set_isl28023, Isl28023_Set_Voltage, "Set Isl28023 Output Voltage"),			

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
			read_isl8272, Isl8272_Read_Reg, "Read 8272 Register Data"),				

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
			dps_disable, Dps_Disbale, "Dps Disbale"),	

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
			get_current, Get_Dps_Current, "Get Dps Channel Current"),				

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
			get_voltage, Get_Dps_Voltage, "Get Dps Channel Voltage"),		

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
			iic_test, IIC_Address_Test, "iic slave address test"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
			dpsa, DPSA_Enable_Disable, "DPSA Enable/Disable"),

	SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
				reboot, reboot, "reboot"),

				
};


/**
 * @brief shell命令长度
 * 
 */
const unsigned short shellCommandCount 
    = sizeof(shellCommandList) / sizeof(ShellCommand);


