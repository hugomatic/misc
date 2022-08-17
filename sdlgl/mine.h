#pragma once

#include <iostream>
#include <array>
#include <tuple>
#include <deque>

const size_t rows=  40;
const size_t cols = 40;

typedef std::array<std::array<char, cols>, rows> map_t;
typedef std::deque<std::tuple<size_t, size_t, char> > changes_t;

void initMap(map_t &map, int bombCount);
std::tuple<bool, changes_t> click(map_t &map, size_t clickR, size_t clickC);
bool isWon(map_t const& map);

std::ostream& operator<<(std::ostream& os, const map_t& map);
// Place a flag to mark it as dangerous

struct layout_t {
  size_t rows;
  size_t cols;

  double x_topLeft;
  double y_topLeft;
  double dx;
  double dy;
  double x_inter;
  double y_inter;
  void color(char value, bool gameOver, double &r, double &g, double &b);
  bool inside(double x, double y, size_t &r, size_t &c);
  void topLeft(size_t r, size_t c, double &x, double &y);
  void bottomRight(size_t r, size_t c, double &x, double &y);
};


