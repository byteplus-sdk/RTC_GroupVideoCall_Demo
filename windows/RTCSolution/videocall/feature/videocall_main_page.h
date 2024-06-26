#pragma once

#include <QWidget>

namespace Ui {
	class VideoCallMainPage;
}

/**
* Video call main page
* The top part displays the call duration, room number and other information
* The middle part is the video rendering area, mainly including FocusVideoView and NormalVideoView
* The bottom part is the room control buttons, including audio, video, beauty, share, call data, settings, end call
*/
class VideoCallMainPage : public QWidget {
	Q_OBJECT
public:
	enum { kNormalPage, kFocusPage };
	explicit VideoCallMainPage(QWidget* parent = nullptr);
	~VideoCallMainPage();

	void updateVideoWidget();
	void showWidget(int cnt);
	void changeViewMode(int mode);
	 int viewMode();
	void froceClose();
	void init();
	void setCameraState(bool on);
	void setMicState(bool on);
	void setBasicBeauty(bool enabled);

signals:
	void sigClose();
	void sigCameraEnabled(bool mute);
	void sigShareButtonClicked();
	void sigVideoCallSetting();
	void sigRealTimeDataClicked();

protected:
	void closeEvent(QCloseEvent*) override;

private:
	void initUi();
	void setDefaultProfiles();
	void initConnections();
	void initCameraOption();
	void initMicOption();
	void initShareOption();

	Ui::VideoCallMainPage* ui;
	int current_page_ = kNormalPage;
	QTimer* main_timer_;
	int64_t tick_count_;
	bool froce_close_ = false;
	bool show_ = false;
	bool beauty_enabled_ = false;
};
