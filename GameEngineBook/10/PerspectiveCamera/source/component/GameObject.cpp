#include "GameObject.h"
#include "Component.h"
#include <rttr/registration>

GameObject::GameObject(std::string name) :name_(name) {

}

Component* GameObject::AddComponent(std::string component_type_name) {
	rttr::type t = rttr::type::get_by_name(component_type_name);//��������ע�������и������Ʋ���
	rttr::variant var = t.create();//����Ĭ�Ϲ��캯������ʵ��
	Component* component = var.get_value<Component*>();//��rttr::variant�л�ȡComponent*���͵�ֵ
	component->set_game_object(this);

	if (components_type_instance_map_.find(component_type_name) == components_type_instance_map_.end()) {
		std::vector<Component*> component_vec;
		component_vec.push_back(component);
		components_type_instance_map_[component_type_name] = component_vec;
	}
	else {
		components_type_instance_map_[component_type_name].push_back(component);
	}
	return component;
}

std::vector<Component*>& GameObject::GetComponents(std::string component_type_name) {
	return components_type_instance_map_[component_type_name];
}

Component* GameObject::GetComponent(std::string component_type_name) {
	if (components_type_instance_map_.find(component_type_name) == components_type_instance_map_.end()) {
		return nullptr;
	}
	if (components_type_instance_map_[component_type_name].size() == 0) {
		return nullptr;
	}
	return components_type_instance_map_[component_type_name][0];
}