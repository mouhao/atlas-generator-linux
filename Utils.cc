//
// Created by wengxiang on 2017/2/4.
//
#include <QtCore/QString>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QRegularExpression>
#include <QTextCodec>

#include <fstream>
#include <iostream>
#include <regex>

#include "Utils.h"
#include "Configuration.h"

using std::string;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::cerr;
using std::endl;

bool file_utils::Copy(const QString &from, const QString &to)
{
    if(QFileInfo(from).isDir())
    {
        mkdirs(to.toStdString());
        QDir fromDir(from), toDir(to);

        QFileInfoList fileList = fromDir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
        for(const QFileInfo &file : fileList)
        {
            QString relative = fromDir.relativeFilePath(file.filePath());
            Copy(file.filePath(), toDir.filePath(relative));
        }
    }
    else
    {
        std::string from_string, to_string;
#ifdef WIN32
        QTextCodec *codec = QTextCodec::codecForName("gbk");
        from_string = codec->fromUnicode(from).toStdString();
        to_string = codec->fromUnicode(to).toStdString();
#else
        from_string = from.toStdString();
        to_string = to.toStdString();
#endif

        char buffer[1024];
        FILE *in, *out;
        in = fopen(from_string.c_str(), "rb");
        out = fopen(to_string.c_str(), "wb");
        if(in && out)
        {
            int len;
            while((len = fread(buffer, sizeof(char), 1024, in)) > 0)
            {
                fwrite(buffer, sizeof(char), len, out);
            }
            fclose(in);
            fclose(out);
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}

void ::file_utils::CopyToResourceDirectory(const QString &path)
{
    if(Configuration::noCopyResources) return;

    QString relative_path = Configuration::inputDirectory.relativeFilePath(path);

    Configuration::resourceDirectory.mkpath(QFileInfo(relative_path).dir().path());

    QString to_path = Configuration::resourceDirectory.filePath(relative_path);
    Copy(path, to_path);
}

QString file_utils::GetRelativeToInputDirectoryPath(QString path)
{
    return Configuration::inputDirectory.relativeFilePath(path);
}

void file_utils::mkdirs(std::string path)
{
    QFile f(path.c_str());
    if(f.exists())
        return;

#ifdef WIN32
    std::regex pattern("/");
    path = std::regex_replace(path, pattern, "\\");

    QTextCodec *code = QTextCodec::codecForName("gbk");
    path = code->fromUnicode(path.c_str()).toStdString();

    std::string cmd = "mkdir ";
    cmd += path;
    std::cout << cmd << std::endl;
    system(cmd.c_str());
#else
    std::string cmd = "mkdir -p ";
    cmd += path;
    system(cmd.c_str());
#endif
}

int ::math_utils::CeilPOT(int value)
{
    int val = 2;
    while(val < value) val *= 2;
    return val;
}
