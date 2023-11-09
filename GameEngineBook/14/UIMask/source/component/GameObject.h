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
	static void Foreach(std::function<void(GameObject*)> func);//遍历所有相机
	static GameObject* Find(std::string name);//全局查找GameObject
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
	std::vector<Component*>& GetComponents(std::string component_type_name);//获取所有同名组件
	void ForeachComponent(std::function<void(Component*)> func);//遍历所有组件
	bool SetParent(GameObject* parent);
private:
	std::string name_;
	std::unordered_map<std::string, std::vector<Component*>> components_type_instance_map_;

	unsigned char layer_{ 0x01 };//将物体分不同的层，用于相机分层，物理碰撞分层等
	bool active_{ true };//是否激活
private:
	inline static Tree game_object_tree_;//用树存储所有的GameObject
};

#endif 