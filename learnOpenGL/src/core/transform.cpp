module tranform;

import glm;
import event;

namespace Engine::inline Core{
    Transform::Transform(const glm::vec3& localPostion, const glm::quat& localRotation, const glm::vec3& localScale){
        TransformDestroyedEvent.AddListener(std::bind(&Transform::OnTransformDestroyed,this));
        GenerateMatricesLocal(localPostion,localRotation,localScale);
    }
    Transform::~Transform(){
        TransformDestroyedEvent.Invoke();
    }
    void Transform::SetParent(Transform& parent){
        m_Parent=&parent;
        m_TransformChangedListenerID=m_Parent->TransformChangedEvent.AddListener(std::bind(&Transform::OnTransformChanged,this));
        UpdateWorldMatrix();
    }
    bool Transform::RemoveParent(){
        if(m_Parent){
            m_Parent->TransformChangedEvent.RemoveListener(m_TransformChangedListenerID);
            m_Parent=nullptr;
            UpdateWorldMatrix();
            return true;
        }
        return false;
    }
    void Transform::GenerateMatricesWorld(const glm::vec3& position, const glm::quat& rotation,const glm::vec3& scale){
        m_WorldPosition=position;
        m_WorldRotation=glm::normalize(rotation);
        m_WorldScale=scale;
        m_WorldMatrix=glm::translate(glm::mat4(1.0f),m_WorldPosition)*glm::mat4_cast(m_WorldRotation)*glm::scale(glm::mat4(1.0f),m_WorldScale);
        UpdateLocalMatrix();
    }
    void Transform::GenerateMatricesLocal(const glm::vec3& position, const glm::quat& rotation,const glm::vec3& scale){
        m_LocalPosition=position;
        m_LocalRotation=glm::normalize(rotation);
        m_LocalScale=scale;
        m_LocalMatrix=glm::translate(glm::mat4(1.0f),m_LocalPosition)*glm::mat4_cast(m_LocalRotation)*glm::scale(glm::mat4(1.0f),m_LocalScale);
        UpdateWorldMatrix();
    }
    void Transform::UpdateWorldMatrix(){
        m_WorldMatrix=m_Parent?m_Parent->GetWorldMatrix()*m_LocalMatrix:m_LocalMatrix;
        PreDecomposeWorldMatrix();
        TransformChangedEvent.Invoke();
    }
    void Transform::UpdateLocalMatrix(){
        m_LocalMatrix=m_Parent?glm::inverse(m_Parent->GetWorldMatrix())*m_WorldMatrix:m_WorldMatrix;
        PreDecomposeLocalMatrix();
        TransformChangedEvent.Invoke();
    }
    void Transform::SetLocalPosition(const glm::vec3& position){
        GenerateMatricesLocal(position,m_LocalRotation,m_LocalScale);
    }
    void Transform::SetLocalRotation(const glm::quat& rotation){
        GenerateMatricesLocal(m_LocalPosition,rotation,m_LocalScale);
    }
    void Transform::SetLocalScale(const glm::vec3& scale){
        GenerateMatricesLocal(m_LocalPosition,m_LocalRotation,scale);
    }
    void Transform::SetWorldPosition(const glm::vec3& position){
        GenerateMatricesWorld(position,m_WorldRotation,m_WorldScale);
    }
    void Transform::SetWorldRotation(const glm::quat& rotation){
        GenerateMatricesWorld(m_WorldPosition,rotation,m_WorldScale);
    }
    void Transform::SetWorldScale(const glm::vec3& scale){
        GenerateMatricesWorld(m_WorldPosition,m_WorldRotation,scale);
    }
    void Transform::TransLateLocal(const glm::vec3& translation){
        SetLocalPosition(m_LocalPosition+translation);
    }
    void Transform::RotateLocal(const glm::quat& rotation){
        SetLocalRotation(rotation*m_LocalRotation);
    }
    void Transform::ScaleLocal(const glm::vec3& scale){
        SetLocalScale(glm::vec3(m_LocalScale.x*scale.x,m_LocalScale.y*scale.y,m_LocalScale.z*scale.z));
    }
    glm::vec3 Transform::GetWorldForward()const{
        return m_WorldRotation*Forward;
    }
    glm::vec3 Transform::GetWorldRight()const{
        return m_WorldRotation*Right;
    }
    glm::vec3 Transform::GetWorldUp()const{
        return m_WorldRotation*Up;
    }
    glm::vec3 Transform::GetLocalForward()const{
        return m_LocalRotation*Forward;
    }
    glm::vec3 Transform::GetLocalRight()const{
        return m_LocalRotation*Right;
    }
    glm::vec3 Transform::GetLocalUp()const{
        return m_LocalRotation*Up;
    }
    void Transform::PreDecomposeWorldMatrix(){
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(m_WorldMatrix,m_WorldScale,m_WorldRotation,m_WorldPosition,skew,perspective);
    }
    void Transform::PreDecomposeLocalMatrix(){
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(m_LocalMatrix,m_LocalScale,m_LocalRotation,m_LocalPosition,skew,perspective);
    }

    void Transform::OnTransformChanged(){
        UpdateWorldMatrix();
    }
    void Transform::OnTransformDestroyed(){
        /*GenerateMatricesLocal(m_WorldPosition,m_WorldRotation,m_WorldScale);
        m_Parent=nullptr;
        UpdateWorldMatrix();*/
        m_Parent=nullptr;
        GenerateMatricesLocal(m_WorldPosition,m_WorldRotation,m_WorldScale);
    }
}