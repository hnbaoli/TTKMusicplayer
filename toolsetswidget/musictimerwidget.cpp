#include "musictimerwidget.h"
#include "ui_musictimerwidget.h"
#include "musicsettingmanager.h"
#include "musicuiobject.h"

MusicTimerWidget::MusicTimerWidget(QWidget *parent)
        : MusicMoveDialogAbstract(parent),
        ui(new Ui::MusicTimerWidget)
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

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::QToolButtonStyle);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton,SIGNAL(clicked()),SLOT(close()));

    connect(ui->timerToPlay,SIGNAL(clicked()),SLOT(changeFirstWidget()));
    connect(ui->timerToStop,SIGNAL(clicked()),SLOT(changeSecondWidget()));
    connect(ui->timerToShutdown,SIGNAL(clicked()),SLOT(changeThreeWidget()));

    ui->timerToPlay->setIcon(QIcon(":/control/timerPlay"));
    ui->timerToStop->setIcon(QIcon(":/control/timerStop"));
    ui->timerToShutdown->setIcon(QIcon(":/control/timerDown"));
    ui->timerToPlay->setStyleSheet(MusicUIObject::MusicSettingMainButton);
    ui->timerToStop->setStyleSheet(MusicUIObject::MusicSettingMainButton);
    ui->timerToShutdown->setStyleSheet(MusicUIObject::MusicSettingMainButton);
    ui->confirm->setStyleSheet(MusicUIObject::MusicSettingSubmitButton);
    ui->cancel->setStyleSheet(MusicUIObject::MusicSettingSubmitButton);
    ui->timerToPlay->setCursor(QCursor(Qt::PointingHandCursor));
    ui->timerToStop->setCursor(QCursor(Qt::PointingHandCursor));
    ui->timerToShutdown->setCursor(QCursor(Qt::PointingHandCursor));
    ui->confirm->setCursor(QCursor(Qt::PointingHandCursor));
    ui->cancel->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->timerToPlay,SIGNAL(clicked()),SLOT(changeFirstWidget()));
    connect(ui->timerToStop,SIGNAL(clicked()),SLOT(changeSecondWidget()));
    connect(ui->timerToShutdown,SIGNAL(clicked()),SLOT(changeThreeWidget()));
    connect(ui->confirm,SIGNAL(clicked()),this,SLOT(commitTheResults()));
    connect(ui->cancel,SIGNAL(clicked()),this,SLOT(close()));

    initComboParameter();
    initFirstWidget();
    initSecondWidget();
    initThreeWidget();
    initParemeter();
}

MusicTimerWidget::~MusicTimerWidget()
{
    delete ui;
}

void MusicTimerWidget::initParemeter()
{
    if(M_SETTING.value(MusicSettingManager::TimerAutoPlayChoiced).toInt() == 1)
    {
        ui->noSetRadioButton1->setChecked(true);
        setEnabledControlFalse1();
    }
    else
        ui->setRadioButton1->setChecked(true);
    ui->hourComboBox1->setCurrentIndex(M_SETTING.value(MusicSettingManager::TimerAutoPlayHourChoiced).toInt());
    ui->secComboBox1->setCurrentIndex(M_SETTING.value(MusicSettingManager::TimerAutoPlaySecondChoiced).toInt());
    ui->repeatComboBox1->setCurrentIndex(M_SETTING.value(MusicSettingManager::TimerAutoPlayRepeatChoiced).toInt());
    ui->plistComboBox->setCurrentIndex(M_SETTING.value(MusicSettingManager::TimerAutoPlayItemIndexChoiced).toInt());

    if(M_SETTING.value(MusicSettingManager::TimerAutoStopChoiced).toInt() == 1)
    {
        ui->noSetRadioButton2->setChecked(true);
        setEnabledControlFalse2();
    }
    else
        ui->setRadioButton2->setChecked(true);
    ui->hourComboBox2->setCurrentIndex(M_SETTING.value(MusicSettingManager::TimerAutoStopHourChoiced).toInt());
    ui->secComboBox2->setCurrentIndex(M_SETTING.value(MusicSettingManager::TimerAutoStopSecondChoiced).toInt());
    ui->repeatComboBox2->setCurrentIndex(M_SETTING.value(MusicSettingManager::TimerAutoStopRepeatChoiced).toInt());

    if(M_SETTING.value(MusicSettingManager::TimerAutoShutdownChoiced).toInt() == 1)
    {
        ui->noSetRadioButton3->setChecked(true);
        setEnabledControlFalse3();
    }
    else
        ui->setRadioButton3->setChecked(true);
    ui->hourComboBox3->setCurrentIndex(M_SETTING.value(MusicSettingManager::TimerAutoShutdownHourChoiced).toInt());
    ui->secComboBox3->setCurrentIndex(M_SETTING.value(MusicSettingManager::TimerAutoShutdownSecondChoiced).toInt());
    ui->repeatComboBox3->setCurrentIndex(M_SETTING.value(MusicSettingManager::TimerAutoShutdownRepeatChoiced).toInt());
}

