#include "GameObject.h"
#include "Component.h"
#include <cassert>
#include <rttr/registration>

GameObject::GameObject(std::string name) :name_(name) {
	game_object_list_.push_back(this);
}

Component* GameObject::AddComponent(std::string component_type_name) {
	rttr::type t = rttr::type::get_by_name(component_type_name);//从所有已注册类型中根据名称查找
	assert(t.is_valid());
	rttr::variant var = t.create();//调用默认构造函数创建实例
	Component* component = var.get_value<Component*>();//从rttr::variant中获取Component*类型的值
	component->set_game_object(this);

	if (components_type_instance_map_.find(component_type_name) == components_type_instance_map_.end()) {
		std::vector<Component*> component_vec;
		component_vec.push_back(component);
		components_type_instance_map_[component_type_name] = component_vec;
	}
	else {
		components_type_instance_map_[component_type_name].push_back(component);
	}
	component->Awake();
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

void GameObject::ForeachComponent(std::function<void(Component*)> func){
	for (auto& [name, vec] : components_type_instance_map_) {
		for (auto& component : vec) {
			func(component);
		}
	}
}

void GameObject::Foreach(std::function<void(GameObject*)> func){
	for (auto& go : game_object_list_) {
		func(go);
	}
}