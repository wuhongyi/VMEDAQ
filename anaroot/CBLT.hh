// this file is used to configure the CBLT chain modules for decoding
// for example, if CBLT chain is v830,v785,v785,v785,v775,v775,v775,
// v792,v792,v792,v1190,madc,madc
// this file should be modified like below
// #define v830m
// #define v830n 0
// #define v830num 1
// #define v830chn 4 // how many channels are enabled
// #define v830head 0 //v830 no header
// #define v830geo 0 // if v830 header is not enabled 
// 
// #define v7xxm
// #define v7xxn 1
// #define v7xxnum 9 // how many v7xx type modules
//
// #define v1190m
// #define v1190n 2
// #define v1190num 1
//
// #define madcm
// #define madcn 3
// #define madcnum 2

#ifndef CBLT_H
#define CBLT_H

// #define v830m
#define v830n 0
#define v830num 1
#define v830chn 8 // how many channels are enabled  这里设置830开启路数
#define v830head 1 //v830 no header
#define v830geo 0 // if v830 header is not enabled 
 
#define v7xxm
#define v7xxn 0
#define v7xxnum 4 // how many v7xx type modules

#define v1190m
#define v1190n 1
#define v1190num 1

// #define v1290m
#define v1290n 3
#define v1290num 1


// #define madcm
#define madcn 4
#define madcnum 1



#endif
