#include <cmath>
#include <random>
#include <functional>
#include "sprite.h"
#include "gameData.h"
#include "imageFactory.h"
#include "explodingSprite.h"

Vector2f Sprite::makeVelocity(int vx, int vy) const {
  float newvx = Gamedata::getInstance().getRandFloat(vx-50,vx+50);;
  float newvy = Gamedata::getInstance().getRandFloat(vy-50,vy+50);;
  //newvx *= [](){ if(rand()%2) return -1; else return 1; }();
  //newvy *= [](){ if(rand()%2) return -1; else return 1; }();

  return Vector2f(newvx, newvy);
}

Vector2f Sprite::makeLoc(int lx, int ly) const {
  float newlx = Gamedata::getInstance().getRandFloat(lx-100,lx+200);;
  float newly = Gamedata::getInstance().getRandFloat(ly-100,ly+200);;

  return Vector2f(newlx, newly);
}

Sprite::Sprite(const string& n, const Vector2f& pos, const Vector2f& vel,
               const Image* img):
  Drawable(n, pos, vel), 
  image( img ),
  explosion(nullptr),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Sprite::Sprite(const Sprite& s) :
  Drawable(s), 
  image(s.image),
  explosion(s.explosion),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }


Sprite::~Sprite() { if ( explosion ) delete explosion; } 


Sprite::Sprite(const std::string& name) :
  Drawable(name,
   /*      makeLoc(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
*/
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 

 /*         Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY")) 
*/
	  makeVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY")) 
           ),

  image( ImageFactory::getInstance().getImage(name) ),
  explosion(nullptr),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

inline namespace{
  constexpr float SCALE_EPSILON = 2e-7;
}
void Sprite::explode() {
  if ( !explosion ) explosion = new ExplodingSprite(*this);
}


void Sprite::draw() const { 
  if(getScale() < SCALE_EPSILON) return;
  if ( explosion ) explosion->draw();
  else{image->draw(getX(), getY(), getScale()); }
}

void Sprite::update(Uint32 ticks) { 
  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
/*

  if ( getY() < 0) {
    setVelocityY( std::abs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -std::abs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( std::abs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -std::abs( getVelocityX() ) );
  }  
*/

    setVelocityY( std::abs( getVelocityY() +10 ) );
}
