void evt(void) {
  //      v7xx_intlevel(ADC1ADDR,0);

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 测试
  // v2718_pulse_ioport(2);
  // v2718_pulse_ioport(2);
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  
  babies_init_event();
  //babies_init_segment(segid)
  //MKSEGID(device, focalplane, detector, module)
  //please see segidlist.h
  //module is important, e.g. C16 is 16bit data
  //device, focalplane, detector, can be defined as you want


  // Read data from V792
  babies_init_segment(MKSEGID(0, 0, PLAQ, V785/*CBLT*/));
  v7xx_dmasegdata(MSTTDCADDR, 4000);
  //      v7xx_dmasegdata(ADC0ADDR, 34); // hardware address , max cnt (word)
  //      v7xx_dmasegdata(ADC1ADDR, 34);
  //      v7xx_dmasegdata(ADC2ADDR, 34);
  //      v7xx_dmasegdata(ADC3ADDR, 40);
  //      v7xx_dmasegdata(ADC4ADDR, 40);
  //      v7xx_dmasegdata(ADC5ADDR, 40);
  babies_end_segment();

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


  // End of event
  babies_end_event();
      
}

