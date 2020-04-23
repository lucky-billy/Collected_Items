#include "qcclscene.h"
#include <Windows.h>
#include <QGraphicsItemAnimation>

QCCLScene::QCCLScene(QObject *parent)
	: QGraphicsScene(parent)
	, m_iStatus(INIT)
	, m_iBlockCount(cxBlocks * cyBlocks)
	, m_background(":/other/background.png")
{

	QTimer* qTimer = new QTimer(this);
	connect(qTimer, SIGNAL(timeout()), this, SLOT(gameMain()));
	qTimer->start(1000 / 30);
}

QCCLScene::~QCCLScene()
{

}

/// 槽
void QCCLScene::setPicSets(QStringList images)
{
	m_imageList = images;
}

void QCCLScene::gameMain()
{
	switch (m_iStatus)
	{
	case INIT:
		{
			break;
		}
	case RUNING:
		{
			// 时间用尽则游戏结束
			if (m_timeBar->isFinished())
				m_iStatus = LOSE;

			// 判断方块是否可以消去
			if (isBlocksRemovable())
			{
				// 可以消去
				doRemove();
			}
			else if (m_selectedBlocks.count() == 2)
			{
				// 不可消去
				doError();
			}

			// 判断任务是否完成
			if (m_iBlockCount == 0)
				m_iStatus = WIN;

			break;
		}
	case WIN:
		{
			m_score->addScore(m_timeBar->getLastTime());
			m_timeBar->reset();
			addItem(new QCCLookText(tr("YOU WIN!"), QRectF(-Width / 2, -Height / 2, Width, Height)));
			// 提交分数
			emit emitScore(m_score->getScore());
			m_iStatus = INIT;
			break;
		}
	case LOSE:
		{
			// 移除剩余方块
			for (int i = 0; i != m_pBlocks.count(); i++)
			{
				if (m_pBlocks[i] != NULL)
					removeItem(m_pBlocks[i]);
			}

			removeItem(m_timeBar);
			addItem(new QCCLookText(tr("YOU LOSE!"), QRectF(-Width / 2, -Height / 2, Width, Height)));
			m_iStatus = INIT;
			break;
		}
	}

	advance();
}

void QCCLScene::gameStart()
{
	initBlocks();
	initTimer();
	initScore();
	setBlockAnimation();

	setGameStatus(RUNING);
}

// 初始化方块
void QCCLScene::initBlocks()
{
	// 清空所有item
	while (items().count() != 0)
		removeItem(items()[0]);

	// 初始化方块直到可解
	while (!isSovable())
	{
		// 清空item
		while (items().count() != 0)
			removeItem(items()[0]);

		// 初始化抽签盒
		QList<int> idBox;
		srand(GetTickCount());
		for (int i = 0; i != cxBlocks * cyBlocks / 2; i++)
		{
			int qRandowmPicNum = rand() % m_imageList.count();
			idBox.append(qRandowmPicNum);
			idBox.append(qRandowmPicNum);
		}

		int x = 0, y = 0;
		m_pBlocks.clear();
		for (int k = 0; k < cxBlocks * cyBlocks; k++)
		{
			QCCLookItem* itemPointer;
			// 从抽签盒中随机抽一张图片,并设置id
			int qRandowmPicNum = rand() % idBox.count();
			itemPointer = new QCCLookItem(m_imageList[idBox[qRandowmPicNum]]);
			itemPointer->setID(idBox[qRandowmPicNum]);
			// 从抽签盒中去掉抽过的签
			idBox.removeAt(qRandowmPicNum);

			addItem(itemPointer);
			m_pBlocks.append(itemPointer);
			itemPointer->setIndex(y * cxBlocks + x);
			itemPointer->setPos(-Width / 2 + x * BlockSize + BlockSize / 2, -Height / 2 + y * BlockSize + BlockSize / 2);

			if (x == cxBlocks - 1)
			{
				x = 0;
				y++;
			}
			else
				x++;
		}
	}

	// 修改方块状态为normal
	for (int i = 0; i != items().count(); i++)
	{
		QCCLookItem* qItemPointer = static_cast<QCCLookItem*>(items().at(i));
		qItemPointer->setStatus(QCCLookItem::NORMAL);
	}

	// 初始化方块数
	m_iBlockCount = cxBlocks * cyBlocks;
}

