export module scene;

namespace Engine::inline Core{
    export class Scene{
    public:
        Scene()=default;
        ~Scene();
        void Play();
        bool IsPlaying()const;
        void Update(float deltaTime);
        void FixedUpdate(float deltaTime);
        void LateUpdate(float deltaTime);
    private:
        bool m_isPlaying{false};
    };
}