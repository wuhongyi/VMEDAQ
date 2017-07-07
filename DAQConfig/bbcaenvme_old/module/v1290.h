/* Header for V1290 Multihit TDC */

void v1290_intlevel(unsigned int maddr, short level);
void v1290_tdcfull(unsigned int maddr, short fl);
void v1290_clear(unsigned int maddr);
int v1290_segdata(unsigned int maddr);
int v1290_dmasegdata(unsigned int maddr, int cnt);


/* Register */
#define V1290_OUTBUFF         0x0000
#define V1290_SOFT_CLEAR      0x1016
#define V1290_ALMOST_FULL     0x1022
#define V1290_INTLEVEL        0x100a
#define V1290_CTRL_REG        0x1000

/* Bit */
#define V1290_SOFT_CLEAR_BIT  1     

#define V1290_TYPE_MASK_S          0xf800
#define V1290_GLOBAL_HEADER_BIT_S  0x4000
#define V1290_TDC_HEADER_BIT_S     0x0800
#define V1290_TDC_DATA_BIT_S       0x0000
#define V1290_TDC_TRAILER_BIT_S    0x1800
#define V1290_TDC_ERROR_BIT_S      0x2000
#define V1290_GLOBAL_TRAILER_BIT_S 0x8000

#define V1290_TYPE_MASK          0xf8000000
#define V1290_GLOBAL_HEADER_BIT  0x40000000
#define V1290_TDC_HEADER_BIT     0x08000000
#define V1290_TDC_DATA_BIT       0x00000000
#define V1290_TDC_TRAILER_BIT    0x18000000
#define V1290_TDC_ERROR_BIT      0x20000000
#define V1290_GLOBAL_TRAILER_BIT 0x80000000
