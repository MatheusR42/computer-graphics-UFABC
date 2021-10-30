#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "asteroids.hpp"
#include "shark.hpp"
#include "starlayers.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint m_starsProgram{};
  GLuint m_objectsProgram{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  GameData m_gameData;

  Asteroids m_asteroids;
  Shark m_shark;
  StarLayers m_starLayers;

  abcg::ElapsedTimer m_restartWaitTimer;

  ImFont* m_font_final{};
  ImFont* m_font_points{};

  std::default_random_engine m_randomEngine;

  void checkCollisions();

  void restart();
  void update();
};

#endif