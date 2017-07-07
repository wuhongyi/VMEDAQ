void start(void){
  // Reconfigure 5th lemo output on v2718 front panel
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 软件busy
  v2718_init_ioport(3,0,0);
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 测试
  v2718_init_ioport(2,0,0);
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  
  // set v2718 1st ioport to generate pulse period: 8us width 1.6us
  v2718_init_ioport(0,0,0);
  v2718_pulsea_configure(0,2,1,1,0);
  v2718_start_pulsea();

  /* v830_clear_all(SCAADDR0); */
  
  v7xx_rst_counter(ADC0ADDR);
  v7xx_rst_counter(ADC1ADDR);
  v7xx_rst_counter(ADC2ADDR);
  v7xx_rst_counter(ADC3ADDR);
  v7xx_rst_counter(ADC4ADDR);
  v7xx_rst_counter(ADC5ADDR);
  //v7xx_mrst_counter(MSTTDCADDR);

  v7xx_clear(ADC0ADDR);
  v7xx_clear(ADC1ADDR);
  v7xx_clear(ADC2ADDR);
  v7xx_clear(ADC3ADDR);
  v7xx_clear(ADC4ADDR);
  v7xx_clear(ADC5ADDR);
  //v7xx_mclear(MSTTDCADDR);
  
  // v1190_clear(V1190ADDR0);
  // v1190_clear(V1190ADDR1);
  

  // 初始化全部madc32插件
  // madc32_mclear(MSTMDCADDR);
  // madc32_mirq_level(MSTMDCADDR,0);
  // madc32_mreset_ctra_counters(MSTMDCADDR);
  // madc32_mfifo_reset(MSTMDCADDR);
  // madc32_mstart_acq(MSTMDCADDR);







  // ==================
  
  v7xx_intlevel(ADC0ADDR, intlevel);

  vme_define_intlevel(intlevel);
  vme_enable_interrupt();

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 软件busy
  v2718_pulse_ioport(3);

  // 硬件busy
  //  v2718_clear_ioport(4);
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  printf("Start\n");
}

