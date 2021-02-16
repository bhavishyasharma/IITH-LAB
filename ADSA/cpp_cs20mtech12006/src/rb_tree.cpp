/*
 * Created on Mon Nov 23 2020
 * Author: Bhavishya Sharma
 *
 * Copyright (c) 2020 IIT Hyderabad
 */

#ifndef IITH_LAB_RB_TREE_H
#define IITH_LAB_RB_TREE_H

#include <cstddef>
#include <iostream>

// enum to maintain node colors
enum class NodeColor {Red, Black};

// template for rb tree node structure
template <typename T>
class RBTreeNode {
public:
    
    T data;     // template to hold data
    NodeColor color;        // node color, red or black
    RBTreeNode *left, *right, *parent;      // pointer to children and parent
    // constructor with template data
    RBTreeNode(T data) {
        this->data = data;
        this->color = NodeColor::Red;
        this->left = this->right = this->parent = NULL;
    }
    // check if node is left child of its parent
    bool isLeftChild(){
        if(this->parent == NULL)
            return false;
        if(this->parent->left == this)
            return true;
        return false;
    }
    // check if node is right child of its parent
    bool isRightChild(){
        if(this->parent == NULL)
            return false;
        if(this->parent->right == this)
            return true;
        return false;
    }
    // check if node has any red child
    bool hasRedChild(){
        return (this->left != NULL && this->left->color == NodeColor::Red) || (this->right != NULL && this->right->color == NodeColor::Red);
    }
    // get uncle of the node
    RBTreeNode<T> *getUncle(){
        if(this->parent == NULL)
            return NULL;
        if(this->parent->isLeftChild())
            return this->parent->parent->right;
        if(this->parent->isRightChild())
            return this->parent->parent->left;
        return NULL;
    }
    // get sigbling of the node
    RBTreeNode<T> *getSibling(){
        if(this->isLeftChild())
            return this->parent->right;
        if(this->isRightChild())
            return this->parent->left;
        return NULL;
    }
    // get in-order successor
    RBTreeNode<T> *getSuccessor(){
        // has right child, so successor is in right sub tree
        if(this->right!=NULL){
            RBTreeNode<T> *pointer = this->right;
            while(pointer->left != NULL)
                pointer = pointer->left;
            return pointer;
        }
        // parent and right child is null, so no successor
        else if(this->parent == NULL){
            return NULL;
        }
        else{
            // success if above the node
            RBTreeNode<T> *pointer = this->parent, *node = this;
            while(pointer != NULL && node->isRightChild()){
                node = pointer;
                pointer = pointer->parent;
            }
            return pointer;
        }
    }
    // get in-order predecessor
    RBTreeNode<T> *getPredecessor(){
        // has left child, so predecessor is in left sub tree
        if(this->left!=NULL){
            RBTreeNode<T> *pointer = this->left;
            while(pointer->right != NULL)
                pointer = pointer->right;
            return pointer;
        }
        // no left child and parent, so no predecessor
        else if(this->parent == NULL){
            return NULL;
        }
        else{
            // predecessor is above the node
            RBTreeNode<T> *pointer = this->parent, *node = this;
            while(pointer != NULL && node->isLeftChild()){
                node = pointer;
                pointer = pointer->parent;
            }
            return pointer;
        }
    }
};

template <typename T>
class RBTree {
    // root node of the tree
    RBTreeNode<T> *root;
    long size = 0;

    // pre-order traveral
    void preorderTraverse(RBTreeNode<T> *node){
        if(node == NULL)
            return;
        std::cout<<node->data<<" ";
        this->preorderTraverse(node->left);
        this->preorderTraverse(node->right);
    }

    // in-order traversal
    void inorderTraverse(RBTreeNode<T> *node){
        if(node == NULL)
            return;
        this->inorderTraverse(node->left);
        std::cout<<node->data<<" ";
        this->inorderTraverse(node->right);
    }

    // post-order traversal
    void postorderTraverse(RBTreeNode<T> *node){
        if(node == NULL)
            return;
        this->postorderTraverse(node->left);
        this->postorderTraverse(node->right);
        std::cout<<node->data<<" ";
    }

    long inorderList(RBTreeNode<T> *node, RBTreeNode<T>** array, long index){
        if(node == NULL)
            return index;
        index = this->inorderList(node->left, array, index);
        array[index++] = node;
        index = this->inorderList(node->right, array, index);
        return index;
    }

