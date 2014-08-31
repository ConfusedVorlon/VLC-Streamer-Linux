/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/


#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class MainDialog;
}

class QLineEdit;

class VLCStreamer;

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(VLCStreamer* pVLCStreamer, QWidget *parent = 0);
    ~MainDialog();

protected:

    virtual void closeEvent(QCloseEvent* event);

    virtual bool event(QEvent* pEvent);

private slots:

  void on_m_pQuitPushButton_clicked();

  void slotStartEdit();

  void slotEndEdit();

  void slotUpdateDisplay();

  void slotUpdateStatus(bool bIsRunning);

  void on_m_pRefreshButton_clicked();

private:

    void setEdit(QLineEdit* pLineEdit, bool isEditable);

    Ui::MainDialog *ui;

    VLCStreamer* m_pVLCStreamer;
};

#endif // MAINDIALOG_H
