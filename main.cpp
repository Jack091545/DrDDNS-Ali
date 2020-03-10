#include <QCoreApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <iostream>
#include "Command.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Command cmd;
    if(cmd.exec() < 0)
        return -1;
    return a.exec();
}
