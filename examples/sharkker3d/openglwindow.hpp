#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "model.hpp"
#include "shark.hpp"
#include "gamedata.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;
  void checkCollisions();

  abcg::ElapsedTimer m_Timer;

  Model m_corals;
  Shark m_shark;

 private:
  static const int m_numBubbles{200};

private:
  static const int m_numCorals{1};

  GLuint m_program{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  std::default_random_engine m_randomEngine;

  Model m_modelBubble;
  Model m_modelShark;
  Model m_modelCoral;

  std::array<glm::vec3, m_numBubbles> m_bubblePositions;
  std::array<glm::vec3, m_numBubbles> m_bubbleRotations;
  
  GameData m_gameData;

  std::array<glm::vec3, m_numCorals> m_coralPositions;
  std::array<glm::vec3, m_numCorals> m_coralRotations;

  float m_angle{};

  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};
  float m_FOV{8.0f};

  ImFont* m_font_final{};
  ImFont* m_font_points{};


  // shark
  abcg::ElapsedTimer m_movimentCoolDownTimer;
  abcg::ElapsedTimer m_movimentTimer;
  
  void restart();

  void randomizeBubble(glm::vec3 &position, glm::vec3 &rotation);
  void randomizeCoral(glm::vec3 &position, glm::vec3 &rotation);
  void update();
};

#endif