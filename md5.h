#ifndef MD5_H
#define MD5_H

#include <QString>

class md5
{
public:
    md5();
    QString md5findhash(QString path);
    int get_md5_line(FILE *fp, unsigned char *digest, char *file);
    int hex_digit(int c);


};

#endif // MD5_H