void MusicTimerWidget::writeParemeter()
{
    M_SETTING.setValue(MusicSettingManager::TimerAutoIndexChoiced,
                     ui->stackedWidget->currentIndex());
    M_SETTING.setValue(MusicSettingManager::TimerAutoPlayChoiced,
                     ui->noSetRadioButton1->isChecked() ? 1 : 0);
    M_SETTING.setValue(MusicSettingManager::TimerAutoPlayHourChoiced,
                     ui->hourComboBox1->currentIndex());
    M_SETTING.setValue(MusicSettingManager::TimerAutoPlaySecondChoiced,
                     ui->secComboBox1->currentIndex());
    M_SETTING.setValue(MusicSettingManager::TimerAutoPlayRepeatChoiced,
                     ui->repeatComboBox1->currentIndex());
    M_SETTING.setValue(MusicSettingManager::TimerAutoPlayItemIndexChoiced,
                     ui->plistComboBox->currentIndex());
    M_SETTING.setValue(MusicSettingManager::TimerAutoPlaySongIndexChoiced,
                     ui->psongComboBox->currentIndex());
    M_SETTING.setValue(MusicSettingManager::TimerAutoStopChoiced,
                     ui->noSetRadioButton2->isChecked() ? 1 : 0);
    M_SETTING.setValue(MusicSettingManager::TimerAutoStopHourChoiced,
                     ui->hourComboBox2->currentIndex());
    M_SETTING.setValue(MusicSettingManager::TimerAutoStopSecondChoiced,
                     ui->secComboBox2->currentIndex());
    M_SETTING.setValue(MusicSettingManager::TimerAutoStopRepeatChoiced,
                     ui->repeatComboBox2->currentIndex());
    M_SETTING.setValue(MusicSettingManager::TimerAutoShutdownChoiced,
                     ui->noSetRadioButton3->isChecked() ? 1 : 0);
    M_SETTING.setValue(MusicSettingManager::TimerAutoShutdownHourChoiced,
                     ui->hourComboBox3->currentIndex());
    M_SETTING.setValue(MusicSettingManager::TimerAutoShutdownSecondChoiced,
                     ui->secComboBox3->currentIndex());
    M_SETTING.setValue(MusicSettingManager::TimerAutoShutdownRepeatChoiced,
                     ui->repeatComboBox3->currentIndex());
}

void MusicTimerWidget::setSongStringList(const QStringList& list)
{
    ui->psongComboBox->addItems(list);
    ui->psongComboBox->setCurrentIndex(M_SETTING.value(MusicSettingManager::TimerAutoPlaySongIndexChoiced).toInt());
}

void MusicTimerWidget::initComboParameter()
{
    for(int i=0; i<24; ++i)
        m_hour<<tr("%1H").arg(i);
    for(int i=0; i<60; ++i)
        m_second<<tr("%1S").arg(i);
    m_repeat<<tr("once")<<tr("evMonth")<<tr("evWeek")<<tr("evDay");
}

void MusicTimerWidget::changeFirstWidget()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MusicTimerWidget::changeSecondWidget()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MusicTimerWidget::changeThreeWidget()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MusicTimerWidget::initFirstWidget()
{
//    ui->noSetRadioButton1->setChecked(true);
    ui->hourComboBox1->addItems(m_hour);
    ui->secComboBox1->addItems(m_second);
    ui->repeatComboBox1->addItems(m_repeat);
    ui->plistComboBox->addItem(tr("defualt"));
    ui->noSetRadioButton1->setStyleSheet(MusicUIObject::MusicSettingRadioButton);
    ui->setRadioButton1->setStyleSheet(MusicUIObject::MusicSettingRadioButton);
    ui->hourComboBox1->setStyleSheet(MusicUIObject::MusicEqComboboxEditStyle);
    ui->secComboBox1->setStyleSheet(MusicUIObject::MusicEqComboboxEditStyle);
    ui->repeatComboBox1->setStyleSheet(MusicUIObject::MusicEqComboboxEditStyle);
    ui->plistComboBox->setStyleSheet(MusicUIObject::MusicEqComboboxEditStyle);
    ui->psongComboBox->setStyleSheet(MusicUIObject::MusicEqComboboxEditStyle);
    connect(ui->noSetRadioButton1,SIGNAL(clicked()),SLOT(setEnabledControlFalse1()));
    connect(ui->setRadioButton1,SIGNAL(clicked()),SLOT(setEnabledControlTrue1()));
}

