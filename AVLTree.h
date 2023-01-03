#ifndef AVLTREE_h
#define AVLTREE_h

#include <new>
#include <memory>
#include "Exception.h"
#include "Node.h"

/*
* Class Tree
* This class is used to create a basic AVL tree sorted by a single key.
*/
template <class N, class T>
class Tree {
public:

    //The tree's root node
    N* m_node;

    /*
    * Constructor of Tree class
    * @param - none
    * @return - A new instance of Tree
    */    
    Tree();

    /*
    * Copy Constructor and Assignment Operator of Tree class
    * world_cup does not allow two of the same player or team (repeating ID's).
    * Therefore the system does not allow a copy constructor or assignment operator.
    */
    Tree(const Tree& other) = delete;
    Tree& operator=(const Tree& other) = delete;

    /*
    * Destructor of Tree class
    * @param - none
    * @return - void
    */
    virtual ~Tree();

    /*
     * Recursively destroy full tree
     * @param - current node
     * @return - none
     */
    void destroy_tree(N* currentNode);

    /*
     * Recursively release the data of the full tree
     * @param - current node
     * @return - none
     */
    void erase_data(N* currentNode);

    /*
     * Insert new node with data, according to the id given
     * @param - New data to insert and the ID of the new node
     * @return - void
     */
    void insert(T data, const int id);

    /*
     * Remove node according to the id given
     * @param - The ID of the node that needs to be removed
     * @return - none
     */
    void remove(const int id);

    /*
     * Rebalance the tree according to the heights of the relevant nodes
     * @param - The node from which the rebalancing needs to start
     * @return - none
     */
    void rebalance_tree(N* currentNode);

    /*
     * Search for a node with specific data, according to the id given
     * @param - The ID of the requested node
     * @return - a reference to the node containing the data
     */
    virtual N& search_specific_id(const int id) const;

    /*
     * Search for a node recursively throughout the tree
     * @param - The ID of the requested node, the current node of the tree
     * @return - a reference to the node
     */
    virtual N& search_recursively(const int id, N* currentNode) const;

    /*
     * Search for node with specific data, according to the id given
     * @param - The ID of the requested node
     * @return - the data the node holds
     */
    virtual T& search_and_return_data(const int id) const;

    /*
    * Get all players in tree - recursive function
    * @param - array of Players
    * @return - none.
    */
    void get_all_data(T* const array) const;

protected:

    /*
    * Make the node a leaf without breaking the sorted tree
    * @param - the node that needs to be made into a leaf
    * @return - a pointer to the node from which the tree is no longer balanced
    */
    N* make_node_leaf(N* node);

};


//-------------------------------Constructor, Destructors, and Helpers--------------------------------------

template <class N, class T>
Tree<N, T>::Tree()
{
    //Creating empty beginning node
    try {
        m_node = new N();
    }
    catch (const std::bad_alloc& e) {
        delete m_node;
        throw std::bad_alloc();
    }
}


template <class N, class T>
Tree<N, T>::~Tree()
{
    destroy_tree(m_node);
}


template <class N, class T>
void Tree<N, T>::destroy_tree(N* currentNode)
{
    if (currentNode != nullptr) {
        destroy_tree(currentNode->m_left);
        destroy_tree(currentNode->m_right);
        if (currentNode->m_parent != nullptr) {
            if (currentNode->m_parent->m_left == currentNode) {
                currentNode->m_parent->m_left = nullptr;
            }
            else if (currentNode->m_parent->m_right == currentNode) {
                currentNode->m_parent->m_right = nullptr;
            }
            currentNode->m_parent = nullptr;
        }
        delete currentNode;
    }
}


//Free tree's data recursively
template <class N, class T>
void Tree<N, T>::erase_data(N* currentNode)
{
    if (currentNode != nullptr) {
        erase_data(currentNode->m_left);
        erase_data(currentNode->m_right);
        delete currentNode->m_data;
    }
}


//----------------------------------Insert, Remove, and Rebalance---------------------------------

template<class N, class T>
void Tree<N, T>::insert(T data, const int id) {
    //If this is the first node in the tree:
    if (m_node->m_height == -1) {
        m_node->m_data = data;
        m_node->m_id = id;
        m_node->m_height++;
        return;
    }
    //Find the proper location of the new node (when it's not the first):
    N* x = m_node;
    N* y = nullptr;
    while (x != nullptr) {
        y = x;
        if (x->m_id == id) {
            //node with that id already exists - invalid operation
            throw InvalidID(); 
        }
        if (id < x->m_id) {
            x = x->m_left;
        }
        else {
            x = x->m_right;
        }
    }
    //Create the new node and add it to the tree:
    N* node;
    try {
        node = new N();
        node->m_parent = y;
        node->m_left = nullptr;
        node->m_right = nullptr;
        node->m_data = data;
        node->m_id = id;
        node->m_height = 0;
        if (id < y->m_id) {
            y->m_left = node;
        }
        else {
            y->m_right = node;
        }
        rebalance_tree((node->m_parent));
    }
    catch(const std::bad_alloc& e)
    {
        delete node;
        throw e;
    }
}


template <class N, class T>
void Tree<N, T>::remove(const int id)
{
    if (m_node->m_id == id && m_node->m_right == nullptr && m_node->m_left == nullptr && m_node->m_parent == nullptr) {
        m_node->m_data = nullptr;
        m_node->m_height = -1;
        m_node->m_bf = 0;
        m_node->m_id = 0;
        return;
    }
    N* toRemove = &(search_specific_id(id));
    N* nodeToFix = make_node_leaf(toRemove);
    delete toRemove;
    //Go up the tree and check the balance factors and complete needed rotations
    rebalance_tree(nodeToFix);
}


