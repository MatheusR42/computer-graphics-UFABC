#ifndef MODEL_HPP_
#define MODEL_HPP_

#include <vector>

#include "abcg.hpp"

struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};

  bool operator==(const Vertex& other) const noexcept {
    static const auto epsilon{std::numeric_limits<float>::epsilon()};
    return glm::all(glm::epsilonEqual(position, other.position, epsilon)) &&
           glm::all(glm::epsilonEqual(normal, other.normal, epsilon));
  }
};

class Model {
 public:
  void loadObj(std::string_view path, bool standardize = true);
  void render(int numTriangles = -1) const;
  void setupVAO(GLuint program);
  void terminateGL();

  [[nodiscard]] int getNumTriangles() const {
    return static_cast<int>(m_indices.size()) / 3;
  }

 private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  bool m_hasNormals{false};

  void computeNormals();
  void createBuffers();
  void standardize();
};

#endif