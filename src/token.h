#ifndef TOKEN_H
#define TOKEN_H

#include "screen.h"
#include <memory>


class Token
{
public:
  Token();
  virtual ~Token();

  virtual Screen* getScreen() =0;
};

inline Token::Token()
{}

inline Token::~Token()
{}


class LeftToken : public Token
{
public:
  LeftToken();
  virtual ~LeftToken();

  virtual Screen* getScreen() { return new LeftScreen; }
};

inline LeftToken::LeftToken()
{
}

inline LeftToken::~LeftToken()
{}


class RightToken : public Token
{
public:
  RightToken();
  virtual ~RightToken();

  virtual Screen* getScreen() { return new RightScreen; }
};

inline RightToken::RightToken()
{
}

inline RightToken::~RightToken()
{}


#endif /* TOKEN_H */
