#pragma once

#include "iterator.h"



template <
    typename T,
    traversalTag Tag,
    typename Comp = std::less<T>,
    typename Allocator = std::allocator<Node<T>>
>
class SearchTree {
private:
    using node_t = Node<T>;
public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = TreeIterator<const T, Tag, node_t>;
    using const_iterator = TreeIterator<const T, Tag, node_t>;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    using node_type = iterator::node_type;

    using key_type = T;
    using key_compare = Comp;
    using value_compare = Comp;

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using allocator_type = Allocator;
    using allocator_traits_type = std::allocator_traits<allocator_type>;

private:
    using internal_iterator = TreeIterator<T, Tag, node_t>;
    size_type max_size_ = allocator_traits_type::max_size(alloc_);

private:
    node_t* head_;
    key_compare comp_;
    allocator_type alloc_;
    size_type size_;

public:
    SearchTree() 
        : head_(nullptr), comp_(Comp()), alloc_(Allocator()), size_(0) {}

    SearchTree(const SearchTree& other) 
        : head_(nullptr), comp_(other.comp_), alloc_(other.alloc_), size_(other.size_) {
        copy(other.head_, head_);
    }

    SearchTree(SearchTree&& other) noexcept 
        : head_(other.head_), comp_(other.comp_), alloc_(other.alloc_), size_(other.size_) {
        other.head_ = nullptr;
        other.size_ = 0;
    }

    SearchTree& operator =(const SearchTree& other) {
        if (this == &other) {
            return *this;
        }
        delete_tree(&head_);
        copy(other.head_, head_);
        comp_ = other.comp_;
        alloc_ = other.alloc_;
        size_ = other.size_;

        return *this;
    }

    SearchTree& operator =(SearchTree&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        delete_tree(&head_);
        head_ = other.head_;
        comp_ = other.comp_;
        alloc_ = other.alloc_;
        size_ = other.size_;

        other.head_ = nullptr;
        other.size_ = 0;

        return *this;
    }

    ~SearchTree() {
        delete_tree(head_);
    }


    iterator begin() {
        if constexpr (std::is_same_v<Tag, pre_order_tag>) {
            return iterator(head_);
        }
        else {
            auto [node, par] = find_left(head_, nullptr);

            return iterator(node);
        }
    }

    iterator end() {
        if constexpr (std::is_same_v<Tag, post_order_tag>) {
            return iterator(nullptr, nullptr, head_);
        }
        else {
            auto [node, par] = find_right(head_, nullptr);

            return iterator(nullptr, nullptr, node);
        }
    }

    const_iterator cbegin() const {
        if constexpr (std::is_same_v<Tag, pre_order_tag>) {
            return const_iterator(head_);
        }
        else {
            auto [node, par] = find_left(head_, nullptr);

            return const_iterator(node);
        }
    }

    const_iterator cend() const {
        if constexpr (std::is_same_v<Tag, post_order_tag>) {
            return const_iterator(nullptr, nullptr, head_);
        }
        else {
            auto [node, par] = find_right(head_, nullptr);

            return const_iterator(nullptr, nullptr, node);
        }
    }


    bool operator ==(const SearchTree& other) const {
        if (size_ != other.size_) {
            return false;
        }
        const_iterator iter_1 = cbegin();
        const_iterator iter_2 = other.cbegin();
        const_iterator end_iter_1 = cend();
        const_iterator end_iter_2 = other.cend();
        while(iter_2 != end_iter_2) {
            if (*iter_1 != *iter_2) {
                return false;
            }
            ++iter_1;
            ++iter_2;
        }

        return iter_1 == end_iter_1 && iter_2 == end_iter_2;
    }

    bool operator !=(const SearchTree& other) const {

        return ! operator==(other);
    }


    void swap(SearchTree& other) {
        std::swap(head_, other.head_);
        std::swap(comp_, other.comp_);
        std::swap(alloc_, other.alloc_);
        std::swap(size_, other.size_);
    }

    size_type size() const {

        return size_;
    }
    size_type max_size() const {

        return max_size_;
    }

    bool empty() const {

        return size_ == 0;
    }

public:
    key_compare key_comp() {

        return key_compare();
    }

    value_compare value_comp() {

        return value_compare();
    }


    std::pair<iterator, bool> insert(const value_type& value) {
        auto [result, par] = smart_find(head_, nullptr, value);
        std::pair<iterator, bool> out;
        if (!result) {
            result = create_node(value, par);
            out = {iterator(result), true};
            ++size_;
        }
        else {
            out = {iterator(result), false};
        }

        return out;
    }

    std::pair<iterator, bool> insert(value_type&& value) {
        value_type lvalue_value = value;
        return insert(lvalue_value);
    }

    iterator insert(const_iterator hint, const value_type& value) = delete;
    iterator insert(const_iterator hint, value_type&& value) = delete;
    template <
        typename input_iter_t
    >
    void insert(input_iter_t lhs, input_iter_t rhs) {
        while (lhs != rhs) {
            insert(*lhs);
            ++lhs;
        }
    }

    size_type erase(const value_type& value) {
        auto [node, par] = smart_find(head_, nullptr, value);
        if (!node) {
            return 0;
        }
        node_t* out = extract_node(node, par);
        destroy_node(out);

        return 1;
    }

    iterator erase(iterator pos) {
        node_t* out = extract_node(pos.node_, pos.node_->par);
        destroy_node(out);

        return end();
    }

