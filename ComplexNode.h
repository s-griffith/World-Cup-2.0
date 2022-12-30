#ifndef COMPLEXNODE_H
#define COMPLEXNODE_H

#include "Node.h"
#include "Player.h"

/*
* Class Complex Node : Node
* This class is used to create separate nodes in the tree, sorted based on three keys:
*       the number of goals the player scored, the number of cards they received and their player ID
*/
template <class T>
class ComplexNode : Node<T> {
public:

    /*
    * Constructor of ComplexNode class
    * @param - none
    * @return - A new instance of ComplexNode
    */ 
    ComplexNode();

    /*
    * Constructor of ComplexNode class
    * Used in unite_teams to make a new node containing the desired player
    * @param - data the node holds
    * @return - A new instance of ComplexNode
    */     
    ComplexNode(T data);

    /*
    * Copy Constructor and Assignment Operator of ComplexNode class
    * world_cup does not allow two of the same player or team (repeating ID's).
    * Therefore the system does not allow a copy constructor or assignment operator.
    */
    ComplexNode(const ComplexNode&) = delete;
    ComplexNode& operator=(const ComplexNode& other) = delete;

    /*
    * Destructor of ComplexNode class
    * @param - none
    * @return - void
    */
    virtual ~ComplexNode() = default;

    /*
    * Helper function for unite_teams in world_cup:
    * Inserts players into a given array, node by node
    * @param - an array, current index
    * @return - current index
    */
    int unite_insert(Player** players, int index);

private:

    /*
     * Left-Right Rotation
     * @param - Node with balance factor of +2
     * @return - pointer to ComplexNode
     */
    ComplexNode* ll_rotation(ComplexNode* node);

    /*
     * Right-Right Rotation
     * @param - Node with balance factor of -2
     * @return - pointer to ComplexNode
     */
    ComplexNode* rr_rotation(ComplexNode* node);

    /*
     * Right-Left Rotation
     * @param - Node with balance factor of -2
     * @return - pointer to ComplexNode
    */
    ComplexNode* rl_rotation(ComplexNode* node);

    /*
     * Left-Left Rotation
     * @param - Node with balance factor of +2
     * @return - pointer to ComplexNode
    */
    ComplexNode* lr_rotation(ComplexNode* node);

    /*
     * Update balance factor of the current node
     * @param - none
     * @return - void
    */
    void update_bf();
    
    /*
     * Update height of the current node
     * @param - none
     * @return - void
    */
    void update_height();

    /*
     * Helper function for get_all_players in world_cup:
     * Recursively inserts the player ID's of the data of the tree into a given array
     * @param - an array, current index
     * @return - void
     */
    int get_data_inorder(int* const array, int index) const;

    /*
     * The internal fields of ComplexNode:
     *   Pointers to the parent node and two child nodes
     *   The goals the player represented by ComplexNode scored
     *   The cards the player represented by ComplexNode received
     */
    ComplexNode<T>* m_parent;
    ComplexNode<T>* m_left;
    ComplexNode<T>* m_right;
    int m_goals;
    int m_cards;

    /*
     * The following classes are friend classes in order to allow full access to private fields and functions of
     * ComplexNode.
     * This allows ComplexNode to be a mostly private class, while allowing the system to run smoothly.
    */
    template <class M>
    friend class MultiTree;

    template <class K>
    friend class TreeExtraPointer;

    template <class ComplexNode, class N>
    friend class Tree;
    
};


//---------------------------------------Constructor and Public Helper Function--------------------------

template <class T>
ComplexNode<T>::ComplexNode() :
        Node<T>(),
        m_parent(nullptr),
        m_left(nullptr),
        m_right(nullptr),
        m_goals(0),
        m_cards(0)
{}


template <class T>
ComplexNode<T>::ComplexNode(T data) :
        Node<T>(data),
        m_parent(nullptr),
        m_left(nullptr),
        m_right(nullptr),
        m_goals(data->get_goals()),
        m_cards(data->get_cards())
{}


