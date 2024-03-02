export module component;

import actor;
import stl;

namespace Engine::inline Core{
    export class Component{
    public:
        Component(Actor& p_owner):owner(p_owner){}
        virtual ~Component(){
            if(owner.IsActive()){
                OnDisable();
                OnDestroy();
            }
        }
        virtual void OnAwake(){}
        virtual void OnStart(){}
        virtual void OnUpdate(float deltaTime){}
        virtual void OnDestroy(){}
        virtual void OnEnable(){}
        virtual void OnDisable(){}
        virtual std::string GetName()const = 0;
    public:
        Actor& owner;
    };
}