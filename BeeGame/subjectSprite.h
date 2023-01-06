#ifndef SUBJECTSPRITE__H
#define SUBJECTSPRITE__H

#include <string>
#include <list>
#include <cmath>
#include "beesprite.h"
#include "smartSprite.h"

//class SmartSprite;

class SubjectSprite : public BeeSprite {
public:
  SubjectSprite(const std::string&);
  SubjectSprite(const SubjectSprite&);

  void update(Uint32 ticks);
  void attach( SmartSprite* o ) { observers.push_back(o); } 
  void detach( SmartSprite* o );

protected:
  std::list<SmartSprite*> observers;
  SubjectSprite& operator=(const SubjectSprite&);
};
#endif
