//
// Created by wengxiang on 2017/1/28.
//

#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QFile>
#include <QtCore/QVector>
#include <QtCore/QFileInfo>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QTextStream>

#include <iomanip>
#include <iostream>

#include "Configuration.h"

// open namespaces
// ---------------
using std::cout;
using std::cerr;

// static member definitions
// -------------------------
// directories
QFileInfo Configuration::input;
QDir Configuration::inputDirectory,
        Configuration::outputDirectory,
        Configuration::resourceDirectory;
bool Configuration::noCopyResources;

// sprite properties
bool Configuration::cropAlpha,
        Configuration::rotation;
int Configuration::spriteSize,
        Configuration::spritePadding,
        Configuration::extrude;

// texture properties
QImage::Format Configuration::pixelFormat;
QString Configuration::textureFormat,
        Configuration::pixelFormatString;
int Configuration::textureQuality,
        Configuration::maxSize;
bool Configuration::POT;

// other properties
bool Configuration::force;
bool Configuration::dataCompact;
QString Configuration::dataFormat;
QVector<QFileInfo> Configuration::excludeImages;
QVector<QFileInfo> Configuration::includeImages;
QVector<QFileInfo> Configuration::extrudeImages;

// static function definitions
void Configuration::parseCommandLine(const QCoreApplication &application)
{
    // command line options
    QCommandLineOption outputOption(
            QStringList() << "o" << "output",
            "The directory for exporting atlas.",
            "directory"
    );

    QCommandLineOption resourceOption(
            QStringList() << "d" << "resource",
            "Thr directory of files cannot be packed.",
            "directory"
    );
    QCommandLineOption maxSizeOption(
            QStringList() << "S" << "maxSize",
            "Max size of atlas.",
            "value",
            "2048"
    );
    QCommandLineOption spriteSizeOption(
            QStringList() << "s" << "spriteSize",
            "Max size of sprite.",
            "value",
            "512"
    );
    QCommandLineOption extrudeOption(
            QStringList() << "e" << "extrude",
            "Repeat the sprite's pixels at the borders.",
            "value",
            "1"
    );
    QCommandLineOption includeImagesOption(
            QStringList() << "i" << "include",
            "The picture in include list have to packed anyway, split by ','",
            "file1,file2..."
    );
    QCommandLineOption excludeImagesOption(
            QStringList() << "x" << "exclude",
            "The picture in exclude list will not be packed, split by ','.",
            "file1,file2..."
    );
    QCommandLineOption extrudeImagesOption(
            QStringList() << "X" << "extrude",
            "The picture in extrude list will be extruded, split by ','.",
            "file1,file2..."
    );
    QCommandLineOption spritePaddingOption(
            QStringList() << "p" << "spritePadding",
            "Tile padding is the space between sprites. Value adds transparent pixels between sprites to avoid artifacts from neighbor sprites. The transparent pixels are not added to the sprites. Default is 2.",
            "value",
            "1"
    );
    QCommandLineOption forceOption(
            QStringList() << "f" << "force",
            "If true, then publish even if picture never be modified.");
    QCommandLineOption POTOption(
            QStringList() << "2" << "POT",
            "If the atlas should be in units of power of 2 or irregular."
    );
    QCommandLineOption cropAlphaOption(
            QStringList() << "c" << "cropAlpha",
            "If source sprites should be cropped to their transparency bounds to pack them even tighter."
    );
    QCommandLineOption rotateOption(
            QStringList() << "r" << "rotation",
            "Allow sprite rotation to fit area better."
    );
    QCommandLineOption pixelFormatOption(
            QStringList() << "pixelFormat",
            "Mono | MonoLSB | Indexed8 | RGB32 | ARGB32 | ARGB32_Premultiplied | RGB16 | ARGB8565_Premultiplied | RGB666 | ARGB6666_Premultiplied | RGB555 | ARGB8555_Premultiplied | RGB888 | RGB444 | ARGB4444_Premultiplied | RGBX8888 | RGBA8888 | RGBA8888_Premultiplied | BGR30 | A2BGR30_Premultiplied | RGB30 | A2RGB30_Premultiplied | Alpha8 | Grayscale8, default ARGB32",
            "pixel format", "ARGB32"
    );
    QCommandLineOption textureQualityOption(
            QStringList() << "q" << "textureQuality",
            "The quality of texture images. The quality factor must be in the range 0 to 100 or -1. Specify 0 to obtain small compressed files, 100 for large uncompressed files, and -1 (the default) to use the default settings.",
            "quality", "-1"
    );
    QCommandLineOption textureFormatOption(
            QStringList() << "textureFormat",
            "The format of texture images. The value is BMP | JPG | JPEG | PNG | XBM | XPM, default PNG.",
            "texture format", "PNG"
    );
    QCommandLineOption initOption(
            QStringList() << "init",
            "Generate a configuration file."
    );
    QCommandLineOption dataCompactOption(
            QStringList() << "dataCompact",
            "If the output data in indented format or compact format."
    );
    QCommandLineOption dataFormatOption(
            QStringList() << "dataFormat",
            "Extension of data file.",
            "extension",
            "atlas"
    );

    // parse options
    QCommandLineParser commandLineParser;

    commandLineParser.addOption(outputOption);
    commandLineParser.addOption(resourceOption);
    commandLineParser.addOption(maxSizeOption);
    commandLineParser.addOption(spriteSizeOption);
    commandLineParser.addOption(extrudeOption);
    commandLineParser.addOption(includeImagesOption);
    commandLineParser.addOption(excludeImagesOption);
    commandLineParser.addOption(spritePaddingOption);
    commandLineParser.addOption(forceOption);
    commandLineParser.addOption(POTOption);
    commandLineParser.addOption(cropAlphaOption);
    commandLineParser.addOption(rotateOption);
    commandLineParser.addOption(pixelFormatOption);
    commandLineParser.addOption(textureQualityOption);
    commandLineParser.addOption(textureFormatOption);
    commandLineParser.addOption(initOption);
    commandLineParser.addOption(dataCompactOption);
    commandLineParser.addOption(dataFormatOption);

    commandLineParser.addHelpOption();
    commandLineParser.addVersionOption();
    commandLineParser.addPositionalArgument("input",
                                            QCoreApplication::translate("main", "input directory or config file."));

    commandLineParser.process(application);

    // process user input
    ProcessInitDirective(commandLineParser.isSet(initOption));

    if (commandLineParser.positionalArguments().isEmpty())
    {
        cerr << commandLineParser.helpText().toStdString() << std::endl;
        exit(EXIT_SUCCESS);
    }

    input = QFileInfo(commandLineParser.positionalArguments().at(0));
    input.makeAbsolute();

    if (input.exists())
    {
        if (input.isDir())
        {
            inputDirectory = QDir(input.filePath());
            extrude = commandLineParser.value(extrudeOption).toInt();
            maxSize = commandLineParser.value(maxSizeOption).toInt();
            spriteSize = commandLineParser.value(spriteSizeOption).toInt();
            spritePadding = commandLineParser.value(spritePaddingOption).toInt();
            textureQuality = commandLineParser.value(textureQualityOption).toInt();
            textureFormat = commandLineParser.value(textureFormatOption);
            POT = commandLineParser.isSet(POTOption);
            cropAlpha = commandLineParser.isSet(cropAlphaOption);
            rotation = commandLineParser.isSet(rotateOption);
            force = commandLineParser.isSet(forceOption);
            dataCompact = commandLineParser.isSet(dataCompactOption);
            dataFormat = commandLineParser.value(dataFormatOption);

            SetupPixelFormat(commandLineParser.value(pixelFormatOption));
            SetUpFileList(commandLineParser.value(excludeImagesOption), excludeImages);
            SetUpFileList(commandLineParser.value(includeImagesOption), includeImages);
            SetUpFileList(commandLineParser.value(extrudeImagesOption), extrudeImages);
            SetupOutputDirectory(commandLineParser.value(outputOption));
            SetupResourceDirectory(commandLineParser.value(resourceOption));
        } else
        {
            ReadConfigurationFile(input.filePath());
            if (!inputDirectory.exists())
            {
                cout << inputDirectory.path().toStdString() << "\" not found.\n";
                exit(EXIT_FAILURE);
            }

            // outputOption & resourceOption can be overwrite by cl args.
            if (commandLineParser.isSet(outputOption))
            {
                SetupOutputDirectory(commandLineParser.value(outputOption));
            }
            if (commandLineParser.isSet(resourceOption))
            {
                SetupResourceDirectory(commandLineParser.value(resourceOption));
            }
        }

        spriteSize = std::min(spriteSize, maxSize);

        PrintConfiguration();
    } else
    {
        cout << input.filePath().toStdString() << "\" not found.\n";
        exit(EXIT_FAILURE);
    }
}

