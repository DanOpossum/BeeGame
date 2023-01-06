#include <sstream>
#include "hud.h"
#include "renderContext.h"

Hud::Hud(const std::string& name) :  
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(
                    Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY")) 
           ),
  renderer(RenderContext::getInstance().getRenderer()),
  barRect( { Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
             Gamedata::getInstance().getXmlInt(name+"/startLoc/y"),
             Gamedata::getInstance().getXmlInt(name+"/width"),
             Gamedata::getInstance().getXmlInt(name+"/height")}
  ),
  clock( Clock::getInstance() ),
  currentLength(barRect.w), 
  increment(barRect.w/Gamedata::getInstance().getXmlInt(name+"/increment")),
  interval(Gamedata::getInstance().getXmlInt(name+"/interval")),
  deltaTime(0),
  barColor(
    {(Uint8)Gamedata::getInstance().getXmlInt(name+"/barColor/r"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/barColor/g"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/barColor/b"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/barColor/a")}
  ),
  backColor(
    {(Uint8)Gamedata::getInstance().getXmlInt(name+"/backColor/r"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/backColor/g"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/backColor/b"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/backColor/a")}
  ),
 io( IoMod::getInstance() ),
 up(false),
 down(false),
 left(false),
 right(false),
 freeAmmo(-1),
 usedAmmo(-1),
 beeHealth(100),
 showHud(true),
 totalFlies(0),
 showWin(false),
 showBegin(true)
{}

void Hud::update(Uint32 ticks) {

  deltaTime += ticks;
/*
  if ( currentLength > 0 && deltaTime > interval ) {
    deltaTime = 0;
    currentLength -= increment;
  }
*/
}

void Hud::toggleHUD(){
  showHud = !showHud;
}

void Hud::draw() const {


if(showHud){
  //background of HUD
  SDL_Color hudColor= {(Uint8)Gamedata::getInstance().getXmlInt("HUD/backColor/r"),
     (Uint8)Gamedata::getInstance().getXmlInt("HUD/backColor/g"),
     (Uint8)Gamedata::getInstance().getXmlInt("HUD/backColor/b"),
     (Uint8)Gamedata::getInstance().getXmlInt("HUD/backColor/a")};
  
  SDL_SetRenderDrawColor( 
    renderer, hudColor.r, hudColor.g, hudColor.b, hudColor.a );

  SDL_Rect hudRect =  { Gamedata::getInstance().getXmlInt("HUD/startLoc/x"),
             Gamedata::getInstance().getXmlInt("HUD/startLoc/y"),
             Gamedata::getInstance().getXmlInt("HUD/width"),
             Gamedata::getInstance().getXmlInt("HUD/height") + 150};
  // Draw the  background
  SDL_RenderFillRect( renderer, &hudRect );



 //draw what buttons are being pressed (movement)
  SDL_Color moveBackColor= {(Uint8)Gamedata::getInstance().getXmlInt("MoveIndicator/backColor/r"),
     (Uint8)Gamedata::getInstance().getXmlInt("MoveIndicator/backColor/g"),
     (Uint8)Gamedata::getInstance().getXmlInt("MoveIndicator/backColor/b"),
     (Uint8)Gamedata::getInstance().getXmlInt("MoveIndicator/backColor/a")};
  
  SDL_SetRenderDrawColor( 
    renderer, moveBackColor.r, moveBackColor.g, moveBackColor.b, moveBackColor.a );


  SDL_Rect moveRect =  { Gamedata::getInstance().getXmlInt("MoveIndicator/startLoc/x"),
             Gamedata::getInstance().getXmlInt("MoveIndicator/startLoc/y"),
             Gamedata::getInstance().getXmlInt("MoveIndicator/width"),
             Gamedata::getInstance().getXmlInt("MoveIndicator/height")};

  SDL_RenderFillRect( renderer, &moveRect );

  SDL_Color moveColor = {(Uint8)Gamedata::getInstance().getXmlInt("MoveIndicator/Color/r"),
     (Uint8)Gamedata::getInstance().getXmlInt("MoveIndicator/Color/g"),
     (Uint8)Gamedata::getInstance().getXmlInt("MoveIndicator/Color/b"),
     (Uint8)Gamedata::getInstance().getXmlInt("MoveIndicator/Color/a")};

  int moveIndicatorX = Gamedata::getInstance().getXmlInt("MoveIndicator/startLoc/x");
  int moveIndicatorY = Gamedata::getInstance().getXmlInt("MoveIndicator/startLoc/y");


  //here we draw only if a corresponding key is being pressed
  if(up){
    io.writeText("up", moveIndicatorX+25, moveIndicatorY+0,moveColor);
  }

  if(down){
    io.writeText("down", moveIndicatorX+25, moveIndicatorY+50,moveColor);
  }

  if(left){
    io.writeText("left", moveIndicatorX+0, moveIndicatorY+25,moveColor);
  }

  if(right){
    io.writeText("right", moveIndicatorX+50, moveIndicatorY+25,moveColor);
  }

  //FPS
  int FPSX = Gamedata::getInstance().getXmlInt("FPS/startLoc/x");
  int FPSY = Gamedata::getInstance().getXmlInt("FPS/startLoc/y");

  std::stringstream strm;
  strm << "FPS: " << clock.getFps();
  SDL_Color color = {148, 0,201, 255};
  io.writeText(strm.str(),FPSX,FPSY,color);

  //AMMO pool stinger

  strm.str("");
  strm << "Free Ammo: " << freeAmmo;
  io.writeText(strm.str(),160,140,color);
 
 strm.str("");
  strm << "Ammo in use: " << usedAmmo;
  io.writeText(strm.str(),160,170,color);


  strm.str("");
  strm << "Flies alive: " << totalFlies;
  io.writeText(strm.str(),160,200,color);


  //instructions

  int InstructionsX = Gamedata::getInstance().getXmlInt("Instructions/startLoc/x");
  int InstructionsY = Gamedata::getInstance().getXmlInt("Instructions/startLoc/y");


  io.writeText("Move the character with: W A S D" ,InstructionsX,InstructionsY-35,color);
  io.writeText("Shoot with: spacebar" ,InstructionsX,InstructionsY,color);
  io.writeText("Toggle the HUD with: F1" ,InstructionsX,InstructionsY+35,color);
  io.writeText("Pause with: P" ,InstructionsX,InstructionsY+70,color);
  io.writeText("Quit with: Q" ,InstructionsX,InstructionsY+105,color);
  io.writeText("Restart with: R" ,InstructionsX,InstructionsY+140,color);
  io.writeText("Toggle God mode with: G" ,InstructionsX,InstructionsY+175,color);
  io.writeText("Toggle Music: M" ,InstructionsX,InstructionsY+210,color);
  io.writeText("Explode everything with: L ", InstructionsX, InstructionsY+245, color);
  io.writeText("Kill some flies" ,InstructionsX,InstructionsY+280,color);



  //Hud
  color = {0, 0,0, 255};
  strm.str("");
  strm << "Health: " << beeHealth;
  io.writeText(strm.str(),20,35,color);

  // First set the blend mode so that alpha blending will work;
  // the default blend mode is SDL_BLENDMODE_NONE!
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  // Set the health bar background color:
  SDL_SetRenderDrawColor( 
    renderer, backColor.r, backColor.g, backColor.b, backColor.a );
  // Draw the  background
  SDL_RenderFillRect( renderer, &barRect );

  SDL_SetRenderDrawColor( 
    renderer, barColor.r, barColor.g, barColor.b, barColor.a );
  int x = static_cast<int>(getX());
  int y = static_cast<int>(getY());
  SDL_Rect healthRect = {x, y, currentLength, barRect.h};
  SDL_RenderFillRect( renderer, &healthRect );

  // Set the  color for the health bar outline:
  SDL_SetRenderDrawColor( 
    renderer, barColor.r, barColor.g, barColor.b, barColor.a );
  SDL_RenderDrawRect( renderer, &barRect );

}
else{


  //background of HUD
  SDL_Color hudColor= {(Uint8)Gamedata::getInstance().getXmlInt("HUD/backColor/r"),
     (Uint8)Gamedata::getInstance().getXmlInt("HUD/backColor/g"),
     (Uint8)Gamedata::getInstance().getXmlInt("HUD/backColor/b"),
     (Uint8)Gamedata::getInstance().getXmlInt("HUD/backColor/a")};
  
  SDL_SetRenderDrawColor( 
    renderer, hudColor.r, hudColor.g, hudColor.b, hudColor.a );

  SDL_Rect hudRect =  { Gamedata::getInstance().getXmlInt("HUD/startLoc/x"),
             Gamedata::getInstance().getXmlInt("HUD/startLoc/y"),
             Gamedata::getInstance().getXmlInt("HUD/width"),
             Gamedata::getInstance().getXmlInt("HUD/height") - 200};
  // Draw the  background
  SDL_RenderFillRect( renderer, &hudRect );



 //draw what buttons are being pressed (movement)
  SDL_Color moveBackColor= {(Uint8)Gamedata::getInstance().getXmlInt("MoveIndicator/backColor/r"),
     (Uint8)Gamedata::getInstance().getXmlInt("MoveIndicator/backColor/g"),
     (Uint8)Gamedata::getInstance().getXmlInt("MoveIndicator/backColor/b"),
     (Uint8)Gamedata::getInstance().getXmlInt("MoveIndicator/backColor/a")};
  
  SDL_SetRenderDrawColor( 
    renderer, moveBackColor.r, moveBackColor.g, moveBackColor.b, moveBackColor.a );


  SDL_Rect moveRect =  { Gamedata::getInstance().getXmlInt("MoveIndicator/startLoc/x"),
             Gamedata::getInstance().getXmlInt("MoveIndicator/startLoc/y"),
             Gamedata::getInstance().getXmlInt("MoveIndicator/width"),
             Gamedata::getInstance().getXmlInt("MoveIndicator/height")};

  SDL_RenderFillRect( renderer, &moveRect );

  SDL_Color moveColor = {(Uint8)Gamedata::getInstance().getXmlInt("MoveIndicator/Color/r"),
     (Uint8)Gamedata::getInstance().getXmlInt("MoveIndicator/Color/g"),
     (Uint8)Gamedata::getInstance().getXmlInt("MoveIndicator/Color/b"),
     (Uint8)Gamedata::getInstance().getXmlInt("MoveIndicator/Color/a")};

  int moveIndicatorX = Gamedata::getInstance().getXmlInt("MoveIndicator/startLoc/x");
  int moveIndicatorY = Gamedata::getInstance().getXmlInt("MoveIndicator/startLoc/y");


  //here we draw only if a corresponding key is being pressed
  if(up){
    io.writeText("up", moveIndicatorX+25, moveIndicatorY+0,moveColor);
  }

  if(down){
    io.writeText("down", moveIndicatorX+25, moveIndicatorY+50,moveColor);
  }

  if(left){
    io.writeText("left", moveIndicatorX+0, moveIndicatorY+25,moveColor);
  }

  if(right){
    io.writeText("right", moveIndicatorX+50, moveIndicatorY+25,moveColor);
  }

  //FPS
  int FPSX = Gamedata::getInstance().getXmlInt("FPS/startLoc/x");
  int FPSY = Gamedata::getInstance().getXmlInt("FPS/startLoc/y");

  std::stringstream strm;
  strm << "FPS: " << clock.getFps();
  SDL_Color color = {148, 0,201, 255};
  io.writeText(strm.str(),FPSX,FPSY,color);

  //AMMO pool stinger

  strm.str("");

  strm << "Free Ammo: " << freeAmmo;

  io.writeText(strm.str(),160,140,color);
  strm.str("");

  strm << "Ammo in use: " << usedAmmo;

  io.writeText(strm.str(),160,170,color);

  strm.str("");
  strm << "Flies alive: " << totalFlies;
  io.writeText(strm.str(),160,200,color);


  //Hud
  color = {0, 0,0, 255};
  strm.str("");
  strm << "Health: " << beeHealth;
  io.writeText(strm.str(),20,35,color);

  // First set the blend mode so that alpha blending will work;
  // the default blend mode is SDL_BLENDMODE_NONE!
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  // Set the health bar background color:
  SDL_SetRenderDrawColor( 
    renderer, backColor.r, backColor.g, backColor.b, backColor.a );
  // Draw the  background
  SDL_RenderFillRect( renderer, &barRect );

  SDL_SetRenderDrawColor( 
    renderer, barColor.r, barColor.g, barColor.b, barColor.a );
  int x = static_cast<int>(getX());
  int y = static_cast<int>(getY());
  SDL_Rect healthRect = {x, y, currentLength, barRect.h};
  SDL_RenderFillRect( renderer, &healthRect );

  // Set the  color for the health bar outline:
  SDL_SetRenderDrawColor( 
    renderer, barColor.r, barColor.g, barColor.b, barColor.a );
  SDL_RenderDrawRect( renderer, &barRect );


}

if(showWin){
 

 //background of HUD
  SDL_Color hudColor= {(Uint8)Gamedata::getInstance().getXmlInt("WinHUD/backColor/r"),
     (Uint8)Gamedata::getInstance().getXmlInt("WinHUD/backColor/g"),
     (Uint8)Gamedata::getInstance().getXmlInt("WinHUD/backColor/b"),
     (Uint8)Gamedata::getInstance().getXmlInt("WinHUD/backColor/a")};
  
  SDL_SetRenderDrawColor( 
    renderer, hudColor.r, hudColor.g, hudColor.b, hudColor.a );

  SDL_Rect hudRect =  { Gamedata::getInstance().getXmlInt("WinHUD/startLoc/x"),
             Gamedata::getInstance().getXmlInt("WinHUD/startLoc/y"),
             Gamedata::getInstance().getXmlInt("WinHUD/width"),
             Gamedata::getInstance().getXmlInt("WinHUD/height")};
  // Draw the  background
  SDL_RenderFillRect( renderer, &hudRect );

 std::stringstream strm;
  strm << "FLIES EXTERMINATED, GOOD JOB!";
  SDL_Color color = {148, 0,201, 255};
  io.writeText(strm.str(),Gamedata::getInstance().getXmlInt("WinHUD/startLoc/x") + 10, Gamedata::getInstance().getXmlInt("WinHUD/startLoc/y") + 10,color);
 }

if(showBegin){

 //background of HUD
  SDL_Color hudColor= {(Uint8)Gamedata::getInstance().getXmlInt("WinHUD/backColor/r"),
     (Uint8)Gamedata::getInstance().getXmlInt("WinHUD/backColor/g"),
     (Uint8)Gamedata::getInstance().getXmlInt("WinHUD/backColor/b"),
     (Uint8)Gamedata::getInstance().getXmlInt("WinHUD/backColor/a")};
  
  SDL_SetRenderDrawColor( 
    renderer, hudColor.r, hudColor.g, hudColor.b, hudColor.a );

  SDL_Rect hudRect =  { Gamedata::getInstance().getXmlInt("WinHUD/startLoc/x"),
             Gamedata::getInstance().getXmlInt("WinHUD/startLoc/y"),
             Gamedata::getInstance().getXmlInt("WinHUD/width")-150,
             Gamedata::getInstance().getXmlInt("WinHUD/height")};
  // Draw the  background
  SDL_RenderFillRect( renderer, &hudRect );

 std::stringstream strm;
  strm << "EXTERMINATE THE FLIES!";
  SDL_Color color = {148, 0,201, 255};
  io.writeText(strm.str(),Gamedata::getInstance().getXmlInt("WinHUD/startLoc/x") + 10, Gamedata::getInstance().getXmlInt("WinHUD/startLoc/y") + 10,color);

}
}

void Hud::getPoolData(int freeAm , int usedAm){

  freeAmmo = freeAm;
  usedAmmo = usedAm;
}

void Hud::getHealthData(int health){

  beeHealth = health;
  currentLength = beeHealth*2;

}


void Hud::setUp(bool yes){

   if(yes){ up = true;}else{up = false;};
}

void Hud::setDown(bool yes){

   if(yes){ down = true;}else{down = false;};
}
void Hud::setLeft(bool yes){

   if(yes){ left = true;}else{left = false;};
}
void Hud::setRight(bool yes){

   if(yes){ right = true;}else{right = false;};
}


void Hud::drawUp() const {

 SDL_Color color = {148, 0,201, 255};

  //HealthBar
  color = {0, 0,0, 255};
  io.writeText("UP", 300, 300,color);

  color = {0, 0,0, 255};
  io.writeText("HEALTH2", 55, 55,color);

}
