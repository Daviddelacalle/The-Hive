#include "Motor2D.hpp"
//#include <iostream>
//#include <SMaterial>
#include <ComponentArch/ObjectManager.hpp>
#include <ComponentArch/Components/CAIEnem.hpp>
#include <Omicron/2D/Imagen2D.hpp>
#include <Omicron/2D/Boton2D.hpp>
#include <Omicron/2D/Cuadrado2D.hpp>
#include <Omicron/2D/Texto2D.hpp>
#include <ComponentArch/Components/CVida.hpp>
/*
std::string imgarmaP;
std::string imgarmaS;
*/
Motor2D::Motor2D(){
    motor = Singleton<Omicron>::Instance();

    imgarmaP="assets/HUD/cf_hud_b.jpg";
    imgarmaS="assets/HUD/cf_hud_b.jpg";
    //font = IrrlichtDevice->getGUIEnvironment()->getFont("assets/Fonts/Debug.png");
    //irr::video::IVideoDriver* driver = IrrlichtDevice->getVideoDriver();

    //ancho=(driver->getScreenSize().Width/2);
    //alto=(driver->getScreenSize().Height/2);

    SS = Singleton<SoundSystem>::Instance();

    s_hover = new SonidoNormal();
    SS->createSound("event:/SFX/Menu/Seleccionar",s_hover);

}


Motor2D::~Motor2D(){
    //delete s_hover;
    CLINMenu();
}
void Motor2D::draw(){

    auto it=TEXT.begin();
    while(it!=TEXT.end()){
        (*it)->Draw();
        it++;
    }
    auto it3=BOTONES.begin();
    while(it3!=BOTONES.end()){
        (*it3)->Draw();
        it3++;
    }
    auto it2=IMAGENES.begin();
    while(it2!=IMAGENES.end()){
        (*it2)->Draw();
        it2++;
    }
}

Cuadrado2D* Motor2D::addRect(float x, float y,float w, float h){
    auto nuevo = new Cuadrado2D(x,y,w,h);
    RECTANGULOS.push_back(nuevo);
    return nuevo;
}

int Motor2D::checkbuton(){
    if(motor->isLClickPressed()==false){
        return -1;
    }
    double x,y;

    motor->getCursorPosition(x,y);
    auto it=BOTONES.begin();
    while(it!=BOTONES.end()){
        auto but=*it;
        if(but->checkOn(x, y)){
            return but->getType();
        }
        it++;
    }
    return -1;
}

void Motor2D::aplyhover(){
    double x,y;
    motor->getCursorPosition(x,y);
    auto it=BOTONES.begin();
    while(it!=BOTONES.end()){
        auto but=*it;
        if(but->checkOn(x, y)){
            ////std::cout << "encima" << '\n';
            //setImage
            if(!but->getHov()){
                s_hover->play();
            }

            but->hover(true);
        }
        else{
            but->hover(false);
        }
        it++;
    }
}

Imagen2D* Motor2D::AddImage(std::string source,float _posx,float _posy,float _width,float _height){
    float x,y,w,h;
    x=_posx/100.0;
    y=_posy/100.0;
    w=(_posx+_width)/100.0;
    h=(_posy+_height)/100.0;
    auto nuevo = new Imagen2D(x,y,w,h,source);


    IMAGENES.push_back(nuevo);
    return nuevo;
}

Boton2D* Motor2D::addButton(float x, float y, float w,float h,EnumButtonType id,std::string imagenP,std::string imagenS,std::string texto,bool focus,glm::vec4 _color){
    h=(y+h)/100.0;
    w=(x+w)/100.0;
    x=x/100.0;
    y=y/100.0;
    auto nuevo=new Boton2D(x,y,w,h,id,imagenP,imagenS,texto,focus);
    nuevo->setColor(_color);
    BOTONES.push_back(nuevo);
    return nuevo;
}

void Motor2D::addText(float x, float y,const std::string &Name,glm::vec4 _color,float tam){
    x=x/100.0;
    y=y/100.0;
    auto nuevo=new Texto2D( x, y,Name,_color,tam);
    TEXT.push_back(nuevo);//
}

void Motor2D::setprogress(int hab,float prog){
    if(hab==0){
        perc=1-prog;

    }else if(hab==1){
        perc2=1-prog;

    }
    else{
        perc3=1-prog;

    }
}

