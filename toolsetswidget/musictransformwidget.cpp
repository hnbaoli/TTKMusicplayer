#include "musictransformwidget.h"
#include "ui_musictransformwidget.h"
#include <QFileDialog>
#include <QProcess>
#include <QMovie>

#if defined Q_OS_WIN32 && defined _MSC_VER
    #include <windows.h>
    #include <mmsystem.h>
    #pragma comment(lib,"winmm.lib")
#endif

MusicTransformWidget::MusicTransformWidget(QWidget *parent)
        : MusicMoveDialogAbstract(parent),m_movie(NULL),
        ui(new Ui::MusicTransformWidget)
{
    ui->setupUi(this);
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),4,4);
    setMask(bmp);
    //set window radius

    m_process = new QProcess(this);
    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::QToolButtonStyle);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton,SIGNAL(clicked()),SLOT(close()));

    ui->inputButton->setStyleSheet(MusicUIObject::MusicSettingMainButton);
    ui->outputButton->setStyleSheet(MusicUIObject::MusicSettingMainButton);
    ui->formatCombo->setStyleSheet(MusicUIObject::MusicEqComboboxEditStyle);
    ui->kbpsCombo->setStyleSheet(MusicUIObject::MusicEqComboboxEditStyle);
    ui->hzCombo->setStyleSheet(MusicUIObject::MusicEqComboboxEditStyle);
    ui->msCombo->setStyleSheet(MusicUIObject::MusicEqComboboxEditStyle);
    ui->transformButton->setStyleSheet(MusicUIObject::MusicSettingMainButton);
    ui->inputLineEdit->setStyleSheet(MusicUIObject::MusicLoginEditStyle);
    ui->outputLineEdit->setStyleSheet(MusicUIObject::MusicLoginEditStyle);

    ui->inputButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->outputButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->transformButton->setCursor(QCursor(Qt::PointingHandCursor));

    connect(ui->inputButton,SIGNAL(clicked()),SLOT(initInputPath()));
    connect(ui->outputButton,SIGNAL(clicked()),SLOT(initOutputPath()));
    connect(ui->transformButton,SIGNAL(clicked()),SLOT(startTransform()));
    connect(m_process,SIGNAL(finished(int)),SLOT(transformFinish(int)));

    ui->folderBox->setStyleSheet(MusicUIObject::MusicSettingCheckButton);
    connect(ui->folderBox,SIGNAL(clicked(bool)),SLOT(folderBoxChecked(bool)));
    initControlParameter();

}

MusicTransformWidget::~MusicTransformWidget()
{
    delete m_movie;
    delete m_process;
    delete ui;
}

void MusicTransformWidget::initControlParameter()
{
    ui->formatCombo->addItems(QStringList()<<"MP3"<<"WAV"<<"WMA"
                              <<"OGG"<<"FLAC"<<"AC3"<<"AAC");
    ui->kbpsCombo->addItems(QStringList()<<"32"<<"48"<<"56"<<"64"<<"80"
                    <<"96"<<"112"<<"128"<<"192"<<"224"<<"256"<<"320");
    ui->hzCombo->addItems(QStringList()<<"8000"<<"12050"<<"16000"
                    <<"22050"<<"24000"<<"32000"<<"44100"<<"48000");
    ui->msCombo->addItems(QStringList()<<"Mono"<<"Stereo");

    ui->kbpsCombo->setCurrentIndex(7);
    ui->hzCombo->setCurrentIndex(6);
}

void MusicTransformWidget::initInputPath()
{
    QString path;
    if(!ui->folderBox->isChecked())
    {
        path =  QFileDialog::getOpenFileName(
                this, "" , "./",
                "Files (*.mp3 *.wav *.wma *.ogg *.flac *.ac3 *.aac)");
        if(path.isEmpty())
            return;
        ui->listWidget->addItem(QFontMetrics(font()).elidedText(path, Qt::ElideLeft,215));
        m_path<<path;
    }
    else
    {
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::Directory );
        dialog.setViewMode(QFileDialog::Detail);
        if(dialog.exec())
        {
            path = dialog.directory().absolutePath();
            QList<QFileInfo> file(dialog.directory().entryInfoList());
            for(int i=0; i<file.count(); ++i)
            {
                if(file[i].isFile())
                   m_path<<file[i].absoluteFilePath();
            }

//            ui->listWidget->addItems(m_path);
            for(int i=0; i<m_path.count(); ++i)
                ui->listWidget->addItem(QFontMetrics(font()).elidedText(m_path[i],
                                                                        Qt::ElideLeft,215));
        }
    }

    if(!path.isEmpty())
        ui->inputLineEdit->setText(path);
}

