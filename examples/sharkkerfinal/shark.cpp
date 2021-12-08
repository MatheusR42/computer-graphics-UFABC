#include "shark.hpp"

void Shark::setDamage() {
  m_nodamage = true;                                      // Shark enters on invincible state when hit
}

void Shark::update(const GameData &gameData) {
  if (m_movimentCoolDownTimer.elapsed() < 35.0/1000.0){    // Shark's axial velocity
    return;
  }
  m_movimentCoolDownTimer.restart();

  // Input Left key
  if (gameData.m_input[static_cast<size_t>(Input::Left)]) {
    if (m_positionX > -.18f) {
      m_positionX = m_positionX - .02;            // Left key sensibility
      m_angleY = 210.0f;
    }
  }

  // Input Right key
  if (gameData.m_input[static_cast<size_t>(Input::Right)]) {
    if (m_positionX < .18f) {
      m_positionX = m_positionX + .02;            // Right key sensibility
      m_angleY = 150.0f;
    }
  }

  // Input Up key
  if (gameData.m_input[static_cast<size_t>(Input::Up)]) {
    if (m_positionY < .18f) {
      m_positionY = m_positionY + .02;            // Up key sensibility
      m_angleX = 26.0f;
    }
  }

  // Input Down key
  if (gameData.m_input[static_cast<size_t>(Input::Down)]) {
    if (m_positionY > -.18f) {
      m_positionY = m_positionY - .02;            // Down key sensibility
      m_angleX = -26.0f;
    }
  }

  if (!gameData.m_input[static_cast<size_t>(Input::Up)] && !gameData.m_input[static_cast<size_t>(Input::Down)]) {
      m_angleX = 0.0f;
  }

  if (!gameData.m_input[static_cast<size_t>(Input::Left)] && !gameData.m_input[static_cast<size_t>(Input::Right)]) {
      if (m_angleY <= 150.0f || m_angleY >= 210.0f) {
        m_angleY = 180.0f;
      }

      if (m_angleY < m_angleYBase - 10.0f) {
        m_shakeDirection = 'R';
      } else if (m_angleY > m_angleYBase + 10.0f){
        m_shakeDirection = 'L';
      }

      if (m_shakeDirection == 'R') {
        m_angleY = m_angleY + 2;
      } else {
        m_angleY = m_angleY - 2;
      }
  }
}
