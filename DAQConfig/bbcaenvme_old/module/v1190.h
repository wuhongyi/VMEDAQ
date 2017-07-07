/* Header for V1190 Multihit TDC */

void v1190_intlevel(unsigned int maddr, short level);
void v1190_clear(unsigned int maddr);
void v1190_tdcfull(unsigned int maddr, short fl);
int v1190_segdata(unsigned int maddr);
int v1190_fxdmasegdata(unsigned int maddr);
int v1190_dmasegdata(unsigned int maddr, int rsize);

/* Register */
#define V1190_OUTBUFF         0x0000
#define V1190_SOFT_CLEAR      0x1016
#define V1190_ALMOST_FULL     0x1022
#define V1190_INTLEVEL        0x100a

/* Bit */
#define V1190_SOFT_CLEAR_BIT  1     

#define V1190_TYPE_MASK_S          0xf800
#define V1190_GLOBAL_HEADER_BIT_S  0x4000
#define V1190_TDC_HEADER_BIT_S     0x0800
#define V1190_TDC_DATA_BIT_S       0x0000
#define V1190_TDC_TRAILER_BIT_S    0x1800
#define V1190_TDC_ERROR_BIT_S      0x2000
#define V1190_GLOBAL_TRAILER_BIT_S 0x8000

/* For V1190, V1290 */
/* Register */
#define V1X90_INT_LEVEL       0x100a
#define V1X90_SOFT_CLEAR      0x1016
#define V1X90_ALMOST_FULL     0x1022
#define V1X90_EVT_FIFO        0x1038

/* Bit */
#define V1X90_SOFT_CLEAR_BIT  1
