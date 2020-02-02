#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

#include <QWidget>
#include <QOpenGLWidget>
#include "../workbench.hpp"

class visualizer : public QOpenGLWidget {
  public:
	visualizer();
	~visualizer();
};

#endif	// VISUALIZER_HPP
