#include "GameWidget.h"

// 颜色数组 存储每个数字对应的背景色
QColor digitBkg[11] = {QColor::fromRgb(0xFF, 0xFF, 0xCC), QColor::fromRgb(0xFF, 0xFF, 0x99),
                            QColor::fromRgb(0xFF, 0xCC, 0xCC), QColor::fromRgb(0xFF, 0xCC, 0x99),
                            QColor::fromRgb(0xFF, 0x99, 0x99), QColor::fromRgb(0xFF, 0x99, 0x66),
                            QColor::fromRgb(0xFF, 0x66, 0x66), QColor::fromRgb(0xCC, 0x99, 0x66),
                            QColor::fromRgb(0xCC, 0x33, 0x33), QColor::fromRgb(0xCC, 0x00, 0x33),
                            QColor::fromRgb(0xFF, 0x00, 0x00)};

// 每个方向位置的增量
QPointF dPos[5] = {QPointF(-10, 0), QPointF(10, 0), QPointF(0, -10), QPointF(0, 10), QPointF(-2, -2)};

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent)
{
    // 连接手势移动信号和相应的槽函数
    connect(this, SIGNAL(GestureMove(GestureDirect)), SLOT(onGestureMove(GestureDirect)));
    // 初始化board数组
    memset(board, 0, sizeof(board));
    // 随机填入两个2
    board[rand() % 4][rand() % 4] = 2;
    board[rand() % 4][rand() % 4] = 2;
    // 分数初始化为0
    score = 0;
    // 数码个数初始化为2
    digitCount = 2;
    isAnimating = false;
    cacheImg = NULL;
}

void GameWidget::mousePressEvent(QMouseEvent *e)
{
    // 获取起点坐标
    startPos = e->pos();
}

void GameWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // 如果在播放动画效果则直接退出防止重复产生手势事件
    if (isAnimating)
        return;
    // 根据终点坐标和起点坐标计算XY坐标的增量
    float dX = (float)(e->pos().x() - startPos.x());
    float dY = (float)(e->pos().y() - startPos.y());
    // 确定手势方向
    if (abs(dX) > abs(dY))
    {
        if (dX < 0)
            emit GestureMove(LEFT);
        else
            emit GestureMove(RIGHT);
    }
    else
    {
        if (dY < 0)
            emit GestureMove(UP);
        else
            emit GestureMove(DOWN);
    }
}

