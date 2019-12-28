// UserDefine.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 9月  8 19:48:49 2017 (+0800)
// Last-Updated: 四 2月  1 16:34:14 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 12
// URL: http://wuhongyi.cn 

// 用户需要修改本文件中的参数

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef _USERDEFINE_H_
#define _USERDEFINE_H_

#define ROOTFILEPATH "/home/wuhongyi/data/rootfile/"
#define ROOTFILENAME "data"



// adc
#define v785_pedo //没有该插件则注释本行
#define v785num 7
#define v785binmin 0
#define v785binmax 200
// #define v785fitrange 50  //+-
#define v785fitrangeabove  0.6   // 0-1
#define v785D2D16 2 // 2/16
#define v785msigma 3


// qdc
// #define v792_pedo //没有该插件则注释本行
#define v792num 1
#define v792binmin 2000
#define v792binmax 2500
// #define v792fitrange 50  //+-
#define v792fitrangeabove  0.3   // 0-1
#define v792D2D16 2 // 2/16
#define v792msigma 3


// madc
#define madc32_pedo //没有该插件则注释本行
#define madc32num 1
#define madc32binmin 0
#define madc32binmax 100
// #define madc32fitrange 4  //+
#define madc32fitrangeabove  0.1   // 0-1
#define madc32msigma 3


#endif /* _USERDEFINE_H_ */

// 
// UserDefine.hh ends here