void QCCLScene::initTimer()
{
	// 初始化时间
	m_timeBar = new QCCLookTimeBar(QRectF(-Width / 2 + 100, -Height / 2 - Margin + 40, Width - 400, Margin - 80), Time);
	addItem(m_timeBar);
	m_timeBar->start();
	QGraphicsItem* clockPic = addPixmap(QPixmap(":/other/clock.png"));
	clockPic->setPos(-Width / 2, -Height / 2 - Margin + 10);
}

void QCCLScene::initScore()
{
	// 初始化分数
	QGraphicsItem* scorePic = addPixmap(QPixmap(":/other/score.png"));
	scorePic->setPos(Width / 2 - 260, -Height / 2 - Margin + 15);
	m_score = new QCCLookScore(QRectF(Width / 2 - 180, -Height / 2 - Margin + 25, 150, 60));
	addItem(m_score);
}

// 获取提示
void QCCLScene::getTip()
{
	m_keyList.clear();
	QList<QGraphicsItem*> itemList = items();
	if (!items().count())
		return;

	// 将items分类
	for (QList<QGraphicsItem*>::iterator i = itemList.begin(); i != itemList.end(); i++)
	{
		QCCLookItem* qItemPointer = static_cast<QCCLookItem*>(*i);
		if (qItemPointer->type() == QGraphicsItem::UserType + 1)
			m_keyList.insert(qItemPointer->getID(), qItemPointer);
	}

	// 代表是否有找到可消去的方块
	bool bFlag = false;
	QList<int> keys = m_keyList.uniqueKeys();

	for (QList<int>::iterator k = keys.begin(); k != keys.end(); k++)
	{
		// 在具有相同id的item中搜索可消去的方块
		QList<QCCLookItem*> items = m_keyList.values(*k);
		int i, j;

		// 判断是否找到可消去的方块，如有则记录为i,j
		for (i = 0; i != items.count() - 1;i++)
		{
			j = i + 1;
			for (; j != items.count(); j++)
			{
				if (isBlockRemovable(items[i]->pos(), items[j]->pos()))
				{
					bFlag = true;
					break;
				}
			}

			if (bFlag)
				break;
		}

		// 找到则标记出来并退出循环
		if (bFlag)
		{
			items[i]->setStatus(QCCLookItem::TIP);
			items[j]->setStatus(QCCLookItem::TIP);
			break;
		}
	}
}

/// 接口
// 添加被选中的方块
bool QCCLScene::addSelectedBlocks(QCCLookItem* item)
{
	if (m_selectedBlocks.count() == 2)
		return false;
	else
	{
		m_selectedBlocks.append(item);
		return true;
	}
}

// 删除被选中的方块
bool QCCLScene::removeSelectedBlocks(QCCLookItem* item)
{
	int iCount = m_selectedBlocks.count();

	if (iCount == 0)
		return false;

	for (int i = 0; i < m_selectedBlocks.count(); i++)
		if (m_selectedBlocks[i] == item)
		{
			m_selectedBlocks.removeAt(i);
			return true;
		}

	return false;
}

///虚函数重载
void QCCLScene::drawBackground(QPainter *painter, const QRectF &rect)
{
	QRectF targetRect = QRectF(-Width / 2 - Margin, -Height / 2 - Margin, Width + Margin * 2, Height + Margin * 2);
	painter->drawPixmap(targetRect, m_background, m_background.rect());
}

/// 私有函数
bool QCCLScene::isBlocksRemovable()
{
	if (m_selectedBlocks.count() != 2)
		return false;
	else
	{
		if (m_selectedBlocks[0]->getID() != m_selectedBlocks[1]->getID())
			return false;

		// 清空路径，重新计算
		m_lPoints.clear();

		QPointF i(m_selectedBlocks.at(0)->pos()), j(m_selectedBlocks.at(1)->pos());
		
		return isBlockRemovable(i, j);
	}
}

