#ifndef CORALS_HPP_
#define CORALS_HPP_

#include <list>
#include <random>

#include "abcg.hpp"
#include "gamedata.hpp"
#include "shark.hpp"

class OpenGLWindow;

class Corals {
 public:
  void initializeGL(GLuint program, int quantity);
  void paintGL();
  void terminateGL();

  void update(const Shark &shark, float deltaTime, GameData &gameData);

 private:
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_colorLoc{};
  GLint m_rotationLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};

  struct Coral {
    GLuint m_vao{};
    GLuint m_vbo{};

    float m_angularVelocity{};
    glm::vec4 m_color{1};
    bool m_hit{false};
    bool m_point{false};
    int m_polygonSides{};
    float m_rotation{};
    float m_scale{};
    glm::vec2 m_translation{glm::vec2(0)};
    glm::vec2 m_velocity{glm::vec2(0)};
  };

  std::list<Coral> m_corals;

  std::default_random_engine m_randomEngine;
  std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};
  std::uniform_real_distribution<float> m_randomDistY{1.3f, 1.7f};
  std::uniform_real_distribution<float> m_randomCreateTime{0.9f, 2.0f};
  std::uniform_real_distribution<float> m_randomScale{0.1f, .2f};

  Corals::Coral createCorals(glm::vec2 translation = glm::vec2(0),
                                     float scale = 0.25f);

  abcg::ElapsedTimer m_createCoolDownTimer;
  abcg::ElapsedTimer m_movimentTimer;
};

#endif