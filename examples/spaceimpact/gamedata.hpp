#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Right, Left };
enum class State { Playing, GameOver, Win };

struct GameData {
  State m_state{State::Playing};
  int points{0};
  int lifes{3};
  std::bitset<2> m_input;  // [ left, right]

  abcg::ElapsedTimer m_lifeCooldown;
};

#endif