void GameWidget::onGestureMove(GestureDirect direct)
{
    int i, j, k;
    Animation a;
    // 是否合并过方格
    bool combine = false;
    // 处理不同方向
    switch (direct)
    {
    // 向左
    case LEFT:
        // 循环每一行
        for (i = 0; i < 4; i++)
        {
            /* 初始化j k为0
             * 这里j表示要交换的数字列号
             * k表示交换到的位置的列号
             * */
            j = 0, k = 0, combine = false;
            while (true)
            {
                // 循环找到第一个不是0的数字对应的列号
                while (j < 4 && board[i][j] == 0)
                    j++;
                // 如果超过了3则说明搜索完毕 推出循环
                if (j > 3)
                    break;
                // 交换两个数字
                qSwap(board[i][k], board[i][j]);
                // 记录动画信息
                a.type = MOVE;
                a.startPos = QPointF(7 + (w + 5) * j, 7 + (h + 5) * i);
                a.endPos = QPointF(7 + (w + 5) * k, 7 + (h + 5) * i);
                a.digit = a.digit2 = board[i][k];
                a.direct = LEFT;
                //如果交换后的数字与其前一列的数字相同
                if (!combine && k > 0 && board[i][k] == board[i][k - 1])
                {
                    // 前一列的数字*2
                    board[i][k - 1] <<= 1;
                    // 这一列的数字置为0
                    board[i][k] = 0;
                    // 记录动画信息
                    a.digit2 = board[i][k - 1];
                    a.endPos = QPointF(7 + (w + 5) * (k - 1), 7 + (h + 5) * i);
                    // 增加分数
                    score += board[i][k - 1];
                    // 发射增加分数的信号
                    emit ScoreInc(score);
                    // 数码个数-1
                    digitCount--;
                    combine = true;
                }
                else
                    k++;
                j++;
                // 添加到动画链表
                animationList.append(a);
            }
        }
        break;
        // 其余三个方向与左向类似
    case RIGHT:
        for (i = 0; i < 4; i++)
        {
            j = 3, k = 3, combine = false;
            while (true)
            {
                while (j > -1 && board[i][j] == 0)
                    j--;
                if (j < 0)
                    break;
                qSwap(board[i][k], board[i][j]);
                a.type = MOVE;
                a.startPos = QPointF(7 + (w + 5) * j, 7 + (h + 5) * i);
                a.endPos = QPointF(7 + (w + 5) * k, 7 + (h + 5) * i);
                a.digit = a.digit2 = board[i][k];
                a.direct = RIGHT;
                if (!combine && k < 3 && board[i][k] == board[i][k + 1])
                {
                    board[i][k + 1] <<= 1;
                    board[i][k] = 0;
                    a.digit2 = board[i][k + 1];
                    a.endPos = QPointF(7 + (w + 5) * (k + 1), 7 + (h + 5) * i);
                    score += board[i][k + 1];
                    emit ScoreInc(score);
                    digitCount--;
                    combine = true;
                }
                else
                    k--;
                j--;
                animationList.append(a);
            }
        }
        break;
    case UP:
        for (i = 0; i < 4; i++)
        {
            j = 0, k = 0, combine = false;
            while (true)
            {
                while (j < 4 && board[j][i] == 0)
                    j++;
                if (j > 3)
                    break;
                qSwap(board[k][i], board[j][i]);
                a.type = MOVE;
                a.startPos = QPointF(7 + (w + 5) * i, 7 + (h + 5) * j);
                a.endPos = QPointF(7 + (w + 5) * i, 7 + (h + 5) * k);
                a.digit = a.digit2 = board[k][i];
                a.direct = UP;
                if (!combine && k > 0 && board[k][i] == board[k - 1][i])
                {
                    board[k - 1][i] <<= 1;
                    board[k][i] = 0;
                    a.digit2 = board[k - 1][i];
                    a.endPos = QPointF(7 + (w + 5) * i, 7 + (h + 5) * (k - 1));
                    score += board[k - 1][i];
                    emit ScoreInc(score);
                    digitCount--;
                    combine = true;
                }
                else
                    k++;
                j++;
                animationList.append(a);
            }
        }
        break;
    case DOWN:
        for (i = 0; i < 4; i++)
        {
            j = 3, k = 3, combine = false;
            while (true)
            {
                while (j > -1 && board[j][i] == 0)
                    j--;
                if (j < 0)
                    break;
                qSwap(board[k][i], board[j][i]);
                a.type = MOVE;
                a.startPos = QPointF(7 + (w + 5) * i, 7 + (h + 5) * j);
                a.endPos = QPointF(7 + (w + 5) * i, 7 + (h + 5) * k);
                a.digit = a.digit2 = board[k][i];
                a.direct = DOWN;
                if (!combine && k < 3 && board[k][i] == board[k + 1][i])
                {
                    board[k + 1][i] <<= 1;
                    board[k][i] = 0;
                    a.digit2 = board[k + 1][i];
                    a.endPos = QPointF(7 + (w + 5) * i, 7 + (h + 5) * (k + 1));
                    score += board[k + 1][i];
                    emit ScoreInc(score);
                    digitCount--;
                    combine = true;
                }
                else
                    k--;
                j--;
                animationList.append(a);
            }
        }
        break;
    }
    // 如果数字木有填满
    if (digitCount != 16)
    {
        // 随机产生行号和列号
        i = rand() % 4, j = rand() % 4;
        // 循环直到行和列对应的元素为0
        while (board[i][j] != 0)
            i = rand() % 4, j = rand() % 4;
        // 填入2
        board[i][j] = (rand() % 2 + 1) * 2;
        // 记录动画信息
        a.type = APPEARANCE;
        a.startPos = a.endPos = QPointF(7 + (w + 5) * j, 7 + (h + 5) * i);
        a.startPos += QPointF(w / 2, h / 2);
        a.digit = board[i][j];
        // 数码个数加一
        digitCount++;
    }
    else
    {
        // 如果数字填满了 检测游戏是否over
        if (checkGameOver())
            emit GameOver();// 如果游戏over了则发射GameOver信号
    }

    // 开始绘制动画效果
    isAnimating = true;
    // 动画列表的迭代器
    QList<Animation>::iterator it;
    // 事件循环 用于延时
    QEventLoop eventLoop;
    // 删除之前的缓存图像
    if (cacheImg)
        delete cacheImg;
    // 建立缓存图像
    cacheImg = new QImage(width(), height(), QImage::Format_ARGB32);
    // 清空图像
    cacheImg->fill(0);
    // 构造一个QPainter对象
    QPainter painter(cacheImg);
    // 字体
    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(40);
    painter.setFont(font);
    // 标识所有方格动画是否都播放完毕
    bool ok = false;
    while (true)
    {
        // 构造一个画刷 颜色为R G B分量分别为141 121 81的颜色
        QBrush brush(QColor::fromRgb(141, 121, 81));
        // 使painter应用这个画刷
        painter.setBrush(brush);

        // 设置画笔为空笔 目的是使绘制的图形没有描边
        painter.setPen(Qt::NoPen);

        // 绘制一个矩形
        painter.drawRect(2, 2, width() - 4, height() - 4);

        // 设置画刷颜色为 RGB分量为171 165 141的颜色
        brush.setColor(QColor::fromRgb(171, 165, 141));
        // 应用这个画刷
        painter.setBrush(brush);

        // 循环绘制游戏面板
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                // 绘制小方格
                painter.drawRect(QRectF(7 + (w + 5) * j, 7 + (h + 5) * i, w, h));

        // 假设都播放完毕
        ok = true;

        // 循环播放每个方格动画
        for (it = animationList.begin(); it != animationList.end(); it++)
            if (!playAnimation(*it, painter))
                ok = false;

        // 刷新部件
        update();

        // 全部播放完则退出
        if (ok)
            break;

        // 延时5ms
        QTimer::singleShot(5, &eventLoop, SLOT(quit()));
        eventLoop.exec();
    }
    // 播放方格出现的动画
    while (!playAnimation(a, painter))
    {
        update();
        QTimer::singleShot(5, &eventLoop, SLOT(quit()));
        eventLoop.exec();
    }
    //清除所有动画
    animationList.clear();
    //刷新当前部件
    isAnimating = false;

    // 检测游戏是否获胜
    if (checkWin())
        emit win();// 如果获胜则发射win信号

    update();
}

