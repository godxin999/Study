#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;
class Component {
public:
	virtual ~Component() = default;
	GameObject* game_object() {
		return game_object_;
	}
	void set_game_object(GameObject* game_object) {
		game_object_ = game_object;
	}

private:
	GameObject* game_object_;
};

#endif