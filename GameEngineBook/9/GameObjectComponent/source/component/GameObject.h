#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

#include <rttr/registration>

class Component;
class GameObject {
public:
	GameObject() = default;
	explicit GameObject(std::string name);
	std::string& name() {
		return name_;
	}
	void set_name(std::string name) {
		name_ = name;
	}

	Component* AddComponent(std::string component_type_name);

	template<typename T>
	T* AddComponent() {
		T* component = new T();
		rttr::type t = rttr::type::get(*component);//获取类型T的类型信息
		std::string component_type_name = t.get_name().to_string();
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

	Component* GetComponent(std::string component_type_name);

	std::vector<Component*>& GetComponents(std::string component_type_name);


private:
	std::string name_;
	std::unordered_map<std::string, std::vector<Component*>> components_type_instance_map_;
};








#endif 