void QCCLScene::addLines()
{
	QPainterPath qPath;

	qPath.moveTo(m_selectedBlocks.at(0)->pos());

	for (int i = 0; i != m_lPoints.count(); i++)
	{
		qPath.lineTo(m_lPoints[i]);
	}

	qPath.lineTo(m_selectedBlocks.at(1)->pos());
	m_line = new QCCLookLine(qPath);
	addItem(m_line);
}

bool QCCLScene::isBlockRemovable(QPointF i, QPointF j)
{
	if (isInLine(i, j))
	{
		// 直接相通或通过两个折点相通
		if (isAbleToLine(i , j)
			|| isAbleToCorner2(i, j))
			return true;
		else
			return false;
	}
	// 两点不在一条直线上
	else
	{
		// 通过一个或两个折点相通
		if (isAbleToCorner(i, j)
			|| isAbleToCorner2(i, j))
		{
			return true;
		}
		else
			return false;
	}
}

bool QCCLScene::isInLine(QPointF i, QPointF j)
{
	if (i.x() == j.x() || i.y() == j.y())
		return true;
	else
		return false;
}

bool QCCLScene::isAbleToLine(QPointF i, QPointF j)
{
	if (i.x() == j .x())
	{
		for (int y = MIN(i.y(), j.y()) + BlockSize; y != MAX(i.y(), j.y()); y += BlockSize)
		{
			if (!isSpace(QPointF(i.x(), y)))
				return false;
		}
	}
	else if (i.y() == j.y())
	{
		for (int x = MIN(i.x(), j.x()) + BlockSize; x != MAX(i.x(), j.x()); x += BlockSize)
		{
			if (!isSpace(QPointF(x, i.y())))
				return false;
		}
	}
	else
		return false;

	return true;
}

bool QCCLScene::isAbleToCorner(QPointF i, QPointF j)
{
	if (isInLine(i, j))
		return false;

	QPointF qCornerPoint[2] = {QPointF(i.x(), j.y()), QPointF(j.x(), i.y())};
	
	for (int k = 0; k != 2; k++)
	{

		if (!isSpace(qCornerPoint[k]))
			continue;
		if (isAbleToLine(qCornerPoint[k], i) && isAbleToLine(qCornerPoint[k], j))
		{
			m_lPoints.append(qCornerPoint[k]);
			return true;
		}
	}
	return false;
}

bool QCCLScene::isAbleToCorner2(QPointF i, QPointF j)
{
	// 向上寻找路径
	for (int k = i.y() - BlockSize; i.x() != j.x() && k > -Height / 2 - Margin; k -= BlockSize)
	{
		if (!isSpace(QPointF(i.x(), k)))
			break;

		if (isAbleToCorner(QPointF(i.x(), k), j))
		{
			m_lPoints.prepend(QPointF(i.x(), k));
			return true;
		}
	}

	// 向下寻找
	for (int k = i.y() + BlockSize; i.x() != j.x() &&  k < Height / 2 + Margin; k += BlockSize)
	{
		if (!isSpace(QPointF(i.x(), k)))
			break;

		if (isAbleToCorner(QPointF(i.x(), k), j))
		{
			m_lPoints.prepend(QPointF(i.x(), k));
			return true;
		}
	}

	// 向左寻找
	for (int k = i.x() - BlockSize; i.y() != j.y() && k > -Width / 2 - Margin; k -= BlockSize)
	{
		if (!isSpace(QPointF(k, i.y())))
			break;

		if (isAbleToCorner(QPointF(k, i.y()), j))
		{
			m_lPoints.prepend(QPointF(k, i.y()));
			return true;
		}
	}

	// 向右寻找
	for (int k = i.x() + BlockSize; i.y() != j.y() && k < Width / 2 + Margin; k += BlockSize)
	{
		if (!isSpace(QPointF(k, i.y())))
			break;

		if (isAbleToCorner(QPointF(k, i.y()), j))
		{
			m_lPoints.prepend(QPointF(k, i.y()));
			return true;
		}
	}

	return false;
}

