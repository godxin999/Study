#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;
class Component {
public:
	Component() = default;
	virtual ~Component() = default;
public:
	GameObject* game_object() {
		return game_object_;
	}
	void set_game_object(GameObject* game_object) {
		game_object_ = game_object;
	}
	virtual void OnEnable();
	virtual void Awake();
	virtual void Update();
	virtual void OnPreRender();
	virtual void OnPostRender();
	virtual void OnDisable();
private:
	GameObject* game_object_{ nullptr };
};

#endif