#include "asteroids.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void Asteroids::initializeGL(GLuint program, int quantity) {
  terminateGL();

  // Start pseudo-random number generator
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Create asteroids
  m_asteroids.clear();
  m_asteroids.resize(quantity);

  for (auto &asteroid : m_asteroids) {
    asteroid = createTrash();

    asteroid.m_translation = {m_randomDist(m_randomEngine),
                              1.5};
  }
}

void Asteroids::paintGL() {
  abcg::glUseProgram(m_program);

  for (const auto &asteroid : m_asteroids) {
    abcg::glBindVertexArray(asteroid.m_vao);

    abcg::glUniform4fv(m_colorLoc, 1, &asteroid.m_color.r);
    abcg::glUniform1f(m_scaleLoc, asteroid.m_scale);
    abcg::glUniform1f(m_rotationLoc, asteroid.m_rotation);

    abcg::glUniform2f(m_translationLoc, asteroid.m_translation.x,
                          asteroid.m_translation.y);

    abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, asteroid.m_polygonSides + 2);

    abcg::glBindVertexArray(0);
  }

  abcg::glUseProgram(0);
}

void Asteroids::terminateGL() {
  for (auto asteroid : m_asteroids) {
    abcg::glDeleteBuffers(1, &asteroid.m_vbo);
    abcg::glDeleteVertexArrays(1, &asteroid.m_vao);
  }
}

void Asteroids::update(const Ship &ship, float deltaTime, GameData &gameData) {
  float time = m_randomCreateTime(m_randomEngine);
  
  if (m_createCoolDownTimer.elapsed() >= time) {
    m_createCoolDownTimer.restart();

    float scale = m_randomScale(m_randomEngine);

    std::generate_n(std::back_inserter(m_asteroids), 1, [&]() {
          return createTrash({m_randomDist(m_randomEngine),1.5}, scale);
        });
  }

  for (auto &asteroid : m_asteroids) {
    if (asteroid.m_translation.y < -1.5) {
      asteroid.m_hit = true;
    }

    if (gameData.m_state == State::Playing && !asteroid.m_point && asteroid.m_translation.y < -1.2) {
      asteroid.m_point = true;
      gameData.points = gameData.points + 1;
    }
    
    asteroid.m_translation -= ship.m_velocity * deltaTime;
    asteroid.m_rotation = glm::wrapAngle(
        asteroid.m_rotation + asteroid.m_angularVelocity * deltaTime);
    asteroid.m_translation += asteroid.m_velocity * deltaTime;
  }

  m_asteroids.remove_if(
      [](const Asteroids::Asteroid &a) { return a.m_hit; });
}

Asteroids::Asteroid Asteroids::createTrash(glm::vec2 translation,
                                              float scale) {
  Asteroid asteroid;

  auto &re{m_randomEngine};  // Shortcut

  // Randomly choose the number of sides
  std::uniform_int_distribution<int> randomSides(6, 20);
  asteroid.m_polygonSides = 20;

  // Choose a random color (actually, a grayscale)
  std::uniform_real_distribution<float> randomIntensity(0.5f, 1.0f);
  asteroid.m_color = glm::vec4(0, 1, 1, 1) * randomIntensity(re);

  asteroid.m_color.a = 0.5f;
  asteroid.m_rotation = 0.0f;
  asteroid.m_scale = scale;
  asteroid.m_translation = translation;

  // Choose a random angular velocity
  asteroid.m_angularVelocity = m_randomDist(re);

  // Choose a random direction
  // glm::vec2 direction{m_randomDist(re), m_randomDist(re)};
  // asteroid.m_velocity = glm::normalize(direction) / 7.0f;
  asteroid.m_velocity = glm::vec2(0);


  // Create geometry
  std::vector<glm::vec2> positions(0);
  positions.emplace_back(0, 0);
  const auto step{M_PI * 2 / asteroid.m_polygonSides};
  std::uniform_real_distribution<float> randomRadius(0.8f, 1.0f);
  for (const auto angle : iter::range(0.0, M_PI * 2, step)) {
    const auto radius{randomRadius(re)};
    positions.emplace_back(radius * std::cos(angle), radius * std::sin(angle));
  }
  positions.push_back(positions.at(1));

  // Generate VBO
  abcg::glGenBuffers(1, &asteroid.m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, asteroid.m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &asteroid.m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(asteroid.m_vao);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, asteroid.m_vbo);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  return asteroid;
}