#include "aesencrypt.h"
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QVector>
#include <QDebug>
#include "qaesencryption.h"
AESencrypt::AESencrypt()
{
    QString key = "0123456789012345";

    qDebug() << "key.size : " << key.toUtf8().size();

    QString string = "Hello World";

    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB, QAESEncryption::PKCS7);
    QByteArray enBA = encryption.encode(string.toUtf8(), key.toUtf8());
    QByteArray enBABase64 = enBA.toBase64();

    qDebug() << "string : " << string;
    qDebug() << "enBA : " << enBABase64;

    enBA = QByteArray::fromBase64(enBABase64);
    QByteArray deBA = encryption.decode(enBA, key.toUtf8());
    //移除填充
    qDebug() << "deBA : " << QAESEncryption::RemovePadding(deBA, QAESEncryption::PKCS7);

}

