#ifndef COMPOSITE_HPP
#define COMPOSITE_HPP

#include "behavior.hpp"
#include <vector>


class composite : public behavior {
public:

   virtual void addChild(behavior* hijo);
   virtual void removeChild(behavior* hijo);
   virtual void clearChildren();
protected:
   typedef std::vector<behavior*> behaviors;
   behaviors m_Children;
};


#endif /* end of include guard: COMPOSITE_HPP */
