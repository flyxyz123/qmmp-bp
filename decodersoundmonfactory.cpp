#include "decodersoundmonfactory.h"
#include "soundmonhelper.h"
#include "decoder_soundmon.h"

#include <QtWidgets/QMessageBox>

bool DecoderSoundMonFactory::canDecode(QIODevice *input) const
{
    QFile *file = static_cast<QFile*>(input);
    if(!file)
    {
        return false;
    }

    SoundMonHelper helper(file->fileName());
    return helper.initialize();
}

DecoderProperties DecoderSoundMonFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("BP SoundMon Plugin");
    properties.shortName = "soundmon";
    properties.filters << "*.bp" << "*.bp3";
    properties.description = "BP SoundMon Audio File";
    properties.protocols << "file";
    properties.noInput = true;
    properties.hasAbout = true;
    return properties;
}

Decoder *DecoderSoundMonFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderSoundMon(path);
}

QList<TrackInfo*> DecoderSoundMonFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);
    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    SoundMonHelper helper(path);
    if(!helper.initialize())
    {
        delete info;
        return QList<TrackInfo*>();
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.depth());
        info->setValue(Qmmp::FORMAT_NAME, "BP SoundMon");
        info->setDuration(helper.totalTime());
    }
    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderSoundMonFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

void DecoderSoundMonFactory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

void DecoderSoundMonFactory::showAbout(QWidget *parent)
{
    QMessageBox::about(parent, tr("About BP Reader Plugin"),
                       tr("Qmmp BP Reader Plugin") + "\n" +
                       tr("Written by: Greedysky <greedysky@163.com>"));
}

QString DecoderSoundMonFactory::translation() const
{
    return QString();
}