void Motor2D::setvida(float _vida){
    // //std::cout << "hacemos algo" << '\n';
    vida=_vida;
}
void Motor2D::setVolDialogo(int _vol){
VolDialogo=_vol;
}
void Motor2D::setVolEffect(int _vol){
VolEffect=_vol;
}
void Motor2D::setVolMusic(int _vol){
VolMusic=_vol;
}
void Motor2D::changeWeapon(){
    //cambiamos las balas
    int aux;
    aux=balaP;
    balaP=balaS;
    balaS=aux;

    aux=balaP_TOT;
    balaP_TOT=balaS_TOT;
    balaS_TOT=aux;


    // hab3

    auto auxi=imgarmaP;
    imgarmaP=imgarmaS;
    imgarmaS=auxi;

    IMAGENES[4]->setImage(imgarmaP);
    IMAGENES[3]->setImage(imgarmaS);


}
void Motor2D::setbullet(int tipo,int b_act, int b_tot){
    if(tipo==0){
        balaP       = b_act;
        balaP_TOT   = b_tot;

    }else {
        balaS       = b_act;
        balaS_TOT   = b_tot;
    }
}

void Motor2D::setWeaponImg(int tipo,std::string img){
    if(tipo==0){//P
            IMAGENES[4]->setImage(img);
            imgarmaP=img;
    }else {//S
        IMAGENES[3]->setImage(img);
        imgarmaS=img;
    }
}



std::string  Motor2D::BoolToString(bool b)
{
  if(b){
     return  std::string("true");
  }
  else{
      return std::string("false");
  }
}

