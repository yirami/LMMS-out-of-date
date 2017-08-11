#include "cdetaildlg.h"

CDetailDlg::CDetailDlg(CDatabasePackage *dbPackage):CDirectEditDlg(dbPackage),textPrinter(QPrinter::HighResolution)
{
    setUI();

    connect(printB,SIGNAL(clicked()),this,SLOT(on_printB_clicked()));
}

void CDetailDlg::setUI()
{
    this->setFixedSize(1020,600);
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowMinimizeButtonHint;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    actionBox = new QGroupBox(this);
    actionBox->setFixedWidth(220);
    actionBox->setTitle("操作");
    printB = new QPushButton(this);
    printB->setMinimumHeight(50);
    printB->setText("打印");
    QVBoxLayout *actionboxlayout = new QVBoxLayout(this);
    actionboxlayout->addStretch(1);
    actionboxlayout->addWidget(queryBox);
    actionboxlayout->addStretch(2);
    actionboxlayout->addWidget(printB);
    actionboxlayout->addStretch(1);
    actionBox->setLayout(actionboxlayout);

    tabV = new QTableView(this);
    tabV->setModel(dataModel);
    tabV->setColumnWidth(0,60);
    tabV->setColumnHidden(0,true);
    tabV->setColumnWidth(1,100);
    tabV->setColumnWidth(2,180);
    tabV->setColumnWidth(3,180);
    tabV->setColumnWidth(4,100);
    tabV->setColumnWidth(5,80);
    tabV->setColumnWidth(6,80);
    tabV->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tabV->verticalHeader()->setMinimumWidth(22);
    tabV->verticalHeader()->hide();
    tabV->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabV->setSelectionMode( QAbstractItemView::MultiSelection);
    tabV->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabV->show();

    mainBox = new QGroupBox(this);
    QHBoxLayout *mlayout = new QHBoxLayout(this);
    mlayout->addWidget(tabV);
    mlayout->addWidget(actionBox);
    mainBox->setLayout(mlayout);

    QVBoxLayout *wlayout = new QVBoxLayout(this);
    wlayout->addWidget(mainBox);
    wlayout->addWidget(progressBox);
    this->setWindowTitle(QString("清单"));
}

void CDetailDlg::on_printB_clicked()
{
    progressInfo->setText(QString("正在准备数据！"));
    QStringList textList = prepareTextList();
    progressInfo->setText(QString("数据准备完成！"));
    progressShow->setRange(0,textList.count());
    progressShow->setHidden(false);
    printConfig();
    progressInfo->setText(QString("正在导出数据至文件：'%1'...").arg(outFileName));
    Q_ASSERT(textPainter.begin(&textPrinter));
    QPointF start(20,400);
    writeText(start,textList);
    textPainter.end();
    progressShow->setValue(textList.count());
    progressInfo->setText(QString("导出完成！"));
}

void CDetailDlg::printConfig()
{
    outFileName = QFileDialog::getSaveFileName(this,"导出至PDF",QString(),"*.pdf");
    if ( !outFileName.isEmpty())
    {
        if ( QFileInfo(outFileName).suffix().isEmpty())
            outFileName.append(".pdf");
        textPrinter.setOutputFormat(QPrinter::PdfFormat);
        textPrinter.setPageSize(QPrinter::A4);
        textPrinter.setOutputFileName(outFileName);
    }
}

QString CDetailDlg::makeTime()
{
    QDateTime time = QDateTime::currentDateTime();
    QString format("yyyy-MM-dd hh:mm:ss");
    QString thisTime = time.toString(format);
    return thisTime;
}

QString CDetailDlg::formatOneRecord(int sel)
{
    QString agentName = dataModel->data(dataModel->index(sel,1)).toString();
    QString name = dataModel->data(dataModel->index(sel,2)).toString();
    QString nameT = alighChinese(name,20);
    QString madeIn = dataModel->data(dataModel->index(sel,3)).toString();
    QString madeInT = alighChinese(madeIn,30);
    QString spec = dataModel->data(dataModel->index(sel,4)).toString();
    int stock = dataModel->data(dataModel->index(sel,5)).toInt();
    float price = dataModel->data(dataModel->index(sel,6)).toFloat();
    QString recordFormat(" -> %1 | %2 | %3 | %4 | %5 | %6");
    QString outRecord(recordFormat.arg(agentName,10).arg(nameT).arg(madeInT).arg(spec,5).arg(stock,5).arg(price,8,'f',4));
    return outRecord;
}

QStringList CDetailDlg::prepareTextList()
{
    QStringList textList;
    QItemSelectionModel *thisSelectionM = tabV->selectionModel();
    if (thisSelectionM->hasSelection())
    {
        QModelIndexList selections = thisSelectionM->selectedIndexes();
        QMap<int, int> rowMap;
        foreach (QModelIndex index, selections)
            rowMap.insert(index.row(), 0);
        QMapIterator<int, int> rowMapIterator(rowMap);
        rowMapIterator.toFront();
        while (rowMapIterator.hasNext())
        {
            rowMapIterator.next();
            int row = rowMapIterator.key();
            textList<<formatOneRecord(row);
        }
        thisSelectionM->clearSelection();
    }
    else
    {
        if (dataModel->rowCount()!=0)
        {
            for (int idx=0;idx<dataModel->rowCount();idx++)
                textList<<formatOneRecord(idx);
        }
    }
    return textList;
}

void CDetailDlg::writeText(QPointF &startP, QStringList &textList)
{
    QPointF deltaP(0,200);
    textPainter.drawText(startP,QString("打印时间 @ ").append(makeTime()));
    startP += deltaP;
    startP += deltaP;
    for (int idx=0;idx<textList.count();idx++)
    {
        progressShow->setValue(idx);
        textPainter.drawText(startP,textList.at(idx));
        startP += deltaP;
    }
}

QString CDetailDlg::alighChinese(const QString &raw, int fieldWidth)
{
    QString out;
    if (fieldWidth==0)
        out = raw;
    else if(fieldWidth>0)
        out = QString("%1%2").arg(" ",fieldWidth-2*raw.length()).arg(raw);
    else
        out = QString("%1%2").arg(raw).arg(" ",-fieldWidth-2*raw.length());
    return out;
}
