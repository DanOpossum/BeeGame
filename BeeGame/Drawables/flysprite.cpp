#include "flysprite.h"
#include "gameData.h"
#include "imageFactory.h"
#include "explodingSprite.h"

Vector2f FlySprite::makeVelocity(int vx, int vy) const {
  float newvx = Gamedata::getInstance().getRandFloat(vx-300,vx+300);;
  float newvy = Gamedata::getInstance().getRandFloat(vy-300,vy+300);;

  return Vector2f(newvx, newvy);
}
Vector2f FlySprite::makeLoc(int lx, int ly) const {
  float newlx = Gamedata::getInstance().getRandFloat(lx+300,lx+1500);;
  float newly = Gamedata::getInstance().getRandFloat(ly-100,ly+500);;

  return Vector2f(newlx, newly);
}



void FlySprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}


}

FlySprite::FlySprite( const std::string& name) :
  Drawable(name,
           makeLoc(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           makeVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( ImageFactory::getInstance().getImages(name) ),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  explosion(nullptr) 
{ }

FlySprite::FlySprite(const FlySprite& s) :
  Drawable(s), 
  images(s.images),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  explosion(nullptr) 
{ }


void FlySprite::draw() const { 

  if(explosion){
    explosion->draw();
    return;
  }
  images[currentFrame]->draw(getX(), getY(), getScale());
}

void FlySprite::explode(){

   if(!explosion){
    Sprite sprite(getName(), getPosition(), getVelocity(), getImage() );
    explosion = new ExplodingSprite(sprite);

  }
}

void FlySprite::update(Uint32 ticks) { 



  if(explosion){
    //move the fly away so it doesn't collide more
     Vector2f distAdd = Vector2f(1000, 1000); 
     setPosition(getPosition() + distAdd) ; 
     
     explosion->update(ticks);
    //Uncomment to respawn themw
     if(explosion->chunkCount() == 0){
   
     //    delete explosion;
     //     explosion = nullptr;
    }
    return;
  }

  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() + getScaledHeight() > worldHeight) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() + getScaledWidth() > worldWidth) {
    setVelocityX( -fabs( getVelocityX() ) );
  }  

}