    void erase(iterator lhs, iterator rhs) {
        while (lhs != rhs) {
            iterator next = lhs;
            ++next;
            errase(lhs);
            lhs = next;
        }
    }

    node_t* extract(const value_type& value) {
        auto [node, par] = smart_find(head_, nullptr, value);
        return extract_node(node, par);
    }
    
    node_t* extract(iterator pos) {
        return extract_node(pos.node_, pos.node_->par);
    }

    void clear() {
        size_ = 0;
        delete_tree(head_);
        head_ = nullptr;
    }


    iterator find(const value_type& value) {
        auto [node, par] = smart_find(head_, nullptr, value);
        return iterator(node);
    }

    const_iterator find(const value_type& value) const {
        auto [node, par] = smart_find(head_, nullptr, value);
        return const_iterator(node);
    }


    iterator lower_bound(const value_type& value) {
        return iterator(lower_bound(head_, value));
    }

    const_iterator lower_bound(const value_type& value) const {
        return const_iterator(lower_bound(head_, value));
    }

    iterator upper_bound(const value_type& value) {
        return iterator(upper_bound(head_, value));
    }

    const_iterator upper_bound(const value_type& value) const {
        return const_iterator(upper_bound(head_, value));
    }

    std::pair<iterator, iterator> equal_range(const value_type& value) {
        return { lower_bound(value), upper_bound(value) };
    }

    std::pair<const_iterator, const_iterator> equal_range(const value_type& value) const {
        return { lower_bound(value), upper_bound(value) };
    }

public:
    reverse_iterator rbegin() {
        return std::reverse_iterator(end());
    }

    const_reverse_iterator crbegin() const {
        return std::reverse_iterator(end());
    }

    reverse_iterator rend() {
        return std::reverse_iterator(begin());
    }

    const_reverse_iterator crend() const {
        return std::reverse_iterator(begin());
    }

public:
    allocator_type get_allocator() const {
        return alloc_;
    }

private:
    std::pair<node_t*, node_t*> find_left(node_t* node, node_t* par) const {
        if (!node) {
            return { node, par };
        }

        while (node->lhs) {
            par = node;
            node = node->lhs;
        }

        return {node, par};
    }


    std::pair<node_t*, node_t*> find_right(node_t* node, node_t* par) const {
        if (!node) {
            return { node, par };
        }

        while (node->rhs) {
            par = node;
            node = node->rhs;
        }

        return {node, par};
    }


    node_t* create_node(const value_type& value, node_t* par) {
        node_t* node = allocator_traits_type::allocate(alloc_, 1);
        allocator_traits_type::construct(alloc_, node, value);
        node->par = par;
        return node;
    }


    void destroy_node(node_t* node) {
        allocator_traits_type::destroy(alloc_, node);
        allocator_traits_type::deallocate(alloc_, node, 1);
    }


    void copy(node_t* in, node_t*& out, node_t* out_par) {
        if (!in) {
            return;
        }
        out = create_node(in->value, out_par);
        copy(in->lhs, out->lhs, out);
        copy(in->rhs, out->rhs, out);
    }


    void delete_tree(node_t* node) {
        if (!node) {
            return;
        }
        if (node->lhs) {  
            delete_tree(node->lhs);
        }
        if (node->rhs) {  
            delete_tree(node->rhs);
        }
        destroy_node(node);
    }


    std::pair<node_t*&, node_t*> smart_find(node_t*& node, node_t* par, const value_type& value) const {
        if (!node) {
            return {node, par};
        }
        if (node->value == value) {
            return {node, par};
        }

        if (comp_(value, node->value)) {
            return smart_find(node->lhs, node, value);
        }
        else {
            return smart_find(node->rhs, node, value);
        }
    }


    node_t* extract_node(node_t* node, node_t* par) {
        if (!node) {
            return nullptr;
        }
        --size_;

        node_t* out = node;

        if (!node->lhs) {
            if (!node->rhs) {
                node = nullptr; 
                if (!par) {
                    head_ = nullptr; 
                }
            }
            else {
                node = node->rhs;
                node->par = par;   
                if (!par) {
                    head_ = node; 
                }
            }
        }
        else if (!node->rhs) {
            node = node->lhs;  
            node->par = par;  
            if (!par) {
                head_ = node;  
            }
        }
        else {
            auto [prev, prev_par] = find_right(node->lhs, node);

            if (prev_par->lhs == prev) {
                prev_par->lhs = prev->lhs; 
            }
            else {
                prev_par->rhs = prev->lhs; 
            }

            prev->lhs = node->lhs;
            prev->rhs = node->rhs;

            if (prev->rhs) {
                prev->rhs->par = prev; 
            }
            if (prev->lhs) {
                prev->lhs->par = prev;  
            }

            node = prev;
            prev->par = par;  
            if (!par) {
                head_ = node;  
            }
        }
        out->par = out->lhs = out->rhs = nullptr;

        return out; 
    }


    node_t* lower_bound(node_t* node, const value_type& value) const {
        node_t* res = nullptr;

        while (node) {
            if (comp_(node->value, value)) {
                node = node->rhs;
            }
            else {
                res = node;
                node = node->lhs;
            }
        }

        return res;
    }


    node_t* upper_bound(node_t* node, const value_type& value) const {
        node_t* res = nullptr;

        while (node) {
            if (comp_(value, node->value) && value != node->value) {
                res = node;
                node = node->lhs;
            }
            else {
                node = node->rhs;
            }
        }

        return res; 
    }

};


