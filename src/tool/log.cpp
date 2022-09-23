#include "log.h"

#include <QMessageLogger>
#include <qlogging.h>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

QFile *gFileLog = NULL;

char *msgHead[] = {"Debug   ", "Warning ", "Critical", "Fatal   ", "Info    "};

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");

    if (gFileLog) {
        QTextStream tWrite(gFileLog);

        QString msgText = "%1 | %6 | %2:%3, %4 | %5\n";
        msgText = msgText.arg(msgHead[type])
                      .arg(context.file)
                      .arg(context.line)
                      .arg(context.function)
                      .arg(localMsg.constData())
                      .arg(current_date_time);
        tWrite << msgText;
    }

    fprintf(stderr, "%s | %s | %s\n", msgHead[type], context.function, localMsg.constData());
}

void logSysInit(QString filePath)
{
    gFileLog = new QFile(filePath);
    if (!gFileLog->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        return;
    }
    //初始化自定义日志处理函数myMessageOutput
    qInstallMessageHandler(myMessageOutput);
}
