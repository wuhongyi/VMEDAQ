// UserDefine.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 6月  6 22:31:15 2018 (+0800)
// Last-Updated: 三 6月  6 23:51:50 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 3
// URL: http://wuhongyi.cn 

// 用户需要修改本文件中的参数

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef _USERDEFINE_H_
#define _USERDEFINE_H_

// V7XX
#define V7XX_CRATE
#define V7XX_CRATE_NUM 8  //插件数
const int V7XX_CRATE_GEO[V7XX_CRATE_NUM] = {0,1,2,3,4,5,6,7};
#define V7XX_HIST_BIN 4200
#define V7XX_HIST_MIN 0
#define V7XX_HIST_MAX 4200

// MADC
#define MADC_CRATE
#define MADC_CRATE_NUM 2  //插件数
const int MADC_CRATE_GEO[MADC_CRATE_NUM] = {10,11};
#define MADC_HIST_BIN 2000
#define MADC_HIST_MIN 0
#define MADC_HIST_MAX 8000

// V1190
#define V1190_CRATE
#define V1190_CRATE_NUM 1  //插件数
const int V1190_CRATE_GEO[V1190_CRATE_NUM] = {20};
#define V1190_HIST_BIN 5000
#define V1190_HIST_MIN 0
#define V1190_HIST_MAX 50000

// V1290
#define V1290_CRATE
#define V1290_CRATE_NUM 1  //插件数
const int V1290_CRATE_GEO[V1290_CRATE_NUM] = {21};
#define V1290_HIST_BIN 5000
#define V1290_HIST_MIN 0
#define V1290_HIST_MAX 500000

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#define DATA_MODE_ONLINE 1
#define DATA_MODE_OFFLINE 0
#define DATA_MODE DATA_MODE_OFFLINE
#define FILE_ONLINE_SID 0
#define FILE_OFFLINE_DATA "/home/wuhongyi/data/data0100.ridf"


#endif /* _USERDEFINE_H_ */

// 
// UserDefine.hh ends here

