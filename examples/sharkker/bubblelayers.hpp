#ifndef BUBBLELAYERS_HPP_
#define BUBBLELAYERS_HPP_

#include <array>
#include <random>

#include "abcg.hpp"
#include "gamedata.hpp"
#include "shark.hpp"

class OpenGLWindow;

class BubbleLayers {
 public:
  void initializeGL(GLuint program, int quantity);
  void paintGL();
  void terminateGL();

  void update(const Shark &shark, float deltaTime);

 private:
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_pointSizeLoc{};
  GLint m_translationLoc{};

  struct BubbleLayer {
    GLuint m_vao{};
    GLuint m_vbo{};

    float m_pointSize{};
    int m_quantity{};
    glm::vec2 m_translation{glm::vec2(0)};
  };

  std::array<BubbleLayer, 5> m_bubbleLayers;

  std::default_random_engine m_randomEngine;
};

#endif