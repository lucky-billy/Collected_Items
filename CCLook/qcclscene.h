/************************************************************
**
** @Name: 游戏场景
** @Description: 主要负责游戏的逻辑控制
** @Date: 2013/04/19 19:33
**
*************************************************************/

#ifndef QCCLSCENE_H
#define QCCLSCENE_H

#include <QGraphicsScene>
#include "globaldefines.h"
#include "qcclookitem.h"
#include "qcclookline.h"
#include "qcclooktext.h"
#include "qcclooktimebar.h"
#include "qcclookscore.h"


template<typename T> inline T MAX(T a, T b){return ((a > b) ? a : b);}
template<typename T> inline T MIN(T a, T b){return ((a < b) ? a : b);}

class QCCLookItem;
class QCCLScene : public QGraphicsScene
{
	Q_OBJECT


public:
	enum gameStatus{INIT, RUNING, WIN, LOSE};
	QCCLScene(QObject *parent);
	~QCCLScene();

/// 信号
signals:
	// 将成绩发送给排行榜
	void emitScore(int);

/// 槽
public slots:
	// 设置游戏状态
	void setGameStatus(gameStatus status)
	{
		m_iStatus = status;
	}
	// 设置方块图片
	void setPicSets(QStringList images);
	// 程序主循环
	void gameMain();
	// 游戏开始
	void gameStart();
	// 初始化方块
	void initBlocks();
	// 初始化时间
	void initTimer();
	// 初始化分数
	void initScore();
	// 获取提示
	void getTip();

/// 接口
public:
	// 增加被选中的方块,由cclookitem调用
	bool addSelectedBlocks(QCCLookItem* item);
	// 减少被选中的方块
	bool removeSelectedBlocks(QCCLookItem* item);
	// 减少剩余方块数
	void reduceBlock(int index)
	{
		m_pBlocks[index] = NULL;
		--m_iBlockCount;
	}

/// 虚函数重载
protected:
	void drawBackground(QPainter *painter, const QRectF &rect);

/// 私有函数
private:
	// 判断方块是否可以消去
	bool isBlocksRemovable();
	// 添加方块之间的连线
	void addLines();
	// 判断两点是否可消去
	bool isBlockRemovable(QPointF i, QPointF j);
	// 判断两点是否在一条直线上
	bool isInLine(QPointF i, QPointF j);
	// 判断两点之间是否可连通成一条直线
	bool isAbleToLine(QPointF i, QPointF j);
	// 判断两点之间是否可通过一个直角相连
	bool isAbleToCorner(QPointF i, QPointF j);
	// 判断两点之间是否可通过两个直角相连
	bool isAbleToCorner2(QPointF i, QPointF j);
	// 判断指定位置是否可走
	bool isSpace(QPointF point);
	// 判断当前布局是否有解
	bool isSovable();
	// 可以消去时的处理
	void doRemove();
	// 不能消去时的处理
	void doError();
	// 方块移动动态效果
	void setBlockAnimation();

private:
	int m_iStatus;								// 游戏状态
	QList<QCCLookItem*> m_pBlocks;				// 存储方块指针
	QList<QCCLookItem*> m_selectedBlocks;		// 存储被选中的方块
	QCCLookLine* m_line;						// 连线
	QList<QPointF> m_lPoints;					// 存储路径
	QStringList m_imageList;					// 图片路径
	QMultiMap<int, QCCLookItem*> m_keyList;		// 存储具有同一id的方块列表
	int m_iBlockCount;							// 剩余砖块数
	QCCLookTimeBar* m_timeBar;					// 时间进度条
	QCCLookScore* m_score;						// 分数
	QPixmap m_background;						// 背景图片
};

#endif // QCCLSCENE_H