bool GameWidget::playAnimation(Animation& a, QPainter& painter)
{
    bool rtn = false;
    QBrush brush(Qt::SolidPattern);

    // 移动方格位置
    if (a.type == MOVE)
    {
        switch (a.direct)
        {
        case LEFT:
            if (a.startPos.x() > a.endPos.x())
                a.startPos += dPos[LEFT];
            else
                a.startPos = a.endPos, rtn = true;
            break;
        case RIGHT:
            if (a.startPos.x() < a.endPos.x())
                a.startPos += dPos[RIGHT];
            else
                a.startPos = a.endPos, rtn = true;
            break;
        case UP:
            if (a.startPos.y() > a.endPos.y())
                a.startPos += dPos[UP];
            else
                a.startPos = a.endPos, rtn = true;
            break;
        case DOWN:
            if (a.startPos.y() < a.endPos.y())
                a.startPos += dPos[DOWN];
            else
                a.startPos = a.endPos, rtn = true;
        }
        // 如果方格移动到终点
        if (!rtn)
        {
            brush.setColor(digitBkg[getBitCount(a.digit)]);
            painter.setBrush(brush);
            painter.drawRect(QRectF(a.startPos.x(), a.startPos.y(), w, h));
            painter.setPen(QColor::fromRgb(0, 0, 0));
            painter.drawText(QRectF(a.startPos.x(), a.startPos.y(), w, h), Qt::AlignCenter,
                             QString::number(a.digit));
        }
        else
        {
            brush.setColor(digitBkg[getBitCount(a.digit2)]);
            painter.setBrush(brush);
            painter.drawRect(QRectF(a.startPos.x(), a.startPos.y(), w, h));
            painter.setPen(QColor::fromRgb(0, 0, 0));
            painter.drawText(QRectF(a.startPos.x(), a.startPos.y(), w, h), Qt::AlignCenter,
                             QString::number(a.digit2));
        }
        painter.setPen(Qt::NoPen);
    }
    else
    {
        // 方格出现的动画效果
        if (a.startPos.x() > a.endPos.x())
            a.startPos += dPos[4];
        else
            a.startPos = a.endPos, rtn = true;
        brush.setColor(digitBkg[getBitCount(a.digit)]);
        painter.setBrush(brush);
        painter.drawRect(QRectF(a.startPos.x(), a.startPos.y(),
                         w - 2 * (a.startPos.x() - a.endPos.x()),
                         h - 2 * (a.startPos.y() - a.endPos.y())));
        painter.setPen(QColor::fromRgb(0, 0, 0));
        painter.drawText(QRectF(a.endPos.x(), a.endPos.y(), w, h),
                         Qt::AlignCenter, QString::number(a.digit));
        painter.setPen(Qt::NoPen);
    }
    return rtn;
}

