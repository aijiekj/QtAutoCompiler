#include "FMainWidgt.h"
#include "ui_FMainWidgt.h"
#include "FTopTip.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QProcess>
#include <QTimer>
#include <QDir>
#include <QDebug>

FMainWidgt::FMainWidgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FMainWidgt)
{
    ui->setupUi(this);

    m_nProcessFlg = eNormal_Process;

    m_pCurrProcess = new QProcess(this);
    m_nCurrentComplingIndex = 0;

    initUI();
    initSignalSlot();
}

FMainWidgt::~FMainWidgt()
{
    delete ui;
}

//添加profile按钮单击处理事件
void FMainWidgt::slot_AddProFileBtn_Clicked()
{
    QStringList lFiles = QFileDialog::getOpenFileNames(
                this,
                "请选择ProFile",
                ".",
                "Pro Files (*.pro)");

    int nRecordNm = m_pSourceProFileModel->rowCount();
    for(int i = 0; i < lFiles.count(); i++)
    {
        QFileInfo info(lFiles.at(i));

        QList<QStandardItem*> lRowItems;

        QStandardItem *pRecordNmItem = new QStandardItem(nRecordNm, eCol_RecordNm);
        pRecordNmItem->setData(nRecordNm+1, Qt::DisplayRole);
        pRecordNmItem->setData(Qt::AlignHCenter, Qt::TextAlignmentRole);
        lRowItems.append(pRecordNmItem);

        QStandardItem *pFileNameItem = new QStandardItem(nRecordNm, eCol_FileName);
        pFileNameItem->setData(info.fileName(), Qt::DisplayRole);
        pFileNameItem->setData(info.absoluteFilePath(), Qt::UserRole+1);
        pFileNameItem->setData(Qt::AlignHCenter, Qt::TextAlignmentRole);
        lRowItems.append(pFileNameItem);

        QStandardItem *pStatusItem = new QStandardItem(nRecordNm, eCol_CompilingStatus);
        pStatusItem->setData(QString("就绪"), Qt::DisplayRole);
        pStatusItem->setData(Qt::AlignHCenter, Qt::TextAlignmentRole);
        lRowItems.append(pStatusItem);

        m_pSourceProFileModel->appendRow(lRowItems);
    }
}

//设置编译路径按钮单击处理事件
void FMainWidgt::slot_SetCompiledPathBtn_Clicked()
{
    QString sDir = QFileDialog::getExistingDirectory(this,
                            tr("请选择编译目录"), ".",
                            QFileDialog::ShowDirsOnly
                                | QFileDialog::DontResolveSymlinks);
    if(!sDir.isEmpty())
    {
        ui->compiledPathLdt->setText(sDir);
    }
}

//编译按钮单击处理事件
void FMainWidgt::slot_CompilBtn_Clicked()
{
    if(checkCompliBeforStatus() != 0)
    {
        return ;
    }

    m_nCurrentComplingIndex = 0;
    ui->logTabWgt->show();
    ui->logTabWgt->setCurrentWidget(ui->normalLogTab);

    sartCompling();
}

//设置QMake路径
void FMainWidgt::slot_SetQmakePathBtn_Clicked()
{
    QString sQMakeFile = QFileDialog::getOpenFileName(
                this,
                "请选择QMake路径",
                ".",
                "qmake (qmake.exe)(qmake.exe)");

    if(!sQMakeFile.isEmpty())
    {
        QFileInfo qmakeInfo(sQMakeFile);

        ui->qmakePathLndt->setText(qmakeInfo.absoluteFilePath());

        QStringList lArguments;
        lArguments << "-v";

        m_nProcessFlg = eGetQtVersion_Process;
        m_pCurrProcess->start(qmakeInfo.absoluteFilePath(), lArguments);
    }
}

