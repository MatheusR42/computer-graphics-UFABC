#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void OpenGLWindow::initializeGL() {
  abcg::glClearColor(0, 0.32, 0.492, 1);      // Background color (Sea) R000 G081 B125

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program = createProgramFromFile(getAssetsPath() + "depth.vert",
                                    getAssetsPath() + "depth.frag");

  // Load model
  m_modelBubble.loadObj(getAssetsPath() + "bubble.obj");
  m_modelShark.loadObj(getAssetsPath() + "shark.obj");
  m_modelCoral.loadObj(getAssetsPath() + "coral.obj");

  m_modelBubble.setupVAO(m_program);
  m_modelShark.setupVAO(m_program);
  m_modelCoral.setupVAO(m_program);

  // Camera at (0,0,0) and looking towards the negative z
  m_viewMatrix =
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f));


  m_sharkPosition = glm::vec3(0.0f, 0.0f, -1.0f);

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
  std::uniform_real_distribution<float> distPosZ(-100.0f, 0.0f);

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
  std::uniform_real_distribution<float> distPosXY(-8.0f, 8.0f);
  std::uniform_real_distribution<float> distPosZ(-100.0f, 0.0f);

  position = glm::vec3(distPosXY(m_randomEngine), distPosXY(m_randomEngine),
                       distPosZ(m_randomEngine));

  //  Get random rotation axis
  std::uniform_real_distribution<float> distRotAxis(-1.0f, 1.0f);

  rotation = glm::normalize(glm::vec3(distRotAxis(m_randomEngine),
                                      distRotAxis(m_randomEngine),
                                      distRotAxis(m_randomEngine)));
}

void OpenGLWindow::paintGL() {
  update();

  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  abcg::glUseProgram(m_program);

  // Get location of uniform variables (could be precomputed)
  const GLint viewMatrixLoc{
      abcg::glGetUniformLocation(m_program, "viewMatrix")};
  const GLint projMatrixLoc{
      abcg::glGetUniformLocation(m_program, "projMatrix")};
  const GLint modelMatrixLoc{
      abcg::glGetUniformLocation(m_program, "modelMatrix")};
  const GLint colorLoc{abcg::glGetUniformLocation(m_program, "color")};

  // Set uniform variables used by every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);

  // Render each bubble
  for (const auto index : iter::range(m_numBubbles)) {
    abcg::glUniform4f(colorLoc, 0.0f, 0.6f, 0.8f, 0.5f);  // Bubbles color
    const auto &position{m_bubblePositions.at(index)};
    const auto &rotation{m_bubbleRotations.at(index)};

    // Compute model matrix of the current bubble
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
    modelMatrix = glm::rotate(modelMatrix, m_angle, rotation);

    // Set uniform variable
    abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);

    m_modelBubble.render();
  }

  // Render each coral
  for (const auto index : iter::range(m_numCorals)) {
    abcg::glUniform4f(colorLoc, 1.0f, 0.6f, 0.6f, 0.5f);  // Corals color
    const auto &position{m_coralPositions.at(index)};
    const auto &rotation{m_coralRotations.at(index)};

    // Compute model matrix of the current coral
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
    modelMatrix = glm::rotate(modelMatrix, m_angle, rotation);

    // Set uniform variable
    abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);

    m_modelCoral.render();
  }

  // Compute model matrix of the current bubble
  glm::mat4 modelMatrix{1.0f};
  abcg::glUniform4f(colorLoc, 0.6f, 0.6f, 0.6f, 0.5f);  // Shark color
  m_sharkRotation = glm::normalize(glm::vec3(m_sharkRotationX,
                                      m_sharkRotationY,
                                      m_sharkRotationZ));

  modelMatrix = glm::translate(modelMatrix, m_sharkPosition);
  modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
  modelMatrix = glm::rotate(modelMatrix, glm::wrapAngle(glm::radians(m_sharkAngle)), m_sharkRotation);

  // Set uniform variable
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);
  m_modelShark.render();

  abcg::glUseProgram(0);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  {
    const auto widgetSize{ImVec2(218, -62)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportWidth - widgetSize.x - 5, 5));
    ImGui::SetNextWindowSize(widgetSize);
    ImGui::Begin("Widget window", nullptr, ImGuiWindowFlags_NoDecoration);

    {
      ImGui::PushItemWidth(120);
      static std::size_t currentIndex{};
      const std::vector<std::string> comboItems{"Perspective", "Orthographic"};

      if (ImGui::BeginCombo("Projection",
                            comboItems.at(currentIndex).c_str())) {
        for (const auto index : iter::range(comboItems.size())) {
          const bool isSelected{currentIndex == index};
          if (ImGui::Selectable(comboItems.at(index).c_str(), isSelected))
            currentIndex = index;
          if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();

      ImGui::PushItemWidth(170);
      const auto aspect{static_cast<float>(m_viewportWidth) /
                        static_cast<float>(m_viewportHeight)};
      if (currentIndex == 0) {
        m_projMatrix =
            glm::perspective(glm::radians(m_FOV), aspect, 0.01f, 100.0f);

        ImGui::SliderFloat("FOV", &m_FOV, 5.0f, 179.0f, "%.0f degrees");
      } else {
        m_projMatrix = glm::ortho(-20.0f * aspect, 20.0f * aspect, -20.0f,
                                  20.0f, 0.01f, 100.0f);
      }
      ImGui::PopItemWidth();
    }

    ImGui::SliderFloat("Angle", &m_sharkAngle, -0.0f, 360.0f, "%.0f degrees");
    ImGui::SliderFloat("X", &m_sharkRotationX, -100.0f, 100.0f, "%.0f degrees");
    ImGui::SliderFloat("Y", &m_sharkRotationY, -100.0f, 100.0f, "%.0f degrees");
    ImGui::SliderFloat("Z", &m_sharkRotationZ, -100.0f, 100.0f, "%.0f degrees");

    ImGui::End();
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
    if (position.z > 0.1f) {
      randomizeBubble(position, rotation);
      position.z = -100.0f;  // Back to -100
    }
  }
    // Update corals
  for (const auto index : iter::range(m_numCorals)) {
    auto &position{m_coralPositions.at(index)};
    auto &rotation{m_coralRotations.at(index)};

    // Z coordinate increases by 10 units per second
    position.z += deltaTime * 10.0f;

    // If this coral is behind the camera, select a new random position and
    // orientation, and move it back to -100
    if (position.z > 0.1f) {
      randomizeCoral(position, rotation);
      position.z = -100.0f;  // Back to -100
    }
  }
}