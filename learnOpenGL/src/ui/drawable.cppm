export module drawable;

namespace Engine::inline UI{
    export class Drawable{
    public:
        virtual void Draw() = 0;
    protected:
        virtual ~Drawable() = default;
    };
}