#include "tree.h"

Tree::Tree() {
	root_node_ = new Node();
}

void Tree::Post(Node* node, std::function<void(Node*)> func) {
	for (auto& child : node->children()) {
		Post(child, func);
	}
	if (node != root_node_) {
		func(node);
	}
}

void Tree::Find(Node* node_parent, std::function<bool(Node*)> func_check, Node** node_result = nullptr) {
	for (auto& child : node_parent->children()) {
		Find(child, func_check, node_result);
		if (*node_result)break;
	}
	if (*node_result)return;
	if (func_check(node_parent)) {
		(*node_result) = node_parent;
	}
}
