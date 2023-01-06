#include "multisprite.h"
#include "gameData.h"
#include "imageFactory.h"


Vector2f MultiSprite::makeVelocity(int vx) const {
  float newvx = Gamedata::getInstance().getRandFloat(vx-400,vx+200);;
  float newvy = 0;

  return Vector2f(newvx, newvy);
}
Vector2f MultiSprite::makeLoc(int lx, int ly) const {
  float newlx = Gamedata::getInstance().getRandFloat(lx-300,lx+2000);;
  float newly = Gamedata::getInstance().getRandFloat(ly-100,ly+350);;

  return Vector2f(newlx, newly);
}


void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {

       		if (getVelocityX() > 0){
        		currentFrame = (currentFrame+1) % (numberOfFrames/2);
			timeSinceLastFrame = 0;
	        }
       		else{
        		currentFrame = ((currentFrame+1) % (numberOfFrames/2)+6);
			timeSinceLastFrame = 0;
	        }

        }
}

MultiSprite::MultiSprite( const std::string& name) :
  Drawable(name, 
           makeLoc(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           makeVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"))
           ),
  images( ImageFactory::getInstance().getImages(name) ),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

MultiSprite::MultiSprite(const MultiSprite& s) :
  Drawable(s), 
  images(s.images),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight )
  { }

MultiSprite& MultiSprite::operator=(const MultiSprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  return *this;
}


void MultiSprite::draw() const { 
  images[currentFrame]->draw(getX(), getY(), getScale());
}

void MultiSprite::update(Uint32 ticks) { 
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