void MusicTransformWidget::initOutputPath()
{
    QString path =  QFileDialog::getExistingDirectory(this, "" , "./");
    if(!path.isEmpty())
        ui->outputLineEdit->setText(path);
}

QString MusicTransformWidget::getTransformSongName() const
{
    if(m_path.isEmpty())
        return QString();
    QString str = m_path[0];
    str.replace("\\","/");
    str = str.split('/').back().split('.').front();
    return str;
}

void MusicTransformWidget::transformFinish(int)
{
#ifdef Q_OS_WIN
    mciSendString(TEXT("close sound.wav"),NULL,0,NULL);
    mciSendString(TEXT("open  sound.wav"),NULL,0,NULL);
    mciSendString(TEXT("play  sound.wav"),NULL,0,NULL);
#endif
    m_path.removeAt(0);
    ui->listWidget->clear();
    if(!m_path.isEmpty())
    {
        for(int i=0; i<m_path.count(); ++i)
            ui->listWidget->addItem(QFontMetrics(font()).elidedText(m_path[i],
                                                                    Qt::ElideLeft,215));
        if(!processTransform(MAKE_TRANSFORM))
            return;
    }
    else
    {
        setCheckedControl(true);
        ui->loadingLabel->hide();
        delete m_movie;
        m_movie = NULL;
    }
}

bool MusicTransformWidget::processTransform(const QString& para)
{
    if(m_path.isEmpty())
        return false;

    QString in = m_path[0].trimmed();
    QString out = ui->outputLineEdit->text().trimmed();

    if(in.isEmpty() || out.isEmpty() )
        return false;

    if(ui->formatCombo->currentText() == "OGG")
        ui->msCombo->setCurrentIndex(1);

    qDebug()<<ui->formatCombo->currentText()
            <<ui->kbpsCombo->currentText()
            <<ui->hzCombo->currentText()
            <<QString::number(ui->msCombo->currentIndex() + 1);

    m_process->start(para, QStringList()<<"-i"<<in<<"-y"
                     <<"-ab"<<ui->kbpsCombo->currentText()+ "k"
                     <<"-ar"<<ui->hzCombo->currentText()
                     <<"-ac"<<QString::number(ui->msCombo->currentIndex()+1)
                     <<QString("%1/%2-Transed.%3").arg(out).arg(getTransformSongName())
                       .arg(ui->formatCombo->currentText().toLower()) );
    return true;
}

void MusicTransformWidget::startTransform()
{
    if(!QFile(MAKE_TRANSFORM).exists())
        return;

    if(!processTransform(MAKE_TRANSFORM))
        return;
    ///////////////////////////////////////////////////////////
    ui->loadingLabel->show();
    ui->loadingLabel->setMovie(m_movie = new QMovie(":/image/loading",QByteArray(),this));
    m_movie->start();
    setCheckedControl(false);
}

void MusicTransformWidget::folderBoxChecked(bool)
{
    ui->inputLineEdit->clear();
    ui->listWidget->clear();
    m_path.clear();
}

void MusicTransformWidget::setCheckedControl(bool enable)
{
    ui->inputButton->setEnabled(enable);
    ui->inputLineEdit->setEnabled(enable);
    ui->outputButton->setEnabled(enable);
    ui->outputLineEdit->setEnabled(enable);
    ui->formatCombo->setEnabled(enable);
    ui->kbpsCombo->setEnabled(enable);
    ui->hzCombo->setEnabled(enable);
    ui->msCombo->setEnabled(enable);
    ui->folderBox->setEnabled(enable);
    ui->transformButton->setEnabled(enable);
}