void GameWidget::paintEvent(QPaintEvent *)
{
    // 构造一个QPainter对象 使用它来进行绘图
    QPainter painter(this);

    // 如果正在播放动画效果则绘制缓存位图
    if (isAnimating)
    {
        painter.drawImage(0, 0, *cacheImg);
        return;
    }

    // 构造一个画刷 颜色为R G B分量分别为141 121 81的颜色
    QBrush brush(QColor::fromRgb(141, 121, 81));
    // 使painter应用这个画刷
    painter.setBrush(brush);

    // 设置画笔为空笔 目的是使绘制的图形没有描边
    painter.setPen(Qt::NoPen);

    // 绘制一个矩形
    painter.drawRect(2, 2, width() - 4, height() - 4);

    // 计算每个小格子的宽度和高度
    w = width() - 4, h = height() - 4;
    w = (w - 25) / 4, h = (h - 25) / 4;

    /* 构造一个字体
     * 字体名字为Consolas
     * 字体设置为粗体
     * 字体大小为40像素
     * */
    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(40);
    // 使painter应用这个字体
    painter.setFont(font);

    // 循环绘制游戏面板
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            // 如果方格中有数字
            if (board[i][j])
            {
                // 设置画刷颜色为数码对应的颜色
                brush.setColor(digitBkg[getBitCount(board[i][j])]);
                // 应用这个画刷
                painter.setBrush(brush);
                // 绘制一个小方格
                painter.drawRect(QRectF(7 + (w + 5) * j, 7 + (h + 5) * i, w, h));
                // 设置画笔为黑色画笔
                painter.setPen(QColor::fromRgb(0, 0, 0));
                // 绘制数码
                painter.drawText(QRectF(7 + (w + 5) * j, 7 + (h + 5) * i, w, h), Qt::AlignCenter,
                                 QString::number(board[i][j]));
                // 设置画笔为空笔
                painter.setPen(Qt::NoPen);
            }
            // 如果方格中没有数字
            else
            {
                // 设置画刷颜色为 RGB分量为171 165 141的颜色
                brush.setColor(QColor::fromRgb(171, 165, 141));
                // 应用这个画刷
                painter.setBrush(brush);
                // 绘制小方格
                painter.drawRect(QRectF(7 + (w + 5) * j, 7 + (h + 5) * i, w, h));
            }
        }
}

void GameWidget::restart()
{
    // 初始化相关变量 同构造函数
    score = 0;
    digitCount = 2;
    memset(board, 0, sizeof(board));
    board[rand() % 4][rand() % 4] = 2;
    board[rand() % 4][rand() % 4] = 2;
    emit ScoreInc(score);
    update();
}

bool GameWidget::checkGameOver()
{
    // 循环检测是否含有相邻的相同数码
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            if (j != 3 && board[i][j] == board[i][j + 1])
                return false;
            if (i != 3 && board[i][j] == board[i + 1][j])
                return false;
        }
    return true;
}

bool GameWidget::checkWin()
{
    // 循环检测是否某个方格的数字为2048
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (board[i][j] == 2048)
                return true;
    return false;
}

int GameWidget::getBitCount(int n)
{
    // 循环获取数字二进制位数
    int c = 0;
    while (n >>= 1)
        c++;
    // 返回位数-1
    return c - 1;
}
