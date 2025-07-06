#pragma once

#include <QWidget>
#include <QLabel>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
class ScoreBoard :public QWidget
{
    Q_OBJECT
public:
    ScoreBoard(QWidget *parent=nullptr);
    ~ScoreBoard(){
        delete board;
    }
    QLabel *board= NULL;
    int total_Score = 0;
    QJsonObject getJsonData() const;
signals:
    //scores that this hero get
    void finalScore(int value);
public slots:
    void gameOver();
    void addScore();
    void toJson() const;
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    mutable QJsonObject ScoreboardData;

};
