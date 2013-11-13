#ifndef MD5HASH_H
#define MD5HASH_H
#include <QString>
#include <QFile>
#include <QCryptographicHash>
#include <QByteArray>
#include <QDebug>

class md5hash
{
public:
    md5hash(QString);
    QByteArray hashFile();
private:
     QString sonfileLocation;
     QFile* sonFichier;
};

#endif // MD5HASH_H
