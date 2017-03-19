void stop(void){
  //v2718_set_ioport(4);
  v7xx_intlevel(ADC0ADDR,0);
  vme_disable_interrupt();
  madc32_mstop_acq(MSTMDCADDR);

  ///madc32_stop_acq(MADC0ADDR);

  //v830_intlevel(SCAADDR0,0);
  v2718_stop_pulsea();
  //v2718_pulse_ioport(4);
  
  printf("Stop\n");
}
