#include "ship.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Ship::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  m_rotation = 0.0f;
  m_translation = glm::vec2{0, -.8f};
  m_velocity = glm::vec2{0, .5f};

  std::array<glm::vec2, 22> positions{
      // Ship body
      glm::vec2{-02.5f, +17.5f}, // 0
      glm::vec2{+02.5f, +17.5f}, // 1
      glm::vec2{00.0f, -13.0f}, // 2 - Body - Central
      glm::vec2{-04.5f, +03.5f}, // 3 - Body - Left
      glm::vec2{+04.5f, +03.5f}, // 4 - Body - Right
      glm::vec2{-05.0f, +09.5f}, // 5 - Body - "Shoulder" Left
      glm::vec2{+05.0f, +09.5f}, // 6 - Body - "Shoulder" Right
      glm::vec2{-06.0f, +04.5f}, // 7
      glm::vec2{+06.0f, +04.5f}, // 8
      glm::vec2{-10.0f, +02.5f}, // 9
      glm::vec2{+10.0f, +02.5f}, // 10
      glm::vec2{00.0f, -06.5f}, // 11
      glm::vec2{00.0f, -06.5f}, // 12
      glm::vec2{00.0f, -08.5f}, // 13
      glm::vec2{00.0f, -08.5f}, // 14
      glm::vec2{00.0f, -09.5f}, // 15
      glm::vec2{00.0f, -09.5f}, // 16
      glm::vec2{00.0f, -11.5f}, // 17
      glm::vec2{00.0f, -11.5f}, // 18
      glm::vec2{-03.0f, -19.5f}, // 19
      glm::vec2{+03.0f, -19.5f}, // 20
      glm::vec2{00.0f, -17.5f}, // 21
      };

  // Normalize
  for (auto &position : positions) {
    position /= glm::vec2{19.5f, 19.5f};
  }

  const std::array indices{2, 1, 0,
                            2, 3, 0,
                            2, 4, 1,
                            3, 5, 0,
                            4, 6, 1,
                            3, 7, 5,
                            4, 8, 6,
                            7, 9, 5,
                            8, 10, 6,
                            13, 15, 11,
                            14, 16, 12,
                            2, 17, 19,
                            2, 18, 20,
                            2, 19, 21,
                            2, 20, 21,
                            };

  // Generate VBO
  abcg::glGenBuffers(1, &m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_ebo);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Ship::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_vao);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform1f(m_rotationLoc, m_rotation);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);

  // Restart thruster blink timer every 100 ms
  if (m_trailBlinkTimer.elapsed() > 1.0 / 1000.0) m_trailBlinkTimer.restart();

  if (gameData.m_input[static_cast<size_t>(Input::Up)]) {
    // Show thruster trail during 50 ms
    if (m_trailBlinkTimer.elapsed() < 500.0 / 1000.0) {
      abcg::glEnable(GL_BLEND);
      abcg::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      // 50% transparent
      abcg::glUniform4f(m_colorLoc, 1, 1, 1, 0.5f);

      abcg::glDrawElements(GL_TRIANGLES, 14 * 3, GL_UNSIGNED_INT, nullptr);

      abcg::glDisable(GL_BLEND);
    }
  }

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Ship::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteBuffers(1, &m_ebo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

void Ship::update(const GameData &gameData, float deltaTime) {
  m_velocity.y = m_velocity.y + deltaTime * .1;

  if (m_movimentCoolDownTimer.elapsed() < 35.0 / 1000.0) {
    return;
  }

  m_movimentCoolDownTimer.restart();

  // Rotate
  if (gameData.m_input[static_cast<size_t>(Input::Left)]) {
    if (m_translation.x > -1) {
      m_translation.x = m_translation.x - .1;
    }
    // m_translation = glm::vec2{-.3f, 0};
  }

  // m_rotation = glm::wrapAngle(m_rotation + 4.0f * deltaTime);
  if (gameData.m_input[static_cast<size_t>(Input::Right)]) {
    if (m_translation.x < 1) {
      m_translation.x = m_translation.x + .1;
    }
  }

    // m_translation = glm::vec2{.3f, 0};

  // Apply thrust
  if (gameData.m_input[static_cast<size_t>(Input::Up)] &&
      gameData.m_state == State::Playing) {
    // Thrust in the forward vector
    glm::vec2 forward = glm::rotate(glm::vec2{0.0f, 1.0f}, m_rotation);
    m_velocity += forward * deltaTime;
  }
}