void Configuration::ReadConfigurationFile(QString configFilePath)
{
    QFile configFile(configFilePath);
    configFile.open(QFile::ReadOnly | QFile::Text);
    if (!configFile.isOpen())
    {
        cerr << "Cannot open " << configFilePath.toStdString() << " for reading" << std::endl;
        exit(EXIT_FAILURE);
    }

    // read content
    QTextStream in_stream(&configFile);
    in_stream.setCodec("utf-8");

    // parse content
    auto content = in_stream.readAll().toUtf8();
    QJsonParseError error;
    QJsonDocument config_document = QJsonDocument::fromJson(content, &error);

    // process parser error
    if (error.error != QJsonParseError::ParseError::NoError)
    {
        cerr << "JSON Parser Error: " << error.errorString().toStdString() << std::endl;
        exit(EXIT_FAILURE);
    }

    // retrieve data
    QJsonObject rootObject = config_document.object();
    QJsonObject atlasObject = rootObject.value("atlas").toObject();
    QJsonObject spriteObject = rootObject.value("sprite").toObject();
    QJsonObject dataObject = rootObject.value("data").toObject();
    QJsonArray excludeArray = rootObject.value("excludeList").toArray();
    QJsonArray includeArray = rootObject.value("includeList").toArray();
    POT = atlasObject.value("POT").toBool();
    textureQuality = atlasObject.value("quality").toInt();
    maxSize = atlasObject.value("size").toInt();
    textureFormat = atlasObject.value("textureFormat").toString();
    force = rootObject.value("force").toBool();
    inputDirectory = rootObject.value("inputDir").toString();
    outputDirectory = rootObject.value("outputDir").toString();
    resourceDirectory = rootObject.value("resDir").toString();
    cropAlpha = spriteObject.value("cropAlpha").toBool();
    extrude = spriteObject.value("extrude").toInt();
    spritePadding = spriteObject.value("padding").toInt();
    rotation = spriteObject.value("rotation").toBool();
    spriteSize = spriteObject.value("size").toInt();

    dataCompact = dataObject.value("compact").toBool();
    dataFormat = dataObject.value("format").toString();

    inputDirectory.makeAbsolute();

    SetUpFileList(rootObject.value("excludeList").toArray(), excludeImages);
    SetUpFileList(rootObject.value("includeList").toArray(), includeImages);
    SetUpFileList(rootObject.value("extrudeList").toArray(), extrudeImages);
    SetupPixelFormat(atlasObject.value("pixelFormat").toString());
    SetupOutputDirectory(rootObject.value("outputDir").toString());
    SetupResourceDirectory(rootObject.value("resDir").toString());
}

