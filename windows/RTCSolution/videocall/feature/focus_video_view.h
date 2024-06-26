#pragma once

#include <QWidget>

class QHBoxLayout;
class QSpacerItem;

namespace Ui {
class FocusVideoView;
}

/**
* The video rendering area class including shared content, 
* the left side is the shared content, and the right side is the user's video arranged vertically
*/
class FocusVideoView : public QWidget {
  Q_OBJECT

 public:
  explicit FocusVideoView(QWidget *parent = nullptr);
  ~FocusVideoView();
  void init();
  void showWidget(int cnt);
  void wheelEvent(QWheelEvent *) override;

 protected:
  void paintEvent(QPaintEvent *) override;

 private:
  Ui::FocusVideoView* ui;
  int cnt_ = 0;
};
