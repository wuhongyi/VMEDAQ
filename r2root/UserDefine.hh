// UserDefine.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 7月 10 16:37:37 2017 (+0800)
// Last-Updated: 日 8月 20 12:27:59 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 10
// URL: http://wuhongyi.cn 

// 用户需要修改本文件中的参数


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef _USERDEFINE_H_
#define _USERDEFINE_H_

// scaler
#define v830_r2root //没有该插件则注释本行
#define v830num 1
const int v830geo[v830num] = {30};//元素个数必须与 v830num 一致


// adc
#define v785_r2root //没有该插件则注释本行
#define v785num 1
const int v785geo[v785num] = {0};//元素个数必须与 v785num 一致


// qdc
#define v792_r2root //没有该插件则注释本行
#define v792num 1
const int v792geo[v792num] = {1};//元素个数必须与 v792num 一致


// tdc
#define v775_r2root //没有该插件则注释本行
#define v775num 1
const int v775geo[v775num] = {2};//元素个数必须与 v775num 一致


// gdc
#define v1190_r2root //没有该插件则注释本行
#define v1190num 2
#define v1190multi 5 //gdc 数组第三维度
#define v1190hitmax 3000
const int v1190geo[v1190num] = {20,21};//元素个数必须与 v1190num 一致


// madc
#define madc32_r2root //没有该插件则注释本行
#define madc32num 2
const int madc32geo[madc32num] = {10,11};//元素个数必须与 madc32num 一致


#endif /* _USERDEFINE_H_ */

// 
// UserDefine.hh ends here
