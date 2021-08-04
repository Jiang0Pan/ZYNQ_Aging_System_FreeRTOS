






#ifndef _DS1337_H
#define _DS1337_H




#include "IIC.h"


typedef enum {
	Seconds = (0x00),
	Minutes = (0x01),
	Hours   = (0x02),
	Day     = (0x03),
	Date    = (0x04),
	Month   = (0x05),
	Year    = (0x06),
}ds_time_reg;

#define ds_ctrl_reg  (0x0E)
#define ds_stat_reg  (0x0F)


typedef union {
	struct {
		u8 Addr;
		union {
			struct {
				u8 _sec   : 4;
				u8 _10sec : 3;
			};
		}SEC;

		union {
			struct {
				u8 _min   : 4;
				u8 _10min : 3;
			};
		}MIN;

		union {
			struct {
				u8 _hou   : 4;
				u8 _10hou : 2;
				u8 _wk    : 1;
			};
		}HOU;

		union {
			struct {
				u8 _day   : 3;
			};
		}DAY;

		union {
			struct {
				u8 _date   : 4;
				u8 _10date : 2;
			};
		}DATE;

		union {
			struct {
				u8 _mon   : 4;
				u8 _10mon : 1;
				u8        : 2;
				u8 _cen   : 1;
			};
		}MON;

		union {
			struct {
				u8 _yea   : 4;
				u8 _10yea : 4;
			};
		}YEA;
	};
	u8 Byte[8];
}DS_Time;

typedef union {
	struct {
		u8 AxIE   : 2;
		u8 INTCN  : 1;
		u8 RS     : 2;
		u8        : 2;
		u8 EOSC   : 1;
	};
	u8 Ctrl[1];
}CTRL;

typedef union {
	struct {
		u8 AxF    : 2;
		u8        : 5;
		u8 OSF    : 1;
	};
	u8 Stat[1];
}STAT;





////////////////////////////////////////////////











#endif

