#include "beesprite.h"
#include "gameData.h"
#include "imageFactory.h"
#include "explodingSprite.h"
#include "bullets.h"

void BeeSprite::advanceFrame(Uint32 ticks) {
	/*timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}*/
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
          if(!superBee){

       		if (getVelocityX() > 0){
                        lastDirection = true;
        		currentFrame = (currentFrame+1) % (numberOfFrames/4);
			timeSinceLastFrame = 0;
	        }
       		if (getVelocityX() < 0){
                        lastDirection = false;
        		currentFrame = ((currentFrame+1) % (numberOfFrames/4)+6);
			timeSinceLastFrame = 0;
	        }
                if (getVelocityX() == 0 && lastDirection == true){
        		currentFrame = (currentFrame+1) % (numberOfFrames/4);
			timeSinceLastFrame = 0;
                }
                if (getVelocityX() == 0 && lastDirection == false){
        		currentFrame = ((currentFrame+1) % (numberOfFrames/4)+6);
			timeSinceLastFrame = 0;
	        }
           }
           else{

       		if (getVelocityX() > 0){
                        lastDirection = true;
        		currentFrame = (currentFrame+1) % (numberOfFrames/4) + 12;
			timeSinceLastFrame = 0;
	        }
       		if (getVelocityX() < 0){
                        lastDirection = false;
        		currentFrame = ((currentFrame+1) % (numberOfFrames/4)+18);
			timeSinceLastFrame = 0;
	        }
                if (getVelocityX() == 0 && lastDirection == true){
        		currentFrame = (currentFrame+1) % (numberOfFrames/4) +12;
			timeSinceLastFrame = 0;
                }
                if (getVelocityX() == 0 && lastDirection == false){
        		currentFrame = ((currentFrame+1) % (numberOfFrames/4)+18) ;
			timeSinceLastFrame = 0;
	        }
           }
        }


}

BeeSprite::BeeSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( ImageFactory::getInstance().getImages(name) ),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  xVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX")),
  yVelocity(Gamedata::getInstance().getXmlInt(name+"/speedY")),
  lastDirection(true),
  explosion(nullptr),
  bulletName(Gamedata::getInstance().getXmlStr(name+"/bulletName")),
  bullets(bulletName),
  bulletSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/speedX")),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastBullet(0),
  health(100),
  superBee(false)
{ }


BeeSprite::BeeSprite( const BeeSprite& s) : 
  Drawable(s),
  images( s.images ),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  xVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX")),
  yVelocity(Gamedata::getInstance().getXmlInt(name+"/speedY")),
  lastDirection(true),
  explosion(nullptr),
  bulletName(Gamedata::getInstance().getXmlStr(name+"/bulletName")),
  bullets(bulletName),
  bulletSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/speedX")),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastBullet(0),
  health(100),
  superBee(false)
{}
void BeeSprite::draw() const { 

  if(explosion){
    explosion->draw();
    return;
  }
  images[currentFrame]->draw(getX(), getY(), getScale()); 
  bullets.draw();
  
}

bool BeeSprite::shot(const Drawable* obj ) {
  if ( bullets.collidedWith(obj) ) return true;
  else return false;

}
void BeeSprite::shoot() { 
  // See if it's time to shoot a bullet:
	if (timeSinceLastBullet > bulletInterval) {
    Vector2f vel = getVelocity();
    Vector2f vel2 = getVelocity();
    Vector2f vel3 = getVelocity();
    float x; 
    float y = (getY()+getScaledHeight()/4+11 ) - 32;
    //right
    if ( vel[0] < 0 ) {
      x = getX() + 30;
      vel[0] = 100;
      vel[1] = 0;
      vel2[0] = -300;
      vel2[1] = 50;
      vel3[0] = -500;
      vel3[1] = 100;
    }
    //left
    else if (vel[0] > 0){
      x = getX() - 30;
    //  vel[0] = -vel[0] + 1200;
      vel[1] = 100;
      vel[0] = 0;
      vel2[0] = 300;
      vel2[1] = -50;
      vel3[0] = 500;
      vel3[1] = -100;


    }
    //right facing
    else if ( lastDirection == true){
      x = getX() - 55;
      vel[0] -= 100;
      vel[1] = 0;
      vel2[0] -= 300;
      vel2[1] = 50;
      vel3[0] -= 500;
      vel3[1] = 100;

    }
    //left
    else{
      x = getX()+ 55;
      vel[0] += 100;
      vel[1] = 0;
      vel2[0] += 300;
      vel2[1] = 50;
      vel3[0] += 500;
      vel3[1] = 100;
    }

    if(!superBee){
      bullets.shoot( Vector2f(x, y), vel );
    }
    else{
      bullets.superShoot( Vector2f(x, y), vel, vel2, vel3 );  
    }
    timeSinceLastBullet = 0;
  }
}



void BeeSprite::explode(){

   if(!explosion){
    Sprite sprite(getName(), getPosition(), getVelocity(), getImage() );
    explosion = new ExplodingSprite(sprite);
    //explosion->setScale(.2);
  }
}
void BeeSprite::becomeSuperBee(){

  if(superBee == false){
    superBee = true;
    xVelocity = xVelocity + 750;
    yVelocity = yVelocity + 50;
   bullets.doublePoolSize();
 }

}


//controls
void BeeSprite::down(){

  if(getY()< worldHeight){
    setVelocityY(yVelocity);
  }
}

void BeeSprite::up(){

  if(getY()>0){
    setVelocityY(-yVelocity);
  }
}

void BeeSprite::left(){

  if(getX()>0){
    setVelocityX(-xVelocity);
  }
}
void BeeSprite::right(){

  if(getX()< worldWidth ){
    setVelocityX(xVelocity);
  }
}
void BeeSprite::stop(){

  setVelocityY(0);
  setVelocityX(0);

}

int BeeSprite::returnFreeBullets(){return bullets.freeCount();}

int BeeSprite::returnUsedBullets(){return bullets.bulletCount();}

int BeeSprite::resetHealth(){return health;}

int BeeSprite::subtractHealth(int amount){
  health = health - amount; 
  return health;
}


void BeeSprite::update(Uint32 ticks) { 

  if(explosion){
    explosion->update(ticks);
    if(explosion->chunkCount() == 0){
      delete explosion;
      explosion = nullptr;
    }
    return;
  }

  advanceFrame(ticks);

  timeSinceLastBullet += ticks;
  bullets.update(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

}
