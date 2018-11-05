//#include <iostream>
//mio
//#include <cstdlib>
//#include <stdio.h>
//#include <stdlib.h>
//#include <termios.h>
//#include <term.h>
//#include <curses.h>
//#include <unistd.h>
//#include <vector>
#include "Agent.hpp"
#include "Vector.hpp"
#include "Time.hpp"
unsigned long timeGetTime(){
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_usec/1000;
  }

CTriggerSystem::CTriggerSystem(){

}
CTriggerSystem::~CTriggerSystem(){

}

unsigned long CTriggerSystem::RegisterTriger(
  EnumTriggerType _eTriggerType, unsigned long _nPriority,
  unsigned long _idSource,const Vector& _vPos, float _fRadius,
  float _fDuration,bool _bDynamicSourcePos)
{




  //Create a trigger record, and fill it in
  TriggerRecordStruct* pTriggerRecord =
  new TriggerRecordStruct(_eTriggerType,_idSource,_vPos,_fRadius,_fDuration,_bDynamicSourcePos);
  //Trigger records are sorted by priority
  m_mapTriggerMap.insert(TRIGGER_MAP::value_type(_nPriority,pTriggerRecord));

  //return unique id for this triger
  return pTriggerRecord->nTriggerID;

}


void CTriggerSystem::RemoveTrigger(unsigned long nTriggerID)
{
  TRIGGER_MAP::iterator it=m_mapTriggerMap.begin();
  while(it!=m_mapTriggerMap.end()){
    if(it->second->nTriggerID == nTriggerID){
      delete(it->second);
      return;
    }
    else ++it;

  }
}

void CTriggerSystem::Update()
{
  CAgent* pAgent=NULL;
  float fDistance=0.f;

  TriggerRecordStruct* pRec;
  TRIGGER_MAP::iterator it;
  unsigned long nCurTime = timeGetTime();
  //unsigned long nCurTime = 0;
  //Delete expired trigger records. For records tha are not
  //expired, update position if the dynamic flag is set
  it =m_mapTriggerMap.begin();
  while(it!=m_mapTriggerMap.end())
  {
    pRec = it->second;
    if( (pRec->nExpirationTime!=0)&&
        (pRec->nExpirationTime< nCurTime))
        {
          delete(pRec);
          it=m_mapTriggerMap.erase(it);

        }else{
          //Update pos if dynamic flag is set.Reset time-stamp
          if(pRec->bDynamicSourcePos==true)
          {
            //Update(pRec->vPos);//funcion para actualizar la posicion
            //pRec->nTimeStamp = nCurTime;
          }
          ++it;
        }
  }
  //Trigger agents
  //int g_nNumAgents=20;//numero actual de agentes
  irr::core::list<CAgent*>::Iterator it2 ;
  it2=CAgent::hola.begin();
  for(unsigned long i=0; i<CAgent::hola.size();++i)
  {
    pAgent=*it2;
    //pAgent->SetNextTriggerUpdate(nCurTime);
    //llop thru exixting trigger records
    for(it=m_mapTriggerMap.begin();
        it!=m_mapTriggerMap.end();++it)
    {
      pRec=it->second;
      //Does agent respond to trigger?
      if(!(pRec->eTriggerType & pAgent->GetTriggerFlags()))
        continue;
      //is source the agent itself?
      if(pRec->idSource==pAgent->nCAgentID)
        continue;
      //Check radius
      //
      fDistance=DIST(pRec->vPos,pAgent->GetPosition());//funcion calcular la distancia
      if(fDistance > pRec->fRadius)
        continue;
      //handletriger return true if the agent responded
      //to the trigger
      if(pAgent->HandleTrig(pRec))
      {
        //Listen to highest priority trig at any instant
        break;
      }
    }
    it2++;
  }
}
TriggerRecordStruct::TriggerRecordStruct(EnumTriggerType _eTriggerType,unsigned long _idSource,const Vector _vPos,float _fRadius,
                    unsigned long _fDuration,bool _bDynamicSourcePos)
{
  eTriggerType=_eTriggerType;
  nTriggerID=id;
  id++;
  idSource=_idSource;
  vPos=_vPos;
  fRadius=_fRadius;
  nTimeStamp=timeGetTime();// eso o sucedaneo
  nExpirationTime=timeGetTime()+_fDuration;
  bDynamicSourcePos=_bDynamicSourcePos;


}

//Pruebas
//unsigned long CTriggerSystem::RegisterTriger(
//  EnumTriggerType _eTriggerType, unsigned long _nPriority,
//  unsigned long _idSource,const Vector& _vPos, float _fRadius,
//  float _fDuration,bool _bDynamicSourcePos){return 0;}
//void CTriggerSystem::RemoveTrigger(unsigned long nTriggerID){}
//void CTriggerSystem::Update(){}
