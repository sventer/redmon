
#ifndef WINDOWS_GRAPHICS_WINDOW_H_
#define WINDOWS_GRAPHICS_WINDOW_H_

#include <QWidget>

class GraphicsWindow : public QWidget {
  Q_OBJECT

public:
  GraphicsWindow(QWidget* parent = 0);
  ~GraphicsWindow();
};

#endif  // WINDOWS_GRAPHICS_WINDOW_H_
