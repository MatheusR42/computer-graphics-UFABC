#ifndef SHARK_HPP_
#define SHARK_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

class Corals;
class OpenGLWindow;
class BubbleLayers;

class Shark {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void update(const GameData &gameData);
  void setDamage();

 private:
  friend Corals;
  friend OpenGLWindow;
  friend BubbleLayers;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};
  GLint m_rotationLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  glm::vec4 m_color{0.6f, 0.6f, 0.6f, 1};     // Shark color = R153 G153 B153 A255 = Grey
  float m_rotation{};
  float m_scale{0.125f};
  bool m_nodamage{false};
  glm::vec2 m_translation{glm::vec2(0)};
  glm::vec2 m_velocity{glm::vec2(0)};

  abcg::ElapsedTimer m_movimentCoolDownTimer;
  abcg::ElapsedTimer m_movimentTimer;
  abcg::ElapsedTimer m_noDamageBlink;
};

#endif