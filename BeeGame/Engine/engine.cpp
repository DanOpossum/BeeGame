#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"
#include "beesprite.h"
#include "flysprite.h"
#include "smartSprite.h"
#include "subjectSprite.h"
#include "honeySprite.h"

Engine::~Engine() { 

  auto it = sprites.begin();
  while ( it != sprites.end() ) {
    delete *it;
    ++it;
  }
  it = spritesBackground.begin();
  while ( it != spritesBackground.end() ) {
    delete *it;
    ++it;
  }
  for ( CollisionStrategy* strategy : strategies ) {
    delete strategy;
  }
  delete subjectSprite;

  delete hud;
  std::cout << "Terminating program" << std::endl;

}

Engine::Engine() :
  rc(& RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  clouds("clouds", Gamedata::getInstance().getXmlInt("clouds/factor") ),
  trees("trees", Gamedata::getInstance().getXmlInt("trees/factor") ),
  viewport( Viewport::getInstance() ),
  sprites( {new FlySprite("Fly")}),
  spritesBackground( {new MultiSprite("BackgroundBee")}),
  subjectSprite(new SubjectSprite("Bee")),
  currentSprite(0),
  makeVideo( false ),
  strategies(),
  currentStrategy(0),
  collision(false),
  hud(new Hud("Hud")),
  hudOn(true),
  godMode(false),
  totalFlies(0),
  start(time(0))

{

  //player character
  subjectSprite->setScale(4);

  //for giving position to new fly objects
  Vector2f pos = subjectSprite->getPosition();

  //retrieve how many flies from XML
  for(int i=0; i<Gamedata::getInstance().getXmlInt("numberOfFlys")-1;i++){

    //large flies
    if(i<20){  

      sprites.push_back(new SmartSprite("Fly", pos, i, 0));
      sprites[i+1]->setScale(3); 
      subjectSprite->attach(dynamic_cast<SmartSprite*>(sprites[i+1]));
    }
  }
  sprites.push_back(new honeySprite("HoneyComb"));
  sprites.back()->setScale(4);
 
  totalFlies = sprites.size() - 1;

  //background bees
  for(int i=0; i<9;i++){
      spritesBackground.push_back(new MultiSprite("BackgroundBee"));

      int randKindOf = i%3;
      if(randKindOf == 0){
        spritesBackground[i+1]->setScale(2); 
      }
      else if(randKindOf ==1){
        spritesBackground[i+1]->setScale(1.5); 
      }
      else{
        spritesBackground[i+1]->setScale(1); 
      }

  }
 
  
  //we only need 1 really, they all have similar performance
  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new PerPixelCollisionStrategy );
  strategies.push_back( new MidPointCollisionStrategy );

  Viewport::getInstance().setObjectToTrack(subjectSprite);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {

  //background
  clouds.draw();
  for(auto* s: spritesBackground) s->draw();
  trees.draw();

  //characters
  for(auto* s: sprites) s->draw();
  subjectSprite->draw();

  //name
  SDL_Color color = {255, 20,0, 255};
  io.writeText("Daniel O'Brien", 5, 850,color);
  
  //hud
  if(hudOn){
    hud->draw();
  }

  viewport.draw();
  viewport.draw();
  SDL_RenderPresent(renderer);

  //strategies[currentStrategy]->draw();
}


void Engine::checkForCollisions() {


  for(auto* s: sprites) {
    if ( strategies[currentStrategy]->execute(*subjectSprite, *s) ) {

      honeySprite* hs = dynamic_cast<honeySprite*>(s);
      if(hs){
        subjectSprite->becomeSuperBee();
        s->explode();
      }
      else{
        if(!godMode){
          if(subjectSprite->subtractHealth(20) <= 0 ){
            subjectSprite->explode();
            subjectSprite->subtractHealth(-100);
          }
        }
        s->explode(); 
        totalFlies = totalFlies -1;
        //delete s;
      }

    }
    if ( subjectSprite->shot(s) ) {

      honeySprite* hs = dynamic_cast<honeySprite*>(s);
      if(hs){
      }
      else{
        s->explode();
        totalFlies = totalFlies -1;
      }
    }
  }

 if(totalFlies < 1) {
   hud->showWinNow();
  // clock.pause();
 }
}


void Engine::update(Uint32 ticks) {

  hud->getPoolData(subjectSprite->returnFreeBullets(), subjectSprite->returnUsedBullets());
  hud->getHealthData(subjectSprite->subtractHealth(0));
  hud->receiveTotalFlies(totalFlies);
  checkForCollisions();
  for(auto* s: sprites) s->update(ticks);
  for(auto* s: spritesBackground) s->update(ticks);
  subjectSprite->update(ticks);


  clouds.update();
  trees.update();
  hud->update(ticks);
  viewport.update(); 

  unsigned int x = clock.getSeconds();
    if(x > 2 ){ hud->dontShowBeginNow();}
}


void Engine::switchSprite(){

  ++currentSprite;
  currentSprite = currentSprite % 2;
  if ( currentSprite ) {
    Viewport::getInstance().setObjectToTrack(sprites[1]);
  }
  else {
    Viewport::getInstance().setObjectToTrack(sprites[0]);
  }
}

bool Engine::play() {

  SDLSound sound;
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {

    //check  for events
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);

      //checking for single press DOWN
      if(event.type == SDL_KEYDOWN) {
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_W] ) {
         hud->setUp(true);
        }
        if ( keystate[SDL_SCANCODE_S] ) {
         hud->setDown(true);
        }
        if ( keystate[SDL_SCANCODE_A] ) {
         hud->setLeft(true);
        }
        if ( keystate[SDL_SCANCODE_D] ) {
         hud->setRight(true);
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if (keystate[SDL_SCANCODE_F1]) {
          // hudOn=!hudOn;
          hud->toggleHUD();
        }
        if (keystate[SDL_SCANCODE_M]) {
            sound.toggleMusic();
        }
        if (keystate[SDL_SCANCODE_G]) {
            godMode = !godMode;
        }
        if (keystate[SDL_SCANCODE_SPACE]) {
         subjectSprite->shoot();
            sound[1];
        }
        if ( keystate[SDL_SCANCODE_L] ) {
         subjectSprite->explode();
         for(auto* s: sprites) {
            s->explode();
            sound[0];
         }
         
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }

      //checking for UP (lifting off key)
      else if(event.type == SDL_KEYUP) {

       if (! keystate[SDL_SCANCODE_W] ) {
          hud->setUp(false);
        }
       if (! keystate[SDL_SCANCODE_A] ) {
          hud->setLeft(false);
        }
        if (! keystate[SDL_SCANCODE_S] ) {
          hud->setDown(false);
        }
        if (! keystate[SDL_SCANCODE_D] ) {
          hud->setRight(false);
        }
      }
      
      if (event.type ==  SDL_QUIT) { done = true; break; }

      if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
      }
      if ( keystate[SDL_SCANCODE_R] ) {
          clock.unpause();
          return true;
        }

      //Check if we are holding keys
      if ( keystate[SDL_SCANCODE_W] && keystate[SDL_SCANCODE_S] ) { 
        subjectSprite->stop();
      }
      else if ( keystate[SDL_SCANCODE_A] && keystate[SDL_SCANCODE_D] ) { 
        subjectSprite->stop();
      }
      else if(keystate[SDL_SCANCODE_S]){
        subjectSprite->down();
      }
      else if(keystate[SDL_SCANCODE_W]){
        subjectSprite->up();
      }
      else if(keystate[SDL_SCANCODE_A]){
        subjectSprite->left();
      }
      else if(keystate[SDL_SCANCODE_D]){
        subjectSprite->right();
      }
      else if ( keystate[SDL_SCANCODE_M] ) { 
        currentStrategy = (1 + currentStrategy) % strategies.size();
        static_cast<Hud*>(hud)->reset();
      }
      else{
        subjectSprite->stop();
      }
  }

    // In this section of the event loop we allow key bounce:
    ticks = clock.getElapsedTicks();

    if ( ticks > 0 ) {
      clock.incrFrame();
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  return false;
}
