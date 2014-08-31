/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/


#include "MainDialog.h"
#include "ui_MainDialog.h"

#include "VLCStreamer.h"
#include "UrlParser.h"

#include "HttpServer.h"
#include "MoviesHandler.h"
#include "CreateHandler.h"

#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <QtCore/QTimer>

#include <QtGui/QApplication>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QMenu>
#include <QtGui/QCloseEvent>

#include <QtNetwork/QHostInfo>
#include <QtGui/QRegExpValidator>
#include <QtGui/QIntValidator>

MainDialog::MainDialog(VLCStreamer* pVLCStreamer, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::MainDialog)
{
  m_pVLCStreamer = pVLCStreamer;

  setWindowIcon(QIcon(":/icon/streamer_icon.png"));

  if (QSystemTrayIcon::isSystemTrayAvailable())
  {
    QSystemTrayIcon* pSysTray = new QSystemTrayIcon(QIcon(":/icon/streamer_icon.png"), this);

    QMenu* pSysMenue = new QMenu(this);

    pSysMenue->addAction("Show VLCStreamer", this, SLOT(showNormal()));
    pSysMenue->addAction("Hide VLCStreamer", this, SLOT(hide()));

    pSysMenue->addSeparator();

    pSysMenue->addAction("Quit", qApp, SLOT(quit()));

    pSysTray->setContextMenu(pSysMenue);

    pSysTray->show();
  }

  ui->setupUi(this);

  ui->m_pVersionLabel->setText(QString("VLC Streamer Client Version %1").arg(qApp->applicationVersion()));

  ui->m_pIPLabel->hide();
  ui->m_pIPLineEdit->hide();

  ui->m_pHomeLineEdit->setText(UrlParser::extractDirFromGetPath("...home..."));
  ui->m_pDrivesDir->setText(UrlParser::extractDirFromGetPath("...drives..."));


  ui->m_pHostLineEdit->setValidator(new QRegExpValidator(QRegExp("[a-z,A-Z,\\s,_,\\.]+"), this));
  ui->m_pPortLineEdit->setValidator(new QIntValidator(0, 65000, this));

  slotUpdateDisplay();

  ui->tabWidget->setCurrentIndex(0);

  connect(ui->m_pStopPushButton,  SIGNAL(clicked()), this, SLOT(slotStartEdit()));
  connect(ui->m_pStartPushButton, SIGNAL(clicked()), this, SLOT(slotEndEdit()));

  connect(m_pVLCStreamer,SIGNAL(sigStatusChanged(bool)), this, SLOT(slotUpdateStatus(bool)));

  QTimer* pTimer = new QTimer(this);
  connect(pTimer, SIGNAL(timeout()), this, SLOT(on_m_pRefreshButton_clicked()));
  pTimer->start(10*1000);

  hide();
}


MainDialog::~MainDialog()
{
  delete ui;
}

void MainDialog::closeEvent(QCloseEvent* event)
{
  if ( QSystemTrayIcon::isSystemTrayAvailable() )
  {
    hide();
    event->ignore();
  }
}

bool MainDialog::event(QEvent *pEvent)
{
  if(pEvent->type() == QEvent::WindowStateChange)
  {
    if ( (isMinimized()) && (QSystemTrayIcon::isSystemTrayAvailable()))
    {
      hide();
    }
  }

  return QDialog::event(pEvent);
}


void MainDialog::slotUpdateDisplay()
{
  QSettings settings;

  ui->m_pHostLineEdit->setText(settings.value("hostname").toString());
  ui->m_pPortLineEdit->setText(settings.value("port").toString());

//  ui->m_pIPLineEdit->setText(QHostInfo::fromName(QHostInfo::localHostName()).addresses().first().toString());

  ui->m_pRootLineEdit->setText(settings.value("root").toString());

//  ui->m_pHomeLineEdit->setText("XXXX");
//  ui->m_pDrivesDir->setText("XXXX");

  setEdit(ui->m_pHostLineEdit, false);
  setEdit(ui->m_pPortLineEdit, false);
  setEdit(ui->m_pIPLineEdit, false);

  setEdit(ui->m_pRootLineEdit, false);
  setEdit(ui->m_pHomeLineEdit, false);
  setEdit(ui->m_pDrivesDir, false);

}


void MainDialog::on_m_pQuitPushButton_clicked()
{
    qApp->quit();
}


void MainDialog::slotStartEdit()
{
  m_pVLCStreamer->slotStopService();

  setEdit(ui->m_pHostLineEdit, true);
  setEdit(ui->m_pPortLineEdit, true);

  setEdit(ui->m_pRootLineEdit, true);
}


void MainDialog::slotEndEdit()
{
  QSettings settings;
  settings.setValue("hostname", QVariant(ui->m_pHostLineEdit->text()));
  settings.setValue("port", QVariant(ui->m_pPortLineEdit->text()));
  settings.setValue("root", QVariant(ui->m_pRootLineEdit->text()));

  settings.sync();

  slotUpdateDisplay();

  m_pVLCStreamer->slotStartService();
}

void MainDialog::setEdit(QLineEdit* pLineEdit, bool isEditable)
{
  pLineEdit->setReadOnly(!isEditable);
  pLineEdit->setEnabled(isEditable);
}

void MainDialog::slotUpdateStatus(bool bIsRunning)
{
  if (bIsRunning)
  {
    ui->m_pStatusLabel->setText(tr("Service is running"));
  }
  else
  {
    ui->m_pStatusLabel->setText(tr("Service is stopped"));
  }
}


void MainDialog::on_m_pRefreshButton_clicked()
{
  ui->m_pMoviesList->clear();

  if (m_pVLCStreamer->isRunning())
  {
    m_pVLCStreamer->getHttpServer()->getMoviesHandler()->updateMovies();

    QStringList sMovies = m_pVLCStreamer->getHttpServer()->getMoviesHandler()->getMovieNames();

    foreach(QString sMovie, sMovies)
    {
      QString sItem;
      Movie* pMovie = m_pVLCStreamer->getHttpServer()->getMoviesHandler()->getMovieFromName(sMovie);

      sItem = QString("Name: %1 Status: %2").arg(sMovie).arg(pMovie->getStatus());

      ui->m_pMoviesList->addItem(sItem);
    }
  }
}
