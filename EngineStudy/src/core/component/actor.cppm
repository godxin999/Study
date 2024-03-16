export module actor;

import stl;
import event;


namespace Engine::inline Core{
    export class Actor{
    public:
        bool IsActive()const{
            return m_isActive;
        }
    private:
        bool m_isActive{true};  
    };
}