    // rotate left on given node
    void rotateLeft(RBTreeNode<T> *node){
        RBTreeNode<T> *rightChild = node->right;
        node->right = rightChild->left;
        if(node->right != NULL)
            node->right->parent = node;
        rightChild->parent = node->parent;
        if(node->parent == NULL)
            this->root = rightChild;
        else if(node == node->parent->left)
            node->parent->left = rightChild;
        else
            node->parent->right = rightChild;
        rightChild->left = node;
        node->parent = rightChild;
    }

    // rotate right on given node
    void rotateRight(RBTreeNode<T> *node){
        RBTreeNode<T> *leftChild = node->left;
        node->left = leftChild->right;
        if(node->left != NULL)
            node->left->parent = node;
        leftChild->parent = node->parent;
        if(node->parent == NULL)
            this->root = leftChild;
        else if(node == node->parent->right)
            node->parent->right = leftChild;
        else
            node->parent->left = leftChild;
        leftChild->right = node;
        node->parent = leftChild;
    }

    // fix given node after insert
    void fixNode(RBTreeNode<T> *node){
        RBTreeNode<T> *parent = NULL, *grandParent = NULL;
        while(node!=this->root && node->color != NodeColor::Black && node->parent->color == NodeColor::Red){
            parent = node->parent;
            grandParent = parent->parent;
            if(parent == grandParent->left){
                RBTreeNode<T> *uncle = grandParent->right;
                if(uncle != NULL && uncle->color == NodeColor::Red){
                    grandParent->color = NodeColor::Red;
                    parent->color = NodeColor::Black;
                    uncle->color = NodeColor::Black;
                    node = grandParent;
                }
                else{
                    if( node == parent->right){
                        this->rotateLeft(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    rotateRight(grandParent);
                    NodeColor temp = parent->color;
                    parent->color = grandParent->color;
                    grandParent->color = temp;
                    node = parent;
                }
            }
            else {
                RBTreeNode<T> *uncle = grandParent->left;
                if(uncle != NULL && uncle->color == NodeColor::Red){
                    grandParent->color = NodeColor::Red;
                    parent->color = NodeColor::Black;
                    uncle->color = NodeColor::Black;
                    node = grandParent;
                }
                else{
                    if( node == parent->left){
                        this->rotateRight(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    rotateLeft(grandParent);
                    NodeColor temp = parent->color;
                    parent->color = grandParent->color;
                    grandParent->color = temp;
                    node = parent;
                }
            }
        }
        this->root->color = NodeColor::Black;
    }

    // delete given node
    void deleteNode(RBTreeNode<T> *node){
        RBTreeNode<T> *replacement = NULL, *parent;
        if(node->left != NULL && node->right != NULL) 
            replacement = node->getSuccessor();
        else if(node->left == NULL)
            replacement = node->right;
        else
            replacement = node->left;
        
        parent = node->parent;
        bool bothBlack = node->color == NodeColor::Black && (replacement == NULL || replacement->color == NodeColor::Black);
        if(replacement == NULL){
            if(node == this->root)
                this->root = NULL;
            else {
                if(bothBlack)
                    this->fixDoubleBlack(node);
                else if(node->getSibling() != NULL){
                    node->getSibling()->color = NodeColor::Red;
                }
                if(node->isLeftChild())
                    parent->left = NULL;
                else
                    parent->right = NULL;
            }
            delete node;
            return;
        }
        if(node->left == NULL || node->right == NULL){
            if(node == this->root){
                node->data = replacement->data;
                node->left = node->right = NULL;
                delete replacement;
            }
            else {
                if(node->isLeftChild())
                    parent->left = replacement;
                else
                    parent->right = replacement;
                delete node;
                replacement->parent = parent;
                if(bothBlack)
                    this->fixDoubleBlack(replacement);
                else
                    replacement->color = NodeColor::Black;
            }
            return;
        }
        T temp = replacement->data;
        replacement->data = node->data;
        node->data = temp;
        this->deleteNode(replacement);
    }

    // fix double black node after deletion
    void fixDoubleBlack(RBTreeNode<T> *node){
        if(node == this->root)
            return;
        
        RBTreeNode<T> *sibling = node->getSibling(), *parent = node->parent;
        if(sibling == NULL)
            this->fixDoubleBlack(parent);
        else{
            if(sibling->color == NodeColor::Red){
                parent->color = NodeColor::Red;
                sibling->color = NodeColor::Black;
                if(sibling->isLeftChild())
                    rotateRight(parent);
                else
                    rotateLeft(parent);
                this->fixDoubleBlack(node);
            }
            else{
                if(sibling->hasRedChild()){
                    if(sibling->left != NULL && sibling->left->color == NodeColor::Red){
                        if(sibling->isLeftChild()){
                            sibling->left->color = sibling->color;
                            sibling->color = parent->color;
                            rotateRight(parent);
                        }
                        else{
                            sibling->left->color = sibling->color;
                            rotateRight(sibling);
                            rotateLeft(parent);
                        }
                    }
                    else{
                        if(sibling->isLeftChild()){
                            sibling->right->color = parent->color;
                            rotateLeft(sibling);
                            rotateRight(parent);
                        }
                        else{
                            sibling->right->color = sibling->color;
                            sibling->color = parent->color;
                            rotateLeft(parent);
                        }
                    }
                    parent->color = NodeColor::Black; 
                }
                else{
                    sibling->color = NodeColor::Red;
                    if(parent->color == NodeColor::Black)
                        fixDoubleBlack(parent);
                    else
                        parent->color = NodeColor::Black;
                }
            }
        }
    }
public:
    // initialise empty tree
    RBTree() {
        this->root = NULL;
    }

    // insert template data into tree
    void insert(T data) {
        RBTreeNode<T> *node = new RBTreeNode<T>(data);
        if(this->root == NULL){
            this->root = node;
        }
        else{
            RBTreeNode<T> *pointer = this->root, *parent = NULL;
            while(pointer!=NULL){
                parent = pointer;
                if(node->data < pointer->data)
                    pointer = pointer->left;
                else
                    pointer = pointer->right;
            }
            if(node->data < parent->data)
                parent->left = node;
            else
                parent->right = node;
            node->parent = parent;
        }
        fixNode(node);
        this->size++;
    }

    // insert template data into tree if it does not exist
    void insertUnique(T data) {
        RBTreeNode<T> *node = new RBTreeNode<T>(data);
        if(this->root == NULL){
            this->root = node;
        }
        else{
            RBTreeNode<T> *pointer = this->root, *parent = NULL;
            while(pointer!=NULL){
                parent = pointer;
                if(node->data < pointer->data)
                    pointer = pointer->left;
                if(node->data > pointer->data)
                    pointer = pointer->right;
                else{
                    return;
                }
            }
            if(node->data < parent->data)
                parent->left = node;
            else
                parent->right = node;
            node->parent = parent;
        }
        fixNode(node);
        this->size++;
    }

    // find node by value are turn pointer
    RBTreeNode<T>* findByValue(T value){
        RBTreeNode<T> *pointer = this->root;
        while (pointer != NULL)
        {
            if(value < pointer->data){
                if(pointer->left == NULL)
                    return NULL;
                else
                    pointer = pointer->left;
            }
            else if(value>pointer->data){
                if(pointer->right == NULL)
                    return NULL;
                else
                    pointer = pointer->right;
            }
            else{
                return pointer;
            }
        }
        return pointer;
    }

    // find first value to fall in range
    RBTreeNode<T>* findInRange(T a, T b){
        RBTreeNode<T> *pointer = this->root;
        while (pointer != NULL)
        {
            if(b < pointer->data){
                if(pointer->left == NULL)
                    return NULL;
                else
                    pointer = pointer->left;
            }
            else if(a >pointer->data){
                if(pointer->right == NULL)
                    return NULL;
                else
                    pointer = pointer->right;
            }
            else{
                return pointer;
            }
        }
        return pointer;
    }

    // find and delete a value
    void deleteByValue(T value){
        if(this->root == NULL)
            return;
        RBTreeNode<T> *node = this->findByValue(value);
        if(node == NULL)
            return;
        this->deleteNode(node);
        this->size--;
    }

    // find minimum node of tree
    RBTreeNode<T>* getMinimum(){
        if(this->root == NULL)
            return NULL;
        RBTreeNode<T> *pointer = this->root;
        while(pointer->left != NULL)
            pointer = pointer->left;
        return pointer;
    }

    // find maximum node of tree
    RBTreeNode<T>* getMaximum(){
        if(this->root == NULL)
            return NULL;
        RBTreeNode<T> *pointer = this->root;
        while(pointer->right != NULL)
            pointer = pointer->right;
        return pointer;
    }

    // count total nodes of tree
    long count(){
        return this->size;
    }

    void printPreorder(){
        this->preorderTraverse(this->root);
    }

    void printInorder(){
        this->inorderTraverse(this->root);
    }

    void printPostorder(){
        this->postorderTraverse(this->root);
    }

    RBTreeNode<T>** getSortedList(){
        RBTreeNode<T>** array = new RBTreeNode<T>*[this->size];
        long index = 0;
        this->inorderList(this->root, array, index);
        return array;
    }
};

#endif