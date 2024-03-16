export module clock;

import stl;

namespace Engine::inline Tools{
    export class Clock{
    public:
        void Update();
        void Scale(float coeff);
        void SetTimeScale(float scale);
        float GetTimeScale() const;
        float GetDeltaTime() const;
        float GetTotalTime() const;
        float GetDeltaTimeUnscaled() const;
        float GetFrameRate() const;

    private:
        void Initialize();
    private:
        std::chrono::steady_clock::time_point m_startTime;
        std::chrono::steady_clock::time_point m_lastTime;
        std::chrono::steady_clock::time_point m_currentTime;
        std::chrono::duration<double> m_elapsedTime;

        bool m_initialized{false};
        float m_timeScale{1.0f};
        float m_deltaTime{0.0f};
        float m_totalTime{0.0f};
    };
}

module : private;

namespace Engine::inline Tools{
    void Clock::Initialize(){
        m_startTime = std::chrono::steady_clock::now();
        m_lastTime = m_startTime;
        m_currentTime = m_startTime;
        m_initialized = true;        
    }

    void Clock::Update(){
        if(!m_initialized){
            Initialize();
        }
        else[[likely]]{
            m_lastTime = m_currentTime;
            m_currentTime = std::chrono::steady_clock::now();
            m_elapsedTime = m_currentTime - m_lastTime;
            m_deltaTime = m_elapsedTime.count()>0.1f?0.1f:static_cast<float>(m_elapsedTime.count());
            m_totalTime += m_deltaTime*m_timeScale;
        }
    }

    void Clock::Scale(float coeff){
        m_timeScale *= coeff;
    }

    void Clock::SetTimeScale(float scale){
        m_timeScale = scale;
    }

    float Clock::GetTimeScale() const{
        return m_timeScale;
    }

    float Clock::GetDeltaTime() const{
        return m_deltaTime*m_timeScale;
    }

    float Clock::GetTotalTime() const{
        return m_totalTime;
    }

    float Clock::GetDeltaTimeUnscaled() const{
        return m_deltaTime;
    }

    float Clock::GetFrameRate() const{
        return 1.f/m_deltaTime;
    }
}