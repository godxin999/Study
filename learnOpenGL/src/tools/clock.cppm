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
        std::chrono::steady_clock::time_point m_StartTime;
        std::chrono::steady_clock::time_point m_LastTime;
        std::chrono::steady_clock::time_point m_CurrentTime;
        std::chrono::duration<double> m_ElapsedTime;

        bool m_Initialized{false};
        float m_TimeScale{1.0f};
        float m_DeltaTime{0.0f};
        float m_TotalTime{0.0f};
    };
}

module : private;

namespace Engine::inline Tools{
    void Clock::Initialize(){
        m_StartTime = std::chrono::steady_clock::now();
        m_LastTime = m_StartTime;
        m_CurrentTime = m_StartTime;
        m_Initialized = true;        
    }

    void Clock::Update(){
        if(!m_Initialized){
            Initialize();
        }
        else[[likely]]{
            m_LastTime = m_CurrentTime;
            m_CurrentTime = std::chrono::steady_clock::now();
            m_ElapsedTime = m_CurrentTime - m_LastTime;
            m_DeltaTime = m_ElapsedTime.count()>0.1f?0.1f:static_cast<float>(m_ElapsedTime.count());
            m_TotalTime += m_DeltaTime*m_TimeScale;
        }
    }

    void Clock::Scale(float coeff){
        m_TimeScale *= coeff;
    }

    void Clock::SetTimeScale(float scale){
        m_TimeScale = scale;
    }

    float Clock::GetTimeScale() const{
        return m_TimeScale;
    }

    float Clock::GetDeltaTime() const{
        return m_DeltaTime*m_TimeScale;
    }

    float Clock::GetTotalTime() const{
        return m_TotalTime;
    }

    float Clock::GetDeltaTimeUnscaled() const{
        return m_DeltaTime;
    }

    float Clock::GetFrameRate() const{
        return 1.f/m_DeltaTime;
    }
}