template <class N, class T>
void Tree<N, T>::rebalance_tree(N* currentNode) {
    if (currentNode == nullptr) {
        return;
    }
    currentNode->update_height();
    currentNode->update_bf();
    if (currentNode->m_bf > 1 || currentNode->m_bf < -1) {
        if (currentNode->m_bf == 2) {
            if (currentNode->m_left->m_bf == -1) {
                m_node = currentNode->lr_rotation(m_node);
            }
            else {
                m_node = currentNode->ll_rotation(m_node);
            }
        }
        else if (currentNode->m_bf == -2) {
            if (currentNode->m_right->m_bf == 1) {
                m_node = currentNode->rl_rotation(m_node);
            }
            else {
                m_node = currentNode->rr_rotation(m_node);
            }
        }
        if (currentNode->m_parent != nullptr) {
            if (currentNode->m_parent->m_left != nullptr) {
                currentNode->m_parent->m_left->update_height();
                currentNode->m_parent->m_left->update_bf();
            }
            if (currentNode->m_parent->m_right != nullptr) {
                currentNode->m_parent->m_right->update_height();
                currentNode->m_parent->m_right->update_bf();
            }
        }
        else {
            if (currentNode->m_left != nullptr) {
                currentNode->m_left->update_height();
                currentNode->m_left->update_bf();
            }
            if (currentNode->m_right != nullptr) {
                currentNode->m_right->update_height();
                currentNode->m_right->update_bf();
            }
        }
    }
    rebalance_tree(currentNode->m_parent);
}


//-----------------------------------------Search Functions-----------------------------------------

template <class N, class T>
N& Tree<N, T>::search_specific_id(const int id) const
{
    return search_recursively(id, m_node);
}


template <class N, class T>
N& Tree<N, T>::search_recursively(const int id, N* currentNode) const
{
    if (currentNode == nullptr) {
        throw NodeNotFound();
    }
    if (currentNode->m_id == id) {
        return *currentNode;
    }
    if (currentNode->m_id < id) {
        return search_recursively(id,currentNode->m_right);
    }
    return search_recursively(id, currentNode->m_left);
}


template <class N, class T>
T& Tree<N, T>::search_and_return_data(const int id) const
{
    N* tmpNode;
    try {
        tmpNode = &(search_recursively(id, m_node));
    }
    catch (const NodeNotFound& e) {
        throw e;
    }
    return tmpNode->m_data;
}


//-----------------------------------------Internal Helper Functions-----------------------------------------

template <class N, class T>
N* Tree<N, T>::make_node_leaf(N* node)
{
    //Node to be deleted is already a leaf
    if (node->m_left == nullptr && node->m_right == nullptr) {
        if(node->m_parent != nullptr) {
            if (node->m_parent->m_right == node) {
                node->m_parent->m_right = nullptr;
            }
            else {
                node->m_parent->m_left = nullptr;
            }
        }
        return node->m_parent;
    }
    //Node to be deleted has one child
    if (node->m_left == nullptr || node->m_right == nullptr) {
        N* tmpChild;
        if (node->m_left != nullptr) {
            tmpChild = node->m_left;
        }
        else {
            tmpChild = node->m_right;
        }
        //Connect child to parent
        tmpChild->m_parent = node->m_parent;
        if (node->m_parent != nullptr) {
            if (node->m_parent->m_left == node) {
                node->m_parent->m_left = tmpChild;
            }
            else {
                node->m_parent->m_right = tmpChild;
            }
        }
        else {
            m_node = tmpChild;
            return m_node;
        }
        return node->m_parent;
    }
    //Node to be deleted has two children
    N* successor = node->m_right;
    while (successor->m_left != nullptr) {
        successor = successor->m_left;
    }
    N* parentToReturn;
    if (successor != node->m_right) {
        parentToReturn = successor->m_parent;
    }
    else {
        parentToReturn = successor;
    }
    if (successor->m_right == nullptr) {
        if (successor->m_parent->m_right == successor) {
            successor->m_parent->m_right = nullptr;
        }
        else {
            successor->m_parent->m_left = nullptr;
        }
    }
    else if (node->m_right != successor) {
        successor->m_right->m_parent = successor->m_parent;
        if (successor->m_parent->m_right == successor) {
            successor->m_parent->m_right = successor->m_right;
        }
        else {
            successor->m_parent->m_left = successor->m_right;
        }
    }
    //Switch between successor and current node
    successor->m_parent = node->m_parent;
    if (node->m_parent != nullptr) {
        if (node->m_parent->m_right == node) {
            node->m_parent->m_right = successor;
        }
        else {
            node->m_parent->m_left = successor;
        }
    }
    else {
        m_node = successor;
    }
    successor->m_left = node->m_left;
    if (node->m_left != nullptr) {
        node->m_left->m_parent = successor;
    }
    if (successor != node->m_right) {
        successor->m_right = node->m_right;
        if (node->m_right != nullptr) {
            node->m_right->m_parent = successor;
        }
    }
    return parentToReturn;
}

template <class N, class T>
void Tree<N, T>::get_all_data(T* const array) const
{
    if (this != nullptr) {
        this->m_node->get_data_inorder(array, 0);
    }
}


//----------------------------------------------------------------------------------------------

#endif //AVLTREE_h