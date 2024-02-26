export module plugin;

namespace Engine::inline UI{
    export class Plugin{
    public:
        virtual void Execute() = 0;
    };
}