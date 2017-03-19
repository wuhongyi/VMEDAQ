/*
   V830 VME Scaler/Counter
*/

void v830_initialize(unsigned int maddr);
void v830_clear_all(unsigned int maddr);
int  v830_segndata(unsigned int maddr, int nch);
int  v830_dmasegdata(unsigned int maddr, int cnt);
int  v830_fxdmasegdata(unsigned int maddr, int rsize);
void  v830_intlevel(unsigned int maddr, short level);


#define V830_NCH                         32
#define V830_MODE1                       0x0004

/* Regster Address Map */
#define V830_OUTBUFF                    0x1000
#define V830_CHANNEL_ENABLE             0x1100
#define V830_DWELL_TIME                 0x1104
#define V830_CONTROLL_REGSTER           0x1108
#define V830_BIT_SET_REGISTER           0x110a
#define V830_BIT_CLEAR_REGISTER         0x110c
#define V830_STATUS_REGISTER            0x110e
#define V830_GEO_REGISTER               0x1110
#define V830_INTERRUPT_LEVEL            0x1112
#define V830_INTERRUPT_VECTOR           0x1114
#define V830_ADER_32                    0x1116
#define V830_ADER_24                    0x1118
#define V830_ENABLE_ADER                0x111a
#define V830_MCST_BASE_ADDRESS          0x111c
#define V830_MCST_CONTROL               0x111e
#define V830_MODULE_RESET               0x1120
#define V830_SOFTWARE_CLEAR             0x1122
#define V830_SOFTWARE_TRIGGER           0x1124
#define V830_TRIGGER_COUNTER            0x1128
#define V830_ALMOST_FULL_LEVEL          0x112c
#define V830_BLT_EVENT_NUMBER           0x1130
#define V830_MEB_EVENT_NUMBER           0x1134
#define V830_DUMMY32                    0x1200
#define V830_DUMMY16                    0x1204
#define V830_CONFIGURATION_ROM          0x4000
#define V830_FIRMWARE                   0x1132

/* Bits for Read Status Register */
#define V830_STATUS_IRQ_SOURCE_2        0x40000000
#define V830_STATUS_IRQ_SOURCE_1        0x20000000
#define V830_STATUS_IRQ_SOURCE_0        0x10000000
#define V830_VME_IRQ                    0x08000000
#define V830_INTERNAL_VME_IRQ           0x04000000
#define V830_STATUS_IRQ_ENABLE_S2       0x00400000
#define V830_STATUS_IRQ_ENABLE_S1       0x00200000
#define V830_STATUS_IRQ_ENABLE_S0       0x00100000
#define V830_RESERVED                   0x00010000
#define V830_GLOBAL_COUNTER_ENABLE      0x00008000
#define V830_GENERAL_OVERFLOW           0x00004000
#define V830_STATUS_ENABLE_REF_PULSER1  0x00002000
#define V830_STATUS_BROADCAST_HANDSHAKE 0x00000080
#define V830_STATUS_BROADCAST           0x00000040
#define V830_STATUS_INPUT_TEST          0x00000020
#define V830_STATUS_25MHZ_TEST_PULSES   0x00000010
#define V830_STATUS_INPUT_BIT1          0x00000008
#define V830_STATUS_INPUT_BIT0          0x00000004
#define V830_STATUS_IRQ_S2_SOFTWARE_IRQ 0x00000002
#define V830_STATUS_USER_LED            0x00000001

/* Bits for Write Control Register */
#define V830_DISABLE_IRQ_SOURCE_2        0x40000000
#define V830_DISABLE_IRQ_SOURCE_1        0x20000000
#define V830_DISABLE_IRQ_SOURCE_0        0x10000000
#define V830_CLEAR_RESERVED_BIT          0x01000000
#define V830_ENABLE_IRQ_ENABLE_S2        0x00400000
#define V830_ENABLE_IRQ_ENABLE_S1        0x00200000
#define V830_ENABLE_IRQ_ENABLE_S0        0x00100000
#define V830_SET_RESERVED_BIT            0x00010000
#define V830_DISABLE_BROADCAST_HANDSHAKE 0x00008000
#define V830_DISABLE_BROADCAST           0x00004000
#define V830_DISABLE_INPUT_TEST          0x00002000
#define V830_DISABLE_25MHZ_TEST_PULSES   0x00001000
#define V830_CLEAR_INPUT_BIT1            0x00000800
#define V830_CLEAR_INPUT_BIT0            0x00000400
#define V830_CLEAR_IRQ_S2_SOFTWARE_IRQ   0x00000200
#define V830_CLEAR_USER_LED              0x00000100
#define V830_ENABLE_BROADCAST_HANDSHAKE  0x00000080
#define V830_ENABLE_BROADCAST            0x00000040
#define V830_ENABLE_INPUT_TEST           0x00000020
#define V830_ENABLE_25MHZ_TEST_PULSES    0x00000010
#define V830_SET_INPUT_BIT1              0x00000008
#define V830_SET_INPUT_BIT0              0x00000004
#define V830_SET_IRQ_S2_SOFTWARE_IRQ     0x00000002
#define V830_SET_USER_LED                0x00000001

