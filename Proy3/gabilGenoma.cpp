#include <ga/GAGenome.h>
#include "baseGABIL.cpp"
class MyGenome : public MyObject, public GAGenome {
public:
  GADefineIdentity("MyGenome", 201);
  static void Init(GAGenome&);
  static int Mutate(GAGenome&, float);
  static float Compare(const GAGenome&, const GAGenome&);
  static float Evaluate(GAGenome&);
  static int Cross(const GAGenome&, const GAGenome&, GAGenome*, GAGenome*);

public:
  MyGenome() : GAGenome(Init, Mutate, Compare) { 
    evaluator(Evaluate); 
    crossover(Cross); 
  }
  MyGenome(const MyGenome& orig) { copy(orig); }
  virtual ~MyGenome() {}
  MyGenome& operator=(const GAGenome& orig){
    if(&orig != this) copy(orig);
    return *this;
  }

  virtual GAGenome* clone(CloneMethod) const {return new MyGenome(*this);}
  virtual void copy(const GAGenome& orig) {
    GAGenome::copy(orig);  // this copies all of the base genome parts
    // copy any parts of MyObject here
    // copy any parts of MyGenome here
  }

  // any data/member functions specific to this new class
};

void 
MyGenome::Init(GAGenome&){
  // your initializer here
}

int 
MyGenome::Mutate(GAGenome&, float){
  // your mutator here
}

float 
MyGenome::Compare(const GAGenome&, const GAGenome&){
  // your comparison here
}

float 
MyGenome::Evaluate(GAGenome&){
  // your evaluation here
}

int
MyGenome::Cross(const GAGenome& mom, const GAGenome& dad,
                GAGenome* sis, GAGenome* bro){
  // your crossover here
}