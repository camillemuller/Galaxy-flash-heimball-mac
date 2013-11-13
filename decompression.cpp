#include "decompression.h"

Decompression::Decompression(QThread *parent,QString unfileLocation) :
    QThread(parent)
{
    this->fileName = unfileLocation;
}

void Decompression::run()
{

    if(this->fileName.endsWith(".zip",Qt::CaseInsensitive) )
    {


        emit valueProgressBar("Uncompress :"+fileName);
        uncompress();
        emit valueProgressBar("End of uncompression :"+fileName);


        QStringList list(QDir(QDir::homePath()+"/tempo/" ).entryList());
        bool bug = true;

        foreach(QString string , list)
        {
            if(string.endsWith("tar.MD5",Qt::CaseInsensitive))
            {
                fileName = QDir::homePath()+"/tempo/"+string;
                bug = false;
            }

        }

        if(bug)
        {

            emit erreurCompress("Erreur");
            emit valueProgressBar("ERROR FILE DOESNT CONTAINTS ROM");

            return ;
        }

    }

    emit valueProgressBar("UnTar of :"+fileName);
    untar();
    emit valueProgressBar("End of decompression");


    emit fileNameChange(this->fileName);
    emit terminated();
}

QString Decompression::uncompress()
{


    QDir *createFolder = new QDir(QDir::homePath());
    createFolder->mkdir("tempo");


    QProcess OProcess;
    QString Command;    //Contains the command to be executed
    QStringList args;   //Contains arguments of the command





    Command = "unzip"; // Starting a shell
    args<<fileName<<"-d"<<createFolder->absolutePath()+"/tempo/";   // Shell command to know kext in use


    OProcess.start(Command,args,QIODevice::ReadOnly); //Starts execution of command
    OProcess.waitForFinished();                       //Waits for execution to complete

    QString temp =  OProcess.readAllStandardOutput(); // Return QString of kext in use



    OProcess.close();


    return temp;
}

QString Decompression::untar()
{

    qDebug() << "nom du fichier a untar" << fileName;


    QProcess OProcess;
    QString Command;    //Contains the command to be executed
    QStringList args;   //Contains arguments of the command
    std::string folderNameDir;

    folderNameDir = fileName.toStdString();

    folderNameDir.erase(folderNameDir.end()-8,folderNameDir.end());

    qDebug() << "nom du dossier untar" << folderNameDir.c_str();

    Command = "tar"; // Starting a shell
    args<<"xvf"<<fileName<<"-C"<<QDir::homePath()+"/tempo/";   // Shell command to know kext in use


    OProcess.start(Command,args,QIODevice::ReadWrite); //Starts execution of command
    OProcess.waitForFinished();     //Waits for execution to complete

    QString temp =  OProcess.readAllStandardOutput(); // Return QString of kext in use



    OProcess.close();


    return temp;
}
