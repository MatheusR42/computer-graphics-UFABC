#include "openglwindow.hpp"

#include <imgui.h>

#include "abcg.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtc/matrix_inverse.hpp>


void OpenGLWindow::handleEvent(SDL_Event& event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)    // Input Left Key
      m_gameData.m_input.set(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)   // Input Right Key
      m_gameData.m_input.set(static_cast<size_t>(Input::Right));
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)       // Input Up Key
      m_gameData.m_input.set(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)     // Input Down Key
      m_gameData.m_input.set(static_cast<size_t>(Input::Down));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Right));
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Down));
  }
}

void OpenGLWindow::initializeGL() { 
  abcg::glClearColor(0, 0.32, 0.492, 1);                         // Background color (Sea) R000 G081 B125
  
  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Load a new font
  ImGuiIO &io{ImGui::GetIO()};
  const auto filename{getAssetsPath() + "Findet-Nemo.ttf"};          // Load "Finding Nemo" font from .ttf (TrueType Font) file
  m_font_final = io.Fonts->AddFontFromFileTTF(filename.c_str(), 30.0f);
  if (m_font_final == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }

  m_font_points = io.Fonts->AddFontFromFileTTF(filename.c_str(), 28.0f);
  if (m_font_points == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }

  const auto path{getAssetsPath() + "shaders/texture"};
  const auto program{createProgramFromFile(path + ".vert", path + ".frag")};
  m_program = program;

  // Load model
  m_modelBubble.loadObj(getAssetsPath() + "bubble.obj");          // Load background bubbles from .obj file
  m_modelShark.loadObj(getAssetsPath() + "shark.obj");            // Load Shark from .obj file
  m_modelCoral.loadObj(getAssetsPath() + "coral.obj");            // Load Corals/obstacles from .obj file

  m_modelBubble.setupVAO(m_program);
  m_modelShark.setupVAO(m_program);
  m_modelCoral.setupVAO(m_program);

  // Camera at (0,0,0) and looking towards the negative z
  m_viewMatrix =
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f));


  // Setup bubbles & corals
  for (const auto index : iter::range(m_numBubbles)) {
    auto &position{m_bubblePositions.at(index)};
    auto &rotation{m_bubbleRotations.at(index)};

    randomizeBubble(position, rotation);
  }
  for (const auto index : iter::range(m_numCorals)) {
    auto &position{m_coralPositions.at(index)};
    auto &rotation{m_coralRotations.at(index)};

    randomizeCoral(position, rotation);
  }
}

// Bubble spawn randomizer
void OpenGLWindow::randomizeBubble(glm::vec3 &position, glm::vec3 &rotation) {
  // Get random position
  std::uniform_real_distribution<float> distPosXY(-8.0f, 8.0f);     // x and y coordinates in the range [-8, 8] (shark's sides/background)
  std::uniform_real_distribution<float> distPosZ(-80.0f, 0.0f);     // z coordinates in the range [-80, 0]

  position = glm::vec3(distPosXY(m_randomEngine), distPosXY(m_randomEngine),
                       distPosZ(m_randomEngine));

  //  Get random rotation axis
  std::uniform_real_distribution<float> distRotAxis(-1.0f, 1.0f);
  rotation = glm::normalize(glm::vec3(distRotAxis(m_randomEngine),
                                      distRotAxis(m_randomEngine),
                                      distRotAxis(m_randomEngine)));
}

// Coral spawn randomizer
void OpenGLWindow::randomizeCoral(glm::vec3 &position, glm::vec3 &rotation) {
  // Get random position 
  std::uniform_real_distribution<float> distPosXY(-0.3f, 0.3f);   // x and y coordinates in the range [-0.3, 0.3] (shark's frontal corridor)
  std::uniform_real_distribution<float> distPosZ(-100.0f, 0.0f);  // z coordinates in the range [-100, 0]

  position = glm::vec3(distPosXY(m_randomEngine), distPosXY(m_randomEngine),
                       distPosZ(m_randomEngine));

  //  Get random rotation axis
  std::uniform_real_distribution<float> distRotAxis(-1.0f, 1.0f);
  rotation = glm::normalize(glm::vec3(distRotAxis(m_randomEngine),
                                      distRotAxis(m_randomEngine),
                                      distRotAxis(m_randomEngine)));
}

