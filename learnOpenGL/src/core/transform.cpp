module transform;

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
        m_parent=&parent;
        m_transformChangedListenerID=m_parent->TransformChangedEvent.AddListener(std::bind(&Transform::OnTransformChanged,this));
        UpdateWorldMatrix();
    }
    
    bool Transform::RemoveParent(){
        if(m_parent){
            m_parent->TransformChangedEvent.RemoveListener(m_transformChangedListenerID);
            m_parent=nullptr;
            UpdateWorldMatrix();
            return true;
        }
        return false;
    }
    
    void Transform::GenerateMatricesWorld(const glm::vec3& position, const glm::quat& rotation,const glm::vec3& scale){
        m_worldPosition=position;
        m_worldRotation=glm::normalize(rotation);
        m_worldScale=scale;
        m_worldMatrix=glm::translate(glm::mat4(1.0f),m_worldPosition)*glm::mat4_cast(m_worldRotation)*glm::scale(glm::mat4(1.0f),m_worldScale);
        UpdateLocalMatrix();
    }
    
    void Transform::GenerateMatricesLocal(const glm::vec3& position, const glm::quat& rotation,const glm::vec3& scale){
        m_localPosition=position;
        m_localRotation=glm::normalize(rotation);
        m_localScale=scale;
        m_localMatrix=glm::translate(glm::mat4(1.0f),m_localPosition)*glm::mat4_cast(m_localRotation)*glm::scale(glm::mat4(1.0f),m_localScale);
        UpdateWorldMatrix();
    }
    
    void Transform::UpdateWorldMatrix(){
        m_worldMatrix=m_parent?m_parent->GetWorldMatrix()*m_localMatrix:m_localMatrix;
        PreDecomposeWorldMatrix();
        TransformChangedEvent.Invoke();
    }
    
    void Transform::UpdateLocalMatrix(){
        m_localMatrix=m_parent?glm::inverse(m_parent->GetWorldMatrix())*m_worldMatrix:m_worldMatrix;
        PreDecomposeLocalMatrix();
        TransformChangedEvent.Invoke();
    }
    
    void Transform::SetLocalPosition(const glm::vec3& position){
        GenerateMatricesLocal(position,m_localRotation,m_localScale);
    }
    
    void Transform::SetLocalRotation(const glm::quat& rotation){
        GenerateMatricesLocal(m_localPosition,rotation,m_localScale);
    }
    
    void Transform::SetLocalScale(const glm::vec3& scale){
        GenerateMatricesLocal(m_localPosition,m_localRotation,scale);
    }
    
    void Transform::SetWorldPosition(const glm::vec3& position){
        GenerateMatricesWorld(position,m_worldRotation,m_worldScale);
    }
    
    void Transform::SetWorldRotation(const glm::quat& rotation){
        GenerateMatricesWorld(m_worldPosition,rotation,m_worldScale);
    }
    
    void Transform::SetWorldScale(const glm::vec3& scale){
        GenerateMatricesWorld(m_worldPosition,m_worldRotation,scale);
    }
    
    void Transform::TransLateLocal(const glm::vec3& translation){
        SetLocalPosition(m_localPosition+translation);
    }
    
    void Transform::RotateLocal(const glm::quat& rotation){
        SetLocalRotation(m_localRotation*rotation);
    }
    
    void Transform::ScaleLocal(const glm::vec3& scale){
        SetLocalScale(glm::vec3(m_localScale.x*scale.x,m_localScale.y*scale.y,m_localScale.z*scale.z));
    }
    
    glm::vec3 Transform::GetWorldForward()const{
        return m_worldRotation*Forward;
    }
    
    glm::vec3 Transform::GetWorldRight()const{
        return m_worldRotation*Right;
    }
    
    glm::vec3 Transform::GetWorldUp()const{
        return m_worldRotation*Up;
    }
    
    glm::vec3 Transform::GetLocalForward()const{
        return m_localRotation*Forward;
    }
    
    glm::vec3 Transform::GetLocalRight()const{
        return m_localRotation*Right;
    }
    
    glm::vec3 Transform::GetLocalUp()const{
        return m_localRotation*Up;
    }
    
    void Transform::PreDecomposeWorldMatrix(){
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(m_worldMatrix,m_worldScale,m_worldRotation,m_worldPosition,skew,perspective);
    }
    
    void Transform::PreDecomposeLocalMatrix(){
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(m_localMatrix,m_localScale,m_localRotation,m_localPosition,skew,perspective);
    }

    void Transform::OnTransformChanged(){
        UpdateWorldMatrix();
    }
    
    void Transform::OnTransformDestroyed(){
        /*GenerateMatricesLocal(m_worldPosition,m_worldRotation,m_worldScale);
        m_parent=nullptr;
        UpdateWorldMatrix();*/
        m_parent=nullptr;
        GenerateMatricesLocal(m_worldPosition,m_worldRotation,m_worldScale);
    }
}