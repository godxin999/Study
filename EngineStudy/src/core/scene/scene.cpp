module scene;

namespace Engine::inline Core{
    Scene::~Scene(){

    }
    void Scene::Play(){
        m_isPlaying = true;
    }
    bool Scene::IsPlaying()const{
        return m_isPlaying;
    }
    void Scene::Update(float deltaTime){

    }
    void Scene::FixedUpdate(float deltaTime){

    }
    void Scene::LateUpdate(float deltaTime){
        
    }
}