void OpenGLWindow::paintGLSpherical() {
  m_mappingMode = 2;  // "From mesh" option

  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  // Use currently selected program
  abcg::glUseProgram(m_program);

  // Get location of uniform variables
  const GLint viewMatrixLoc{abcg::glGetUniformLocation(m_program, "viewMatrix")};
  const GLint projMatrixLoc{abcg::glGetUniformLocation(m_program, "projMatrix")};
  const GLint modelMatrixLoc{
      abcg::glGetUniformLocation(m_program, "modelMatrix")};
  const GLint normalMatrixLoc{
      abcg::glGetUniformLocation(m_program, "normalMatrix")};
  const GLint lightDirLoc{
      abcg::glGetUniformLocation(m_program, "lightDirWorldSpace")};
  const GLint shininessLoc{abcg::glGetUniformLocation(m_program, "shininess")};
  const GLint IaLoc{abcg::glGetUniformLocation(m_program, "Ia")};
  const GLint IdLoc{abcg::glGetUniformLocation(m_program, "Id")};
  const GLint IsLoc{abcg::glGetUniformLocation(m_program, "Is")};
  const GLint KaLoc{abcg::glGetUniformLocation(m_program, "Ka")};
  const GLint KdLoc{abcg::glGetUniformLocation(m_program, "Kd")};
  const GLint KsLoc{abcg::glGetUniformLocation(m_program, "Ks")};
  const GLint diffuseTexLoc{abcg::glGetUniformLocation(m_program, "diffuseTex")};
  const GLint normalTexLoc{abcg::glGetUniformLocation(m_program, "normalTex")};
  const GLint mappingModeLoc{
      abcg::glGetUniformLocation(m_program, "mappingMode")};

  // Set uniform variables used by every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
  abcg::glUniform1i(diffuseTexLoc, 0);
  abcg::glUniform1i(normalTexLoc, 1);
  abcg::glUniform1i(mappingModeLoc, m_mappingMode);
  abcg::glUniform4fv(lightDirLoc, 1, &m_lightDir.x);
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);
  
  abcg::glUniform1f(shininessLoc, m_shininess);
  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);

  
  // Render each bubble
  for (const auto index : iter::range(m_numBubbles)) {
    const auto &position{m_bubblePositions.at(index)};
    const auto &rotation{m_bubbleRotations.at(index)};

    // Compute model matrix of the current bubble
    glm::mat4 modelMatrix{0.3f};
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f));
    modelMatrix = glm::rotate(modelMatrix, m_angle, rotation);

    // Set uniform variable
    abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);
    const auto modelViewMatrix{glm::mat3(m_viewMatrix * modelMatrix)};
    glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
    abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

    m_modelBubble.render();
  }

  // Render each coral
  for (const auto index : iter::range(m_numCorals)) {
    const auto &position{m_coralPositions.at(index)};
    const auto &rotation{m_coralRotations.at(index)};

    // Compute model matrix of the current coral
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(.2f));
    modelMatrix = glm::rotate(modelMatrix, m_angle, rotation);

    // Set uniform variable
    abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);
    const auto modelViewMatrix{glm::mat3(m_viewMatrix * modelMatrix)};
    glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
    abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

    m_modelCoral.render();
  }

  abcg::glUseProgram(0);
}


