#ifndef FLYSPRITE_H
#define FLYSPRITE_H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class ExplodingSprite;

class FlySprite : public Drawable {
public:
  FlySprite(const std::string&);
  FlySprite(const FlySprite&); //= delete;
  FlySprite& operator=(const FlySprite&) = delete;

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { 
    return images[currentFrame]; 
  }
  int getScaledWidth()  const { 
    return getScale()*images[currentFrame]->getWidth(); 
  } 
  int getScaledHeight()  const { 
    return getScale()*images[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    return images[currentFrame]->getSurface();
  }

  virtual void explode();

protected:
  std::vector<Image *> images;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  void advanceFrame(Uint32 ticks);

  Vector2f makeVelocity(int, int) const;
  Vector2f makeLoc(int, int) const;
  ExplodingSprite* explosion;
};
#endif
