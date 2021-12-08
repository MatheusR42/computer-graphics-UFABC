#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "model.hpp"
#include "shark.hpp"
#include "gamedata.hpp"
#include "trackball.hpp"

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
  static const int m_numCorals{10};

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

  Model m_model;
  int m_trianglesToDraw{};

  TrackBall m_trackBallModel;
  TrackBall m_trackBallLight;
  float m_zoom{};

  glm::mat4 m_modelMatrix{1.0f};
  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};
  float m_FOV{8.0f};

  ImFont* m_font_final{};
  ImFont* m_font_points{};


  // shark
  abcg::ElapsedTimer m_movimentCoolDownTimer;
  abcg::ElapsedTimer m_movimentTimer;

  // Shaders
  std::vector<const char*> m_shaderNames{
      "normalmapping", "texture", "blinnphong", "phong",
      "gouraud",       "normal",  "depth"};
  std::vector<GLuint> m_programs;
  int m_currentProgramIndex{1};

  // Mapping mode
  // 0: triplanar; 1: cylindrical; 2: spherical; 3: from mesh
  int m_mappingMode{};

  // Light and material properties
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{0.7f};
  glm::vec4 m_Kd{0.41f, 0.36f, 0.36f, 1.0f};
  glm::vec4 m_Ks{0.7f};
  float m_shininess{265.0f};

  void loadModel(std::string_view path);
  void restart();

  void randomizeBubble(glm::vec3 &position, glm::vec3 &rotation);
  void randomizeCoral(glm::vec3 &position, glm::vec3 &rotation);
  void update();
  void paintGLSpherical();
};

#endif