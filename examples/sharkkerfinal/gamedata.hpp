#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Right, Left, Up, Down };
enum class State { Playing, GameOver, Win };

struct GameData {
  State m_state{State::Playing};
  int points{0};
  int lifes{3};
  std::bitset<4> m_input;  // [ left, right]

  abcg::ElapsedTimer m_lifeCooldown;
};

#endif
