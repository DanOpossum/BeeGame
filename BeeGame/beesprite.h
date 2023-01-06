#ifndef BEESPRITE__H
#define BEESPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "bullets.h"

class ExplodingSprite;

class BeeSprite : public Drawable {
public:
  BeeSprite(const std::string&);

 //unused, delete so it isn't auto generated
  BeeSprite(const BeeSprite&);// = delete;
  BeeSprite& operator=(const BeeSprite&) = delete;

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual void explode();

  void shoot();
  bool shot(const Drawable*);
  int returnFreeBullets();
  int returnUsedBullets();
  int resetHealth();
  int subtractHealth(int amount);
  void becomeSuperBee();

  //handle movement
  void down();
  void up();
  void left();
  void right();
  void stop();

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


protected:

  std::vector<Image *> images;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  
  int worldWidth;
  int worldHeight;

  //starting velocity
  float xVelocity;
  float yVelocity;
  
  bool lastDirection;

  void advanceFrame(Uint32 ticks);

  ExplodingSprite* explosion;

  std::string bulletName;
  Bullets bullets;
  float bulletSpeed;
  int bulletInterval;
  int timeSinceLastBullet;
  int health;
  bool superBee;
  

};
#endif
