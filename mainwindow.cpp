#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <iostream>
#include <QMessageBox>
#include <QProcess>
#include <QObject>
#include <QApplication>
#include <QTimer>
#include <QPushButton>
#include <QtDebug>
#include <QFileDialog>
#include <QCryptographicHash>
#include <stdio.h>
#include <string.h>
#include <assert.h>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    //ui->pushButton_2->setEnabled(false);
    // ui->lineEdit->setEnabled(false);


    QTimer *timer = new QTimer();
    timer->connect(timer, SIGNAL(timeout()),this, SLOT(maj()));
    timer->start(100);



     Liste = new QList<QCheckBox*>();

    Liste->append(ui->ABOOT);
    Liste->append(ui->APNHLOS);
    Liste->append(ui->BOOT);
    Liste->append(ui->CACHE);
    Liste->append(ui->HIDDEN);
    Liste->append(ui->MDM);
    Liste->append(ui->TZ);
    Liste->append(ui->SYSTEM);
    Liste->append(ui->SBL3);
    Liste->append(ui->SBL2);
    Liste->append(ui->RPM);
    Liste->append(ui->RECOVERY);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::maj()
{
    if( retourneKext().contains("Samsung",Qt::CaseInsensitive)  )
    {
        ui->checkBox_2->setChecked(true);
        ui->pushButton_2->setEnabled(true);
        ui->lineEdit->setEnabled(true);

    }
    else
    {

        ui->checkBox_2->setChecked(false);
        ui->pushButton_2->setEnabled(false);
        ui->lineEdit->setEnabled(false);

    }


    if(isLunch() )
    {
        ui->checkBox->setChecked(true);
        ui->Bp_flash->setEnabled(false);
    }
    else
    {
        ui->checkBox->setChecked(false);
    }


    if( Checkdevice().contains("Device",Qt::CaseInsensitive)  )
    {

        ui->checkBox_3->setChecked(1);
        ui->Bp_flash->setEnabled(true);

    }
    else
    {
        ui->checkBox_3->setChecked(false);
        ui->Bp_flash->setEnabled(false);

    }


}

bool MainWindow::isLunch()
{
    QProcess OProcess;
    QString Command;    //Contains the command to be executed
    QStringList args;   //Contains arguments of the command


    Command = "sh"; // Starting a shell
    args<<"-c"<<"ps -ef | grep Kies | wc -l";   // Shell command to know kext in use



    OProcess.start(Command,args,QIODevice::ReadOnly); //Starts execution of command
    OProcess.waitForFinished();                       //Waits for execution to complete



    QString temp =  OProcess.readAllStandardOutput(); // Return QString of kext in use

    OProcess.close();


    if( temp.toInt() >= 3 )
    {

        return true;
    }
    else
    {
        return false;
    }

}

QString MainWindow::Checkdevice()
{
    QProcess OProcess;
    QString Command;    //Contains the command to be executed
    QStringList args;   //Contains arguments of the command


    Command = "sh"; // Starting a shell
    args<<"-c"<<"heimdall detect ";   // Shell command to know kext in use



    OProcess.start(Command,args,QIODevice::ReadOnly); //Starts execution of command
    OProcess.waitForFinished();                       //Waits for execution to complete



    QString temp =  OProcess.readAllStandardOutput(); // Return QString of kext in use

    OProcess.close();


    return temp;

}



/*
 *Return Array of kext in current use
 *Retourne la liste des kext en cours d'utilisation
 */
QString MainWindow::retourneKext()
{
    QProcess OProcess;
    QString Command;    //Contains the command to be executed
    QStringList args;   //Contains arguments of the command


    Command = "sh"; // Starting a shell
    args<<"-c"<<"kextstat";   // Shell command to know kext in use



    OProcess.start(Command,args,QIODevice::ReadOnly); //Starts execution of command
    OProcess.waitForFinished();                       //Waits for execution to complete



    QString temp =  OProcess.readAllStandardOutput(); // Return QString of kext in use

    OProcess.close();


    return temp;
}

/*
 *Desactivate KIES KEXT
 *
 */
QString MainWindow::kiesFix(QString passwordRoot)
{

    // Kill Kies process
    QProcess OProcess;
    QString Command;    //Contains the command to be executed
    QStringList args;   //Contains arguments of the command


    Command = "sh"; // Starting a shell

    /*
         *We use a BADASS trick to be SUDOERS
         *
         *  Args  : -S get passwd from STDIN
         */

    QString concat = "echo " + passwordRoot + " | sudo -S kextunload -b com.devguru.driver.SamsungComposite | sudo  kextunload -b com.devguru.driver.SamsungACMData | sudo  kextunload -b  com.devguru.driver.SamsungACMControl | sudo  kextunload -b com.devguru.driver.SamsungMTP ";

    args<<"-c"<< concat ;


    OProcess.start(Command,args,QIODevice::ReadOnly); //Starts execution of command
    OProcess.waitForFinished();                       //Waits for execution to complete


    QString temp =  OProcess.readAllStandardOutput(); // Return QString of kext in use
    OProcess.close();
    return temp;
}


