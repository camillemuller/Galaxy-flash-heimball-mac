#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <map>
#include <decompression.h>
#include <md5hash.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString retourneKext();
    QString kiesFix(QString passwordRoot);
    QString Checkdevice();
    bool isLunch();
    bool cleanTempo();


private slots:

    void on_pushButton_2_clicked();
    void maj();


    void on_pb_filelocation_clicked();

    void on_pushButton_3_clicked();


    void changeFileName(QString);

    void majProgressBar(QString);

    void on_pushButton_4_clicked();


    void majCheckbox();

    void on_Bp_flash_clicked();

private:
    Ui::MainWindow *ui;
    QList<QCheckBox*>* Liste;
    QMap<int,QString>* relie;
    QList<QString>* lchoixCheckBox;
    Decompression* threadunpress;
    QString fileName;
    bool decompresssuccess = false;
protected:
     void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
