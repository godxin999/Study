#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include <list>
#include <data_struct/tree.h>
#include <rttr/registration>

class Component;
class GameObject :public Tree::Node {
public:
	GameObject() = default;
	explicit GameObject(std::string name);
public:
	static void Foreach(std::function<void(GameObject*)> func);//�����������
	static GameObject* Find(std::string name);//ȫ�ֲ���GameObject
public:
	std::string name() {
		return name_;
	}
	void SetName(std::string name) {
		name_ = name;
	}
	unsigned char layer() {
		return layer_;
	}
	void SetLayer(unsigned char layer) {
		layer_ = layer;
	}
	bool active() {
		return active_;
	}
	void SetActive(bool active) {
		active_ = active;
	}
	Component* AddComponent(std::string component_type_name);
	Component* GetComponent(std::string component_type_name);
	std::vector<Component*>& GetComponents(std::string component_type_name);//��ȡ����ͬ�����
	void ForeachComponent(std::function<void(Component*)> func);//�����������
	bool SetParent(GameObject* parent);
private:
	std::string name_;
	std::unordered_map<std::string, std::vector<Component*>> components_type_instance_map_;

	unsigned char layer_{ 0x01 };//������ֲ�ͬ�Ĳ㣬��������ֲ㣬������ײ�ֲ��
	bool active_{ true };//�Ƿ񼤻�
private:
	inline static Tree game_object_tree_;//�����洢���е�GameObject
};

#endif 