int Motor2D::InitAIDebug(int id){
    CLINMenu();
    ObjectManager* Manager = Singleton<ObjectManager>::Instance();
    // //std::cout << "id:" <<id<< '\n';
    CAIEnem* AIEnem = static_cast<CAIEnem*>(Manager->getComponent(gg::AIENEM,id));
    CVida* Vida = static_cast<CVida*>(Manager->getComponent(gg::VIDA,id));


    std::string tipo;

    switch (AIEnem->getEnemyType()) {
        case gg::SOLDIER:
            tipo="SOLDIER";
            break;
        case gg::TANK:
            tipo="TANK";
            break;
        case gg::RUSHER:
            tipo="RUSHER";
            break;
        case gg::SWARM:
            tipo="SWARM";
            break;
        case gg::TRACKER:
            break;
    }
    addText(45, 10,"Debug Enemigo");

    addText(10, 10,"Tipo de enemigo:"  +tipo , glm::vec4(1,0,0,1));

    addText(10,15,"Vida:"                              +std::to_string(Vida->getVida()                      ),glm::vec4(1,0,0,1));

    addText(10,20,"Estoy viendo al jugador:"           +BoolToString(AIEnem->playerSeeing                   ),glm::vec4(1,0,0,1));
    addText(10,25,"He visto al jugador:"               +BoolToString(AIEnem->playerSeen                     ),glm::vec4(1,0,0,1));
    addText(10,30,"Estoy afectado por ultrasonido:"    +BoolToString(AIEnem->ultrasonido                    ),glm::vec4(1,0,0,1));
    addText(10,35,"Estoy afectado por senyuelo:"       +BoolToString(AIEnem->senyuelo                       ),glm::vec4(1,0,0,1));
    addText(10,40,"Jugador a rango:"                   +BoolToString(AIEnem->playerOnRange                  ),glm::vec4(1,0,0,1));
    addText(10,45,"Puedo atacar:"                      +BoolToString(AIEnem->getImAttacking()               ),glm::vec4(1,0,0,1));
    addText(10,50,"Acaba de morir un aliado cercano:"  +BoolToString(AIEnem->getCloserAllyIsDead()          ),glm::vec4(1,0,0,1));
    addText(10,55,"Me esta atacando el jugador:"       +BoolToString(AIEnem->getPlayerIsAttacking()         ),glm::vec4(1,0,0,1));

    addText(10,60,"Rango de vision:"                   +std::to_string(AIEnem->Vrange                       ),glm::vec4(1,0,0,1));
    addText(10,65,"Rango de ataque:"                   +std::to_string(AIEnem->Arange                       ),glm::vec4(1,0,0,1));
    addText(10,70,"Enfado:"                            +std::to_string(AIEnem->getRage()                    ),glm::vec4(1,0,0,1));

    addText(10,75,"Tarea actual:"                      +std::to_string(AIEnem->arbol->taskactual()          ),glm::vec4(1,0,0,1));




/*
    AddStaticTextToBuffer(porc_ancho(45),porc_alto(10),std::string("Debug Enemigo"),gg::Color(0,0,0,1));

    AddStaticTextToBuffer(porc_ancho(10),porc_alto(10),std::string("Tipo de enemigo:"                              +tipo                      ),gg::Color(255,0,0,1));

    AddStaticTextToBuffer(porc_ancho(10),porc_alto(15),std::string("Vida:"                              +std::to_string(Vida->getVida()                      )),gg::Color(255,0,0,1));

    AddStaticTextToBuffer(porc_ancho(10),porc_alto(20),std::string("Estoy viendo al jugador:"           +BoolToString(AIEnem->playerSeeing              )),gg::Color(255,0,0,1));
    AddStaticTextToBuffer(porc_ancho(10),porc_alto(25),std::string("He visto al jugador:"               +BoolToString(AIEnem->playerSeen                )),gg::Color(255,0,0,1));
    AddStaticTextToBuffer(porc_ancho(10),porc_alto(30),std::string("Estoy afectado por ultrasonido:"    +BoolToString(AIEnem->ultrasonido               )),gg::Color(255,0,0,1));
    AddStaticTextToBuffer(porc_ancho(10),porc_alto(35),std::string("Estoy afectado por senyuelo:"       +BoolToString(AIEnem->senyuelo                  )),gg::Color(255,0,0,1));
    AddStaticTextToBuffer(porc_ancho(10),porc_alto(40),std::string("Jugador a rango:"                   +BoolToString(AIEnem->playerOnRange             )),gg::Color(255,0,0,1));
    AddStaticTextToBuffer(porc_ancho(10),porc_alto(45),std::string("Puedo atacar:"                      +BoolToString(AIEnem->getImAttacking()          )),gg::Color(255,0,0,1));
    AddStaticTextToBuffer(porc_ancho(10),porc_alto(50),std::string("Acaba de morir un aliado cercano:"  +BoolToString(AIEnem->getCloserAllyIsDead()     )),gg::Color(255,0,0,1));
    AddStaticTextToBuffer(porc_ancho(10),porc_alto(55),std::string("Me esta atacando el jugador:"       +BoolToString(AIEnem->getPlayerIsAttacking()    )),gg::Color(255,0,0,1));

    AddStaticTextToBuffer(porc_ancho(10),porc_alto(60),std::string("Rango de vision:"                   +std::to_string(AIEnem->Vrange                    )),gg::Color(255,0,0,1));
    AddStaticTextToBuffer(porc_ancho(10),porc_alto(65),std::string("Rango de ataque:"                   +std::to_string(AIEnem->Arange                    )),gg::Color(255,0,0,1));
    AddStaticTextToBuffer(porc_ancho(10),porc_alto(70),std::string("Enfado:"                            +std::to_string(AIEnem->getRage()                 )),gg::Color(255,0,0,1));

    AddStaticTextToBuffer(porc_ancho(10),porc_alto(75),std::string("Tarea actual:"                      +std::to_string(AIEnem->arbol->taskactual()       )),gg::Color(255,0,0,1));
*/

    //ObjectManager* Manager = Singleton<ObjectManager>::Instance()->getComponent(gg::TRANSFORM, pRec->idSource);


    addButton(45,50,10,9,CONTINUE,"assets/HUD/Botonsolo.png","assets/HUD/Botonsolo.png","Continuar",true);
    return 1;

}

