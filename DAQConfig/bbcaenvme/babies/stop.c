void stop(void){
  
#ifdef V7XXINTERRUPT
  v7xx_intlevel(ADC0ADDR, 0);
#endif
#ifdef SCAINTERRUPT
  v830_intlevel(SCAADDR0, 0);
  v830_fulllevel(SCAADDR0,64);//default:64
#endif
#ifdef V1X90INTERRUPT
  v1190_intlevel(V1x90ADDR0, 0);
#endif
#ifdef MADC32INTERRUPT
  madc32_irq_level(MADC0ADDR, 0);
#endif
  
  vme_disable_interrupt(); 

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改
  
  // 有 MADC32 插件
  // madc32_mstop_acq(MSTMDCADDR);

  // 也可以采用以下方式每个插件单独设置
  // madc32_stop_acq(MADC0ADDR);
  // madc32_stop_acq(MADC1ADDR);

  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#ifdef SOFTWAREBUSY
#ifdef SOFTWAREBUSYMULTICRATE
  // 软件busy中的多机箱同步
  vx718_set_ioport(3);
#endif
#else
  // 硬件busy 
  vx718_set_ioport(3);
#endif


  vx718_stop_pulsea();
  
  printf("Stop\n");
}
