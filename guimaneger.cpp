#include "guimaneger.h"
#include <QDebug>

GuiManeger::GuiManeger()
    :image(NULL),greyScalePic(NULL), greyScaledChannels{NULL,NULL,NULL}, orgPic(NULL), newPic(NULL)
{

}

GuiManeger::~GuiManeger()
{
    delete greyScalePic;
    delete image;
    delete greyScaledChannels[0];
    delete greyScaledChannels[1];
    delete greyScaledChannels[2];
}

void GuiManeger::init(Canvas *a_orgPic, Canvas *a_newPic)
{
    orgPic = a_orgPic;
    newPic = a_newPic;
}

void GuiManeger::converte(QString fileName)
{
    image = new QImage(fileName);
    greyScaledChannels = converter.getAsGreyScale(image);
    //greyScaledChannels[0]->save("redChannel.jpg");
    //greyScaledChannels[1]->save("greenChannel.jpg");
    //greyScaledChannels[2]->save("blueChannel.jpg");

    QImage* greyScalePic = converter.combineChannels(greyScaledChannels);
    greyScalePic->save(fileName + "[gray]" + ".jpg");
    qDebug() << fileName + "[gray]" + ".jpg";

    orgPic->setImage(*image);


}
