
#include <iostream>

#include "Succeeder.hpp"

// Rama del arbol con solo un hijo
// Los comportamientos del hijo pueden repetirse n veces si se quiere

//-----------------------------------------//
// Esta es la estructura de cada DECORATOR //
//-----------------------------------------//

Status Succeeder::update(){
  m_pChild->tick();
  if(m_pChild->getStatus()==BH_FAILURE){
    return BH_SUCCESS;
  }else {
    if(m_pChild->getStatus()==BH_RUNNING) return update();
    return m_pChild->getStatus();
  }
}