//当前进程完成状态
void FMainWidgt::slot_CurProcess_Finished(int nExitCode,
                        QProcess::ExitStatus eExitStatus)
{
    QString sOut = m_pCurrProcess->readAllStandardOutput();
    ui->normalTextBw->append(sOut);

    qDebug() << "Qmake >>>" << sOut;
    qDebug() << "ExitCode :::" << nExitCode
              << "ExitStatus:: " << eExitStatus;

    QString sError = m_pCurrProcess->readAllStandardError();

    ui->errorTextBw->append(sError);

    if(eExitStatus == QProcess::NormalExit)
    {
        if(m_nProcessFlg == eGetQtVersion_Process)
        {
            QStringList lOut = sOut.split("\n");
            if(lOut.count() == 3)
            {
                QStringList lQtVersion = lOut.at(1).split(" in ");

                if(lQtVersion.count() ==2)
                {
                    QString sQtVersion = lQtVersion.at(0);
                    ui->qtVersionLndt->setText(sQtVersion.replace("Using ", ""));
                }
            }
        }
        else if(m_nProcessFlg == eCompiling_Process)
        {
            QString sStatus;

            if(nExitCode == 0)
            {
                sStatus = QString("成功");
            }
            else if(nExitCode == 2)
            {
                sStatus = QString("失败");
            }

            m_pSourceProFileModel->setData(m_pSourceProFileModel->index(m_nCurrentComplingIndex, eCol_CompilingStatus),
                                           sStatus,
                                           Qt::DisplayRole);

            QDir::setCurrent(QCoreApplication::applicationDirPath ());

            m_nCurrentComplingIndex++;

            sartCompling();
        }
        else if(m_nProcessFlg == eCreateMakeFile_Process)
        {
            QStringList lArguments;
            lArguments << "-f" << "Makefile";

            m_nProcessFlg = eCompiling_Process;
            m_pCurrProcess->start(ui->makePathLndt->text(), lArguments);
        }
    }
}

//设置Make路径按钮单击处理事件
void FMainWidgt::slot_SetMakePathBtn_Clicked()
{
    QString sMakeFile = QFileDialog::getOpenFileName(
                this,
                "请选择Make路径",
                ".",
                "make (make.exe)(mingw32-make.exe)");

    if(!sMakeFile.isEmpty())
    {
        QFileInfo makeInfo(sMakeFile);

        ui->makePathLndt->setText(makeInfo.absoluteFilePath());
    }
}

//设置NormalLog自动滚动
void FMainWidgt::slot_NormalLogTextBw_cursorPositionChanged()
{
    QTextCursor cursor = ui->normalTextBw->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->normalTextBw->setTextCursor(cursor);
}

void FMainWidgt::slot_ProFileListTableView_CustomContextMenuRequested(const QPoint &pos)
{
    if(ui->proFileTableVw->currentIndex().isValid())
    {
        m_pDelRowAct->setDisabled(false);
    }
    else
    {
        m_pDelRowAct->setDisabled(true);
    }

    m_pProFileTableMenu->exec(QCursor::pos());
}

////ProFile文件列表单击处理事件
//void FMainWidgt::slot_ProFileListTableView_Pressed(const QModelIndex &index)
//{
//    if(!index.isValid())
//    {
//        return ;
//    }

//    m_pProFileTableMenu->move(QCursor::pos());
//    m_pProFileTableMenu->show();
//}

//删除菜单单击处理事件
void FMainWidgt::slot_DelAct_Triggered()
{
    QModelIndex index = ui->proFileTableVw->currentIndex();
    m_pSourceProFileModel->removeRow(index.row());
}

//初始化UI
void FMainWidgt::initUI()
{
    m_pProFileTableMenu = new QMenu(this);

    m_pDelRowAct = new QAction(m_pProFileTableMenu);
    m_pDelRowAct->setText(QString("删除"));

    m_pProFileTableMenu->addAction(m_pDelRowAct);


    //初始化profile文件列表
    initTableVw();
    ui->logTabWgt->hide();
}

//初始化列表
void FMainWidgt::initTableVw()
{
    ui->proFileTableVw->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->proFileTableVw->setFocusPolicy(Qt::NoFocus);
    //ui->proFileTableVw->setShowGrid(false);
    ui->proFileTableVw->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->proFileTableVw->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->proFileTableVw->setEditTriggers(QAbstractItemView::NoEditTriggers);//不能编辑
//    ui->proFileTableVw->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    ui->proFileTableVw->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    ui->proFileTableVw->setFont(ZTools::getFont(ZTools::eFont_MidType));
//    ui->proFileTableVw->horizontalHeader()->setFont(ZTools::getFont(ZTools::eFont_MidType));
    ui->proFileTableVw->horizontalHeader()->setMinimumHeight(40);
    ui->proFileTableVw->horizontalHeader()->setSectionResizeMode (QHeaderView::Stretch);
    ui->proFileTableVw->verticalHeader()->setVisible(false);
//    ui->proFileTableVw->horizontalScrollBar()->setDisabled(true);
//    ui->proFileTableVw->verticalScrollBar()->setDisabled(true);

    m_pSourceProFileModel = new QStandardItemModel;
    m_pSourceProFileModel->setColumnCount(eCol_ColTotalCount);

    m_pSourceProFileModel->setHeaderData(eCol_RecordNm, Qt::Horizontal, QString("序号"));
    m_pSourceProFileModel->setHeaderData(eCol_FileName, Qt::Horizontal, QString("文件名"));
    m_pSourceProFileModel->setHeaderData(eCol_CompilingStatus, Qt::Horizontal, QString("编译状态"));

    ui->proFileTableVw->setModel(m_pSourceProFileModel);
}

