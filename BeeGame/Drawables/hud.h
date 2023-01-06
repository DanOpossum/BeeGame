#include "SDL.h"
#include "gameData.h"
#include "clock.h"
#include "drawable.h"
#include "ioMod.h"

class Hud : public Drawable {
public:
  Hud(const std::string&);
  void draw() const;
  void drawUp() const;
  void setUp(bool);
  void setDown(bool);
  void setLeft(bool);
  void setRight(bool);
  void update(Uint32);
  void getPoolData(int , int);
  void getHealthData(int);
  void reset() { currentLength = barRect.w; }
  void toggleHUD();
  void receiveTotalFlies(int x) {totalFlies = x;}
  void showWinNow(){ showWin = true;};
  void dontShowBeginNow(){ showBegin = false;};

  Hud(const Hud&) = delete;
  Hud& operator=(const Hud&) = delete;

  virtual int getScaledWidth() const  { return barRect.w; }
  virtual int getScaledHeight() const { return barRect.h; }
  virtual const Image* getImage() const { 
    throw std::string("No image in Hud");
  }
  virtual const SDL_Surface* getSurface() const { 
    //return image->getSurface();
    throw std::string("No image in HealthBar");
  }
private:
  SDL_Renderer* renderer;
  SDL_Rect barRect;
  Clock& clock;
  int currentLength;
  int increment;  
  float interval; // elapsed time for each decrement (1000ms=1sec)
  int deltaTime;
  SDL_Color barColor;
  SDL_Color backColor;
  IoMod& io;
  bool up;
  bool down;
  bool left;
  bool right;
  int freeAmmo;
  int usedAmmo;
  int beeHealth;
  bool showHud;
  int totalFlies;
  bool showWin;
  bool showBegin;
};