void Configuration::SetupOutputDirectory(const QString &value)
{
    if (value.isEmpty())
        outputDirectory = inputDirectory.filePath("../default_output");
    else
        outputDirectory = QDir(value);

    outputDirectory.mkpath(".");
    outputDirectory.makeAbsolute();
}

void Configuration::SetupResourceDirectory(const QString &value)
{
    noCopyResources = value.isEmpty();

    if(!noCopyResources)
    {
        resourceDirectory = QDir(value);
        resourceDirectory.mkpath(".");
        resourceDirectory.makeAbsolute();
    }
}

void ::Configuration::SetUpFileList(const QString &value, QVector<QFileInfo> &container)
{
    if (value.isEmpty())
    {
        return;
    }

    QStringList exclude_files_in_string = value.split(',');
    for (int i = 0; i < exclude_files_in_string.size(); ++i)
    {
        QString file_path(exclude_files_in_string.at(i));
        QFileInfo file_info(file_path);
        if (file_info.isRelative())
        {
            file_info.setFile(inputDirectory.filePath(file_path));
        }

        container.push_back(file_info);
    }
    cout << "\n";
}

void Configuration::SetUpFileList(const QJsonArray &list, QVector<QFileInfo> &container)
{
    for (auto pos = list.constBegin(); pos != list.constEnd(); ++pos)
    {
        QString path((*pos).toString());
        QFileInfo fileInfo(path);
        if (fileInfo.isRelative())
            fileInfo.setFile(inputDirectory.filePath(path));
        container.push_back(fileInfo);
    }
}

