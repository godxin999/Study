export module service_locator;

import stl;

namespace Engine::inline Tools{
    export class ServiceLocator{
    public:
        template<typename T>
        static void Register(T& service){
            Services[std::type_index(typeid(T))] = std::any(&service);
        }

        template<typename T>
        static T& Get(){
            return *std::any_cast<T*>(Services[std::type_index(typeid(T))]);
        }

    private:
        inline static std::unordered_map<std::type_index, std::any> Services{};
    };
}