#ifndef FTOPTIP_H
#define FTOPTIP_H

#include <QWidget>
#include <QLabel>

class FTopTip : public QWidget
{
    Q_OBJECT
public:
    explicit FTopTip(const QString &sTip, QWidget *parent = 0);

signals:

public slots:

private:
    //初始化UI
    void initUI(const QString &sTip);
    //初始化样式
    void initStyleSheet();

private:
    QLabel      *m_pTipLbl;

};

#endif // FTOPTIP_H
