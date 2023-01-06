#ifndef HONEYSPRITE__H
#define HONEYSPRITE__H
#include <string>
#include "drawable.h"

class ExplodingSprite;

class honeySprite : public Drawable {
public:
  honeySprite(const std::string&);
  honeySprite(const std::string&, const Vector2f& pos, const Vector2f& vel, 
         const Image*);
  //honeySprite(const honeySprite&) = delete;
  honeySprite(const honeySprite&);
  virtual ~honeySprite();
  honeySprite& operator=(const honeySprite&) = delete;

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { return image; }
  virtual const SDL_Surface* getSurface() const { 
    return image->getSurface();
  }
  int getScaledWidth()  const { return getScale()*image->getWidth();  } 
  int getScaledHeight() const { return getScale()*image->getHeight(); } 
  
  virtual void explode();

private:
  const Image * image;
  ExplodingSprite* explosion;

protected:
  int worldWidth;
  int worldHeight;

  int getDistance(const honeySprite*) const;
  Vector2f makeVelocity(int, int) const;
  Vector2f makeLoc(int, int) const;
};
#endif
