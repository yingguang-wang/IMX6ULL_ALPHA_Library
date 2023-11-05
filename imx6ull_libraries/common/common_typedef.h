#ifndef __COMMON_TYPEDEF_H_
#define __COMMON_TYPEDEF_H_
/* AUTOSAR integer data types*/

/* unsigned char with a bit length that is the shortest one natively supported
  by the platform.*/
  /* [cover parentID={F7B51F71-6687-4e05-8408-7F5AAC55C638}] boolean [/cover]*/
typedef unsigned char       boolean;        /* for use with TRUE/FALSE      */

/* 8bit unsigned :  0 .. 255 [0X00 .. 0XFF]*/
/* [cover parentID={4269E5AB-7F28-4803-8D60-7B4EC91CB087}] uint8 [/cover] */
typedef unsigned char       uint8;

/* 16bit unsigned:  0..65535 [0x0000..0xFFFF]*/
/* [cover parentID={66E964CA-35D5-4013-BB61-1E824636D713}] uint16 [/cover] */
typedef unsigned short      uint16;

/* 32bit unsigned:  0..4294967295 [0x00000000..0xFFFFFFFF]*/
/* [cover parentID={DA33B7A0-7CD3-45e7-9C9A-6D63FB8BA3DC}] uint32 [/cover] */
typedef unsigned long       uint32;

/* 64bit unsigned
*          0..18446744073709551615   [0x0000000000000000..0xFFFFFFFFFFFFFFFF]*/
/* [cover parentID={3409E2A3-BF2D-44a5-9B00-A72300848166}] uint64 */
typedef unsigned long long  uint64;

/* 8bit signed, 7 bit + 1 bit sign -128..+127 [0x80..0x7F]*/
/* [cover parentID={94E0756B-993D-4cae-9499-416CDFD6FEAF}] sint8[/cover]*/
typedef signed char         sint8;

/* 16bit signed, 15 bit + 1 bit sign -32768..+32767 [0x8000..0x7FFF]*/
/* [cover parentID={B3482DFF-8DFF-41bd-95E0-0406E2451CB0}] sint16 [/cover]*/
typedef short               sint16;

/* 32bit signed, 31 bit + 1 bit sign
 -2147483648..+2147483647 [0x80000000..0x7FFFFFFF]*/
 /* [cover parentID={B027B471-A1A2-456c-A015-35F4A34A88EF}] sint32 [/cover]*/
typedef long                sint32;
/*
* 64bit signed, 63 bit + 1 bit sign
* -9223372036854775808..9223372036854775807
* [0x8000000000000000..0x7FFFFFFFFFFFFFFF]
*/
/* [cover parentID={3CF3471C-EB1A-450c-B78F-4B96D226A1F5}] sint64 [/cover]*/
typedef long long           sint64;

/* At least 8 bit*/
/* [cover parentID={F8719785-0A16-486e-AB85-0A2859402037}] uint8_least[/cover]*/
typedef unsigned long       uint8_least;

/* At least 16 bit*/
/* [cover parentID={BEAD868D-0EC1-44f0-AFEE-B57401CC9E65}]uint16_least[/cover]*/
typedef unsigned long       uint16_least;

/* least 32 bit*/
/* [cover parentID={9B9CC46A-0F61-4d25-8001-679CF210C135}]uint32_least[/cover]*/
typedef unsigned long       uint32_least;

/* At least 7 bit + 1 bit sign*/
/* [cover parentID={5C0DE046-8407-4708-8D26-41B96731D89D}]sint8_least[/cover]*/
typedef signed long         sint8_least;

/* At least 15 bit + 1 bit sign*/
/* [cover parentID={0A83DB6E-ECD8-42f0-B97C-057F9FBFEB6E}]sint16_least[/cover]*/
typedef signed long         sint16_least;

/* At least 31 bit + 1 bit sign*/
/* [cover parentID={A65F0248-A0A7-4ab7-BAFA-A5428F4E8A96}]sint32_least[/cover]*/
typedef signed long         sint32_least;

/* IEEE754-2008 single precision
* -3.4028235e+38..+3.4028235e+38*/
/* [cover parentID={BBC4F70E-DA81-4d37-BCA4-628A89B29517}] float32  [/cover]*/
typedef float               float32;        /* IEEE754-2008 single precision */

/* IEEE754-2008 double precision
* -1.7976931348623157e+308..+1.7976931348623157e+308*/
/* [cover parentID={0D62172C-9309-493a-8028-06A7299D7906}] float64 [/cover]*/
typedef double              float64;        /* IEEE754-2008 double precision */

// 数据类型声明(兼容库1)
// 尽量使用 stdint.h 定义的类型名称 避免冲突 这里可以裁剪
typedef signed char         int8;                                               // 有符号  8 bits
typedef signed short int    int16;                                              // 有符号 16 bits
typedef signed int          int32;                                              // 有符号 32 bits
typedef signed long long    int64;                                              // 有符号 64 bits

typedef volatile uint8      vuint8;                                             // 易变性修饰 无符号  8 bits
typedef volatile uint16     vuint16;                                            // 易变性修饰 无符号 16 bits
typedef volatile uint32     vuint32;                                            // 易变性修饰 无符号 32 bits
typedef volatile uint64     vuint64;                                            // 易变性修饰 无符号 64 bits

typedef volatile int8       vint8;                                              // 易变性修饰 有符号  8 bits
typedef volatile int16      vint16;                                             // 易变性修饰 有符号 16 bits
typedef volatile int32      vint32;                                             // 易变性修饰 有符号 32 bits
typedef volatile int64      vint64;                                             // 易变性修饰 有符号 64 bits
// 数据类型声明(兼容库2)
typedef signed char            int8_t;
typedef signed short int        int16_t;
typedef signed int              int32_t;
typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
typedef unsigned int            uint32_t;
typedef unsigned long long      uint64_t;
typedef	signed char  	 	        s8;		
typedef	signed short int        s16;
typedef	signed int 		          s32;
typedef	signed long long int    s64;
typedef	unsigned char 		      u8;
typedef	unsigned short int      u16;
typedef	unsigned int 		        u32;
typedef unsigned long long int  u64;

#endif
