#ifndef AIOLOS_TREE_STRUCT_HPP
#define AIOLOS_TREE_STRUCT_HPP

#include <vector>
#include <functional>
#include <utility>
#include <memory>
#include <iostream>

#include "common.hpp"

using std::vector;
using std::pair;
using std::function;
using std::shared_ptr;
using std::cout;
using std::endl;

namespace aiolos
{

/**
 * T is the objective type
 */

/**
 * the struct of TreeNode
 */
template<typename T>
struct TreeNode
{
    //the result value
    union {
        T val; //classify is int, regression is float or double
        //the col axis
        size_t axis; // this may be use union~, later update
    };
    //bool to decide is result node
    bool is_val = false;
    //is is_val is false, then not the end, is use cart tree, then the vector is size two
    vector<pair<function<bool(Type)>, shared_ptr<TreeNode<T>>> > m_tree_modes;
};

/**
 * struct of Cart and Model Tree Node
 */
template<typename T>
struct CartTreeNode
{
    bool is_leaf = false;
    T val; // if it is leaf, then this is the leaf value
    size_t axis; //if it is not leaf, then this is the node axis, and val is the axis val
    vector<T> ws;
    shared_ptr<CartTreeNode<T> > left; // if S[axis] > val
    shared_ptr<CartTreeNode<T> > right; // ifS[axis] <= val
};

/**
 * the struct of Stump, for adaboost as the base classifier
 */
template<typename T>
struct Stump {
    size_t dim = 0; //the axis of features
    T thresh; // the theshold to judge
    short ineq = 0; // 0 for lt, 1 for gt
};


/**
 * the struct of DecisionTree
 */
template<typename T>
class DecisionTree
{

using TreeNodePtr = shared_ptr<TreeNode<T>>;

private:
    //the Tree
    TreeNodePtr m_trees { nullptr };

public:
    /**
     * Create by a T value, as single node
     */
    DecisionTree() = default;

    DecisionTree(T value) {
        //construct a only value tree
        m_trees = TreeNodePtr(new TreeNode<T>());
        m_trees->val = value;
        m_trees->is_val = true;
    }

    /**
     * Create by a axis value, use a bool to huafen
     */
    DecisionTree(size_t axis, bool) {
        m_trees = TreeNodePtr(new TreeNode<T>());
        m_trees->axis = axis;
    }

    /**
     * getter
     */
    const TreeNodePtr get_trees() const {
        return m_trees;
    }

    TreeNodePtr get_trees() {
        return m_trees;
    }

    /**
     * add son Decision Tree
     */
    template<typename Fun>
    void append(Fun&& f, const DecisionTree<T>& son) {
        m_trees->m_tree_modes.emplace_back(std::forward<Fun>(f),
         son.get_trees());
    }

    void print_tree() const {
        cout<<"{ ";
        print(m_trees);
        cout<<" }"<<endl;
    }

private:
    void print(TreeNodePtr ptr) const {
        if(ptr->is_val) {
            cout<<" value is: "<<ptr->val;
        }
        else {
            cout<<" axis: "<<ptr->axis<<" {";
            for(auto& p : ptr->m_tree_modes) {
                cout<<" [ ";
                print(p.second);
                cout<<" ] ";
            }
            cout<<" }";
        }
    }

};

template<typename T>
class CartTree
{

using CartTreeNodePtr = shared_ptr<CartTreeNode<T> >;

private:
    //tree root
    CartTreeNodePtr m_trees {nullptr};

public:
    CartTree() = default;

    CartTree(T value) {
        m_trees = CartTreeNodePtr(new CartTreeNode<T>());
        m_trees->val = value;
        m_trees->is_leaf = true;
    }

    CartTree(vector<T>&& other) {
        m_trees = CartTreeNodePtr(new CartTreeNode<T>());
        m_trees->is_leaf = true;
        m_trees->ws = other;
    }

    CartTree(size_t index, T value) {
        m_trees = CartTreeNodePtr(new CartTreeNode<T>());
        m_trees->val = value;
        m_trees->axis = index;
    }

    /**
     * getter
     */
    inline const CartTreeNodePtr get_trees() const { return m_trees; }

    inline CartTreeNodePtr get_trees() { return m_trees; }

    inline void append_left(const CartTree<T>& son) {
        m_trees->left = son.get_trees();
    }

    inline void append_right(const CartTree<T>& son) {
        m_trees->right = son.get_trees();
    }

    void print_tree() const {
        cout<<"{ ";
        print(m_trees);
        cout<<" }"<<endl;
    }

private:
    void print(CartTreeNodePtr ptr) const {
        if(ptr->is_leaf) {
            cout<<" leaf value is: "<<ptr->val;
        }
        else {
            cout<<" axis: "<<ptr->axis<<" > "<<ptr->val<<" {";
            cout<<" [ ";
            print(ptr->left);
            cout<<" ] ";
            cout<<" } ";
            cout<<" axis: "<<ptr->axis<<" <= "<<ptr->val<<" {";
            cout<<" [ ";
            print(ptr->right);
            cout<<" ] ";
            cout<<" } ";
        }
    }

};

} //aiolos

#endif /*AIOLOS_TREE_STRUCT_HPP*/