//Menu principal
int Motor2D::InitPause(){
    CLINMenu();
    addButton(40,31,20,10,CONTINUE,"assets/HUD/Botonsolo.png","assets/HUD/Botonsolo.png","Continuar",true);
    addButton(40,43,20,10,GOOPTIONS,"assets/HUD/Botonsolo.png","assets/HUD/Botonsolo.png","Opciones");
    addButton(40,55,20,10,RETURNMENU,"assets/HUD/Botonsolo.png","assets/HUD/Botonsolo.png","Salir al menu");


    return 3;

}
//Main Menu
int Motor2D::InitMenu(){
    CLINMenu();
    AddImage("assets/HUD/menu.png",0,0,100,100);
    //addText(45,10,"Menu principal");

    auto but1=addButton(39,57,9,12,GOPLAY,"assets/HUD/Botonsolo.png","assets/HUD/Botonsolo.png","Play");
    but1->setSesgo(-0.015);
    but1=addButton(37,70,10,14,GOCREDITS,"assets/HUD/Botonsolo.png","assets/HUD/Botonsolo.png","Credits");
    but1->setSesgo(-0.02);
    but1=addButton(54,57,9,12,GOOPTIONS,"assets/HUD/Botonsolo.png","assets/HUD/Botonsolo.png","Options");
    but1->setSesgo(0.015);
    but1=addButton(55,70,10,14,CLOSE,"assets/HUD/Botonsolo.png","assets/HUD/Botonsolo.png","Exit");
    but1->setSesgo(0.02);
    return 4;
}
//Jugar
int Motor2D::InitMenu2(){
    CLINMenu();
    AddImage("assets/HUD/menucerca.png",0,0,100,100);

    //addText(45,10,"Play");
    addButton(40,21,20,10,DIFF1,"assets/HUD/Botonsolo.png","assets/HUD/Botonsolo.png","Larva",true);
    addButton(40,33,20,10,DIFF2,"assets/HUD/Botonsolo.png","assets/HUD/Botonsolo.png","Alien");
    addButton(40,45,20,10,DIFF3,"assets/HUD/Botonsolo.png","assets/HUD/Botonsolo.png","The hive");
    addButton(30,60,40,20,START,"assets/HUD/Botonsolo.png","assets/HUD/Botonsolo.png","Play");

    addButton(20,20,5,9,GOMAIN,"assets/HUD/Botonsoloatras.png","assets/HUD/Botonsoloatras.png","");
return 5;
}
//Creditos
int Motor2D::InitMenu3(){
    CLINMenu();
    AddImage("assets/HUD/menucerca.png",0,0,100,100);

    //addText(45,10,"Credits");
    addButton(20,20,5,9,GOMAIN,"assets/HUD/Botonsoloatras.png","assets/HUD/Botonsoloatras.png","");

    return 1;
}
//Opciones
int Motor2D::InitMenu4(){
    CLINMenu();
    AddImage("assets/HUD/menucerca.png",0,0,100,100);

    //addText(45,10,"Options");
    addButton(40,31,20,10,GOVIDEO,"assets/HUD/Botonsolo.png","assets/HUD/Botonsolo.png","Graphics");
    addButton(40,43,20,10,GOMUSIC,"assets/HUD/Botonsolo.png","assets/HUD/Botonsolo.png","Audio");
    addButton(40,55,20,10,GOCONTROLLS,"assets/HUD/Botonsolo.png","assets/HUD/Botonsolo.png","Controlls");

    addButton(20,20,5,9,CONTINUE,"assets/HUD/Botonsoloatras.png","assets/HUD/Botonsoloatras.png","");

    return 4;
}
//graficos
int Motor2D::InitMenu5(){
    CLINMenu();
    AddImage("assets/HUD/menucerca.png",0,0,100,100);

    //addText(45,10,"Video");
    addButton(20,20,5,9,GOINITOPTIONS,"assets/HUD/Botonsoloatras.png","assets/HUD/Botonsoloatras.png","");

    return 1;
}
//sonido
int Motor2D::InitMenu6(){
    CLINMenu();

    AddImage("assets/HUD/menucerca.png",0,0,100,100);




    //addText(45,10,"Audio"    );
    addText(30,51,"Dialogues"    );
    addText(40,51, std::to_string(VolDialogo));

    addButton(50,50,2,5,MOREDIALOD,"assets/HUD/Botonsolo.png","assets/HUD/Botonsolo.png","+");
    addButton(53,50,2,5,LESSDIALOD,"assets/HUD/Botonsolo.png","assets/HUD/Botonsolo.png","-");
    addText(30,57,"Music"  );
    addText(40,57,std::to_string(VolMusic));
    addButton(50,56,2,5,MOREMUSIC,"assets/HUD/Botonsolo.png","assets/HUD/Botonsolo.png","+");
    addButton(53,56,2,5,LESSMUSIC,"assets/HUD/Botonsolo.png","assets/HUD/Botonsolo.png","-");
    addText(30,63,"Effects"    );
    addText(40,63, std::to_string(VolEffect) );
    addButton(50,62,2,5,MOREEFFECT,"assets/HUD/Botonsolo.png","assets/HUD/Botonsolo.png","+");
    addButton(53,62,2,5,LESSEFFECT,"assets/HUD/Botonsolo.png","assets/HUD/Botonsolo.png","-");

    addButton(20,20,5,9,GOINITOPTIONS,"assets/HUD/Botonsoloatras.png","assets/HUD/Botonsoloatras.png","");

    return 7;
}
//controles
int Motor2D::InitMenu7(){
    CLINMenu();
    AddImage("assets/HUD/menucerca.png",0,0,100,100);

    //addText(45,10,"Controlls");

    addButton(20,20,5,9,GOINITOPTIONS,"assets/HUD/Botonsoloatras.png","assets/HUD/Botonsoloatras.png","");

    return 1;
}