void MusicTimerWidget::initSecondWidget()
{
//    ui->noSetRadioButton2->setChecked(true);
    ui->hourComboBox2->addItems(m_hour);
    ui->secComboBox2->addItems(m_second);
    ui->repeatComboBox2->addItems(m_repeat);
    ui->noSetRadioButton2->setStyleSheet(MusicUIObject::MusicSettingRadioButton);
    ui->setRadioButton2->setStyleSheet(MusicUIObject::MusicSettingRadioButton);
    ui->hourComboBox2->setStyleSheet(MusicUIObject::MusicEqComboboxEditStyle);
    ui->secComboBox2->setStyleSheet(MusicUIObject::MusicEqComboboxEditStyle);
    ui->repeatComboBox2->setStyleSheet(MusicUIObject::MusicEqComboboxEditStyle);
    connect(ui->noSetRadioButton2,SIGNAL(clicked()),SLOT(setEnabledControlFalse2()));
    connect(ui->setRadioButton2,SIGNAL(clicked()),SLOT(setEnabledControlTrue2()));
}

void MusicTimerWidget::initThreeWidget()
{
//    ui->noSetRadioButton3->setChecked(true);
    ui->hourComboBox3->addItems(m_hour);
    ui->secComboBox3->addItems(m_second);
    ui->repeatComboBox3->addItems(m_repeat);
    ui->noSetRadioButton3->setStyleSheet(MusicUIObject::MusicSettingRadioButton);
    ui->setRadioButton3->setStyleSheet(MusicUIObject::MusicSettingRadioButton);
    ui->hourComboBox3->setStyleSheet(MusicUIObject::MusicEqComboboxEditStyle);
    ui->secComboBox3->setStyleSheet(MusicUIObject::MusicEqComboboxEditStyle);
    ui->repeatComboBox3->setStyleSheet(MusicUIObject::MusicEqComboboxEditStyle);
    connect(ui->noSetRadioButton3,SIGNAL(clicked()),SLOT(setEnabledControlFalse3()));
    connect(ui->setRadioButton3,SIGNAL(clicked()),SLOT(setEnabledControlTrue3()));
}

void MusicTimerWidget::commitTheResults()
{
    writeParemeter();
    emit timerParameterChanged();
    close();
}

void MusicTimerWidget::setEnabledControlFalse1()
{
    ui->hourComboBox1->setEnabled(false);
    ui->secComboBox1->setEnabled(false);
    ui->repeatComboBox1->setEnabled(false);
    ui->plistComboBox->setEnabled(false);
    ui->psongComboBox->setEnabled(false);
}

void MusicTimerWidget::setEnabledControlTrue1()
{
    ui->hourComboBox1->setEnabled(true);
    ui->secComboBox1->setEnabled(true);
    ui->repeatComboBox1->setEnabled(true);
    ui->plistComboBox->setEnabled(true);
    ui->psongComboBox->setEnabled(true);
}

void MusicTimerWidget::setEnabledControlFalse2()
{
    ui->hourComboBox2->setEnabled(false);
    ui->secComboBox2->setEnabled(false);
    ui->repeatComboBox2->setEnabled(false);
}

void MusicTimerWidget::setEnabledControlTrue2()
{
    ui->hourComboBox2->setEnabled(true);
    ui->secComboBox2->setEnabled(true);
    ui->repeatComboBox2->setEnabled(true);
}

void MusicTimerWidget::setEnabledControlFalse3()
{
    ui->hourComboBox3->setEnabled(false);
    ui->secComboBox3->setEnabled(false);
    ui->repeatComboBox3->setEnabled(false);
}

void MusicTimerWidget::setEnabledControlTrue3()
{
    ui->hourComboBox3->setEnabled(true);
    ui->secComboBox3->setEnabled(true);
    ui->repeatComboBox3->setEnabled(true);
}
