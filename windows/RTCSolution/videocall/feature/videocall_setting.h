#pragma once

#include <QDialog>
#include "videocall/core/videocall_model.h"

namespace Ui {
    class VideoCallSetting;
}

/**
* Audio and video call setting page, used to set resolution, audio quality, mirroring or not
*/
class VideoCallSetting : public QDialog {
    Q_OBJECT

public:
    explicit VideoCallSetting(QWidget* parent = nullptr);
    ~VideoCallSetting();

    void initView();

protected:
    void showEvent(QShowEvent* e) override;
    bool eventFilter(QObject* o, QEvent* e) override;
    void closeEvent(QCloseEvent* e)override;

public slots:
    void onConfirm();
    void onClose();
    void onCancel();

private:
    int getIdxFromResolution(const videocall::VideoResolution& resolution);
    void initConnect();
    void initUITranslations();

private:
    QDialog::DialogCode code_;
    videocall::VideoCallSettingModel setting_;
    Ui::VideoCallSetting* ui;
};
