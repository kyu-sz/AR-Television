////////////////////////////////////////////////////////////
/// AR Television
/// Copyright(c) 2017 Carnegie Mellon University
/// Licensed under The MIT License[see LICENSE for details]
/// Written by Kai Yu, Zhongxu Wang, Ruoyuan Zhao, Qiqi Xiao
////////////////////////////////////////////////////////////
// Cross-platform micros.
#ifdef _WIN32
#include <Windows.h>
#define AR_PAUSE system("PAUSE")
#define AR_SLEEP(x) Sleep(x)
#else
//#include <cstdio>
#include <unistd.h>
#define AR_PAUSE getchar()
#define AR_SLEEP(x) usleep(x * 1000)
#endif
