WARNING: Lost samples detected! See /home/dan-sensei/The-Hive/thehive/oprofile_data/samples/operf.log for details.
CPU: Intel Haswell microarchitecture, speed 3400 MHz (estimated)
Counted CPU_CLK_UNHALTED events (Clock cycles when not halted) with a unit mask of 0x00 (No unit mask) count 3400000
samples  %        symbol name
1949     23.8264  std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_mutate(unsigned long, unsigned long, char const*, unsigned long)
652       7.9707  ggDynWorld::inito(float, float, float)
596       7.2861  Motor2D::InitMenu4()
482       5.8924  TTransform::inverse()
441       5.3912  ObjectManager::swapComponents(gg::EComponentType, unsigned short, IComponent**)
392       4.7922  Motor2D::InitMenu3()
386       4.7188  Motor2D::InitPause()
350       4.2787  ObjectManager::ObjectManager()
258       3.1540  GUIController::StartGame()
209       2.5550  Boton2D::setSesgo(float)
149       1.8215  Motor2D::InitMenu5()
147       1.7971  ObjectManager::addComponentToEntity(IComponent*, gg::EComponentType, unsigned short)
122       1.4914  std::_Rb_tree_iterator<std::pair<unsigned short const, IComponent*> > std::_Rb_tree<unsigned short, std::pair<unsigned short const, IComponent*>, std::_Select1st<std::pair<unsigned short const, IComponent*> >, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, IComponent*> > >::_M_emplace_hint_unique<std::piecewise_construct_t const&, std::tuple<unsigned short const&>, std::tuple<> >(std::_Rb_tree_const_iterator<std::pair<unsigned short const, IComponent*> >, std::piecewise_construct_t const&, std::tuple<unsigned short const&>&&, std::tuple<>&&) [clone .constprop.188]
121       1.4792  jo_write_jpg.part.54
106       1.2958  ObjectManager::returnIDFromRigid(btRigidBody*)
91        1.1125  Motor2D::aplyhover()
86        1.0513  Game::Game()
83        1.0147  std::vector<float, std::allocator<float> >::_M_default_append(unsigned long)
79        0.9658  OptionState::Update()
67        0.8191  CRigidBody::CRigidBody(bool, bool, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, float, float, float, float, float, float, float, float, float, float, float)
62        0.7579  btDefaultSerializer::getUniquePointer(void*)
61        0.7457  ObjectManager::removeEntity(unsigned short)
58        0.7090  Factory::createHero(glm::vec<3, float, (glm::qualifier)0> const&, signed char) [clone .cold.109]
53        0.6479  FMOD_ErrorString(FMOD_RESULT)
44        0.5379  std::deque<TNodo*, std::allocator<TNodo*> >::_M_erase(std::_Deque_iterator<TNodo*, TNodo*&, TNodo**>)
41        0.5012  CNavmeshAgent::SetDestination(glm::vec<3, float, (glm::qualifier)0> const&)
39        0.4768  GUIController::Continue()
36        0.4401  ObjectManager::sendMessageToEntity(unsigned short, Message const&)
34        0.4156  GUIController::ReturnMain()
34        0.4156  ggDynWorld::handleRayCastTo(glm::vec<3, float, (glm::qualifier)0>, glm::vec<3, float, (glm::qualifier)0>, float)
33        0.4034  ggDynWorld::handleRayCast(glm::vec<3, float, (glm::qualifier)0>, glm::vec<3, float, (glm::qualifier)0>, float)
33        0.4034  std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_replace(unsigned long, unsigned long, char const*, unsigned long)
31        0.3790  ParticleSystem::Draw()
27        0.3301  TNodo::ROOT_LightsUpdate()
25        0.3056  GUIController::Init()
23        0.2812  CDynamicModel::CDynamicModel(ZMaterial*)
23        0.2812  Motor2D::InitMenu2()
22        0.2689  std::_Rb_tree_node<std::pair<unsigned short const, IComponent*> >* std::_Rb_tree<unsigned short, std::pair<unsigned short const, IComponent*>, std::_Select1st<std::pair<unsigned short const, IComponent*> >, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, IComponent*> > >::_M_copy<std::_Rb_tree<unsigned short, std::pair<unsigned short const, IComponent*>, std::_Select1st<std::pair<unsigned short const, IComponent*> >, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, IComponent*> > >::_Reuse_or_alloc_node>(std::_Rb_tree_node<std::pair<unsigned short const, IComponent*> > const*, std::_Rb_tree_node_base*, std::_Rb_tree<unsigned short, std::pair<unsigned short const, IComponent*>, std::_Select1st<std::pair<unsigned short const, IComponent*> >, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, IComponent*> > >::_Reuse_or_alloc_node&)
21        0.2567  GUIController::gotoMain()
20        0.2445  AssetManager::loadInit() [clone .cold.237]
19        0.2323  Motor2D::setprogress(int, float)
18        0.2200  Game::Init()
18        0.2200  OptionState::Resume()
18        0.2200  ggDynWorld::clean()
18        0.2200  void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_construct<char*>(char*, char*, std::forward_iterator_tag) [clone .constprop.74]
15        0.1834  CDynamicModel::processMessage(Message const&)
15        0.1834  TNodo::ROOT_OkameraUpdate()
14        0.1711  CPlayerController::ThrowGranade()
14        0.1711  Omicron::getMVP()
14        0.1711  SoundEvent::play()
14        0.1711  Treecontroller::arbolrusher() [clone .cold.131]
14        0.1711  stbi__create_png_image_raw.isra.40
13        0.1589  Motor2D::CLINTexto()
12        0.1467  TCamara::beginDraw()
11        0.1345  BinaryParser::FillBuffers(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int&) [clone .cold.278]
11        0.1345  CCamera::fixCameraPositionOnCollision(glm::vec<3, float, (glm::qualifier)0>&)
11        0.1345  CStaticModel::addLOD(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >)
11        0.1345  ObjectManager::removeComponentFromEntity(gg::EComponentType, unsigned short, bool)
11        0.1345  std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > __gnu_cxx::__to_xstring<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, char>(int (*)(char*, unsigned long, char const*, __va_list_tag*), unsigned long, char const*, ...) [clone .constprop.199]
10        0.1222  Imagen2D::setImage(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)
10        0.1222  Omicron::getVP()
10        0.1222  Treecontroller::arbolsoldado()
9         0.1100  CAgent::Init()
9         0.1100  CFlock::CFlock(bool, unsigned short)
9         0.1100  CPlayerController::RotationBetween(glm::vec<3, float, (glm::qualifier)0>&, glm::vec<3, float, (glm::qualifier)0>&)
9         0.1100  Motor2D::InitHUD()
9         0.1100  TCamara::setPerspectiva(float)
9         0.1100  TNodo::drawRoot_M()
9         0.1100  Treecontroller::arbolsoldado() [clone .cold.132]
9         0.1100  btDefaultMotionState::getWorldTransform(btTransform&) const
9         0.1100  gg::genFloatRandom(float, float)
8         0.0978  BinaryParser::ReadBoundingBox(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, BoundingBox*) [clone .cold.279]
8         0.0978  Boton2D::Boton2D(float, float, float, float, EnumButtonType, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, bool, glm::vec<4, float, (glm::qualifier)0>, float)
8         0.0978  GUIController::lessMusic()
8         0.0978  Hability2::Hability2(int)
8         0.0978  Treecontroller::arbolrusher()
8         0.0978  Treecontroller::arbolswarm() [clone .cold.129]
7         0.0856  CGranade0::CGranade0()
7         0.0856  FMOD_ErrorString(FMOD_RESULT)
7         0.0856  std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string<std::allocator<char> >(char const*, std::allocator<char> const&) [clone .constprop.211]
6         0.0733  CAgent::STAY_func_kTrig_Pickable(TriggerRecordStruct*)
6         0.0733  CFlock::setNewLeader() [clone .part.125]
6         0.0733  CFlock::~CFlock()
6         0.0733  Imagen2D::Imagen2D(float, float, float, float, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)
6         0.0733  TNodo::drawRoot()
6         0.0733  TNodo::~TNodo()
6         0.0733  gg::genIntRandom(unsigned short, unsigned short)
6         0.0733  std::_Deque_base<TNodo*, std::allocator<TNodo*> >::_M_initialize_map(unsigned long)
6         0.0733  std::pair<std::_Rb_tree_iterator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, SoundEvent*> >, bool> std::_Rb_tree<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, SoundEvent*>, std::_Select1st<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, SoundEvent*> >, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, SoundEvent*> > >::_M_emplace_unique<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, SoundEvent*> >(std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, SoundEvent*>&&)
5         0.0611  Action::look_around()
5         0.0611  CMeshCollider::CMeshCollider(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, float, float, float)
5         0.0611  CRigidBody::CRigidBody(float, float, float, float, float, float, float, float, float, float)
5         0.0611  Factory::createCollectableWeapon(glm::vec<3, float, (glm::qualifier)0> const&, int) [clone .cold.120]
5         0.0611  Factory::createRusher(glm::vec<3, float, (glm::qualifier)0> const&, float const&)
5         0.0611  GUIController::moreMusic()
5         0.0611  void std::vector<CTransform*, std::allocator<CTransform*> >::_M_realloc_insert<CTransform*&>(__gnu_cxx::__normal_iterator<CTransform**, std::vector<CTransform*, std::allocator<CTransform*> > >, CTransform*&)
4         0.0489  CDynamicModel::Update()
4         0.0489  CFlock::Muerte()
4         0.0489  CFlock::addFlocked(unsigned short)
4         0.0489  GUIController::update()
4         0.0489  Motor2D::CLINMenu()
4         0.0489  Motor2D::InitMenu7()
4         0.0489  SonidoSalud::~SonidoSalud()
4         0.0489  StateMachine::ProcessStateChanges()
4         0.0489  TNodo::addHijo(TNodo*)
4         0.0489  TNodo::draw()
3         0.0367  AIDirector::AIDirector()
3         0.0367  CBoxCollider::CBoxCollider(float, float, float, float, float, float, float, float, float, float)
3         0.0367  CBoxCollider::~CBoxCollider()
3         0.0367  CDynamicModel::~CDynamicModel()
3         0.0367  CRigidBody::MHandler_SETPTRS()
3         0.0367  Decompress.constprop.69
3         0.0367  Factory::createSingleSwarm(glm::vec<3, float, (glm::qualifier)0> const&, float const&) [clone .cold.115]
3         0.0367  Motor2D::CLINNormal()
3         0.0367  SonidoImpacto::~SonidoImpacto()
3         0.0367  TNodo::setVisibility(bool)
3         0.0367  gg::getWeaponInformation(float&, float&, float&, float&, int&, int&, int, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&)
3         0.0367  void std::deque<State*, std::allocator<State*> >::_M_push_back_aux<State* const&>(State* const&)
2         0.0244  Boton2D::getType()
2         0.0244  CAgent::EXIT_func_kTrig_Senyuelo(TriggerRecordStruct*)
2         0.0244  CFlock::CFlock()
2         0.0244  CFlock::copyFlocked(int)
2         0.0244  Factory::createHolyBomb(glm::vec<3, float, (glm::qualifier)0> const&, glm::vec<3, float, (glm::qualifier)0> const&) [clone .cold.119]
2         0.0244  Imagen2D::getImage[abi:cxx11]()
2         0.0244  Imagen2D::setPos(float, float, float, float)
2         0.0244  Motor2D::HUD_hability1()
2         0.0244  Motor2D::HUD_hability3()
2         0.0244  Motor2D::InitPause() [clone .cold.220]
2         0.0244  Motor2D::addButton(float, float, float, float, EnumButtonType, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, bool, glm::vec<4, float, (glm::qualifier)0>) [clone .constprop.206]
2         0.0244  TNodo::drawRoot() [clone .part.5]
2         0.0244  Treecontroller::arbolswarm()
2         0.0244  Treecontroller::arboltank()
2         0.0244  _GLOBAL__sub_I_main
2         0.0244  void std::vector<Behavior*, std::allocator<Behavior*> >::_M_realloc_insert<Behavior* const&>(__gnu_cxx::__normal_iterator<Behavior**, std::vector<Behavior*, std::allocator<Behavior*> > >, Behavior* const&)
1         0.0122  CAgent::ENTER_func_kTrig_Senyuelo(TriggerRecordStruct*)
1         0.0122  CBoxCollider::~CBoxCollider()
1         0.0122  CClock::CClock()
1         0.0122  CFlock::removerFlocked(int)
1         0.0122  CFlock::~CFlock()
1         0.0122  CGranade1::explosion()
1         0.0122  CHabilityController::MHandler_SETPTRS()
1         0.0122  CNavmeshAgent::FixedUpdate()
1         0.0122  CPlayerController::MHandler_SETPTRS()
1         0.0122  CPlayerController::MostrarTexto()
1         0.0122  Factory::createSoldierWandering(glm::vec<3, float, (glm::qualifier)0> const&, float const&) [clone .cold.111]
1         0.0122  Factory::createTank(glm::vec<3, float, (glm::qualifier)0> const&, float const&) [clone .cold.114]
1         0.0122  GUIController::initOptions()
1         0.0122  GUIController::lessDialog()
1         0.0122  GUIController::moreDialog()
1         0.0122  GUIController::moreEffect()
1         0.0122  GUIController::musicaMenuStop()
1         0.0122  Hability1::Hab1_init()
1         0.0122  Hability1::~Hability1()
1         0.0122  Imagen2D::setSesgado(float)
1         0.0122  MenuState::MenuState() [clone .cold.4]
1         0.0122  MenuState::Update() [clone .cold.3]
1         0.0122  Monitor::addAction(Behavior*)
1         0.0122  Monitor::addCondition(Behavior*)
1         0.0122  Motor2D::addRect(float, float, float, float)
1         0.0122  ObjectManager::FixedUpdateAll()
1         0.0122  ObjectManager::getComponent(gg::EComponentType const&, unsigned short const&)
1         0.0122  Selector::onTerminate(Status)
1         0.0122  Selector::~Selector()
1         0.0122  Sequence::onInitialize()
1         0.0122  Shader::getUniformBlockLocation(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >)
1         0.0122  SonidoEscopeta::SonidoEscopeta()
1         0.0122  TData::TData(TData const&)
1         0.0122  TTransform::transpose()
1         0.0122  Texto2D::Texto2D(float, float, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, glm::vec<4, float, (glm::qualifier)0>, float)
1         0.0122  cvflann::anyimpl::big_any_policy<cvflann::anyimpl::empty_any>::clone(void* const*, void**)
1         0.0122  cvflann::anyimpl::big_any_policy<cvflann::flann_algorithm_t>::clone(void* const*, void**)
1         0.0122  cvflann::anyimpl::big_any_policy<cvflann::flann_centers_init_t>::static_delete(void**)
1         0.0122  ggDynWorld::getIDFromRaycast()
1         0.0122  ggDynWorld::setGravity(float, float, float)
1         0.0122  stbi__bmp_load.isra.31
1         0.0122  stbi__convert_format16
1         0.0122  stbi__idct_simd
1         0.0122  std::_Rb_tree<int, std::pair<int const, void (CPlayerController::*)()>, std::_Select1st<std::pair<int const, void (CPlayerController::*)()> >, std::less<int>, std::allocator<std::pair<int const, void (CPlayerController::*)()> > >::_M_erase(std::_Rb_tree_node<std::pair<int const, void (CPlayerController::*)()> >*)
1         0.0122  std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::compare(char const*) const
1         0.0122  void std::vector<gg::EComponentType, std::allocator<gg::EComponentType> >::_M_realloc_insert<gg::EComponentType>(__gnu_cxx::__normal_iterator<gg::EComponentType*, std::vector<gg::EComponentType, std::allocator<gg::EComponentType> > >, gg::EComponentType&&)