template<class T>
int ComplexNode<T>::unite_insert(Player** players, int index) {
    if (this != nullptr && this->m_data != nullptr) {
        index = m_left->unite_insert(players, index);
        *(players+(index++)) = this->m_data;
        index = m_right->unite_insert(players, index);
    }
    return index;
}

//-----------------------------------------Rotations--------------------------------------------

//Left-Left tree rotation, on the node with balance factor of +2
template <class T>
typename ComplexNode<T>::ComplexNode* ComplexNode<T>::ll_rotation(ComplexNode<T>* node)
{
    ComplexNode<T>* tmpToReturn = node;
    //Changing A->B to A->Parent
    m_left->m_parent = m_parent;
    //Changing Parent->B to Parent->A
    if (m_parent != nullptr) {
        if (m_parent->m_left == this) {
            m_parent->m_left = m_left;
        }
        else {
            m_parent->m_right = m_left;
        }
    }
    else {
        tmpToReturn = m_left;
    }
    //Changing B->Parent to B->A
    m_parent = m_left;
    //Changing Ar->A to Ar->B
    if (m_left->m_right != nullptr) {
        m_left->m_right->m_parent = this;
    }
    //Changing B->A to B->Ar
    m_left = m_left->m_right;
    //Changing A->Ar to A->B
    m_parent->m_right = this;
    return tmpToReturn;
}


//Right-Right tree rotation, on the node with balance factor of -2
template <class T>
typename ComplexNode<T>::ComplexNode* ComplexNode<T>::rr_rotation(ComplexNode<T>* node)
{
    ComplexNode<T>* tmpToReturn = node;
    m_right->m_parent = m_parent;
    if (m_parent != nullptr) {
        if (m_parent->m_right == this) {
            m_parent->m_right = m_right;
        }
        else {
            m_parent->m_left = m_right;
        }
    }
    else {
        tmpToReturn = m_right;
    }
    m_parent = m_right;
    if(m_right->m_left != nullptr) {
        m_right->m_left->m_parent = this;
    }
    m_right = m_right->m_left;
    m_parent->m_left = this;
    return tmpToReturn;
}


//Right-Left tree rotation, on the node with balance factor of -2
template <class T>
typename ComplexNode<T>::ComplexNode* ComplexNode<T>::rl_rotation(ComplexNode<T>* node)
{
    ComplexNode<T>* tmp = m_right->ll_rotation(node);
    tmp = rr_rotation(tmp);
    return tmp;
}


//Left-Right tree rotation, on the node with balance factor of +2
template <class T>
typename ComplexNode<T>::ComplexNode* ComplexNode<T>::lr_rotation(ComplexNode<T>* node)
{
    ComplexNode<T>* tmp = m_left->rr_rotation(node);
    tmp = ll_rotation(tmp);
    return tmp;
}


//----------------------------------------------Node Stats---------------------------------------------------

//Update the balance factor of the specific node
template <class T>
void ComplexNode<T>::update_bf()
{
    int heightLeft = 0, heightRight = 0;
    if (m_left != nullptr) {
        heightLeft = m_left->m_height + 1;
    }
    if (m_right != nullptr) {
        heightRight = m_right->m_height + 1;
    }
    this->m_bf = heightLeft - heightRight;
}


//Update the subtree height of the specific node
template <class T>
void ComplexNode<T>::update_height()
{
    int heightLeft = 0, heightRight = 0;
    if (m_left != nullptr) {
        heightLeft = m_left->m_height + 1;
    }
    if (m_right != nullptr) {
        heightRight = m_right->m_height + 1;
    }
    if (heightLeft >= heightRight) {
        this->m_height = heightLeft;
    }
    else {
        this->m_height = heightRight;
    }
}


//--------------------------------------Private Helper Function for world_cup---------------------------------------

template <class T>
int ComplexNode<T>::get_data_inorder(int* const array, int index) const
{
    if (this != nullptr) {
        index = m_left->get_data_inorder(array, index);
        array[index++] = this->m_id;
        index = m_right->get_data_inorder(array, index);
    }
    return index;
}


//-----------------------------------------------------------------------------------------------------------

#endif //COMPLEXNODE_H