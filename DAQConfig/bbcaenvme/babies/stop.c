void stop(void){

  v7xx_intlevel(ADC0ADDR,0);
  vme_disable_interrupt();
  // madc32_mstop_acq(MSTMDCADDR);



  v2718_stop_pulsea();
  
  printf("Stop\n");
}
