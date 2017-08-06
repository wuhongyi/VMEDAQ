void clear(void){

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 测试
  // v2718_set_ioport(2);
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改

  // 软件BUSY模式下6036->0x1不需要以下清除，6036->0x3需要以下清除，6036->0x0需要以下清除
  
  // 有 MADC32 插件  
  madc32_mclear(MSTMDCADDR);
  
  // 也可以采用以下方式每个插件单独设置
  // madc32_clear(MADC0ADDR);
  // madc32_clear(MADC1ADDR);

  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 测试
  // v2718_clear_ioport(2);
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

   
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改
  
  // 软件busy
  // v2718_pulse_ioport(4);

  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
}

