void clear(void){
  
  // madc32_mclear(MSTMDCADDR);

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 软件busy
  v2718_pulse_ioport(3);
//  v2718_clear_ioport(4);
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
}

