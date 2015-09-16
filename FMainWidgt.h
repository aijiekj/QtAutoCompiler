#ifndef FMAINWIDGT_H
#define FMAINWIDGT_H

#include <QWidget>
#include <QStandardItemModel>
#include <QProcess>
#include <QMenu>

namespace Ui {
class FMainWidgt;
}

class FMainWidgt : public QWidget
{
    Q_OBJECT

public:
    explicit FMainWidgt(QWidget *parent = 0);
    ~FMainWidgt();

    enum eCol{
        eCol_RecordNm = 0,
        eCol_FileName,
        eCol_CompilingStatus,
        eCol_ColTotalCount
    };

    enum eCurProcess {
        eGetQtVersion_Process = 0,  //获取当前Qt版本
        eCreateMakeFile_Process,     //创建MakeFile
        eCompiling_Process,         //编译
        eNormal_Process             //未使用
    };

    enum eErrorCode{
        eNoError = 0,               //没有错误
        eProFileList_IsEmpty,       //ProFile文件列表为空
        eQMake_UnSetting,           //未设置QMake路径
        eMake_UnSetting,            //未设置Make
        eCompliPath_IsEmpty,        //编译路径为空
        eCompliPath_IsNullity,      //编译路径无效
        eCompli_Completed,          //编译完成
        eUnKnow_Error               //未识别错误
    };

private slots:
    //添加profile按钮单击处理事件
    void slot_AddProFileBtn_Clicked();
    //设置编译路径按钮单击处理事件
    void slot_SetCompiledPathBtn_Clicked();
    //编译按钮单击处理事件
    void slot_CompilBtn_Clicked();
    //设置QMake路径
    void slot_SetQmakePathBtn_Clicked();
    //当前进程完成状态
    void slot_CurProcess_Finished(int nExitCode, QProcess::ExitStatus eExitStatus);
    //设置Make路径按钮单击处理事件
    void slot_SetMakePathBtn_Clicked();
    //设置NormalLog自动滚动
    void slot_NormalLogTextBw_cursorPositionChanged();
    //ProFile文件列表右键菜单单击处理事件
    void slot_ProFileListTableView_CustomContextMenuRequested(const QPoint & pos);
    //删除菜单单击处理事件
    void slot_DelAct_Triggered();


private:
    //初始化UI
    void initUI();
    //初始化列表
    void initTableVw();
    //初始化信号与槽
    void initSignalSlot();
    /*
     *      检查编译前状态
     *
     *      返回值：成功-0
     *             其他-状态值
    */
    int checkCompliBeforStatus();
    //设置错误状态提示
    void setErrorStatusTip(int nErrorCode);

    //开始编译
    void sartCompling();


private:
    Ui::FMainWidgt *ui;

    QMenu               *m_pProFileTableMenu;
    QAction             *m_pDelRowAct;

    QStandardItemModel  *m_pSourceProFileModel;
    QProcess            *m_pCurrProcess;

    int                  m_nProcessFlg;
    int                  m_nCurrentComplingIndex;
};

#endif // FMAINWIDGT_H
