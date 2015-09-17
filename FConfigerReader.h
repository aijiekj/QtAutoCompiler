/*
 *      配置文件读取
*/

#ifndef FCONFIGERREADER_H
#define FCONFIGERREADER_H

#include <QString>

class FConfigerReader
{
public:
    ~FConfigerReader();
    static FConfigerReader &getInstance();

    /*
     *  设置工具配置信息
    */
    void setToolsConfigInfo(const QString &sQMakePath, const QString &sMakePath);

    /*
     *  获取工具配置信息
    */
    void getToolsConfigInfo(QString &sQMakePath, QString &sMakePath);

private:
    FConfigerReader();

private:
    static FConfigerReader* m_pInstance;    //
};

#endif // FCONFIGERREADER_H
