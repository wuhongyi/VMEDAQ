/*
   V7XX, 32 channel Chage Sensing Converter
*/

int v7xx_segdata(unsigned int maddr);
int v7xx_segdata2(unsigned int maddr);
int v7xx_dmasegdata(unsigned int maddr, int cnt);
void v7xx_clear(unsigned int maddr);
void v7xx_mclear(unsigned int maddr);
void v7xx_intlevelmulti(unsigned int maddr, short level, short evtn);
void v7xx_intlevel(unsigned int maddr, short level);
void v7xx_multievtberr(unsigned int maddr);
void v7xx_singleevtberr(unsigned int maddr);
void v7xx_noberr(unsigned int maddr);
void v7xx_rst_counter(unsigned int maddr);
void v7xx_mrst_counter(unsigned int maddr);

/* Address Map */
#define V7XX_OUTBUFF        0x0000   /* - 0x07ff (D32) */
#define V7XX_FREV           0x1000
#define V7XX_GEO_ADDR       0x1002
#define V7XX_MCST_ADDR      0x1004
#define V7XX_BIT_SET1       0x1006
#define V7XX_BIT_CLE1       0x1008
#define V7XX_INT_REG1       0x100a
#define V7XX_INT_VEC1       0x100c
#define V7XX_STS_REG1       0x100e
#define V7XX_CTRL_REG1      0x1010
#define V7XX_ADER_H         0x1012
#define V7XX_ADER_L         0x1014
#define V7XX_SS_RST         0x1016
#define V7XX_MCST_CTRL      0x101a
#define V7XX_EVT_TRIG_REG   0x1020
#define V7XX_STS_REG2       0x1022
#define V7XX_EVT_CNT_L      0x1024
#define V7XX_EVT_CNT_H      0x1026
#define V7XX_INC_EVT        0x1028
#define V7XX_INC_OFF        0x102a
#define V7XX_LOAD_TEST_REG  0x102c
#define V7XX_FCLR_WINDOW    0x102e
#define V7XX_BIT_SET2       0x1032
#define V7XX_BIT_CLE2       0x1034
#define V7XX_WMEM_TEST      0x1036
#define V7XX_MEM_TEST_H     0x1038
#define V7XX_MEM_TEST_L     0x103a
#define V7XX_CRATE_SEL      0x103c
#define V7XX_TEST_EVT       0x103e
#define V7XX_EVT_CNT_RST    0x1040
#define V7XX_RTEST          0x1064
#define V7XX_SW_COMM        0x1068
#define V7XX_SLIDE_CONST    0x106a
#define V7XX_AAD            0x1070
#define V7XX_BAD            0x1072
#define V7XX_THRESH         0x1080 /* - 0x10BF */

/* Bits */
#define V7XX_TYPE_MASK      0x06000000
#define V7XX_HEADER_BIT     0x02000000
#define V7XX_DATA_BIT       0x00000000
#define V7XX_EOB_BIT        0x04000000
#define V7XX_ERROR_BIT      0x06000000
#define V7XX_TYPE_MASK_S    0x0600
#define V7XX_HEADER_BIT_S   0x0200
#define V7XX_DATA_BIT_S     0x0000
#define V7XX_EOB_BIT_S      0x0400
#define V7XX_ERROR_BIT_S    0x0600
