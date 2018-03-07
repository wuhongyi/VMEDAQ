void start(void){
  // Reconfigure 5th lemo output on v2718 front panel
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改
  
  // 软件busy
  v2718_init_ioport(4,0,0);

  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 测试
  // v2718_init_ioport(2,0,0);
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  // v2718_pulsea_configure(unsigned int ioport, unsigned int period, unsigned int width, unsigned 
  // int unit, unsigned int pulseno);
  // ioport : 0 → 1, period: 1 → 255,  width: 1 → 255, unit: 0 → 4 : 25ns, 1.6us, 410us, 104ms, 
  // pulseno: 0 → 255 , 0 is infinite

  // set v2718 1st ioport to generate pulse period
  v2718_init_ioport(0,0,0);
  v2718_pulsea_configure(0,2,1,1,0);// 3.2us width 1.6us
  v2718_start_pulsea();

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改
  
  // 有 V830 插件
  v830_clear_all(SCAADDR0);

  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改
  
  // 有 V7xx 插件
  // 每个插件单独设置
  v7xx_rst_counter(ADC0ADDR);
  v7xx_rst_counter(ADC1ADDR);
  v7xx_rst_counter(ADC2ADDR);
  // v7xx_rst_counter(ADC3ADDR);
  // v7xx_rst_counter(ADC4ADDR);
  // v7xx_rst_counter(ADC5ADDR);
  // v7xx_rst_counter(ADC6ADDR);
  // v7xx_rst_counter(ADC7ADDR);
  
  v7xx_clear(ADC0ADDR);
  v7xx_clear(ADC1ADDR);
  v7xx_clear(ADC2ADDR);
  // v7xx_clear(ADC3ADDR);
  // v7xx_clear(ADC4ADDR);
  // v7xx_clear(ADC5ADDR);
  // v7xx_clear(ADC6ADDR);
  // v7xx_clear(ADC7ADDR);

  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改
  
  // 有 V1190/V1290 插件
  // 每个插件单独clear
  // v1190_clear(V1x90ADDR0);
  // v1290_clear(V1x90ADDR1);

  v1190_clear(V1x90ADDR0);
  v1190_clear(V1x90ADDR1);
  // v1290_clear(V1x90ADDR0);
  // v1290_clear(V1x90ADDR1);
  
  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改
  
  // 有 MADC32 插件
  madc32_mclear(MSTMDCADDR);
  madc32_mirq_level(MSTMDCADDR,0);
  madc32_mreset_ctra_counters(MSTMDCADDR);
  madc32_mfifo_reset(MSTMDCADDR);
  madc32_mstart_acq(MSTMDCADDR);


  // 也可以采用以下方式每个插件单独设置
  // madc32_clear(MADC0ADDR);
  // madc32_irq_level(MADC0ADDR,0);
  // madc32_reset_ctra_counters(MADC0ADDR);
  // madc32_fifo_reset(MADC0ADDR);
  // madc32_start_acq(MADC0ADDR);
  // madc32_clear(MADC1ADDR);
  // madc32_irq_level(MADC1ADDR,0);
  // madc32_reset_ctra_counters(MADC1ADDR);
  // madc32_fifo_reset(MADC1ADDR);
  // madc32_start_acq(MADC1ADDR);
  
  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


  // ==================

  v7xx_intlevel(ADC0ADDR, intlevel);
  vme_define_intlevel(intlevel);
  vme_enable_interrupt();

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改

  // 硬件busy / 软件busy中的多机箱同步
  // v2718_clear_ioport(3);
  
  // 软件busy
  v2718_pulse_ioport(4);


  
  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  printf("Start\n");
}
