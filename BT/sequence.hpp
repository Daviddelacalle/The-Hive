#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP
#include <iostream>

#include "composite.hpp"


class sequence : public composite{
public:
 //Status update();
 sequence();
 ~sequence();
protected:
  behaviors::iterator m_CurrentChild;
 virtual void onInitialize();
 virtual void onTerminate(Status state);
  virtual Status update();

};

#endif /* end of include guard: SEQUENCE_HPP */