bool ::Configuration::IsExclude(const QFileInfo &fileInfo)
{
    auto find_index = std::find(excludeImages.cbegin(), excludeImages.cend(), fileInfo);
    return find_index != excludeImages.cend();
}

bool Configuration::IsInclude(const QFileInfo &fileInfo)
{
    auto find_index = std::find(includeImages.cbegin(), includeImages.cend(), fileInfo);
    return find_index != includeImages.cend();
}

bool ::Configuration::IsExtrude(const QFileInfo &fileInfo)
{
    auto find_index = std::find(extrudeImages.cbegin(), extrudeImages.cend(), fileInfo);
    return find_index != extrudeImages.cend();
}

void Configuration::SetupPixelFormat(QString pixelFormatString)
{
    Configuration::pixelFormatString = pixelFormatString;
    if (pixelFormatString == "Mono")
        pixelFormat = QImage::Format::Format_Mono;
    else if (pixelFormatString == "MonoLSB")
        pixelFormat = QImage::Format::Format_MonoLSB;
    else if (pixelFormatString == "Indexed8")
        pixelFormat = QImage::Format::Format_Indexed8;
    else if (pixelFormatString == "RGB32")
        pixelFormat = QImage::Format::Format_RGB32;
    else if (pixelFormatString == "ARGB32")
        pixelFormat = QImage::Format::Format_ARGB32;
    else if (pixelFormatString == "ARGB32_Premultiplied")
        pixelFormat = QImage::Format::Format_ARGB32_Premultiplied;
    else if (pixelFormatString == "RGB16")
        pixelFormat = QImage::Format::Format_RGB16;
    else if (pixelFormatString == "ARGB8565_Premultiplied")
        pixelFormat = QImage::Format::Format_ARGB8565_Premultiplied;
    else if (pixelFormatString == "RGB666")
        pixelFormat = QImage::Format::Format_RGB666;
    else if (pixelFormatString == "ARGB6666_Premultiplied")
        pixelFormat = QImage::Format::Format_ARGB6666_Premultiplied;
    else if (pixelFormatString == "RGB555")
        pixelFormat = QImage::Format::Format_RGB555;
    else if (pixelFormatString == "ARGB8555_Premultiplied")
        pixelFormat = QImage::Format::Format_ARGB8555_Premultiplied;
    else if (pixelFormatString == "RGB888")
        pixelFormat = QImage::Format::Format_RGB888;
    else if (pixelFormatString == "RGB444")
        pixelFormat = QImage::Format::Format_RGB444;
    else if (pixelFormatString == "ARGB4444_Premultiplied")
        pixelFormat = QImage::Format::Format_ARGB4444_Premultiplied;
    else if (pixelFormatString == "RGBX8888")
        pixelFormat = QImage::Format::Format_RGBX8888;
    else if (pixelFormatString == "RGBA8888")
        pixelFormat = QImage::Format::Format_RGBA8888;
    else if (pixelFormatString == "RGBA8888_Premultiplied")
        pixelFormat = QImage::Format::Format_RGBA8888_Premultiplied;
    else if (pixelFormatString == "BGR30")
        pixelFormat = QImage::Format::Format_BGR30;
    else if (pixelFormatString == "A2BGR30_Premultiplied")
        pixelFormat = QImage::Format::Format_A2BGR30_Premultiplied;
    else if (pixelFormatString == "RGB30")
        pixelFormat = QImage::Format::Format_RGB30;
    else if (pixelFormatString == "A2RGB30_Premultiplied")
        pixelFormat = QImage::Format::Format_A2RGB30_Premultiplied;
    else if (pixelFormatString == "Alpha8")
        pixelFormat = QImage::Format::Format_Alpha8;
    else if (pixelFormatString == "Grayscale8")
        pixelFormat = QImage::Format::Format_Grayscale8;
    else
        std::cerr << "NO PIXEL FORMAT " << pixelFormatString.toStdString() << '\n';
}

