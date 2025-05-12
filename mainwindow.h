#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QSerialPort"
#include "QSerialPortInfo"
#include <QLineEdit>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void processLine(const QString &line);
    QColor numberToColor(uint32_t number) const;

private slots:
    void on_btn_serialport_refresh_clicked();  //声明按键“刷新串口”点击函数
    void on_btn_serialport_onoff_clicked();    //声明按键“打开串口”点击函数
    void on_btn_enter_tx_clicked();       //声明按键“发送”点击函数
    void on_btn_clear_tx_clicked(); //声明按键“清除发送”点击函数
    void on_btn_clear_rx_clicked(); //声明按键“清除接收”点击函数
    void on_btn_stop_rx_clicked();    // 停止串口接收
    void RX_FUN();                      //串口接收



    void on_btn_save_data_clicked();

    void on_btn_set_save_path_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *COM;
    QString serialBuffer;
    QLineEdit *dataDisplays[4][4];
    bool isReceiving;

    // save
    int saveScanCounter;
    QString saveFilePath;
};
#endif // MAINWINDOW_H
