#include "FTopTip.h"

#include <QHBoxLayout>
#include <QDebug>

FTopTip::FTopTip(const QString &sTip, QWidget *parent) :
    QWidget(parent)
{
    this->setWindowFlags(Qt::ToolTip);

    this->setFixedHeight(20);
    initUI(sTip);
    initStyleSheet();
}

void FTopTip::initUI(const QString &sTip)
{
    m_pTipLbl = new QLabel(this);
    m_pTipLbl->setObjectName("tipLbl");
    m_pTipLbl->setAlignment(Qt::AlignCenter);
    m_pTipLbl->setText(sTip);

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget(m_pTipLbl);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);

    this->setLayout(pLayout);
}

//初始化样式
void FTopTip::initStyleSheet()
{
    this->setObjectName("fTopTip");
    this->setStyleSheet("QWidget#fTopTip{ background-color: rgb(238, 238, 118);}"
                        "QLabel#tipLbl{ color: rgb(240, 0, 0);}");
}
