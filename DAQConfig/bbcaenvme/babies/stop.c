void stop(void){

  v7xx_intlevel(ADC0ADDR,0);
  vme_disable_interrupt();
  
  // madc32_mstop_acq(MSTMDCADDR);

  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 硬件busy
  // v2718_set_ioport(4);
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  v2718_stop_pulsea();
  
  printf("Stop\n");
}
