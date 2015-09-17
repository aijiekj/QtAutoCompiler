#include "FConfigerReader.h"

#include <QSettings>
#include <QApplication>

FConfigerReader::FConfigerReader()
{

}

FConfigerReader* FConfigerReader::m_pInstance = NULL;

FConfigerReader::~FConfigerReader()
{
    delete m_pInstance;
    m_pInstance = NULL;
}

FConfigerReader &FConfigerReader::getInstance()
{
    if(m_pInstance == NULL)
    {
        m_pInstance = new FConfigerReader;
    }

    return *m_pInstance;
}

//设置QMakePath
void FConfigerReader::setToolsConfigInfo(const QString &sQMakePath, const QString &sMakePath)
{
    QString fileName = QCoreApplication::applicationDirPath() +
                                QString("/Config/config.ini");
    QSettings *settings = new QSettings(fileName, QSettings::IniFormat);
    settings->setIniCodec("UTF8");

    if(!sQMakePath.isEmpty())
    {
        settings->beginGroup("Tools");
        settings->setValue("QMakePath", sQMakePath);
        settings->endGroup();
    }

    if(!sMakePath.isEmpty())
    {
        settings->beginGroup("Tools");
        settings->setValue("MakePath", sMakePath);
        settings->endGroup();
    }    

    delete settings;
    settings = NULL;
}

//获取双屏配置文件
void FConfigerReader::getToolsConfigInfo(QString &sQMakePath, QString &sMakePath)
{
    QString fileName = QCoreApplication::applicationDirPath() +
                            QString("/Config/config.ini");
    QSettings *settings = new QSettings(fileName, QSettings::IniFormat);
    settings->setIniCodec("UTF8");

    sQMakePath = settings->value("Tools/QMakePath").toString();

    sMakePath = settings->value("Tools/MakePath").toString();

    delete settings;
    settings = NULL;
}


