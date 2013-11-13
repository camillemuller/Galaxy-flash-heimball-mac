#include "md5hash.h"
#include <stdio.h>
#include <openssl/md5.h>

md5hash::md5hash(QString unFileLocation)
    :sonfileLocation(unFileLocation)
{
    QFile file(unFileLocation);

    if (file.open(QIODevice::ReadWrite)) {
    QByteArray fileData = file.readAll();
    QByteArray hashData = QCryptographicHash::hash(fileData, QCryptographicHash::Md5);

    qDebug() << hashData.toHex();
    }





}

QByteArray md5hash::hashFile()
{
    return 0;

}

