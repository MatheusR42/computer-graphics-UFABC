#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "model.hpp"
#include "shark.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  static const int m_numStars{10};

  GLuint m_program{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  std::default_random_engine m_randomEngine;

  Model m_model;
  Model m_modelShark;

  GameData m_gameData;
  Shark m_shark;

  std::array<glm::vec3, m_numStars> m_starPositions;
  std::array<glm::vec3, m_numStars> m_starRotations;
  
  float m_angle{};

  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};
  float m_FOV{30.0f};

  ImFont* m_font_final{};
  ImFont* m_font_points{};
  
  void restart();

  void randomizeStar(glm::vec3 &position, glm::vec3 &rotation);
  void update();
};

#endif