#define EFN     10
#define MAXBUFF 3000
#define VME
#define VMEINT
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// 以下部分用户需要修改

#define V2718   //如果使用 V1718 则注释本行

// 以上部分用户需要修改
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#define DBUFF

#define INTLEVEL   1
#define IOREG0     0x00000000
#define IODAQCLR   0x0010
#define IOCLR      0x0
#define IOEVTSTART 0x1
#define IOEVTSTOP  0x2

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// 以下部分用户需要修改

// 以地址 0xAAAABBBB 为例， AAAA 四位表示硬件地址， BBBB 四位表示寄存器地址，这里均设置为 0，表示起始位置。

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
#define ADC10ADDR  0x100A0000
#define ADC11ADDR  0x100B0000
#define ADC12ADDR  0x100C0000
#define ADC13ADDR  0x100D0000
#define ADC14ADDR  0x100E0000
#define ADC15ADDR  0x100F0000
#define ADC16ADDR  0x10100000
#define ADC17ADDR  0x10110000
#define ADC18ADDR  0x10120000
#define ADC19ADDR  0x10130000


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
#define MADC10ADDR  0x200A0000
#define MADC11ADDR  0x200B0000
#define MADC12ADDR  0x200C0000
#define MADC13ADDR  0x200D0000
#define MADC14ADDR  0x200E0000
#define MADC15ADDR  0x200F0000
#define MADC16ADDR  0x20100000
#define MADC17ADDR  0x20110000
#define MADC18ADDR  0x20120000
#define MADC19ADDR  0x20130000


//v1190a v1290a
#define V1x90ADDR0 0x40000000
#define V1x90ADDR1 0x40010000
#define V1x90ADDR2 0x40020000
#define V1x90ADDR3 0x40030000
#define V1x90ADDR4 0x40040000


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
