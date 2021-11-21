#include "shark.hpp"

void Shark::setDamage() {
  m_nodamage = true;                                      // Shark enters on invincible state when hit
}

void Shark::update(const GameData &gameData) {
  // auto grey = glm::vec4{0.6f, 0.6f, 0.6f, 1.0f};          // R153 G153 B153 A255
  // auto damageColor = glm::vec4{1.0f, 0.0f, 0.0f, 1.0f};   // R255 G000 B000 A255

  // if (m_nodamage) {
  //   m_color = damageColor;                                // Shark turns Red when hit
  // } else {
  //   m_color = grey;                                       // Shark turns Grey when hit
  // }
  
  // if (m_movimentTimer.elapsed() > 3) {                    // Every X seconds the shark accelerates forward
  //     m_positionY = m_positionY + .1;                   // Shark's acceleration
  //     m_movimentTimer.restart();
  // }

  if (m_movimentCoolDownTimer.elapsed() < 35.0/1000.0){    // Shark's axial velocity
    return;
  }
  m_movimentCoolDownTimer.restart();

  // Input Left key
  if (gameData.m_input[static_cast<size_t>(Input::Left)]) {
    if (m_positionX > -1) {
      m_positionX = m_positionX - .02;            // Left key sensibility
    }
  }

  // Input Right key
  if (gameData.m_input[static_cast<size_t>(Input::Right)]) {
    if (m_positionX < 1) {
      m_positionX = m_positionX + .02;            // Right key sensibility
    }
  }

  // Input Up key
  if (gameData.m_input[static_cast<size_t>(Input::Up)]) {
    if (m_positionY < 1) {
      m_positionY = m_positionY + .02;            // Up key sensibility
    }
  }

  // Input Down key
  if (gameData.m_input[static_cast<size_t>(Input::Down)]) {
    if (m_positionY < 1) {
      m_positionY = m_positionY - .02;            // Down key sensibility
    }
  }
}
