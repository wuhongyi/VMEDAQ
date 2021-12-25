void clear(void){

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 测试
  // vx718_set_ioport(2);
  // vx718_clear_ioport(2);
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifdef SOFTWAREBUSY
  // 软件busy
  vx718_pulse_ioport(4);
#endif   
 
}

