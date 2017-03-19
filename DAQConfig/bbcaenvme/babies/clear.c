void clear(void){
  // write clear function i.e. send end-of-busy pulse
  //printf("Clear\n");

  // 以下部分弃用
  //madc32_clear(MADC0ADDR);
  //madc32_clear(MADC1ADDR);
  //madc32_clear(MADC2ADDR);
  //madc32_clear(MADC3ADDR);
  //madc32_clear(MADC4ADDR);
  
  madc32_mclear(MSTMDCADDR);
  v2718_pulse_ioport(4);
//  v977_set_output(IOREG0,0x1);
//  v977_set_output(IOREG0,0x2);
//  v7xx_intlevel(ADC1ADDR,1);
}

