void evt(void) {

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 测试
  // vx718_pulse_ioport(2); 
  // vx718_pulse_ioport(2);
  // vx718_set_ioport(3);
  // vx718_set_ioport(2);
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  
  babies_init_event();
  //babies_init_segment(segid)
  //MKSEGID(device, focalplane, detector, module)
  //please see segidlist.h
  //module is important, e.g. C16 is 16bit data
  //device, focalplane, detector, can be defined as you want


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改

  // Read data 
  babies_init_segment(MKSEGID(0, 0, PLAQ, CBLT));
#ifdef V7XXINTERRUPT
  v7xx_dmasegdata(MSTTDCADDR, 4000);
#endif
#ifdef SCAINTERRUPT
  usleep(10);
  v830_dmasegdata(MSTTDCADDR, 4000);
#endif
#ifdef V1X90INTERRUPT
  v1190_dmasegdata(MSTTDCADDR, 4000);
#endif
#ifdef MADC32INTERRUPT
  madc32_dmasegdata(MSTTDCADDR, 4000);
#endif
  babies_end_segment();
  

  // 插件
  // babies_init_segment(MKSEGID(0, 0, PLAQ, V785));
  // v7xx_dmasegdata(ADC0ADDR, 34);
  // babies_end_segment();

  // babies_init_segment(MKSEGID(0, 0, PLAQ, V830));
  // v830_dmasegdata(SCAADDR0, 34);
  // babies_end_segment();

  // babies_init_segment(MKSEGID(0, 0, PLAQ, V1190));//V1290
  // v1190_dmasegdata(V1x90ADDR0, 1000);
  // babies_end_segment();

  // babies_init_segment(MKSEGID(0, 0, PLAQ, MADC));
  // madc32_dmasegdata(MADC0ADDR, 34);
  // babies_end_segment();  

  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  

  // End of event
  babies_end_event();


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 测试
  // vx718_clear_ioport(2);
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改

  // 软件BUSY模式下6036->0x1不需要以下清除，6036->0x3需要以下清除，6036->0x0需要以下清除
  // 硬件BUSY模式下只能采用6036->0x3，需要以下清除
  
  // 有 MADC32 插件  
  // madc32_mclear(MSTMDCADDR);
  
  // 也可以采用以下方式每个插件单独设置
  // madc32_clear(MADC0ADDR);
  // madc32_clear(MADC1ADDR);

  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// 测试 逐个插件逐个Event word去读取数据，速度较慢
// babies_init_segment(MKSEGID(0, 0, PLAQ, V785/*CBLT*/));
// v7xx_dmasegdata(ADC0ADDR, 34); // hardware address , max cnt (word)
// v7xx_dmasegdata(ADC1ADDR, 34);
// v7xx_dmasegdata(ADC2ADDR, 34);
// v7xx_dmasegdata(ADC3ADDR, 40);
// v7xx_dmasegdata(ADC4ADDR, 40);
// v7xx_dmasegdata(ADC5ADDR, 40);
// babies_end_segment();
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  


// 以下部分无用
// babies_init_segment(MKSEGID(0, 0, PLAQ, CBLT));
// babies_init_segment(MKSEGID(0, 0, PLAQ, MADC32));
// madc32_dmasegdata(MSTQDCADDR, 4000);
// babies_end_segment();
//v7xx_dmasegdata(ADC1ADDR, 250);
//v7xx_dmasegdata(ADC1ADDR, 250);
//v830_dmasegdata(SCAADDR0, 250);
//babies_init_segment(MKSEGID(0, 1, PLAQ, CBLT));
//v7xx_dmasegdata(MSTQDCADDR, 4000);
//babies_end_segment();
// Read data from V1290
//babies_init_segment(MKSEGID(0, 0, PLAT, MADC32));
//madc32_dmasegdata(MADC2ADDR, 600);
//v1290_segdata(V1190ADDR0);
//v1190_dmasegdata(V1190ADDR0,1000);
//babies_end_segment();
