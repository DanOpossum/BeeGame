#ifndef BULLETS__H
#define BULLETS__H

#include <list>
#include "vector2f.h"
#include "drawable.h"
#include "bullet.h"

class CollisionStrategy;
class Bullets {
public:
  Bullets(const std::string&);
  Bullets(const Bullets&) = default;
  Bullets& operator=(const Bullets&) = default;
  ~Bullets() = default;
  void draw() const;
  void update(int ticks);
  void shoot(const Vector2f& pos, const Vector2f& objVel);
  void superShoot(const Vector2f& pos, const Vector2f& objVel1, const Vector2f& objVel2, const Vector2f& objVel3);

  bool shooting() const { return !bulletList.empty(); }
  bool collidedWith(const Drawable*) const;

  int bulletCount() const {return bulletList.size();}
  int freeCount() const {/*return freeList.size() ;*/ return poolSize - bulletList.size();}
  int getPoolSize() const { return poolSize; }
  void doublePoolSize() {poolSize = poolSize *2; }

private:
  std::string name;
  Vector2f myVelocity;
  std::list<Bullet> bulletList;
  std::list<Bullet> freeList;
  CollisionStrategy* strategy;
  int poolSize;
};
#endif
