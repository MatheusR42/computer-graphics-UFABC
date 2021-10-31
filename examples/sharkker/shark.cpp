#include "shark.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

// Shark
void Shark::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  m_rotation = 0.0f;
  m_translation = glm::vec2{0, -.8f};
  m_velocity = glm::vec2{0, .5f};

  // Shark body
  std::array<glm::vec2, 17> positions{
      glm::vec2{-02.5f, +17.5f},          // 0 - Nose - Left
      glm::vec2{+02.5f, +17.5f},          // 1 - Nose - Right
      glm::vec2{00.0f, -17.5f},           // 2 - Body - Center Inferior
      glm::vec2{00.0f, +03.5f},           // 3 - Body - Center
      glm::vec2{-05.0f, +09.5f},          // 4 - Body - "Shoulder" Left
      glm::vec2{+05.0f, +09.5f},          // 5 - Body - "Shoulder" Right
      glm::vec2{-06.0f, +04.5f},          // 6 - Flipper - "Armpit" Left 
      glm::vec2{+06.0f, +04.5f},          // 7 - Flipper - "Armpit" Right
      glm::vec2{-10.0f, +02.5f},          // 8 - Flipper - Left 
      glm::vec2{+10.0f, +02.5f},          // 9 - Flipper - Right
      glm::vec2{00.0f, -04.5f},           // 10 - Fin - Center
      glm::vec2{00.0f, -10.5f},           // 11 - Fin - Center Inferior
      glm::vec2{-04.5f, -07.5f},          // 12 - Fin - Left
      glm::vec2{04.5f, -07.5f},           // 13 - Fin - Right
      glm::vec2{00.0f, -11.5f},           // 14 - Tail - Center Inferior
      glm::vec2{-03.0f, -19.5f},          // 15 - Tail - Left End
      glm::vec2{00.5f, -17.5f},           // 16 - Tail - Start
      };

  // Normalize
  for (auto &position : positions) {
    position /= glm::vec2{15.0f, 15.0f};  // Shark Size
  }

  // Shark Triangles 
  const std::array indices{2, 1, 0,       // Body Center
                            2, 4, 0,      // Body Left
                            2, 5, 1,      // Body Right
                            3, 4, 0,      // Shoulder Left
                            3, 5, 1,      // Shoulder Right
                            6, 8, 4,      // Flipper Left
                            7, 9, 5,      // Flipper Right   
                            11, 12, 10,   // Fin Left                  
                            11, 13, 10,   // Fin Right                        
                            3, 6, 4,      // Forearm Left
                            3, 7, 5,      // Forearm Right
                            16, 14, 15,   // Tail
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

void Shark::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_vao);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform1f(m_rotationLoc, m_rotation);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);



  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Shark::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteBuffers(1, &m_ebo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

void Shark::setDamage() {
  m_nodamage = true;                                      // Shark enters on invincible state when hit
}

void Shark::update(const GameData &gameData) {
  auto grey = glm::vec4{0.6f, 0.6f, 0.6f, 1.0f};          // R153 G153 B153 A255
  auto damageColor = glm::vec4{1.0f, 0.0f, 0.0f, 1.0f};   // R255 G000 B000 A255

  if (m_nodamage) {
    m_color = damageColor;                                // Shark turns Red when hit
  } else {
    m_color = grey;                                       // Shark turns Grey when hit
  }
  
  if (m_movimentTimer.elapsed() > 3) {                    // Every X seconds the shark accelerates forward
      m_velocity.y = m_velocity.y + .1;                   // Shark's acceleration
      m_movimentTimer.restart();
  }

  if (m_movimentCoolDownTimer.elapsed()< 35.0/1000.0){    // Shark's axial velocity
    return;
  }
  m_movimentCoolDownTimer.restart();

  // Input Left key
  if (gameData.m_input[static_cast<size_t>(Input::Left)]) {
    if (m_translation.x > -1) {
      m_translation.x = m_translation.x - .06;            // Left key sensibility
    }
  }

  // Input Right key
  if (gameData.m_input[static_cast<size_t>(Input::Right)]) {
    if (m_translation.x < 1) {
      m_translation.x = m_translation.x + .06;            // Right key sensibility
    }
  }
}