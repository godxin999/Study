export module transform;

import glm;
import event;

namespace Engine::inline Core{
    export class Transform{
    public:
        Transform()=default;
        Transform(const glm::vec3& localPostion, const glm::quat& localRotation, const glm::vec3& localScale);
        ~Transform();

        void SetParent(Transform& parent);
        bool RemoveParent();
        bool HasParent()const{return m_parent;}
        void GenerateMatricesWorld(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);
        void GenerateMatricesLocal(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);
        void UpdateWorldMatrix();
        void UpdateLocalMatrix();
        void SetLocalPosition(const glm::vec3& position);
        void SetLocalRotation(const glm::quat& rotation);
        void SetLocalScale(const glm::vec3& scale);
        void SetWorldPosition(const glm::vec3& position);
        void SetWorldRotation(const glm::quat& rotation);
        void SetWorldScale(const glm::vec3& scale);
        void TransLateLocal(const glm::vec3& translation);
        void RotateLocal(const glm::quat& rotation);
        void ScaleLocal(const glm::vec3& scale);
        const glm::vec3& GetLocalPosition()const{
            return m_localPosition;
        }
        const glm::quat& GetLocalRotation()const{
            return m_localRotation;
        }
        const glm::vec3& GetLocalScale()const{
            return m_localScale;
        }
        const glm::mat4& GetLocalMatrix()const{
            return m_localMatrix;
        }
        const glm::vec3& GetWorldPosition()const{
            return m_worldPosition;
        }
        const glm::quat& GetWorldRotation()const{
            return m_worldRotation;
        }
        const glm::vec3& GetWorldScale()const{
            return m_worldScale;
        }
        const glm::mat4& GetWorldMatrix()const{
            return m_worldMatrix;
        }
        glm::vec3 GetWorldForward()const;
        glm::vec3 GetWorldRight()const;
        glm::vec3 GetWorldUp()const;
        glm::vec3 GetLocalForward()const;
        glm::vec3 GetLocalRight()const;
        glm::vec3 GetLocalUp()const;
    private:
        void PreDecomposeWorldMatrix();
        void PreDecomposeLocalMatrix();
        
        void OnTransformChanged();
        void OnTransformDestroyed();
    public:
        Event<> TransformChangedEvent{};
        Event<> TransformDestroyedEvent{};
    private:
        ListenerID m_transformChangedListenerID{0};

        glm::vec3 m_localPosition{};
        glm::quat m_localRotation{};
        glm::vec3 m_localScale{};
        glm::mat4 m_localMatrix{};

        glm::vec3 m_worldPosition{};
        glm::quat m_worldRotation{};
        glm::vec3 m_worldScale{};
        glm::mat4 m_worldMatrix{};

        Transform* m_parent{nullptr};
    };

    export constexpr glm::vec3 Forward{0.0f,0.0f,1.0f};
    export constexpr glm::vec3 Right{1.0f,0.0f,0.0f};
    export constexpr glm::vec3 Up{0.0f,1.0f,0.0f};
}