#pragma once
#ifndef TREE_H
#define TREE_H

#include <list>
#include <functional>

class Tree {
public:
	class Node {
	public:
		Node() = default;
		virtual ~Node() = default;

		Node* parent() {
			return parent_;
		}
		void SetParent(Node* parent) {
			parent_ = parent;
		}
		std::list<Node*>& children() {
			return children_;
		}
		void AddChild(Node* child) {
			if (child->parent()) {
				child->parent()->RemoveChild(child);
			}
			children_.push_back(child);
			child->SetParent(this);
		}
		void RemoveChild(Node* child) {
			children_.remove(child);
		}
		bool Empty() {
			return children_.size() > 0;
		}
	private:
		Node* parent_{ nullptr };
		std::list<Node*> children_;
	};
public:
	Tree();
	~Tree() = default;
	Node* root_node() {
		return root_node_;
	}
	void Post(Node* node, std::function<void(Node*)> func);
	void Find(Node* node_parent, std::function<bool(Node*)> func_check, Node** node_result);
private:
	Node* root_node_{ nullptr };
};



#endif // !TREE_H