void Motor2D::InitHUD(){


    AddImage("assets/HUD/hab1.png",  2, 90,7,10);
    auto boton=addRect(                     2, 90,7,10);
    boton->setColor(glm::vec4(1,1,1,0.25));


    AddImage("assets/HUD/hab2.png",  12, 90,7,10);
    boton=addRect(                          12, 90,7,10);
    boton->setColor(glm::vec4(1,1,1,0.25));


    AddImage("assets/HUD/hab3.png",  22,90,7,10);
    //AddImage("hab3","assets/HUD/AMETRALLADORA_HUD.png",  22,90,7,10);
    boton=addRect(                          22,90,7,10);
    boton->setColor(glm::vec4(1,1,1,0.25));


    float _x,_y,x,y,w,h;
    x=75;
    y=85;
    w=20;
    h=15;
    _x= x+w*0.65;
    _y= y+h*0.7;
    auto yep=AddImage(imgarmaS,75,85,20,15);//secundaria
    yep->setZindex(-0.9997);
    addText(_x, _y,"arma0",glm::vec4(1,1,1,1),30);
    x=70;
    y=80;
    w=20;
    h=15;
    _x= x+w*0.65;
    _y= y+h*0.7;
    //AddImage("1arma","assets/HUD/AMETRALLADORA_HUD.png",70,80,20,15); // Principal
    yep=AddImage(imgarmaP,70,80,20,15); // Principal
    yep->setZindex(-0.9998);
    addText(_x, _y,"arma1",glm::vec4(1,1,1,1),30);


    AddImage("assets/HUD/Vida.png",  60,3.77,35,7);
    boton=addRect(                          60,2,35,7);
    boton->setColor(glm::vec4(0,0.5,0,1));
    boton->setZindex(-0.9998);
    boton=addRect(                          0.665,0.0377,0.95,0.1047);
    boton->setColor(glm::vec4(0.4,0.4,0.4,1));
    boton->setZindex(-0.9997);
    //cruceta
    float anchura,longitud,c;

    //cruceta negra
    anchura=0.002;
    longitud=0.011;
    c=0.5;
    boton=addRect(c-longitud,c-anchura,c+longitud,c+anchura);
    boton->setColor(glm::vec4(0,0,0,1));
    //ratio d epantalla para tenerlo exacto :D
    anchura=0.0012;
    longitud=0.0185;
    boton=addRect(c-anchura,c-longitud,c+anchura,c+longitud);
    boton->setColor(glm::vec4(0,0,0,1));

    //cruceta blanca

    anchura=0.001;
    longitud=0.01;
    c=0.5;
    boton=addRect(c-longitud,c-anchura,c+longitud,c+anchura);
    boton->setColor(glm::vec4(100,100,100,1));
    //ratio d epantalla para tenerlo exacto :D
    anchura=0.001;
    longitud=0.0165;
    boton=addRect(c-anchura,c-longitud,c+anchura,c+longitud);
    boton->setColor(glm::vec4(100,100,100,1));



    //AddImage("G1","assets/HUD/Botonsolo.png",porc_ancho(2),porc_alto(2),porc_alto(10),porc_alto(10));
    //AddImage("G2","assets/HUD/Botonsolo.png",porc_ancho(13),porc_alto(2),porc_alto(10),porc_alto(10));
    //AddImage("G3","assets/HUD/Botonsolo.png",porc_ancho(24),porc_alto(2),porc_alto(10),porc_alto(10));

    // MAPA A FUNCIONES MOLON
    //mapHudFunctions[0]=&Motor2D::HUD_hability1;
    //mapHudFunctions[1]=&Motor2D::HUD_hability2;
    //mapHudFunctions[2]=&Motor2D::HUD_hability3;
    //mapHudFunctions[3]=&Motor2D::HUD_vida;
    //mapHudFunctions[4]=&Motor2D::HUD_arma0;
    //mapHudFunctions[5]=&Motor2D::HUD_arma1;

    perc        =   0;
    perc2       =   0;
    perc3       =   0;
    balaP       =   0;
    balaS       =   0;
    vida        =   1;
    VolDialogo  =   50;
    VolEffect   =   50;
    VolMusic    =   50;

}



