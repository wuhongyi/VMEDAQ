/*****************************
 * Released By baba-san@Riken
 *
 * Modified By JingLi/Lee @PKU
 * E-mail: lijinger02@126.com
 * **************************/

#ifndef LIBBBCAENVME_H
#define LIBBBCAENVME_H
#include <CAENVMEtypes.h>
#endif

int init_caen(CVBoardTypes VMEBoard, short Link, short Device);
int init_caen_v2718(void);
int init_caen_v1718(void);
unsigned short vme_read16(unsigned int addr);
unsigned int vme_read32(unsigned int addr);
int vme_write16(unsigned int addr,unsigned short sval);
int vme_write32(unsigned int addr,unsigned int lval);
int vme_mwrite16(unsigned int maddr, unsigned short sval);
int vme_mwrite32(unsigned int maddr, unsigned int lval);
int vme_amsr(unsigned int lval);
void release_caen();
int vread32(unsigned int addr, int *val);
short vread16(unsigned int addr, short *val);
void vwrite32(unsigned int addr, int *val);
void vwrite16(unsigned int addr, short *val);
void vmwrite32(unsigned int maddr, int *val);
void vmwrite16(unsigned int maddr, short *val);
int dma_vread32(unsigned int addr, char *buff, int size);
/* level range: 1-->7 
 * the module used to generate interrupt should have 
 * same interrupt level*/
void vme_define_intlevel(int level);
void vme_enable_interrupt(void);
void vme_disable_interrupt(void);
/** tout = in milliseconds, return 0=no interrupt, 1=is intterupt */
int vme_wait_interrupt(int tout);
/** better to user vme_check_interrupt instead of wait interrupt*/
int vme_check_interrupt(void);

int v2718_init_ioport(unsigned int ioport, unsigned int polarity, unsigned int ledpol);
/* This function: set the front panel output propagate on software appointment
 * ioport: 0-4 --> 1st-5th LEMO output of V2718
 * polarity: 0 Normal, 1 Inverted
 * ledpol : 0 LED emits on signal high level, 1 on low level
 */

int v2718_clear_ioport(unsigned int ioport);
/* set the ioport to be permanent ground level */

int v2718_set_ioport(unsigned int ioport);
/* Set the ioport to be permanent signal level */

int v2718_pulse_ioport(unsigned int ioport);
/* generate a pulse at specific ioport */

int v2718_mon_berr(void);
/* set 4th output to moniter BERR signal */

int v2718_pulsea_configure(unsigned int ioport, unsigned int period, unsigned int width, unsigned int unit, unsigned int pulseno);
/* Set the front panel lemo output 1 or 2 to generate pulese
 * ioport: 0-->1
 * period: 0-->255
 * width:  0-->255
 * unit:   0-->4: 25ns,1.6us,410us, 104ms
 * pulserno: 0 --> infinite 1-255
 */

int v2718_start_pulsea();

int v2718_stop_pulsea();
