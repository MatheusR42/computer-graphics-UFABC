#ifndef SHARK_HPP_
#define SHARK_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;


class Shark {
 public:
  void setDamage();

  void update(const GameData &gameData);

 private:
  friend OpenGLWindow;

  float m_positionX{0.0f};
  float m_positionY{0.0f};
  float m_positionZ{-3.0f};

  float m_angleX{90.0f};
  float m_angleY{180.0f};
  float m_angleZBase{180.0f};
  float m_angleZ{180.0f};
  
  bool m_nodamage{false};
  char m_shakeDirection{'L'};

  
  // GLuint m_program{};

  // glm::vec4 m_color{0.6f, 0.6f, 0.6f, 1};     // Shark color = R153 G153 B153 A255 = Grey
  // glm::vec2 m_velocity{glm::vec2(0)};

  abcg::ElapsedTimer m_movimentCoolDownTimer;
  abcg::ElapsedTimer m_movimentTimer;
  abcg::ElapsedTimer m_shakeCoolDownTimer;
  abcg::ElapsedTimer m_shakeTimer;
  // abcg::ElapsedTimer m_noDamageBlink;
};

#endif