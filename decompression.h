#ifndef DECOMPRESSION_H
#define DECOMPRESSION_H

#include <QThread>
#include <QStringList>
#include <QDir>
#include <QCryptographicHash>
#include <QProcess>
#include <QDebug>

class Decompression : public QThread
{
    Q_OBJECT
public:
    explicit Decompression(QThread *parent = 0,QString unfileLocation = "");
    void run();
    QString uncompress();
    QString untar();

private:
    QString fileName;

    signals:
    void valueProgressBar(QString s);
    void fileNameChange( QString s);
    void resultReady( QString s);
    void erreurCompress( QString s);
    void terminated();

};

#endif // DECOMPRESSION_H
