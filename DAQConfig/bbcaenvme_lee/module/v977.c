#include "libbabies.h"
#include "libbbcaenvme.h"
#include "v977.h"

void v977_set_output(unsigned int maddr, short val)
{
  vwrite16(maddr + V977_OUT_SET, &val);
}

void v977_set_interrupt(unsigned int maddr, short val)
{
  vwrite16(maddr + V977_INT_LEV, &val);
}

/*
int v977_segdata(unsigned int maddr){
	int wordcnt;

	wordcnt = 0;
//	printk("mp@IO = %d \n",mp);
	vread16(maddr+V977_SING_READ,(short *)(data+mp));
//	vread16(maddr+V977_TEST_COR,(short *)(data+mp));
	mp++;
	segmentsize++;
	wordcnt++;

	return segmentsize;
}

int v977_segmod(short segid, unsigned int maddr){
	int tmp, wordcnt;

	wordcnt = 0;
	tmp = mp;

	mp++;

	vread16(maddr+V977_SING_READ,(short *)(data+mp));
//	vread16(maddr+V977_TEST_COR,(short *)(data+mp));
	mp++;
	wordcnt++;

        mp = tmp;
	data[mp++] = wordcnt*2 +2;
	data[mp++] = segid;

	mp += wordcnt + 2;	
	eventsize += wordcnt +2;
	return wordcnt + 2;
}

#ifdef VMEINT
void v977_intlevel(unsigned int maddr){
	short sval;

	sval = INTLEVEL;

	vwrite16(maddr + V977_INT_LEV, &sval);
}
#endif
*/

void v977_clear(unsigned int maddr){
	short sval;

	sval = 0x01;
	vwrite16(maddr + V977_CLE_OUT, &sval);
//	vwrite16(maddr + V977_TEST_COR, &sval);
} 
