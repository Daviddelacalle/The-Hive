#include "SoundSystem.hpp"

SoundSystem::SoundSystem(){
	system 				= nullptr;
	lowLevelSystem 		= nullptr;

	masterBank 			= nullptr;
	stringsBank 		= nullptr;
	sfxBank				= nullptr;
	ambienteBank 		= nullptr;
	musicaBank			= nullptr;
	vocesBank			= nullptr;

	// Create the Studio System object
	ERRCHECK(FMOD::Studio::System::create		(&system));
	ERRCHECK(system->getLowLevelSystem			(&lowLevelSystem));
	ERRCHECK(lowLevelSystem->setSoftwareFormat	(0,FMOD_SPEAKERMODE_5POINT1,0));
	ERRCHECK(lowLevelSystem->setOutput			(FMOD_OUTPUTTYPE_AUTODETECT));
	ERRCHECK(system->initialize					(512, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_NORMAL, 0));

	system->loadBankFile("assets/FMOD/Master_Bank.bank",FMOD_STUDIO_LOAD_BANK_NORMAL,&masterBank);
	system->loadBankFile("assets/FMOD/Master_Bank.strings.bank",FMOD_STUDIO_LOAD_BANK_NORMAL,&stringsBank);
	system->loadBankFile("assets/FMOD/Ambiente.bank",FMOD_STUDIO_LOAD_BANK_NORMAL,&ambienteBank);
	system->loadBankFile("assets/FMOD/SFX.bank",FMOD_STUDIO_LOAD_BANK_NORMAL,&sfxBank);
	system->loadBankFile("assets/FMOD/Musica.bank",FMOD_STUDIO_LOAD_BANK_NORMAL,&musicaBank);
	system->loadBankFile("assets/FMOD/Voces.bank",FMOD_STUDIO_LOAD_BANK_NORMAL,&vocesBank);
}


SoundSystem::~SoundSystem(){}

SoundEvent* SoundSystem::createSound(const std::string &_str){
	FMOD::Studio::EventDescription 	*event 		= nullptr;
	FMOD::Studio::EventInstance 	*instance 	= nullptr;
	ForestSound *ret;

	if(soundEvents.find(_str) == soundEvents.end()){

		ERRCHECK(system->getEvent(_str.c_str(),&event));
		ERRCHECK(event->createInstance(&instance));

		ret = new ForestSound(instance);
		soundEvents.insert(std::make_pair(_str,ret));
	}
	else{
		ret = static_cast<ForestSound*>(soundEvents[_str]);
	}

	return ret;
}

void SoundSystem::update(){
	if(system){
		system->update();
	}
}

void SoundSystem::CLIN(){
	// A clinear!
	std::map<std::string,SoundEvent*>::iterator it = soundEvents.begin();
	while(it!=soundEvents.end()){
		delete it->second;
		it++;
	}
	soundEvents.clear();

	masterBank->unload();
	stringsBank->unload();
	sfxBank->unload();
	ambienteBank->unload();
	musicaBank->unload();
	vocesBank->unload();
	system->release();
}
