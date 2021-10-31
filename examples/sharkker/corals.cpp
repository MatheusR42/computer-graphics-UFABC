#include "corals.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void Corals::initializeGL(GLuint program, int quantity) {
  terminateGL();

  // Start pseudo-random number generator
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Create corals
  m_corals.clear();
  m_corals.resize(quantity);

  for (auto &coral : m_corals) {
    coral = createCorals();

    coral.m_translation = {m_randomDist(m_randomEngine),
                              1.5};
  }
}

void Corals::paintGL() {
  abcg::glUseProgram(m_program);

  for (const auto &coral : m_corals) {
    abcg::glBindVertexArray(coral.m_vao);

    abcg::glUniform4fv(m_colorLoc, 1, &coral.m_color.r);
    abcg::glUniform1f(m_scaleLoc, coral.m_scale);
    abcg::glUniform1f(m_rotationLoc, coral.m_rotation);

    abcg::glUniform2f(m_translationLoc, coral.m_translation.x,
                          coral.m_translation.y);

    abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, coral.m_polygonSides + 2);

    abcg::glBindVertexArray(0);
  }

  abcg::glUseProgram(0);
}

void Corals::terminateGL() {
  for (auto coral : m_corals) {
    abcg::glDeleteBuffers(1, &coral.m_vbo);
    abcg::glDeleteVertexArrays(1, &coral.m_vao);
  }
}

void Corals::update(const Shark &shark, float deltaTime, GameData &gameData) {

  // random time to create the next generation of corals
  float time = m_randomCreateTime(m_randomEngine) * 0.8;
  
  if (m_createCoolDownTimer.elapsed() >= time) {
    m_createCoolDownTimer.restart();

    float scale = m_randomScale(m_randomEngine);

    // quantity of corals increase when you have more points
    int quantity = gameData.points/10 + 1;
    quantity = quantity > 4 ? 4 :  quantity;


    // max quantity of corals
    if (gameData.points > 100)  {
      quantity = 5;
    }

    std::generate_n(std::back_inserter(m_corals), quantity, [&]() {
          return createCorals({m_randomDist(m_randomEngine),m_randomDistY(m_randomEngine)}, scale);
        });
  }

  for (auto &coral : m_corals) {

    // delete corals after they disappear from screen
    if (coral.m_translation.y < -1.5) {
      coral.m_hit = true;
    }

    // set m_point to prevent count twice the same coral
    if (gameData.m_state == State::Playing && !coral.m_point && coral.m_translation.y < -1.2) {
      coral.m_point = true;
      gameData.points = gameData.points + 1;
    }
    
    // change the coral position and rotation
    coral.m_translation -= shark.m_velocity * deltaTime;
    coral.m_rotation = glm::wrapAngle(
        coral.m_rotation + coral.m_angularVelocity * deltaTime);
    coral.m_translation += coral.m_velocity * deltaTime;
  }

  m_corals.remove_if(
      [](const Corals::Coral &a) { return a.m_hit; });
}

Corals::Coral Corals::createCorals(glm::vec2 translation,
                                              float scale) {
  Coral coral;

  auto &re{m_randomEngine};  // Shortcut

  // Randomly choose the number of sides
  std::uniform_int_distribution<int> randomSides(50, 70);
  coral.m_polygonSides = randomSides(m_randomEngine);

  // Choose a random color (actually, a grayscale)
  std::uniform_real_distribution<float> randomIntensity(0.5f, 1.0f);
  coral.m_color = glm::vec4(1.0f, 0.6f, 0.6f, 1) * randomIntensity(re);

  coral.m_color.a = 0.5f;
  coral.m_rotation = 0.0f;
  coral.m_scale = scale;
  coral.m_translation = translation;

  // Choose a random angular velocity
  coral.m_angularVelocity = m_randomDist(re);

  // Choose a random direction
  // glm::vec2 direction{m_randomDist(re), m_randomDist(re)};
  // coral.m_velocity = glm::normalize(direction) / 7.0f;
  coral.m_velocity = glm::vec2(0);


  // Create geometry
  std::vector<glm::vec2> positions(0);
  positions.emplace_back(0, 0);
  const auto step{M_PI * 2 / coral.m_polygonSides};
  std::uniform_real_distribution<float> randomRadius(0.5f, 1.0f);
  for (const auto angle : iter::range(0.0, M_PI * 2, step)) {
    const auto radius{randomRadius(re)};
    positions.emplace_back(radius * std::cos(angle), radius * std::sin(angle));
  }
  positions.push_back(positions.at(1));

  // Generate VBO
  abcg::glGenBuffers(1, &coral.m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, coral.m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &coral.m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(coral.m_vao);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, coral.m_vbo);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  return coral;
}