void MainWindow::on_pushButton_2_clicked() // Slop on click of Debug Kext
{
    this->kiesFix(ui->lineEdit->text()); // Send to the function Root password
}


bool MainWindow::cleanTempo()
{
    QDir* leDossier = new QDir(QDir::homePath()+"/tempo/");
    if(leDossier->exists())
    {
        if( leDossier->removeRecursively())
        return true;
        else
        return false;
    }
   else
    {
        return true;
    }




}



void MainWindow::on_pb_filelocation_clicked()
{
    /*
     * Nettoyage du dossier temporaire
     */
    this->cleanTempo();

    /*
     *Demande la localisation du fichier a l'utilisateur
     */
    QFileDialog* liensFichier = new QFileDialog(this);
     this->fileName= liensFichier->getOpenFileName(this,tr("Open Image"), "/home/jana", tr("Image Files (*.zip *.MD5)"));
     ui->lineEdit_2->setText(fileName);

    /*
     *Thread de décompression
     */
    this->threadunpress = new Decompression(0,this->fileName);
     connect(threadunpress,SIGNAL(fileNameChange(QString)),this,SLOT(changeFileName(QString)));
    connect(threadunpress,SIGNAL(valueProgressBar(QString)),this,SLOT(majProgressBar(QString)));
    connect(threadunpress,SIGNAL(terminated()),this,SLOT(majCheckbox()));
    this->threadunpress->start();

    this->decompresssuccess = true;

}


void MainWindow::on_pushButton_3_clicked()
{
    close();
}




/**
 * @brief MainWindow::changeFileName Change le nom du fichier et effectue du hash MD5
 * @param s
 */
void MainWindow::changeFileName(QString s)
{
    this->fileName = s;

    qDebug() << "file location" << fileName;


    if(this->threadunpress->isFinished())
    {
        md5hash * unHash = new md5hash(this->fileName);
        qDebug() << "Thread finis";
    }


}

/**
 * @brief MainWindow::majProgressBar Mise a jour de la Progress BAR pour le thread uncompress
 * @param s
 */
void MainWindow::majProgressBar(QString s)
{

    ui->label_6->setText(s);
    if(ui->progressBar->value() == 0)
    ui->progressBar->setValue(25);
    else
    ui->progressBar->setValue(ui->progressBar->value()+25);


}

/**
 * @brief MainWindow::on_pushButton_4_clicked EXPERT MODE
 */
void MainWindow::on_pushButton_4_clicked()
{
    QList<QCheckBox*>::iterator i;
    static bool clicked = false;

    if(!clicked)
    {
        for (i = Liste->begin(); i != Liste->end(); ++i)
        {
            QCheckBox*  laBox =  *i;
            laBox->setEnabled(true);
            clicked = true;
        }


    }else
    {
        for (i = Liste->begin(); i != Liste->end(); ++i)
        {
            QCheckBox*  laBox =  *i;
            laBox->setEnabled(false);
            clicked = false;
        }

    }




}

/**
 * @brief MainWindow::majCheckbox Mise à jour des checkbox en fonction du fichier
 */
void MainWindow::majCheckbox()
{
    QStringList list(QDir(QDir::homePath()+"/tempo/" ).entryList());



    QList<QCheckBox*>::iterator i;


    relie = new   QMap<int,QString>();

    int aa=0;


    for (i = Liste->begin(); i != Liste->end(); ++i)


        foreach(QString string , list)
        {
            QCheckBox*  laBox =  *i;


            if( string.contains(laBox->text(),Qt::CaseInsensitive)
                    || (string == "NON-HLOS.bin" && laBox->text() =="APNHLOS")
                    || (string == "modem.bin"&& laBox->text() =="MDM")
              )
            {

                relie->insert(aa,string);
                aa++;
                laBox->setChecked(true);
            }

        }

}


/**
 * @brief MainWindow::on_Bp_flash_clicked Slot après que le bp flash ai étés cliquer
 */
void MainWindow::on_Bp_flash_clicked()
{
    QList<QCheckBox*>::iterator i;

    lchoixCheckBox = new QList<QString>();

    int aa=0;

    for (i = Liste->begin(); i != Liste->end(); ++i)
    {

        QCheckBox*  laBox =  *i;

        if(laBox->isChecked())
        {

            qDebug() << "Le texte" << relie->find(aa).value();

            lchoixCheckBox->append( relie->find(aa).value() );


        }
        aa++;

    }


}



/**
 * @brief MainWindow::closeEvent nettoyage du dossier après la fermeture
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent *event) {
     // do some data saves or something else
    this->cleanTempo();
}