QString Configuration::GetDefaultConfigContent()
{
    QJsonDocument document;
    QJsonObject rootObject;

    rootObject.insert("inputDir", "required");
    rootObject.insert("outputDir", "");
    rootObject.insert("resDir", "");
    rootObject.insert("force", true);
    rootObject.insert("includeList", QJsonArray());
    rootObject.insert("excludeList", QJsonArray());
    rootObject.insert("extrudeList", QJsonArray());

    QJsonObject atlasObject;
    atlasObject.insert("size", 2048);
    atlasObject.insert("POT", false);
    atlasObject.insert("pixelFormat", "ARGB32");
    atlasObject.insert("quality", -1);
    atlasObject.insert("textureFormat", "PNG");
    rootObject.insert("atlas", atlasObject);

    QJsonObject spriteObject;
    spriteObject.insert("padding", 1);
    spriteObject.insert("rotation", false);
    spriteObject.insert("size", 512);
    spriteObject.insert("cropAlpha", true);
    spriteObject.insert("extrude", 1);
    rootObject.insert("sprite", spriteObject);

    QJsonObject dataObject;
    dataObject.insert("format", "atlas");
    dataObject.insert("compact", false);
    rootObject.insert("data", dataObject);

    document.setObject(rootObject);
    return document.toJson();
}

void Configuration::
ProcessInitDirective(bool is_init)
{
    if (!is_init)
        return;

    QFile out_file("atlasConfig");
    if (!out_file.open(QFile::WriteOnly | QFile::Text))
    {
        cerr << "Can not open file 'atlasConfig' for writing." << std::endl;
        exit(EXIT_FAILURE);
    }

    QTextStream out_stream(&out_file);
    out_stream << GetDefaultConfigContent();
    out_stream.flush();
    out_file.close();

    cout << "SAVE " << QFileInfo(out_file).absoluteFilePath().toStdString() << std::endl;
    exit(EXIT_SUCCESS);
}

void Configuration::PrintConfiguration()
{
    cout
            << std::setw(25) << std::left << "input directory" << inputDirectory.path().toStdString() << "\n"
            << std::setw(25) << std::left << "output directory" << outputDirectory.path().toStdString() << "\n"
            << std::setw(25) << std::left << "resource directory" << (noCopyResources ? "" : resourceDirectory.path().toStdString()) << "\n"
            << std::setw(25) << std::left << "crop alpha" << std::boolalpha << cropAlpha << "\n"
            << std::setw(25) << std::left << "allow rotation" << rotation << "\n"
            << std::setw(25) << std::left << "max sprite size" << spriteSize << "\n"
            << std::setw(25) << std::left << "max texture size" << maxSize << "\n"
            << std::setw(25) << std::left << "sprite padding" << spritePadding << "\n"
            << std::setw(25) << std::left << "extrude" << extrude << "\n"
            << std::setw(25) << std::left << "pixel format" << pixelFormatString.toStdString() << "\n"
            << std::setw(25) << std::left << "texture format" << textureFormat.toStdString() << "\n"
            << std::setw(25) << std::left << "texture quality" << textureQuality << "\n"
            << std::setw(25) << std::left << "data compact" << std::boolalpha << dataCompact << "\n"
            << std::setw(25) << std::left << "data format" << dataFormat.toStdString() << "\n"
            << std::setw(25) << std::left << "POT" << POT << "\n\n"
            << "EXCLUDE IMAGES\n";
    for (const QFileInfo &fileInfo : excludeImages)
        cout << "    " << fileInfo.absoluteFilePath().toStdString() << "\n";
    cout
            << "\n"
            << "INCLUDE IMAGES\n";
    for (const QFileInfo &fileInfo : includeImages)
        cout << "    " << fileInfo.absoluteFilePath().toStdString() << "\n";
    cout
            << "\n"
            << "EXTRUDE IMAGES\n";
    for (const QFileInfo &fileInfo : extrudeImages)
        cout << "    " << fileInfo.absoluteFilePath().toStdString() << "\n";
    cout << "\n";
}
