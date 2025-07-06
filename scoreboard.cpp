#include "scoreboard.h"

ScoreBoard::ScoreBoard(QWidget *parent):QWidget(parent)
{

    this->resize(120,100);
    board = new QLabel(this);
    QFont font("Arial",80,QFont::Bold);
    board->resize(120,110);
    board->setFont(font);
    board->setStyleSheet("color:pink;background-color:lightgreen");
    board->setAlignment(Qt::AlignCenter);
}
void ScoreBoard::addScore()
{
    total_Score+=1;
    this->update();
}
void ScoreBoard::gameOver()
{
    emit finalScore(total_Score);
}
QJsonObject ScoreBoard::getJsonData() const
{
    return ScoreboardData;
}
void ScoreBoard::toJson() const
{
    qDebug()<<"scoreboard json";
    QJsonObject boardJ;
    boardJ["fontfamily"] = board->font().family();
    boardJ["fontsize"] = board->font().pointSize();
    boardJ["fontbold"]= board->font().bold();
    boardJ["width"] = board->width();
    boardJ["height"] = board->height();
    boardJ["x"] = board->x();
    boardJ["y"] = board->y();
    boardJ["text"] = board->text();
    boardJ["stylesheet"] = board->styleSheet();
    QJsonValue alignmentStr;
    if (board->alignment() == Qt::AlignLeft) {
        alignmentStr = "AlignLeft";
    } else if (board->alignment() == Qt::AlignRight) {
        alignmentStr = "AlignRight";
    } else if (board->alignment() == Qt::AlignHCenter) {
        alignmentStr = "AlignHCenter";
    } else if (board->alignment() == Qt::AlignCenter) {
        alignmentStr = "AlignCenter";
    }
    boardJ["alignment"] = alignmentStr;
    ScoreboardData["board"] = boardJ;
    ScoreboardData["total_Score"] = total_Score;
}
void ScoreBoard::paintEvent(QPaintEvent *event)
{
    board->setText(QString("%1").arg(total_Score));
}
