#include "dialog.h"
#include "ui_dialog.h"
#include<QString>
#include<qbytearray.h>
#include<QUrl>
#include<qdebug.h>
#include<qmessagebox.h>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QString>
#include <QDebug>
#include<qdebug.h>
#include <QDebug>

#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonObject>
#include<QString>
#include<qbytearray.h>
#include<QUrl>
#include<qdebug.h>
#include<qmessagebox.h>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QString>
#include <QDebug>
#include<qdebug.h>
#include<QString>
#include<qbytearray.h>
#include<QUrl>
#include<qdebug.h>
#include<qmessagebox.h>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QString>
#include <QDebug>
#include<qdebug.h>
#include <QDebug>

#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

#include <QJsonObject>
#include <QJsonDocument>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    init();
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    qDebug() << manager->supportedSchemes();
}
Dialog::~Dialog()
{
    delete ui;
}
void Dialog::init()
{
    manager = new QNetworkAccessManager(this);

}
void Dialog::getHttpData(const QString&modelUrl,QByteArray&data)//获取网页信息
{
    QUrl url(modelUrl);
    QNetworkRequest request;
    request.setUrl(url);
    QNetworkReply * reply_ = manager->get(request);
    QEventLoop eventLoop2;
    connect(reply_, &QNetworkReply::finished, &eventLoop2, &QEventLoop::quit);
    eventLoop2.exec(QEventLoop::ExcludeUserInputEvents);
    data = reply_->readAll();
    reply_->deleteLater();
}
void Dialog::handleData(QByteArray &data)//预处理股价
{
        QString str = QString(data);
       QRegExp exp("<span class=\"stock-price (?:increase|decrease)\">\\s*(\\d+\\.\\d+)\\s*</span>");
        //exp.setMinimal(false);
        QStringList list = str.split("\n");
        urlList.clear();
        for (int i = 0;i < list.size();++i)
        {
            int pos = 0;
            while ((pos = exp.indexIn(list.at(i) , pos)) != -1)
            {
                pos += exp.matchedLength();
                qDebug() << "pos :" << pos;
                qDebug() << exp.cap(0);
                urlList << exp.cap(0);
            }
        }
}
void Dialog::handleData2(QByteArray &data)//预处理涨跌
{
        QString str = QString(data);
        QRegExp exp("<span class=\"stock-change \\w+\">([+-]?\\d+\\.\\d+)</span>");
        //exp.setMinimal(false);
        QStringList list = str.split("\n");
        urlList2.clear();
        for (int i = 0;i < list.size();++i)
        {
            int pos = 0;
            while ((pos = exp.indexIn(list.at(i) , pos)) != -1)
            {
                pos += exp.matchedLength();
                qDebug() << "pos :" << pos;
                qDebug() << exp.cap(0);
                urlList2 << exp.cap(0);
            }
        }
}
void Dialog::handleData3(QByteArray &data)//预处理涨跌百分比
{
        QString str = QString(data);
       QRegExp exp("<span class=\"stock-change-rate \\w+\">([+-]?\\d+\\.\\d+%?)</span>");
        //exp.setMinimal(false);
        QStringList list = str.split("\n");
        urlList3.clear();
        for (int i = 0;i < list.size();++i)
        {
            int pos = 0;
            while ((pos = exp.indexIn(list.at(i) , pos)) != -1)
            {
                pos += exp.matchedLength();
                qDebug() << "pos :" << pos;
                qDebug() << exp.cap(0);
                urlList3 << exp.cap(0);
            }
        }
}
void Dialog::extractNumbers(QStringList &urlList) //提取股价
{
    QRegExp numExp("\\d+\\.\\d+");  // 创建用于匹配浮点数的正则表达式
    QStringList numberList;         // 创建一个列表存储提取的数字
    foreach (const QString &entry, urlList) {
        int pos = 0;
        while ((pos = numExp.indexIn(entry, pos)) != -1) {
            QString number = numExp.cap(0);  // 捕获匹配到的浮点数
            numberList.append(number);       // 将浮点数添加到列表中
            pos += numExp.matchedLength();   // 移动到下一个可能的匹配位置
            qDebug() << "Extracted number:" << number;
        }
        urlList[0]=numberList[0];
    }
}
void Dialog::extractdecreaseNumbers(QStringList &urlList)
{
    QRegExp numExp("[+-]?\\d+\\.\\d+");  // 创建用于匹配浮点数的正则表达式
    QStringList numberList;         // 创建一个列表存储提取的数字
    foreach (const QString &entry, urlList) {
        int pos = 0;
        while ((pos = numExp.indexIn(entry, pos)) != -1) {
            QString number = numExp.cap(0);  // 捕获匹配到的浮点数
            numberList.append(number);       // 将浮点数添加到列表中
            pos += numExp.matchedLength();   // 移动到下一个可能的匹配位置
            qDebug() << "Decrease number:" << number;
        }
        urlList2[0]=numberList[0];
    }
}
void Dialog::extractrateNumbers(QStringList &urlList)
{
    QRegExp numExp("[+-]?\\d+\\.\\d+%?");  // 创建用于匹配浮点数的正则表达式
    QStringList numberList;         // 创建一个列表存储提取的数字
    foreach (const QString &entry, urlList) {
        int pos = 0;
        while ((pos = numExp.indexIn(entry, pos)) != -1) {
            QString number = numExp.cap(0);  // 捕获匹配到的浮点数
            numberList.append(number);       // 将浮点数添加到列表中
            pos += numExp.matchedLength();   // 移动到下一个可能的匹配位置
            qDebug() << "Rate number:" << number;
        }
        urlList3[0]=numberList[0];
    }
}
void Dialog::on_find_clicked()//查询股票信息
{
    QString stocknumber=ui->stockinput->text();
    QString web="https://www.laohu8.com/stock/"+stocknumber;//输入股票代码
    QByteArray data;
    getHttpData(web,data);
    handleData(data);
    extractNumbers(urlList);
    ui->stockprice_2->append(urlList[0]);

    handleData2(data);
    extractdecreaseNumbers(urlList2);
    ui->stockdecrease->append(urlList2[0]);

    handleData3(data);
    extractrateNumbers(urlList3);
    ui->stockrate->append(urlList3[0]);
}