bool QCCLScene::isSpace(QPointF point)
{
	QList<QGraphicsItem*> itemList = items(point);
	if (itemList.count() == 0)
		return true;

	// 非方块item也视为空地
	int iBlockCount = 0;
	for (int i = 0; i != itemList.count(); i++)
		if (itemList[i]->type() == QGraphicsItem::UserType + 1)
			iBlockCount++;
	if (!iBlockCount)
		return true;

	QCCLookItem* qItem = static_cast<QCCLookItem*>(itemList[0]);

	if (qItem->getStatus() < QCCLookItem::FADING)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool QCCLScene::isSovable()
{
	m_keyList.clear();

	// 将items按id分类
	QList<QGraphicsItem*> itemList = items();
	if (!items().count())
		return false;

	for (QList<QGraphicsItem*>::iterator i = itemList.begin(); i != itemList.end(); i++)
	{
		QCCLookItem* qItemPointer = static_cast<QCCLookItem*>(*i);
		m_keyList.insert(qItemPointer->getID(), qItemPointer);
	}

	// 求解
	while(!m_keyList.isEmpty())
	{
		// 代表是否有方块在此次循环消去
		bool bFlag = false;

		QList<int> keys = m_keyList.uniqueKeys();
		for (QList<int>::iterator k = keys.begin(); k != keys.end(); k++)
		{
			// 在具有相同id的方块中搜索可消去的

			// 代表是否有方块在子循环中消去
			bool bSubFlag = false;
			QList<QCCLookItem*> items = m_keyList.values(*k);
			int i, j;

			// 判断是否有两点相通，如有则记录为i,j
			for (i = 0; i != items.count() - 1;i++)
			{
				j = i + 1;
				for (; j != items.count(); j++)
				{
					if (isBlockRemovable(items[i]->pos(), items[j]->pos()))
					{
						// 找到相通的点
						bFlag = true;
						bSubFlag = true;
						break;
					}
				}
				if (bSubFlag)
					break;
			}

			// 删除相通的点
			if (bSubFlag)
			{
				m_keyList.remove(*k, items[j]);
				m_keyList.remove(*k, items[i]);
				items[i]->setStatus(QCCLookItem::TESTING);
				items[j]->setStatus(QCCLookItem::TESTING);
			}
		}

		// 所有方块都不可消除，无解
		if (!bFlag)
			return false;
	}

	return true;
}

void QCCLScene::doRemove()
{
	addLines();
	m_selectedBlocks.at(0)->setStatus(QCCLookItem::FADING);
	m_selectedBlocks.at(1)->setStatus(QCCLookItem::FADING);
	m_selectedBlocks.clear();
	// 加分
	m_score->addScore(2);
}

void QCCLScene::doError()
{
	m_selectedBlocks.at(0)->setToggle(false);
	m_selectedBlocks.at(1)->setToggle(false);
	m_selectedBlocks.clear();
	// 减分
	m_score->addScore(-1);
}

void QCCLScene::setBlockAnimation()
{
	for (int i = 0; i != m_pBlocks.count(); i++)
	{
		QGraphicsItem *block = m_pBlocks[i];

		QTimeLine *timer = new QTimeLine(500);
		timer->setFrameRange(0, 100);

		QGraphicsItemAnimation *animation = new QGraphicsItemAnimation;
		animation->setItem(block);
		animation->setTimeLine(timer);
		QPointF oriPos = block->pos();
		block->setPos(oriPos.x() - Width, oriPos.y());

		for (int i = 0; i < 200; ++i)
			animation->setPosAt(i / 200.0, QPointF(oriPos.x() - Width + i * Width / 200.0, oriPos.y()));
		animation->setPosAt(1, oriPos);

		timer->setCurveShape(QTimeLine::EaseInCurve);
		timer->start();
	}
}
