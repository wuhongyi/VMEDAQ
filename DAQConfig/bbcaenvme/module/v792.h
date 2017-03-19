/*
   V792, 32 channel Chage Sensing Converter
*/

int v792_segdata(unsigned int maddr);
int v792_segdata2(unsigned int maddr);
int v792_dmasegdata(unsigned int maddr, int cnt);
void v792_clear(unsigned int maddr);
void v792_intlevelmulti(unsigned int maddr, short level, short evtn);
void v792_intlevel(unsigned int maddr, short level);
void v792_multievtberr(unsigned int maddr);
void v792_singleevtberr(unsigned int maddr);
void v792_noberr(unsigned int maddr);

/* Address Map */
#define V792_OUTBUFF        0x0000   /* - 0x07ff (D32) */
#define V792_FREV           0x1000
#define V792_GEO_ADDR       0x1002
#define V792_MCST_ADDR      0x1004
#define V792_BIT_SET1       0x1006
#define V792_BIT_CLE1       0x1008
#define V792_INT_REG1       0x100a
#define V792_INT_VEC1       0x100c
#define V792_STS_REG1       0x100e
#define V792_CTRL_REG1      0x1010
#define V792_ADER_H         0x1012
#define V792_ADER_L         0x1014
#define V792_SS_RST         0x1016
#define V792_MCST_CTRL      0x101a
#define V792_EVT_TRIG_REG   0x1020
#define V792_STS_REG2       0x1022
#define V792_EVT_CNT_L      0x1024
#define V792_EVT_CNT_H      0x1026
#define V792_INC_EVT        0x1028
#define V792_INC_OFF        0x102a
#define V792_LOAD_TEST_REG  0x102c
#define V792_FCLR_WINDOW    0x102e
#define V792_BIT_SET2       0x1032
#define V792_BIT_CLE2       0x1034
#define V792_WMEM_TEST      0x1036
#define V792_MEM_TEST_H     0x1038
#define V792_MEM_TEST_L     0x103a
#define V792_CRATE_SEL      0x103c
#define V792_TEST_EVT       0x103e
#define V792_EVT_CNT_RST    0x1040
#define V792_RTEST          0x1064
#define V792_SW_COMM        0x1068
#define V792_SLIDE_CONST    0x106a
#define V792_AAD            0x1070
#define V792_BAD            0x1072
#define V792_THRESH         0x1080 /* - 0x10BF */

/* Bits */
#define V792_TYPE_MASK      0x06000000
#define V792_HEADER_BIT     0x02000000
#define V792_DATA_BIT       0x00000000
#define V792_EOB_BIT        0x04000000
#define V792_ERROR_BIT      0x06000000
#define V792_TYPE_MASK_S    0x0600
#define V792_HEADER_BIT_S   0x0200
#define V792_DATA_BIT_S     0x0000
#define V792_EOB_BIT_S      0x0400
#define V792_ERROR_BIT_S    0x0600
