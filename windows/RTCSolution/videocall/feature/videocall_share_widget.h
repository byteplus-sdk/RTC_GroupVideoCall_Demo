#pragma once

#include <QDialog>

namespace Ui {
    class VideoCallShareWidget;
}

/**
* Select the shared content page, you can choose the screen desktop or window
*/
class VideoCallShareWidget : public QDialog
{
    Q_OBJECT
public:
    explicit VideoCallShareWidget(QWidget *parent = nullptr);
    ~VideoCallShareWidget();

    void updateData();
    bool canStartSharing();

private:
    void initTranslations();

    Ui::VideoCallShareWidget *ui;
};

