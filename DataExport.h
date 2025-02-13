//
// Created by survivor on 17-6-9.
//

#ifndef ATLAS_GENERATOR_DATA_EXPORT_H
#define ATLAS_GENERATOR_DATA_EXPORT_H

struct ImageInfo;

class DataExport
{
public:
    virtual void AddImageDescription(const ImageInfo &image_info) = 0;
    virtual void SetMetaImages(QString images) = 0;
    virtual void SetMetaPrefix(QString prefix) = 0;
    virtual void SetMetaFormat(QString format) = 0;
    virtual void SetMetaSize(int width, int height) = 0;
    virtual void Export(const QString &file_path)= 0;
    virtual void Clear() = 0;
};


#endif //ATLAS_GENERATOR_DATA_EXPORT_H
