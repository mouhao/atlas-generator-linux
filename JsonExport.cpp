//
// Created by survivor on 17-6-9.
//

#include <QtCore/QJsonObject>
#include <iostream>
#include <QtCore/QTextStream>

#include "AtlasPacker.h"
#include "Utils.h"
#include "JsonExport.h"
#include "Configuration.h"

JsonExport::JsonExport():
    json_document(),
    frames_object(),
    meta_object()
{

}

void JsonExport::Clear()
{
    while(!frames_object.isEmpty())
        frames_object.erase(frames_object.begin());
    while(!meta_object.isEmpty())
        meta_object.erase(meta_object.begin());
}

void JsonExport::AddImageDescription(const ImageInfo &image_info)
{
    QJsonObject frame_object;
    frame_object.insert("x", image_info.frame.x);
    frame_object.insert("y", image_info.frame.y);
    frame_object.insert("w", image_info.frame.width);
    frame_object.insert("h", image_info.frame.height);
    // idx居然在frame里？？
    frame_object.insert("idx", image_info.image_index);

    QJsonObject sprite_source_size_object;
    sprite_source_size_object.insert("x", image_info.sprite_source_size.x);
    sprite_source_size_object.insert("y", image_info.sprite_source_size.y);
    sprite_source_size_object.insert("w", image_info.sprite_source_size.width);
    sprite_source_size_object.insert("h", image_info.sprite_source_size.height);

    QJsonObject source_size_object;
    source_size_object.insert("w", image_info.source_size.width);
    source_size_object.insert("h", image_info.source_size.height);

    QJsonObject sprite_object;
    sprite_object.insert("frame", frame_object);
    sprite_object.insert("rotated", image_info.rotated);
    sprite_object.insert("trimmed", image_info.trimmed);
    sprite_object.insert("spriteSourceSize", sprite_source_size_object);
    sprite_object.insert("sourceSize", source_size_object);

    frames_object.insert(QFileInfo(image_info.filename).fileName(), sprite_object);
}

void JsonExport::SetMetaImages(QString images)
{
    meta_object.insert("image", images);
}

void JsonExport::SetMetaPrefix(QString prefix) {
    meta_object.insert("prefix", prefix);
}

void JsonExport::SetMetaFormat(QString format)
{
    meta_object.insert("format", format);
}

void JsonExport::SetMetaSize(int width, int height)
{
    QJsonObject size_object;
    size_object.insert("w", width);
    size_object.insert("h", height);
    meta_object.insert("size", size_object);
}

void JsonExport::Export(const QString &file_path)
{
    if(json_document.isEmpty())
    {
        QJsonObject root_object;
        root_object.insert("frames", frames_object);
        root_object.insert("meta", meta_object);
        json_document.setObject(root_object);
    }

    // generate file.
    QFile out_file(file_path);
    out_file.open(QFile::WriteOnly | QFile::Text);

    if(!out_file.isOpen())
    {
        std::cerr << "Can not open " << file_path.toStdString() << '\n';
        return;
    }

    QTextStream out_stream(&out_file);
    out_stream.setCodec("utf-8");
    out_stream << json_document.toJson(Configuration::dataCompact ? QJsonDocument::Compact : QJsonDocument::Indented);
    out_stream.flush();

    out_file.close();
}
