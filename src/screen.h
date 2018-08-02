#ifndef SCREEN_H
#define SCREEN_H

#include "token.h"


class Screen
{
public:
  static int LeftScreenIntensity;
  static int RightScreenIntensity;

public:
  Screen();
  virtual ~Screen();
  virtual void hit() =0;
};

inline Screen::Screen()  {}
inline Screen::~Screen() {}


class LeftScreen : public Screen
{
public:
  LeftScreen();
  virtual ~LeftScreen();
  virtual void hit()
  {
    Screen::LeftScreenIntensity++;
  }
};

inline LeftScreen::LeftScreen()  {}
inline LeftScreen::~LeftScreen() {}


class RightScreen : public Screen
{
public:
  RightScreen();
  virtual ~RightScreen();
  virtual void hit()
  {
    Screen::RightScreenIntensity++;
  }
};

inline RightScreen::RightScreen()  {}
inline RightScreen::~RightScreen() {}


#endif /* SCREEN_H */
