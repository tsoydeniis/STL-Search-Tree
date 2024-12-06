#pragma once

#include <iostream>
#include <cstdint>
#include <memory>
#include <type_traits>


struct in_order_tag {};
struct pre_order_tag {};
struct post_order_tag {};


template <typename T>
struct Node {
    T value;
    Node* par;
    Node* lhs;
    Node* rhs;

    Node(const T& value)
        : value(value), par(nullptr), lhs(nullptr), rhs(nullptr) {
    }
};


template <typename T>
concept traversalTag = requires() {
    std::is_same_v<T, in_order_tag> || std::is_same_v<T, pre_order_tag> || std::is_same_v<T, post_order_tag>;
};


template<
    typename T, 
    traversalTag Tag,
    typename node_t = Node<T>
> 
class TreeIterator {
    template <typename, traversalTag, typename, typename>
    friend class SearchTree;
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    using node_type = node_t;

public:
    TreeIterator() : node_(nullptr) {}
    TreeIterator(node_t* node) : node_(node) {} 

    reference operator *() {
        return node_->value;
    }
    pointer operator ->() {
        return &(node_->value);
    }

    bool operator ==(const TreeIterator& arg) const {
        return node_ == arg.node_;
    }
    bool operator !=(const TreeIterator& arg) const {
        return node_ != arg.node_;
    }

public:
    TreeIterator& operator ++() {
        if (!node_) {
            node_ = next_if_nullptr_;
        }
        else {
            prev_if_nullptr_ = node_;
            increase();
        }
        
        return *this;
    }
    TreeIterator operator ++(int) {
        TreeIterator temp = *this;
        increase();

        return temp;
    }

    TreeIterator& operator --() {
        if (!node_) {
            node_ = prev_if_nullptr_;
        }
        else {
            next_if_nullptr_ = node_;
            decrease();
        }

        return *this;
    }
    TreeIterator operator --(int) {
        TreeIterator temp = *this;
        decrease();

        return temp;
    }

private:
    TreeIterator(node_t* node, node_t* begin, node_t* end) 
        : node_(node), next_if_nullptr_(begin), prev_if_nullptr_(end) {
    }

    void increase() {
        if constexpr (std::is_same_v<Tag, in_order_tag>) {
            if (node_->rhs != nullptr) {
                node_ = node_->rhs;
                while (node_->lhs != nullptr) {
                    node_ = node_->lhs;
                }
            }
            else {
                node_t* parent = node_->par;
                while (parent != nullptr && node_ == parent->rhs) {
                    node_ = parent;
                    parent = parent->par;
                }
                node_ = parent;
            }
        }
        else if constexpr (std::is_same_v<Tag, pre_order_tag>) {
            if (node_->lhs != nullptr) {
                node_ = node_->lhs;
            }
            else if (node_->rhs != nullptr) {
                node_ = node_->rhs;
            }
            else {
                node_t* parent = node_->par;
                while (parent != nullptr && (node_ == parent->rhs || parent->rhs == nullptr)) {
                    node_ = parent;
                    parent = parent->par;
                }
                node_ = parent != nullptr ? parent->rhs : nullptr;
            }
        }
        else if constexpr (std::is_same_v<Tag, post_order_tag>) {
            node_t* parent = node_->par;
            if (parent != nullptr && node_ == parent->lhs && parent->rhs != nullptr) {
                node_ = parent->rhs;
                while (node_->lhs != nullptr || node_->rhs != nullptr) {
                    if (node_->lhs != nullptr) {
                        node_ = node_->lhs;
                    }
                    else {
                        node_ = node_->rhs;
                    }
                }
            }
            else {
                node_ = parent;
            }
        }
    }

    void decrease() {
        if constexpr (std::is_same_v<Tag, in_order_tag>) {
            if (node_->lhs != nullptr) {
                node_ = node_->lhs;
                while (node_->rhs != nullptr) {
                    node_ = node_->rhs;
                }
            }
            else {
                node_t* parent = node_->par;
                while (parent != nullptr && node_ == parent->lhs) {
                    node_ = parent;
                    parent = parent->par;
                }
                node_ = parent;
            }
        }
        else if constexpr (std::is_same_v<Tag, pre_order_tag>) {
            node_t* parent = node_->par;
            if (parent != nullptr && node_ == parent->rhs && parent->lhs != nullptr) {
                node_ = parent->lhs;
                while (node_->rhs != nullptr) {
                    node_ = node_->rhs;
                }
            }
            else {
                node_ = parent;
            }
        }
        else if constexpr (std::is_same_v<Tag, post_order_tag>) {
            if (node_->rhs != nullptr) {
                node_ = node_->rhs;
                while (node_->lhs != nullptr) {
                    node_ = node_->lhs;
                }
            }
            else {
                node_t* parent = node_->par;
                while (parent != nullptr && node_ == parent->lhs) {
                    node_ = parent;
                    parent = parent->par;
                }
                node_ = parent;
            }
        }
    }

private:
    node_t* node_;
    node_t* prev_if_nullptr_ = nullptr;
    node_t* next_if_nullptr_ = nullptr;
};




