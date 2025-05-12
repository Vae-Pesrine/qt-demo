#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QTimer>
#include <QFileDialog>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isReceiving(true),
    saveScanCounter(1)
{
    ui->setupUi(this);

    COM = new QSerialPort();

    // data display
    dataDisplays[0][0] = ui->data_0_0;
    dataDisplays[0][1] = ui->data_0_1;
    dataDisplays[0][2] = ui->data_0_2;
    dataDisplays[0][3] = ui->data_0_3;
    dataDisplays[1][0] = ui->data_1_0;
    dataDisplays[1][1] = ui->data_1_1;
    dataDisplays[1][2] = ui->data_1_2;
    dataDisplays[1][3] = ui->data_1_3;
    dataDisplays[2][0] = ui->data_2_0;
    dataDisplays[2][1] = ui->data_2_1;
    dataDisplays[2][2] = ui->data_2_2;
    dataDisplays[2][3] = ui->data_2_3;
    dataDisplays[3][0] = ui->data_3_0;
    dataDisplays[3][1] = ui->data_3_1;
    dataDisplays[3][2] = ui->data_3_2;
    dataDisplays[3][3] = ui->data_3_3;

    // btn
    ui->btn_clear_rx->setStyleSheet("background-color:white");
    ui->btn_clear_tx->setStyleSheet("background-color:white");
    ui->btn_enter_tx->setStyleSheet("background-color:white");
    ui->btn_stop_rx->setStyleSheet("background-color:white");
    ui->btn_serialport_onoff->setStyleSheet("background-color:white");
    ui->key_Refresh_COM->setStyleSheet("background-color:white");
    ui->btn_set_save_path->setStyleSheet("background-color:#E0FFFF;");
    ui->btn_save_data->setStyleSheet("background-color:#98FB98;");

    // find all ports
    foreach( const QSerialPortInfo &info,QSerialPortInfo::availablePorts() )
    {
        ui->set_com->addItem(info.portName());
    }

    // bind
    connect(COM,SIGNAL(readyRead()),this,SLOT(RX_FUN()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_serialport_onoff_clicked()
{
    // PortName
    COM->setPortName(ui->set_com->currentText());
    qDebug() << "Serial port name: " << ui->set_com->currentText();

    // BaudRate
    COM->setBaudRate(ui->set_baudrate->currentText().toInt());
    qDebug() << "BaudRate: " << ui->set_baudrate->currentText().toInt();

    // DataBits
    switch(ui->set_databits->currentIndex())
    {
    case 0:
        COM->setDataBits(QSerialPort::Data5);
        break;
    case 1:
        COM->setDataBits(QSerialPort::Data6);
        break;
    case 2:
        COM->setDataBits(QSerialPort::Data7);
        break;
    case 3:
        COM->setDataBits(QSerialPort::Data8);
        break;
    default:
        break;
    }
    qDebug() << "Data bits: " << COM->dataBits();

    // Parity
    switch (ui->set_parity->currentIndex()) {
    case 0:
        COM->setParity(QSerialPort::NoParity);
        break;
    case 1:
        COM->setParity(QSerialPort::EvenParity);
        break;
    case 2:
        COM->setParity(QSerialPort::MarkParity);
        break;
    case 3:
        COM->setParity(QSerialPort::OddParity);
        break;
    default:
        break;
    }
    qDebug() << "Parity: " << COM->parity();

    // StopBits
    switch(ui->set_stopbits->currentIndex())
    {
    case 0:
        COM->setStopBits(QSerialPort::OneStop);
        break;
    case 1:
        COM->setStopBits(QSerialPort::OneAndHalfStop);
        break;
    case 2:
        COM->setStopBits(QSerialPort::TwoStop);
        break;
    default:
        break;
    }
    qDebug() << "Stop bits: " << COM->stopBits();

    // Control
    COM->setFlowControl(QSerialPort::NoFlowControl);

    if(ui->btn_serialport_onoff->text() == "打开串口" ){
         if(COM->open(QIODevice::ReadWrite) == true){
            ui->btn_serialport_onoff->setText("关闭串口");
            ui->btn_serialport_onoff->setStyleSheet("background-color: rgb(173, 216, 230);");
         } else{
             QMessageBox::critical(this,"错误提示","打开串口失败！或其他错误。\r\n请选择正确的串口或该串口被占用");
             COM->close();
             return;
         }
    } else if(ui->btn_serialport_onoff->text() == "关闭串口" ){
        COM->close();
        ui->btn_serialport_onoff->setText("打开串口");
        ui->btn_serialport_onoff->setStyleSheet("background-color:white");
    }
}

void MainWindow::on_btn_serialport_refresh_clicked()
{
    ui->set_com->clear();
    foreach( const QSerialPortInfo &info,QSerialPortInfo::availablePorts() )
    {
        ui->set_com->addItem(info.portName());
    }
}

void MainWindow::on_btn_clear_rx_clicked()
{
    ui->text_rx->clear();
    ui->data_0_0->clear();
    ui->data_0_1->clear();
    ui->data_0_2->clear();
    ui->data_0_3->clear();
    ui->data_1_0->clear();
    ui->data_1_1->clear();
    ui->data_1_2->clear();
    ui->data_1_3->clear();
    ui->data_2_0->clear();
    ui->data_2_1->clear();
    ui->data_2_2->clear();
    ui->data_2_3->clear();
    ui->data_3_0->clear();
    ui->data_3_1->clear();
    ui->data_3_2->clear();
    ui->data_3_3->clear();
}

void MainWindow::on_btn_stop_rx_clicked()
{
    isReceiving = !isReceiving;
    if(isReceiving){
        ui->btn_stop_rx->setText("停止接收");
        ui->btn_stop_rx->setStyleSheet("background-color:white");
    } else{
        ui->btn_stop_rx->setText("开始接收");
        ui->btn_stop_rx->setStyleSheet("background-color: rgb(173, 216, 230);");
        serialBuffer.clear();
    }
}

void MainWindow::on_btn_enter_tx_clicked()
{
    COM->write(ui->text_tx->toPlainText().toLatin1());
}

void MainWindow::on_btn_clear_tx_clicked()
{
    ui->text_tx->clear();
}

//串口接收
void MainWindow::RX_FUN()
{
    if(!isReceiving){
        COM->readAll();
        return;
    }

    QByteArray newData = COM->readAll();
    serialBuffer.append(QString::fromLatin1(newData));

    ui->text_rx->insertPlainText(tr(newData));
    ui->text_rx->moveCursor(QTextCursor::End);

    // 按行处理数据
    int endIndex;
    while((endIndex = serialBuffer.indexOf('\n')) != -1){
        int crIdx = (endIndex > 0 && serialBuffer[endIndex-1] == '\r') ? endIndex-1 : -1;
        QString line = crIdx != -1 ? serialBuffer.left(crIdx) : serialBuffer.left(endIndex);
        serialBuffer = serialBuffer.mid(endIndex+1);
        processLine(line);
    }
}

void MainWindow::processLine(const QString &line)
{
    QStringList parts = line.split('\t');
    if (parts.size() != 2) return;

    QString key = parts[0].trimmed();
    QString valueStr = parts[1].trimmed();

    // 解析数值
    bool ok;
    int value = valueStr.toInt(&ok);
    if (!ok) return;

    // 解析data-i-j格式
    QStringList keyParts = key.split('-');
    if (keyParts.size() != 3 || keyParts[0] != "data") return;

    bool iOk, jOk;
    int i = keyParts[1].toInt(&iOk);
    int j = keyParts[2].toInt(&jOk);

    if (iOk && jOk && i >= 0 && i < 4 && j >= 0 && j < 4) {
        QLineEdit *edit = dataDisplays[i][j];
        if (edit) {
            edit->setText(QString::number(value));

            // 设置背景颜色
            QColor bgColor = numberToColor(value);
            QString style = QString("background-color: rgb(%1, %2, %3);")
                                .arg(bgColor.red())
                                .arg(bgColor.green())
                                .arg(bgColor.blue());
            edit->setStyleSheet(style);
        }
    }
}

QColor MainWindow::numberToColor(uint32_t number) const
{
    const uint32_t MAX_VALUE = 4095;
    const uint8_t COLOR_SEGMENTS = 4;
    const uint16_t SEGMENT_SIZE = (MAX_VALUE + 1) / COLOR_SEGMENTS; // 1024

    number = qMin<uint32_t>(number, MAX_VALUE);

    uint8_t segment = number / SEGMENT_SIZE;
    segment = qMin<uint8_t>(segment, COLOR_SEGMENTS - 1);

    uint16_t segment_val = number % SEGMENT_SIZE;
    uint32_t ratio = (segment_val << 8) / SEGMENT_SIZE;

    struct ColorSegment {
        uint8_t r_start, g_start, b_start;
        uint8_t r_end, g_end, b_end;
    };

    // 调整色段顺序和颜色，实现从浅蓝到红的过渡
    const ColorSegment segments[COLOR_SEGMENTS] = {
        {0, 128, 255,   0, 255, 255}, // 浅蓝 → 青
        {0, 255, 255,   0, 255, 0},    // 青 → 绿
        {0, 255, 0,   255, 255, 0},    // 绿 → 黄
        {255, 255, 0,   255, 0, 0}     // 黄 → 红
    };

    const auto& s = segments[segment];

    int16_t r_diff = s.r_end - s.r_start;
    int16_t g_diff = s.g_end - s.g_start;
    int16_t b_diff = s.b_end - s.b_start;

    uint8_t r = s.r_start + (r_diff * ratio) / 256;
    uint8_t g = s.g_start + (g_diff * ratio) / 256;
    uint8_t b = s.b_start + (b_diff * ratio) / 256;

    return QColor(r, g, b);
}

void MainWindow::on_btn_save_data_clicked()
{
    if (saveFilePath.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先设置保存路径！");
        return;
    }

    QFile file(saveFilePath);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::critical(this, "错误", "文件打开失败：" + file.errorString());
        return;
    }

    QTextStream out(&file);

    out << QString("\n---- Scan %1 ----\n").arg(saveScanCounter, 3, 10, QLatin1Char('0')); // 三位数编号

    QDateTime currentTime = QDateTime::currentDateTime();
    out << "Time: " << currentTime.toString("yyyy-MM-dd hh:mm:ss.zzz\n");

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            QString val = dataDisplays[i][j]->text().isEmpty() ?
                              "N/A" : dataDisplays[i][j]->text();
            out << val << "\t";
        }
        out << "\n";
    }

    file.close();

    ui->lbl_status->setText(QString("已保存扫描#%1 | 路径：%2")
                                .arg(saveScanCounter)
                                .arg(QFileInfo(saveFilePath).fileName()));
    saveScanCounter++;

    QTimer::singleShot(300, [this](){
        ui->lbl_status->setStyleSheet("color: blue;");
        QTimer::singleShot(2000, [this](){
            ui->lbl_status->setStyleSheet("color: black;");
        });
    });
}

void MainWindow::on_btn_set_save_path_clicked()
{
    QString path = QFileDialog::getSaveFileName(
        this,
        "选择保存文件",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/scan_data.txt",
        "文本文件 (*.txt);;所有文件 (*.*)"
        );

    if (!path.isEmpty()) {
        saveFilePath = path;
        saveScanCounter = 1;  // 新路径重置计数器
        QMessageBox::information(this,
                                 "路径设置成功",
                                 QString("当前保存路径：\n%1\n\n扫描计数器已重置为1").arg(saveFilePath)
                                 );
    }
}
