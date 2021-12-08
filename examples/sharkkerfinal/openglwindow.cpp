#include "openglwindow.hpp"

#include <imgui.h>

#include "abcg.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtc/matrix_inverse.hpp>

// From https://github.com/AirGuanZ/imgui-filebrowser
#include "imfilebrowser.h"

void OpenGLWindow::handleEvent(SDL_Event& event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(static_cast<size_t>(Input::Right));
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.set(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
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
  abcg::glClearColor(0, 0.32, 0.492, 1);      // Background color (Sea) R000 G081 B125
  
  // Load cubemap
  m_model.loadCubeTexture(getAssetsPath() + "maps/cube/");

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Load a new font
  ImGuiIO &io{ImGui::GetIO()};
  const auto filename{getAssetsPath() + "Findet-Nemo.ttf"};
  m_font_final = io.Fonts->AddFontFromFileTTF(filename.c_str(), 30.0f);
  if (m_font_final == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }

  m_font_points = io.Fonts->AddFontFromFileTTF(filename.c_str(), 28.0f);
  if (m_font_points == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }

  // Create programs
  for (const auto& name : m_shaderNames) {
    const auto path{getAssetsPath() + "shaders/" + name};
    const auto program{createProgramFromFile(path + ".vert", path + ".frag")};
    m_programs.push_back(program);
  }

  // Load model
  m_modelBubble.loadObj(getAssetsPath() + "bubble.obj");
  m_modelShark.loadObj(getAssetsPath() + "shark.obj");
  m_modelCoral.loadObj(getAssetsPath() + "coral.obj");

  m_currentProgramIndex = 1;
  m_modelBubble.setupVAO(m_programs.at(m_currentProgramIndex));
  m_modelShark.setupVAO(m_programs.at(m_currentProgramIndex));
  m_modelCoral.setupVAO(m_programs.at(m_currentProgramIndex));

  // Use material properties from the loaded model
  // m_Ka = m_modelShark.getKa();
  // m_Kd = m_modelShark.getKd();
  // m_Ks = m_modelShark.getKs();
  // m_shininess = m_modelShark.getShininess();

  // loadModel(getAssetsPath() + "shark.obj");
 

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

void OpenGLWindow::randomizeBubble(glm::vec3 &position, glm::vec3 &rotation) {
  // Get random position
  // x and y coordinates in the range [-20, 20]
  // z coordinates in the range [-100, 0]
  std::uniform_real_distribution<float> distPosXY(-8.0f, 8.0f);
  std::uniform_real_distribution<float> distPosZ(-80.0f, 0.0f);

  position = glm::vec3(distPosXY(m_randomEngine), distPosXY(m_randomEngine),
                       distPosZ(m_randomEngine));

  //  Get random rotation axis
  std::uniform_real_distribution<float> distRotAxis(-1.0f, 1.0f);

  rotation = glm::normalize(glm::vec3(distRotAxis(m_randomEngine),
                                      distRotAxis(m_randomEngine),
                                      distRotAxis(m_randomEngine)));
}

void OpenGLWindow::randomizeCoral(glm::vec3 &position, glm::vec3 &rotation) {
  // Get random position
  // x and y coordinates in the range [-20, 20]
  // z coordinates in the range [-100, 0]
  std::uniform_real_distribution<float> distPosXY(-0.3f, 0.3f);
  std::uniform_real_distribution<float> distPosZ(-100.0f, 0.0f);

  position = glm::vec3(distPosXY(m_randomEngine), distPosXY(m_randomEngine),
                       distPosZ(m_randomEngine));

  //  Get random rotation axis
  std::uniform_real_distribution<float> distRotAxis(-1.0f, 1.0f);

  rotation = glm::normalize(glm::vec3(distRotAxis(m_randomEngine),
                                      distRotAxis(m_randomEngine),
                                      distRotAxis(m_randomEngine)));
}

// void OpenGLWindow::loadModel(std::string_view path) {
//   m_model.terminateGL();

//   m_model.loadObj(path);
//   m_model.setupVAO(m_programs.at(m_currentProgramIndex));

  // // Use material properties from the loaded model
  // m_Ka = m_model.getKa();
  // m_Kd = m_model.getKd();
  // m_Ks = m_model.getKs();
  // m_shininess = m_model.getShininess();
// }

void OpenGLWindow::paintGLSpherical() {
  m_currentProgramIndex = 1;
  m_mappingMode = 2;  // "From mesh" option

  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  // Use currently selected program
  const auto program{m_programs.at(m_currentProgramIndex)};
  abcg::glUseProgram(program);

  // Get location of uniform variables
  const GLint viewMatrixLoc{abcg::glGetUniformLocation(program, "viewMatrix")};
  const GLint projMatrixLoc{abcg::glGetUniformLocation(program, "projMatrix")};
  const GLint modelMatrixLoc{
      abcg::glGetUniformLocation(program, "modelMatrix")};
  const GLint normalMatrixLoc{
      abcg::glGetUniformLocation(program, "normalMatrix")};
  const GLint lightDirLoc{
      abcg::glGetUniformLocation(program, "lightDirWorldSpace")};
  const GLint shininessLoc{abcg::glGetUniformLocation(program, "shininess")};
  const GLint IaLoc{abcg::glGetUniformLocation(program, "Ia")};
  const GLint IdLoc{abcg::glGetUniformLocation(program, "Id")};
  const GLint IsLoc{abcg::glGetUniformLocation(program, "Is")};
  const GLint KaLoc{abcg::glGetUniformLocation(program, "Ka")};
  const GLint KdLoc{abcg::glGetUniformLocation(program, "Kd")};
  const GLint KsLoc{abcg::glGetUniformLocation(program, "Ks")};
  const GLint diffuseTexLoc{abcg::glGetUniformLocation(program, "diffuseTex")};
  const GLint normalTexLoc{abcg::glGetUniformLocation(program, "normalTex")};
  const GLint mappingModeLoc{
      abcg::glGetUniformLocation(program, "mappingMode")};

  // Set uniform variables used by every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
  abcg::glUniform1i(diffuseTexLoc, 0);
  abcg::glUniform1i(normalTexLoc, 1);
  abcg::glUniform1i(mappingModeLoc, m_mappingMode);

  const auto lightDirRotated{m_trackBallLight.getRotation() * m_lightDir};
  abcg::glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);
  
  abcg::glUniform1f(shininessLoc, m_shininess);
  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);

  // abcg::glUniform1f(shininessLoc, m_shininess);
  // abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  // abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  // abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);
  
  // Render each bubble
  for (const auto index : iter::range(m_numBubbles)) {
    // TODO Bubbles color
    // abcg::glUniform4f(colorLoc, 0.0f, 0.6f, 0.8f, 0.5f);  
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
    // TODO Corals color
    // abcg::glUniform4f(colorLoc, 1.0f, 0.6f, 0.6f, 0.5f);

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

  m_currentProgramIndex = 1;
  m_mappingMode = 3;  // "From mesh" option

  // Use currently selected program
  const auto program{m_programs.at(m_currentProgramIndex)};
  abcg::glUseProgram(program);

  // Get location of uniform variables
  const GLint viewMatrixLoc{abcg::glGetUniformLocation(program, "viewMatrix")};
  const GLint projMatrixLoc{abcg::glGetUniformLocation(program, "projMatrix")};
  const GLint modelMatrixLoc{
      abcg::glGetUniformLocation(program, "modelMatrix")};
  const GLint normalMatrixLoc{
      abcg::glGetUniformLocation(program, "normalMatrix")};
  const GLint lightDirLoc{
      abcg::glGetUniformLocation(program, "lightDirWorldSpace")};
  const GLint shininessLoc{abcg::glGetUniformLocation(program, "shininess")};
  const GLint IaLoc{abcg::glGetUniformLocation(program, "Ia")};
  const GLint IdLoc{abcg::glGetUniformLocation(program, "Id")};
  const GLint IsLoc{abcg::glGetUniformLocation(program, "Is")};
  const GLint KaLoc{abcg::glGetUniformLocation(program, "Ka")};
  const GLint KdLoc{abcg::glGetUniformLocation(program, "Kd")};
  const GLint KsLoc{abcg::glGetUniformLocation(program, "Ks")};
  const GLint diffuseTexLoc{abcg::glGetUniformLocation(program, "diffuseTex")};
  const GLint normalTexLoc{abcg::glGetUniformLocation(program, "normalTex")};
  const GLint mappingModeLoc{
      abcg::glGetUniformLocation(program, "mappingMode")};

  // Set uniform variables used by every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
  abcg::glUniform1i(diffuseTexLoc, 0);
  abcg::glUniform1i(normalTexLoc, 1);
  abcg::glUniform1i(mappingModeLoc, m_mappingMode);

  const auto lightDirRotated{m_trackBallLight.getRotation() * m_lightDir};
  
  abcg::glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);

  if (!m_shark.m_nodamage) {
    abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  } else {
    glm::vec4 m_Ia{0.9f, 0.0f, 0.0f, 0.0f};
    abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  }
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);
  
  abcg::glUniform1f(shininessLoc, m_shininess);
  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);

  // abcg::glUniform1f(shininessLoc, m_shininess);
  // abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  // abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  // abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);
  
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

  // Create window for light sources
  // if (m_currentProgramIndex < 4) {
  //   const auto widgetSize{ImVec2(222, 244)};
  //   ImGui::SetNextWindowPos(ImVec2(m_viewportWidth - widgetSize.x - 5,
  //                                  m_viewportHeight - widgetSize.y - 5));
  //   ImGui::SetNextWindowSize(widgetSize);
  //   ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoDecoration);

  //   ImGui::Text("Light properties");

  //   // Slider to control light properties
  //   ImGui::PushItemWidth(widgetSize.x - 36);
  //   ImGui::ColorEdit3("Ia", &m_Ia.x, ImGuiColorEditFlags_Float);
  //   ImGui::ColorEdit3("Id", &m_Id.x, ImGuiColorEditFlags_Float);
  //   ImGui::ColorEdit3("Is", &m_Is.x, ImGuiColorEditFlags_Float);
  //   ImGui::PopItemWidth();

  //   ImGui::Spacing();

  //   ImGui::Text("Material properties");

  //   // Slider to control material properties
  //   ImGui::PushItemWidth(widgetSize.x - 36);
  //   ImGui::ColorEdit3("Ka", &m_Ka.x, ImGuiColorEditFlags_Float);
  //   ImGui::ColorEdit3("Kd", &m_Kd.x, ImGuiColorEditFlags_Float);
  //   ImGui::ColorEdit3("Ks", &m_Ks.x, ImGuiColorEditFlags_Float);
  //   ImGui::PopItemWidth();

  //   // Slider to control the specular shininess
  //   ImGui::PushItemWidth(widgetSize.x - 16);
  //   ImGui::SliderFloat("", &m_shininess, 0.0f, 500.0f, "shininess: %.1f");
  //   ImGui::PopItemWidth();

  //   ImGui::End();
  // }
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
  for (const auto& program : m_programs) {
    abcg::glDeleteProgram(program);
  }
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
      // position.z = -100.0f;  // Back to -100
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

    if (distance < .15f && distanceZ < 4.0f && m_gameData.m_state == State::Playing && m_gameData.m_lifeCooldown.elapsed() > 1) {
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