#include "openglwindow.hpp"

#include <imgui.h>

#include "abcg.hpp"

void OpenGLWindow::handleEvent(SDL_Event &event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(static_cast<size_t>(Input::Right));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Right));
  }
}

void OpenGLWindow::initializeGL() {
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

  // Create program to render the bubble
  m_bubbleProgram = createProgramFromFile(getAssetsPath() + "bubble.vert",
                                         getAssetsPath() + "bubble.frag");
                                         
  // Create program to render the other objects
  m_objectsProgram = createProgramFromFile(getAssetsPath() + "objects.vert",
                                           getAssetsPath() + "objects.frag");

  abcg::glClearColor(0, 0.32, 0.492, 1);

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  // Start pseudo-random number generator
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  restart();
}

void OpenGLWindow::restart() {
  m_gameData.m_state = State::Playing;
  m_gameData.points = 0;
  m_gameData.lifes = 3;

  m_bubbleLayers.initializeGL(m_bubbleProgram, 25);
  m_shark.initializeGL(m_objectsProgram);
  m_corals.initializeGL(m_objectsProgram, 1);
}

void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  // Wait 8 seconds before restarting
  if (m_gameData.m_state != State::Playing &&
      m_Timer.elapsed() > 7) {
    restart();
    return;
  }

  m_shark.update(m_gameData);
  m_bubbleLayers.update(m_shark, deltaTime);
  m_corals.update(m_shark, deltaTime, m_gameData);

  if (m_gameData.m_state == State::Playing) {
    checkCollisions();
  }
}

void OpenGLWindow::paintGL() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  m_bubbleLayers.paintGL();
  m_corals.paintGL();
  m_shark.paintGL(m_gameData);
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
      ImGui::Text("          Game Over\nContinue a nadar.");

    // Display developers name after "Game Over" message
    }
    if (m_gameData.m_state == State::GameOver && 
      m_Timer.elapsed() > 4) {
      ImGui::Text("Matheus Araujo\nGiovanne Galdino");
    }
     
    // "Sharkker" inicial message display
    if (m_gameData.m_state == State::Playing && 
      m_Timer.elapsed() < 4) {                    // Text display time
    ImGui::Text("Sharkker");
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
  
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
  abcg::glDeleteProgram(m_bubbleProgram);
  abcg::glDeleteProgram(m_objectsProgram);

  m_corals.terminateGL();
  m_shark.terminateGL();
  m_bubbleLayers.terminateGL();
}

void OpenGLWindow::checkCollisions() {
  if (m_gameData.m_lifeCooldown.elapsed() < 1) {
    return;
  }

  m_shark.m_nodamage = false;

  for (const auto &coral : m_corals.m_corals) {
    const auto coralTranslation{coral.m_translation};
    const auto distance{
        glm::distance(m_shark.m_translation, coralTranslation)};

    if (distance < m_shark.m_scale * 0.6f + coral.m_scale * 0.6f) {
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
