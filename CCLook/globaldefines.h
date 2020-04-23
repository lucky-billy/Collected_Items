#ifndef GLOBALDEFINES_H
#define GLOBALDEFINES_H
#include <Windows.h>

const int Width = 900;						// 画布长
const int Height = 600;						// 画布宽
const int Margin = 100;						// 上下左右预留的空间
const int BlockSize = 100;					// 方块大小
const int cxBlocks = Width / BlockSize;		// 横向方块数量
const int cyBlocks = Height / BlockSize;	// 纵向方块数量

const int AnimationTime = 200;				// 消除方块动画时间，单位毫秒
const int Time = 60;						// 游戏时间，单位秒

#endif