//初始化信号与槽
void FMainWidgt::initSignalSlot()
{
    connect(ui->appendBtn, SIGNAL(clicked()),
            this, SLOT(slot_AddProFileBtn_Clicked()));
    connect(ui->setCompilePathBtn, SIGNAL(clicked()),
            this, SLOT(slot_SetCompiledPathBtn_Clicked()));
    connect(ui->compilingBtn, SIGNAL(clicked()),
            this, SLOT(slot_CompilBtn_Clicked()));
    connect(ui->setQmakePathBtn, SIGNAL(clicked()),
            this, SLOT(slot_SetQmakePathBtn_Clicked()));
    connect(m_pCurrProcess, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(slot_CurProcess_Finished(int,QProcess::ExitStatus)));
    connect(ui->setMakePathBtn, SIGNAL(clicked()),
            this, SLOT(slot_SetMakePathBtn_Clicked()));
    connect(ui->normalTextBw, SIGNAL(cursorPositionChanged()),
            this, SLOT(slot_NormalLogTextBw_cursorPositionChanged()));
    connect(m_pDelRowAct, SIGNAL(triggered()),
            this, SLOT(slot_DelAct_Triggered()));
    connect(ui->proFileTableVw, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(slot_ProFileListTableView_CustomContextMenuRequested(QPoint)));
}

//检查编译前状态
int FMainWidgt::checkCompliBeforStatus()
{
    int nRet = eNoError;

    if(m_pSourceProFileModel->rowCount() == 0)
    {
        setErrorStatusTip(eProFileList_IsEmpty);
        return eProFileList_IsEmpty;
    }

    if(ui->qmakePathLndt->text().isEmpty())
    {
        setErrorStatusTip(eQMake_UnSetting);
        return eQMake_UnSetting;
    }

    if(ui->makePathLndt->text().isEmpty())
    {
        setErrorStatusTip(eMake_UnSetting);
        return eMake_UnSetting;
    }

    if(ui->compiledPathLdt->text().isEmpty())
    {
        setErrorStatusTip(eCompliPath_IsEmpty);
        return eCompliPath_IsEmpty;
    }

    QDir compliDir(ui->compiledPathLdt->text());

    if(!compliDir.exists())
    {
        setErrorStatusTip(eCompliPath_IsNullity);
        return eCompliPath_IsNullity;
    }

    return nRet;
}

//设置错误状态提示
void FMainWidgt::setErrorStatusTip(int nErrorCode)
{
    QString sError;

    switch(nErrorCode)
    {
    case eProFileList_IsEmpty:
        sError = QString("Pro Files 文件列表为空！");
        break;
    case eQMake_UnSetting:
        sError = QString("未设置QMake路径！");
        break;
    case eMake_UnSetting:
        sError = QString("未设置Make路径");
        break;
    case eCompliPath_IsEmpty:
        sError = QString("编译路径为空！");
        break;
    case eCompliPath_IsNullity:
        sError = QString("编译路径无效！");
        break;
    case eCompli_Completed:
        sError = QString("编译完成！");
        break;
    default:
        sError = QString("未知错误！");
    }

    FTopTip *pTip = new FTopTip(sError, this);
    pTip->setFixedWidth(this->width());

    QPoint globalPos = this->mapToGlobal(QPoint(0,0));
    pTip->move(globalPos);
    pTip->show();

    QTimer::singleShot(2000, pTip, SLOT(close()));
}

//开始编译
void FMainWidgt::sartCompling()
{
    qDebug() << ">>>>> Current ComplingIndex>>> " << m_nCurrentComplingIndex;
    if(m_nCurrentComplingIndex == m_pSourceProFileModel->rowCount())
    {
        setErrorStatusTip(eCompli_Completed);
        return ;
    }

    QString sProFileName = m_pSourceProFileModel->data(m_pSourceProFileModel->index(m_nCurrentComplingIndex,
                                                          eCol_FileName),
                                        Qt::UserRole+1).toString();

    QFileInfo proFileInfo(sProFileName);

    qDebug() << proFileInfo.baseName();

    QString sBuildPath = ui->compiledPathLdt->text()+ "/" +
                            proFileInfo.baseName() + "_build";

    QDir buildDir;
    buildDir.setPath(sBuildPath);

    if(!buildDir.exists())
    {
        buildDir.mkpath(sBuildPath);
    }

    QDir::setCurrent(sBuildPath);

    QStringList lArguments;
    lArguments << sProFileName;

    m_nProcessFlg = eCreateMakeFile_Process;
    m_pCurrProcess->start(ui->qmakePathLndt->text(), lArguments);

    //buildDir.cdUp();
}