void OpenGLWindow::paintGL() {
  update();
  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  paintGLSpherical();

  m_mappingMode = 3;  // "From mesh" option

  // Use currently selected program
  abcg::glUseProgram(m_program);

  // Get location of uniform variables
  const GLint viewMatrixLoc{abcg::glGetUniformLocation(m_program, "viewMatrix")};
  const GLint projMatrixLoc{abcg::glGetUniformLocation(m_program, "projMatrix")};
  const GLint modelMatrixLoc{
      abcg::glGetUniformLocation(m_program, "modelMatrix")};
  const GLint normalMatrixLoc{
      abcg::glGetUniformLocation(m_program, "normalMatrix")};
  const GLint lightDirLoc{
      abcg::glGetUniformLocation(m_program, "lightDirWorldSpace")};
  const GLint shininessLoc{abcg::glGetUniformLocation(m_program, "shininess")};
  const GLint IaLoc{abcg::glGetUniformLocation(m_program, "Ia")};
  const GLint IdLoc{abcg::glGetUniformLocation(m_program, "Id")};
  const GLint IsLoc{abcg::glGetUniformLocation(m_program, "Is")};
  const GLint KaLoc{abcg::glGetUniformLocation(m_program, "Ka")};
  const GLint KdLoc{abcg::glGetUniformLocation(m_program, "Kd")};
  const GLint KsLoc{abcg::glGetUniformLocation(m_program, "Ks")};
  const GLint diffuseTexLoc{abcg::glGetUniformLocation(m_program, "diffuseTex")};
  const GLint normalTexLoc{abcg::glGetUniformLocation(m_program, "normalTex")};
  const GLint mappingModeLoc{
      abcg::glGetUniformLocation(m_program, "mappingMode")};

  // Set uniform variables used by every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
  abcg::glUniform1i(diffuseTexLoc, 0);
  abcg::glUniform1i(normalTexLoc, 1);
  abcg::glUniform1i(mappingModeLoc, m_mappingMode);
  abcg::glUniform4fv(lightDirLoc, 1, &m_lightDir.x);

  if (!m_shark.m_nodamage) {
    abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  } else {
    glm::vec4 m_Ia{0.9f, 0.0f, 0.0f, 0.0f};           // Turns shark red when hurt
    abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  }
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);
  
  abcg::glUniform1f(shininessLoc, m_shininess);
  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);
 
  // Set uniform variables of the current object
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix[0][0]);

  const auto modelViewMatrix{glm::mat3(m_viewMatrix * m_modelMatrix)};
  glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

  if (m_gameData.m_state == State::Playing) {
    // Compute model matrix of the current bubble
    glm::mat4 modelMatrix{10.0f};

    auto position = glm::vec3(m_shark.m_positionX / 100.0f, m_shark.m_positionY / 100.0f, m_shark.m_positionZ / 100.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(m_shark.m_angleX), glm::vec3(1.f, 0.f, 0.f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(m_shark.m_angleY), glm::vec3(0.f, 1.f, 0.f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(m_shark.m_angleZ), glm::vec3(0.f, 0.f, 1.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.001f));

    // Set uniform variable
    abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);
    
    m_modelShark.render();
  }

  abcg::glUseProgram(0);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  {
    const auto size{ImVec2(340, 85)};
    const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                               (m_viewportHeight - size.y) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font_final);

    // "Game Over" message display
    if (m_gameData.m_state == State::GameOver && 
      m_Timer.elapsed() < 4) {
      ImGui::Text("       Game Over\nContinue a nadar.");

    // Display developers name after "Game Over" message
    }
    if (m_gameData.m_state == State::GameOver && 
      m_Timer.elapsed() > 4) {
      ImGui::Text(" Matheus Araujo\nGiovanne Galdino");
    }
    // "Sharkker" inicial message display
    if (m_gameData.m_state == State::Playing && 
      m_Timer.elapsed() < 4) {                    // Text display time
      ImGui::Text("         Sharkker");
    }
  }

  ImGui::PopFont();
  ImGui::End();
  
  // "Points" Display
  {
    const auto position{ImVec2((16),
                                (16))};
    ImGui::SetNextWindowSize(ImVec2(190, 100));
    ImGui::SetNextWindowPos(position);
    auto flags{ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize};
    ImGui::Begin("points", nullptr, flags);

    // Menu Bar
    if (ImGui::BeginMenuBar()) {
      // File menu
      ImGui::EndMenuBar();
    }

    ImGui::PushFont(m_font_points);
    ImGui::Text("Points: %d", m_gameData.points);
    ImGui::PopFont();
    ImGui::End();
  }

  // "Lifes" Display
  {
    const auto position{ImVec2((m_viewportWidth - 140),
                                (16))};
    ImGui::SetNextWindowSize(ImVec2(190, 100));
    ImGui::SetNextWindowPos(position);
    auto flags{ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize};
    ImGui::Begin("lifes", nullptr, flags);

    // Menu Bar
    if (ImGui::BeginMenuBar()) {
      // File menu
      ImGui::EndMenuBar();
    }

    ImGui::PushFont(m_font_points);
    ImGui::Text("Lifes: %d", m_gameData.lifes);
    ImGui::PopFont();
    ImGui::End();
  }

  {
    const auto aspect{static_cast<float>(m_viewportWidth) /
                        static_cast<float>(m_viewportHeight)};

    m_projMatrix = glm::perspective(glm::radians(m_FOV), aspect, 0.01f, 100.0f);
  }
}

