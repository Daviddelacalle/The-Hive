#include "CGranade0.hpp"

CGranade0::CGranade0()
: CGranade(30,200),damage(2000)
{}

CGranade0::~CGranade0() {}

void CGranade0::explosion(){
    TData mes;
    mes.add(kDat_Damage,damage);
    EventSystem->RegisterTriger(kTrig_Explosion,1,0,cTransform->getPosition(), radius, 50,false,mes);
}