//void Motor2D::AddTextToBuffer(const std::string &Text, const gg::Color &color){
//    if(BUFFER.size()>35)
//        BUFFER.pop_front();

//    BUFFER.emplace_back(Text, color);
//}
//void Motor2D::AddStaticTextToBuffer(int x,int y, std::string Text,  gg::Color color){
//    TEXT_BUFFER.emplace_back(x,y,Text, color);
//}

void Motor2D::CLINMenu(){
    //liberar eso
    //std::vector<Cuadrado2D*> RECTANGULOS;
    //std::vector<Boton2D*> BOTONES;
    //std::vector<Boton2D*> TEXT;


    auto it=IMAGENES.begin();
    while(it!=IMAGENES.end()){
        delete (*it);
        it++;
    }
    IMAGENES.clear();


    auto it2=TEXT.begin();
    while(it2!=TEXT.end()){
        delete (*it2);
        it2++;
    }
    TEXT.clear();

    auto it3=BOTONES.begin();
    while(it3!=BOTONES.end()){
        delete (*it3);
        it3++;
    }
    BOTONES.clear();

    auto it4=RECTANGULOS.begin();
    while(it4!=RECTANGULOS.end()){
        delete (*it4);
        it4++;
    }
    RECTANGULOS.clear();
}

void Motor2D::CLINNormal(){
    //liberar eso
    //std::vector<Cuadrado2D*> RECTANGULOS;
    //std::vector<Boton2D*> BOTONES;
    //std::vector<Boton2D*> TEXT;
    auto it=IMAGENES.begin();
    while(it!=IMAGENES.end()){
        delete (*it);
        it++;
    }
    IMAGENES.clear();
}

void Motor2D::DisplayHUD(){
    if(true){

        auto it=IMAGENES.begin();
        Imagen2D*img;
        while(it!=IMAGENES.end()){
            (*it)->Draw();
            it++;
        }
        HUD_hability1();

        RECTANGULOS[7]->Draw();
        RECTANGULOS[8]->Draw();
        RECTANGULOS[5]->Draw();
        RECTANGULOS[6]->Draw();

        HUD_hability1( );
        HUD_hability2( );
        HUD_hability3( );
        HUD_vida(      );
        HUD_arma0(     );
        HUD_arma1(     );


    }
}

//////////////////////////////////////////////////////////////////////////////////////
void Motor2D::HUD_hability1(){
    float x,y,w,h;
    auto it =IMAGENES[0];
    x=it->getX();
    y=it->getY();
    w=it->getW();
    h=it->getH();

    RECTANGULOS[0]->setPos(x, y, w, y+(h-y)*perc);
    RECTANGULOS[0]->Draw();
}

void Motor2D::HUD_hability2(){
    float x,y,w,h;
    auto it =IMAGENES[1];
    x=it->getX();
    y=it->getY();
    w=it->getW();
    h=it->getH();

    RECTANGULOS[1]->setPos(x, y, w, y+(h-y)*perc2);
    RECTANGULOS[1]->Draw();
}

void Motor2D::HUD_hability3(){
    float x,y,w,h;
    auto it =IMAGENES[2];
    x=it->getX();
    y=it->getY();
    w=it->getW();
    h=it->getH();
    RECTANGULOS[2]->setPos(x, y, w, y+(h-y)*perc3);
    RECTANGULOS[2]->Draw();
}

void Motor2D::HUD_vida(){
    auto it =IMAGENES[5];
    float x,y,w,h;
    x=it->getX()+0.065;
    y=it->getY();
    w=it->getW();
    h=it->getH()-0.003;


    //Cuadrado2D boton(0 ,0  ,1  ,1);
    RECTANGULOS[4]->Draw();
    RECTANGULOS[3]->setPos(x, y, x+(w-x)*vida , h);
    RECTANGULOS[3]->Draw();
}
//necesitamos escribir por pantalla
void Motor2D::HUD_arma0(){
auto it =IMAGENES[3];
    std::string hola=std::to_string(balaS)+"/"+std::to_string(balaS_TOT);
    TEXT[0]->setText(hola);
    TEXT[0]->Draw();
}

void Motor2D::HUD_arma1(){
    auto it =IMAGENES[4];
    std::string hola=std::to_string(balaP)+"/"+std::to_string(balaP_TOT);
    TEXT[1]->setText(hola);
    TEXT[1]->Draw();
}
