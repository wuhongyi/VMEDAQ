void stop(void){

  v7xx_intlevel(ADC0ADDR,0);
  vme_disable_interrupt();

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改
  
  // 有 MADC32 插件
  madc32_mstop_acq(MSTMDCADDR);

  // 也可以采用以下方式每个插件单独设置
  // madc32_stop_acq(MADC0ADDR);
  // madc32_stop_acq(MADC1ADDR);

  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改
  
  // 硬件busy / 软件busy多机箱同步
  // v2718_set_ioport(3);

  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  v2718_stop_pulsea();
  
  printf("Stop\n");
}
