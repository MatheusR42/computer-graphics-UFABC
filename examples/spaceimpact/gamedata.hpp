#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Right, Left, Fire };
enum class State { Playing, GameOver, Win };

struct GameData {
  State m_state{State::Playing};
  int points{0};
  std::bitset<3> m_input;  // [fire, left, right]
};

#endif
