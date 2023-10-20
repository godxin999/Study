#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <list>

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
	unsigned char layer() {
		return layer_;
	}
	void set_layer(unsigned char layer) {
		layer_ = layer;
	}

	Component* AddComponent(std::string component_type_name);
	Component* GetComponent(std::string component_type_name);

	std::vector<Component*>& GetComponents(std::string component_type_name);//��ȡ����ͬ�����

	void ForeachComponent(std::function<void(Component* component)> func);

	static void Foreach(std::function<void(GameObject* game_object)> func);
private:
	std::string name_;
	std::unordered_map<std::string, std::vector<Component*>> components_type_instance_map_;

	unsigned char layer_{ 0x01 };//������ֲ�ͬ�Ĳ㣬��������ֲ㣬������ײ�ֲ��

	inline static std::list<GameObject*> game_object_list_;//�洢���е�GameObject
};

#endif 