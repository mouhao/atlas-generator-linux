//
// Created by wengxiang on 2017/2/4.
//

#ifndef ATLASGENERATOR_UTILS_H
#define ATLASGENERATOR_UTILS_H

#ifdef WIN32
#include <io.h>
#endif

namespace file_utils
{
    QString GetRelativeToInputDirectoryPath(QString path);
    bool Copy(const QString &from, const QString &to);
    void CopyToResourceDirectory(const QString &path);
    void mkdirs(std::string path);
}

namespace math_utils
{
    int CeilPOT(int value);
}


#endif //ATLASGENERATOR_UTILS_H
