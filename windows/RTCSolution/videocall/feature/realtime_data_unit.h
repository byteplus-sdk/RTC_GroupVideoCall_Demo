#pragma once

#include <QWidget>
#include "videocall/core/data_mgr.h"

namespace Ui {
    class realTimeDataUnit;
}

/**
* Audio or video data display unit for each user in the call statistics page
* Video data includes resolution, frame rate, bit rate, delay, packet loss rate, network status
* Audio data includes bit rate, delay, packet loss rate, network status
*/
class realTimeDataUnit : public QWidget {
    Q_OBJECT

public:
    explicit realTimeDataUnit(QWidget* parent = nullptr);
    ~realTimeDataUnit();
    void updateInfo(const videocall::StreamInfo& info, bool isVideoInfo);

private:
    Ui::realTimeDataUnit* ui;
};
