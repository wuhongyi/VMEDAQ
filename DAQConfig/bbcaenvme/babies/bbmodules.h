#define EFN     10
#define MAXBUFF 3000
#define VME
#define VMEINT
#define V2718

#define DBUFF

#define INTLEVEL   1
#define IOREG0     0x00000000
#define IODAQCLR   0x0010
#define IOCLR      0x0
#define IOEVTSTART 0x1
#define IOEVTSTOP  0x2

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// 以下部分用户需要修改

#define ADC0ADDR   0x10000000
#define ADC1ADDR   0x10010000
#define ADC2ADDR   0x10020000
#define ADC3ADDR   0x10030000
#define ADC4ADDR   0x10040000
#define ADC5ADDR   0x10050000
#define ADC6ADDR   0x10060000
#define ADC7ADDR   0x10070000
#define ADC8ADDR   0x10080000
#define ADC9ADDR   0x10090000
// #define ADC10ADDR  0x100A0000
// #define ADC11ADDR  0x100B0000
// #define ADC12ADDR  0x100C0000
// #define ADC13ADDR  0x100D0000

#define MADC0ADDR   0x20000000
#define MADC1ADDR   0x20010000
#define MADC2ADDR   0x20020000
#define MADC3ADDR   0x20030000
#define MADC4ADDR   0x20040000
#define MADC5ADDR   0x20050000
#define MADC6ADDR   0x20060000
#define MADC7ADDR   0x20070000
#define MADC8ADDR   0x20080000
#define MADC9ADDR   0x20090000

//v1190a v1290a
#define V1x90ADDR0 0x40000000
#define V1x90ADDR1 0x40010000

//v830
#define SCAADDR0   0x50000000
#define SCAADDR1   0x50010000
#define SCA_NCH    32

// 以上部分用户需要修改
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#define MSTTDCADDR    0xbb000000
#define MSTQDCADDR    0xcc000000
#define MSTMDCADDR    0xdd000000
#define USE_CAEN
#define USE_LUPO

#define A32 0x09
