//
// Created by wengxiang on 2017/1/28.
//

#ifndef ATLASGENERATOR_CLI_ARGS_H
#define ATLASGENERATOR_CLI_ARGS_H

#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QCommandLineParser>
#include <QtGui/QImage>

class QCoreApplication;

class Configuration
{
public:
    // static member declarations
    // --------------------------
    // directories
    static QFileInfo input;
    static QDir inputDirectory,
                outputDirectory,
                resourceDirectory;

    static bool noCopyResources;

    // sprite properties
    static bool cropAlpha,
                rotation;
    static int spriteSize,
               spritePadding,
               extrude;

    // texture properties
    static QImage::Format pixelFormat;
    static QString        textureFormat;
    static int textureQuality,
               maxSize;
    static bool POT;

    // other properties
    static bool force;
    static bool dataCompact;
    static QString dataFormat;
    static QVector<QFileInfo> excludeImages;
    static QVector<QFileInfo> includeImages;
    static QVector<QFileInfo> extrudeImages;

    // static function declarations
    static void parseCommandLine(const QCoreApplication &application);
    static bool IsExclude(const QFileInfo &fileInfo);
    static bool IsInclude(const QFileInfo &fileInfo);
    static bool IsExtrude(const QFileInfo &fileInfo);

private:
    static void ReadConfigurationFile(QString configFilePath);
    static QString GetDefaultConfigContent();
    static void SetupOutputDirectory    (const QString &value);
    static void SetupResourceDirectory  (const QString &value);
    static void SetUpFileList           (const QString &value, QVector<QFileInfo>& container);
    static void SetUpFileList           (const QJsonArray& list, QVector<QFileInfo>& container);
    static void SetupPixelFormat        (QString pixelFormatString);
    static void ProcessInitDirective    (bool is_init);
    static void PrintConfiguration();

    static QString pixelFormatString;
};

#endif //ATLASGENERATOR_CLI_ARGS_H
