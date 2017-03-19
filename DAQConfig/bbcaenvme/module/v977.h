/*
   V977, 16ch I/O register
*/

/* functions for v977 */
void v977_set_output(unsigned int maddr, short val);
void v977_set_interrupt(unsigned int maddr, short val);
void v977_clear(unsigned int maddr);

/* Address Map */
#define V977_INT_SET	0x0000
#define V977_INT_MASK	0x0002
#define V977_INT_READ	0x0004
#define V977_SING_READ	0x0006
#define V977_MULT_READ  0x0008
#define V977_OUT_SET	0x000A
#define V977_OUT_MASK   0x000C
#define V977_INTE_MASK  0x000E
#define V977_CLE_OUT    0x0010
#define V977_RESERVED   0x0012
#define V977_RESERVED2  0x0014
#define V977_SING_RCL   0x0016
#define V977_MULT_RCL   0x0018
#define V977_TEST_COR   0x001A
#define V977_RESERVED3  0x001C
#define V977_RESERVED4  0x001E
#define V977_INT_LEV    0x0020
#define V977_INT_VEC    0x0022
#define V977_SER_NUM    0x0024
#define V977_FIRM_REV   0x0026
#define V977_CONT_REG   0x0028
#define V977_DUM_REG    0x002A
#define V977_RESERVED5  0x002C
#define V977_SEFT_RST   0x002E

