//
// Created by wengxiang on 2017/1/30.
//

#ifndef ATLASGENERATOR_ATLAS_PACKER_H
#define ATLASGENERATOR_ATLAS_PACKER_H

#include <QtCore/QVector>
#include <QtGui/QImage>
#include <QtCore/QDir>

#include "RectangleBinPack/Rect.h"
#include "RectangleBinPack/MaxRectsBinPack.h"
#include "DataExport.h"

struct ImageInfo
{
    QImage *image = nullptr;
    QString filename;
    rbp::Rect frame;
    bool rotated = false;
    bool trimmed = false;
    int image_index;

    rbp::Rect sprite_source_size;
    rbp::RectSize source_size;
};
struct HeuristicResult
{
    rbp::MaxRectsBinPack::FreeRectChoiceHeuristic method;
    std::vector<rbp::Rect> rects;
    std::vector<ImageInfo> images;
    double occupancy;
    rbp::RectSize opacity_size;
    rbp::RectSize bin_size;
};

class AtlasPacker
{
private:
    /**
     * crop transparency around this image.
     *
     * @param input         the image to crop.
     * @param newBounds     bounds of opacity rectangular area.
     * @return  the image cropped.
     */
    QImage* ImageCropAlpha(const QImage *input, rbp::Rect &newBounds);

    /**
     * extrude the pixels around this image.
     *
     * @param input     the image to extrude.
     * @param imageInfo the image's corresponding ImageInfo object.
     * @return the image extruded.
     */
    QImage* ImageExtrude(QImage *input, ImageInfo& imageInfo);

    /**
     * draw a image to another image.
     *
     * @param canvas    destination image
     * @param image     source image
     * @param destX     destination x axes
     * @param destY     destination y axes
     */
    void ImageDrawImage(QImage &canvas, const QImage &image, int destX, int destY);

    /**
     * evaluate appropriate size of bin by given heuristic method.
     * get insert result by Insert().
     * storage the result.
     *
     * @param heuristicResult   container to storage result.
     * @param method            specify heuristic method.
     * @param type              0或1，宽度优先或高度优先
     */
    void StorageInsertResult(QVector<HeuristicResult> &heuristicResult,
                             rbp::MaxRectsBinPack::FreeRectChoiceHeuristic method,
                             int type);

    /**
     * generate atlas recursively.
     */
    void GenerateAtlas();
    /**
     * insert all images into bin in given size and heuristic method, and storage result.
     *
     * @param bin_width     specify bin's width
     * @param bin_height    specify bin's height
     * @param result        to storage the result
     * @param method        specify heuristic method
     * @return true stand for the given size can accomodate all images
     */
    bool Insert(int bin_width, int bin_height,
                HeuristicResult &result,
                rbp::MaxRectsBinPack::FreeRectChoiceHeuristic method);

    rbp::MaxRectsBinPack bin_pack;
    std::vector<ImageInfo> images;
    std::vector<QImage*> canvases;
    DataExport* data_export;

public:

    AtlasPacker();
    ~AtlasPacker();
    /**
     * add a image to AtlasPacker.
     *
     * @param filename  the path to image.
     * @param image image object.
     */
    void AddImage(QString filename, QImage* image);
    /**
     * pack all images into bin.
     * if there is no image storage in this atlas packer.
     * then ignore.
     */
    void PackBin();
    /**
     * export atlas.
     * if there is no image to export, then ignore.
     *
     * @param relative_path relative path to out directory.
     */
    void ExportAtlas(QString relative_path);
    /*
     * is this atlas packer empty.
     * it should be invoke before PackBin().
     * because PackBin() will clear all images it contains.
     */
    bool IsEmpty();
};

#endif //ATLASGENERATOR_ATLAS_PACKER_H
