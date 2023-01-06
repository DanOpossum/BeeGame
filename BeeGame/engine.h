#include <vector>
#include <SDL.h>
//#include "ioMod.h"
#include "renderContext.h"
//#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "beesprite.h"
#include "subjectSprite.h"
#include "collisionStrategy.h"
#include "hud.h"
#include "sound.h"

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();
  void switchSprite();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
private:

  const RenderContext* rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World clouds;
  World trees;
  Viewport& viewport;

  //Drawable* star;
  //Drawable* spinningStar;

  std::vector<Drawable*>  sprites;
  std::vector<Drawable*>  spritesBackground;
  SubjectSprite* subjectSprite;
  int currentSprite;

  bool makeVideo;


  std::vector<CollisionStrategy*> strategies;
  int currentStrategy;
  bool collision;

  void draw() const;
  void update(Uint32);

  void printScales() const;
  void checkForCollisions();

  Hud* hud;

  bool hudOn;
  bool godMode;
  int totalFlies;
  time_t start;


};
