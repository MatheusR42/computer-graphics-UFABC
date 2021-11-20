#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "model.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  static const int m_numBubbles{20};

private:
  static const int m_numCorals{40};

  GLuint m_program{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  std::default_random_engine m_randomEngine;

  Model m_modelBubble;
  Model m_modelShark;
  Model m_modelCoral;

  std::array<glm::vec3, m_numBubbles> m_bubblePositions;
  std::array<glm::vec3, m_numBubbles> m_bubbleRotations;
  
  glm::vec3 m_sharkPosition;
  glm::vec3 m_sharkRotation;

  std::array<glm::vec3, m_numCorals> m_coralPositions;
  std::array<glm::vec3, m_numCorals> m_coralRotations;

  float m_angle{};
  float m_sharkAngle{};
  float m_sharkRotationX{};
  float m_sharkRotationY{};
  float m_sharkRotationZ{};

  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};
  float m_FOV{8.0f};

  ImFont* m_font_final{};
  ImFont* m_font_points{};
  
    void restart();

  void randomizeBubble(glm::vec3 &position, glm::vec3 &rotation);
  void randomizeCoral(glm::vec3 &position, glm::vec3 &rotation);
  void update();
};

#endif