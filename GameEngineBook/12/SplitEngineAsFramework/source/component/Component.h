#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;
class Component {
public:
	Component() = default;
	virtual ~Component() = default;
	GameObject* game_object() {
		return game_object_;
	}
	void set_game_object(GameObject* game_object) {
		game_object_ = game_object;
	}
	virtual void Awake();
	virtual void Update();
private:
	GameObject* game_object_{ nullptr };
};

#endif