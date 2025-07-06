#include "blocks.h"
#include "constNum.h"
//生成地图的block
Blocks::Blocks(QWidget *parent):QWidget(parent){
    currentType = this->random();
    resizedPixmap1 = resizePixmap(pixmap1);
    resizedPixmap2 = resizePixmap(pixmap2);
    resizedPixmap3 = resizePixmap(pixmap3);
    imageLabel = new QLabel(this);
    switch(currentType){
    case type1:
        pixmap = resizedPixmap1;
        if (imageLabel != nullptr) { // 检查 imageLabel 是否为有效指针
            imageLabel->setPixmap(pixmap);} // 设置 pixmap 到 imageLabel
        break;
    case type2:
        pixmap = resizedPixmap2;
        if (imageLabel != nullptr) {
            imageLabel->setPixmap(pixmap);}
        break;
    case type3:
        pixmap = resizedPixmap3;
        if (imageLabel != nullptr) {
            imageLabel->setPixmap(pixmap);}
        break;
    }
}
//生成bonus的block
Blocks::Blocks(QWidget *parent,int x):QWidget(parent)
{
    //bonus1
    if(x==1){
        imageLabel = new QLabel(this);
        pixmap  = resizePixmap(pixmap4);
        if (imageLabel != nullptr){
            imageLabel->setPixmap(pixmap);
        }
    }
    //bonus2
    if(x==2){
        imageLabel = new QLabel(this);
        pixmap  = resizePixmap(pixmap5);
        if (imageLabel != nullptr){
            imageLabel->setPixmap(pixmap);
        }
    }
    //bonus3
    if(x==3){
        imageLabel = new QLabel(this);
        pixmap  = resizePixmap(pixmap6);
        if (imageLabel != nullptr){
            imageLabel->setPixmap(pixmap);
        }
    }
    //bonus4
    if(x==4){
        imageLabel = new QLabel(this);
        pixmap  = resizePixmap(pixmap7);
        if (imageLabel != nullptr){
            imageLabel->setPixmap(pixmap);
        }
    }
}
Blocks::Blocks(QWidget *parent,bool state,int x):QWidget(parent),deadState(state)
{
    resizedPixmap1 = resizePixmap(pixmap1);
    resizedPixmap2 = resizePixmap(pixmap2);
    resizedPixmap3 = resizePixmap(pixmap3);
    imageLabel = new QLabel(this);
    if(x==0){
        deadState = true;
        currentType = type0;
        imageLabel = NULL;
    }
    if(x==1){
        pixmap = resizedPixmap1;
        if (imageLabel != nullptr) {// 检查 imageLabel 是否为有效指针
            imageLabel->setPixmap(pixmap);} // 设置 pixmap 到 imageLabel
        currentType = type1;
    }
    if(x==2){
        pixmap = resizedPixmap2;
        if (imageLabel != nullptr) {
            imageLabel->setPixmap(pixmap);}
        currentType = type2;
    }
    if(x==3){
        pixmap = resizedPixmap3;
        if (imageLabel != nullptr) {
            imageLabel->setPixmap(pixmap);}
        currentType = type3;
    }
}
QJsonObject Blocks::getJsonData() const
{
    return BlocksData;
}
QJsonValue Blocks::pixmapToJsonValue(const QPixmap &pixmap)const {
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");  // 保存 pixmap 数据到缓冲区，使用 PNG 格式
    QString base64String = QString::fromLatin1(byteArray.toBase64().data());
    return QJsonValue(base64String);
}
QPixmap Blocks::jsonValueToPixmap(const QJsonValue &value) const{
    QByteArray byteArray = QByteArray::fromBase64(value.toString().toLatin1());
    QPixmap pixmap;
    pixmap.loadFromData(byteArray, "PNG");
    return pixmap;
}
void Blocks::toJson() const
{
    BlocksData["currentType"] = currentType;
    BlocksData["anchorState1"] = anchorState1;
    BlocksData["anchorState2"] = anchorState2;
    BlocksData["deadState"] = deadState;
    BlocksData["coupled"] = coupled;
    BlocksData["X"] = X;
    BlocksData["Y"] = Y;
    BlocksData["pixmap"] = this->pixmapToJsonValue(pixmap);
    BlocksData["imageLabel"] = this->pixmapToJsonValue(pixmap);//记住之后是要把整个pixmap利用setpixmap来分配给imageLabel的 。
}
QPixmap Blocks::resizePixmap(QPixmap *pixmap_tmp)
{
    int scaledWidth = num;
    int scaledHeight = num;
    int originalWidth;
    int originalHeight;
    int cropSize;
    QPixmap resizedImage;
    originalWidth = (*pixmap_tmp).width();
    originalHeight = (*pixmap_tmp).height();
    cropSize = qMin(originalWidth, originalHeight);
    QRect cropRect1((originalWidth - cropSize) / 2, (originalHeight - cropSize) / 2, cropSize, cropSize);
    QPixmap croppedImage = (*pixmap_tmp).copy(cropRect1);
    resizedImage = croppedImage.scaled(scaledWidth, scaledHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    return resizedImage;
}

Blocks::~Blocks(){
    if(pixmap1)
        delete pixmap1;
    if(pixmap2)
        delete pixmap2;
    if(pixmap3)
        delete pixmap3;
    if(pixmap4)
        delete pixmap4;
    if(pixmap5)
        delete pixmap5;
    if(pixmap6)
        delete pixmap6;
    if(pixmap7)
        delete pixmap7;
    if(imageLabel)
        delete imageLabel;
};
Blocks::Type Blocks::random()
{
    int random = QRandomGenerator::global()->bounded(1,4);
    // 根据生成的随机数返回对应的Type
    switch(random) {
    case 1:
        return type1;
    case 2:
        return type2;
    case 3:
        return type3;
    default:
        qDebug() <<"blocks random wrong!";
    }
}
void Blocks::setPosition(int x, int y)
{
    if(currentType!=type0)
        imageLabel->setGeometry(x, y, pixmap.width(), pixmap.height());
    X = x;
    Y = y;
}