void OpenGLWindow::restart() {
  m_gameData.m_state = State::Playing;
  m_gameData.points = 0;
  m_gameData.lifes = 3;
  m_shark.m_positionX = 0.0f;
  m_shark.m_positionY = 0.0f;

  std::uniform_real_distribution<float> distPosZ(-100.0f, -50.0f);

  for (const auto index : iter::range(m_numCorals)) {
    auto &position{m_coralPositions.at(index)};
    auto &rotation{m_coralRotations.at(index)};

    randomizeCoral(position, rotation);
    position.z = distPosZ(m_randomEngine);  
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;
}

void OpenGLWindow::terminateGL() {
  m_modelBubble.terminateGL();
  m_modelShark.terminateGL();
  m_modelCoral.terminateGL();
  abcg::glDeleteProgram(m_program);
}

void OpenGLWindow::update() {
  // Wait 8 seconds before restarting
  if (m_gameData.m_state != State::Playing &&
      m_Timer.elapsed() > 7) {
    restart();
    return;
  }

  m_shark.update(m_gameData);
  // Animate angle by 90 degrees per second
  const float deltaTime{static_cast<float>(getDeltaTime())};
  m_angle = glm::wrapAngle(m_angle + glm::radians(90.0f) * deltaTime);

  // Update bubbles
  for (const auto index : iter::range(m_numBubbles)) {
    auto &position{m_bubblePositions.at(index)};
    auto &rotation{m_bubbleRotations.at(index)};

    // Z coordinate increases by 10 units per second
    position.z += deltaTime * 10.0f;

    // If this bubble is behind the camera, select a new random position and
    // orientation, and move it back to -100
    if (position.z > -1.0f) {
      randomizeBubble(position, rotation);
    }
  }
    // Update corals
  for (const auto index : iter::range(m_numCorals)) {
    auto &position{m_coralPositions.at(index)};
    auto &rotation{m_coralRotations.at(index)};

    // Z coordinate increases by 20 units per second
    position.z += deltaTime * 20.0f;

    // If this coral is behind the camera, select a new random position and
    // orientation, and move it back to -100
    if (position.z > -4.0f) {
      if (m_gameData.m_state == State::Playing) {
        m_gameData.points += 1;
      }

      randomizeCoral(position, rotation);
      position.z = -100.0f;  // Back to -100
    }
  }

  if (m_gameData.m_state == State::Playing) {
    checkCollisions();
  }
}
// Corals as obstacles
void OpenGLWindow::checkCollisions() {
  if (m_gameData.m_lifeCooldown.elapsed() < 1) {
    return;
  }

  m_shark.m_nodamage = false;

  for (const auto index : iter::range(m_numCorals)) {
    auto &position{m_coralPositions.at(index)};
    auto coralTranslation = glm::vec2(position.x, position.y);
    auto sharkTranslation = glm::vec2(m_shark.m_positionX, m_shark.m_positionY);
    const auto distance{
        glm::distance(sharkTranslation, coralTranslation)};

    const auto distanceZ{
        glm::distance(m_shark.m_positionZ, position.z)};

    if (distance < .14f && distanceZ < 4.0f && m_gameData.m_state == State::Playing && m_gameData.m_lifeCooldown.elapsed() > 1) {
      m_shark.setDamage();
      m_gameData.lifes--;
      m_gameData.m_lifeCooldown.restart();

      if (m_gameData.lifes == 0) {
        m_gameData.m_state = State::GameOver;
        m_Timer.restart();
      }
    }
  }
}