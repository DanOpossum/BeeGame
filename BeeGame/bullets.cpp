#include <iostream>
#include <cmath>
#include "bullets.h"
#include "imageFactory.h"
#include "collisionStrategy.h"

Bullets::Bullets(const std::string& n) :
  name(n),
  myVelocity(
    Gamedata::getInstance().getXmlInt(name+"/speed/x"), 
    Gamedata::getInstance().getXmlInt(name+"/speed/y")
  ),
  bulletList(),
  freeList(),
  strategy( new PerPixelCollisionStrategy),
  poolSize(    Gamedata::getInstance().getXmlInt(name+"/poolSize") )
{ 
/*
  const string thisStrategy = 
    Gamedata::getInstance().getXmlStr("collisionStrategy");
  if ( thisStrategy == "PerPixel" ) {
    strategy = new PerPixelCollisionStrategy;
  }
  else if ( thisStrategy == "Rectangular" ) {
    strategy = new RectangularCollisionStrategy;
  }
  else if ( thisStrategy == "MidPoint" ) {
    strategy = new MidPointCollisionStrategy;
  }
*/
}

void Bullets::shoot(const Vector2f& pos, const Vector2f& objVel) {
 /*
  Bullet b( name,  pos, objVel );
  bulletList.push_back( b );
*/


  if( this->bulletCount() < this->getPoolSize()){
  if( freeList.empty() ) {
    Bullet b( name,  pos, objVel );
    b.setScale(4);
    bulletList.push_back( b );
  }
  else{
    Bullet b = freeList.front();
    b.setScale(4);
    freeList.pop_front();
    b.reset();
    b.setVelocity(objVel);
    b.setPosition(pos);
    bulletList.push_back(b);
  }}

}

void Bullets::superShoot(const Vector2f& pos, const Vector2f& objVel1, const Vector2f& objVel2, const Vector2f& objVel3) {



  if( this->bulletCount() < this->getPoolSize()){
    if( freeList.empty() ) {
      Bullet b( name,  pos, objVel1 );
      b.setScale(4);
      bulletList.push_back( b );

      Bullet b2( name,  pos, objVel2  );
      b2.setScale(4);
      bulletList.push_back( b2 );

      Bullet b3( name,  pos, objVel3 );
      b3.setScale(4);
      bulletList.push_back( b3 );
    }
    else{
      Bullet b = freeList.front();
      b.setScale(4);
      freeList.pop_front();
      b.reset();
      b.setVelocity(objVel1 );
      b.setPosition(pos);
      bulletList.push_back(b);

      if(!freeList.empty()){
      Bullet b2 = freeList.front();
      b2.setScale(4);
      freeList.pop_front();
      b2.reset();
      b2.setVelocity(objVel2);
      b2.setPosition(pos);
      bulletList.push_back(b2);
      }
      if(!freeList.empty()){
      Bullet b3 = freeList.front();
      b3.setScale(4);
      freeList.pop_front();
      b3.reset();
      b3.setVelocity(objVel3 );
      b3.setPosition(pos);
      bulletList.push_back(b3);
      }
    }
  }
}

bool Bullets::collidedWith(const Drawable* obj) const { 
  for (  auto & bullet : bulletList ) {
    if ( strategy->execute(bullet, *obj) ) {
      return true;
    }
  }
  return false;
}


void Bullets::draw() const { 
  for ( const auto& bullet : bulletList ) {
    bullet.draw();
  }
}

void Bullets::update(int ticks) { 
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    ptr->update(ticks);
    if (ptr->goneTooFar()) {  // Check to see if we should free a chunk
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
    }   
    else ++ptr;
  }
}
