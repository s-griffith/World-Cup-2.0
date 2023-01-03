#ifndef AVLMULTITREE_H
#define AVLMULTITREE_H

#include "AVLTree.h"
#include "Node.h"
#include "ComplexNode.h"
#include "Exception.h"
#include <iostream>

/*
* Class MultiTree : Tree
* This class is used to create a tree based on ComplexNode that is sorted by three keys:
*       the number of goals the player scored, the number of cards they received, and their player ID
*/
template <class T>
class MultiTree : public Tree<ComplexNode<T>, T> {
public:

    /*
    * Constructor of MultiTree class
    * @param - none
    * @return - A new instance of MultiTree
    */
    MultiTree();

    /*
    * Copy Constructor and Assignment Operator of MultiTree class
    * world_cup does not allow two of the same player or team (repeating ID's).
    * Therefore the system does not allow a copy constructor or assignment operator.
    */
    MultiTree(const MultiTree& other) = delete;
    MultiTree& operator=(const MultiTree& other) = delete;

    /*
    * Destructor of MultiTree class
    * @param - none
    * @return - void
    */
    ~MultiTree() = default;

    /*
     * Insert new node with data, according to the id, goals, and cards given
     * @param - New data to insert and the ID, goals, and cards of the new node
     * @return - void
     */
    void insert(T data, const int id, const int ability);

    /*
     * Remove node according to the id, goals, and cards given
     * @param - The ID, goals, and cards of the node that needs to be removed
     * @return - void
     */
    void remove(const int id, const int ability);

    ComplexNode<T>* make_node_leaf(ComplexNode<T>* node);

    /*
     * Search for max node by going down the right side of the tree
     * @param - none
     * @return - the data of the max node
     */
    //T& search_and_return_max();
    
    /*
     * Search for a specific node, according to the id, goals, and cards given
     * @param - The ID, goals, and cards of the requested node
     * @return - the data of the node
     */
    T& search_and_return_data(const int id, const int ability);

    /*
     * Search for node with a specific id, according to the id, goals, and cards given
     * @param - The ID, goals, and cards of the requested node
     * @return - the requested node
     */
    ComplexNode<T>& search_specific_id(const int id, const int ability);

    /*
     * Recursively search the tree, according to the id, goals, and cards given, starting with the root
     * @param - The ID, goals, and cards of the requested node, and the root of its tree
     * @return - the requested node
     */
    ComplexNode<T>& search_recursively(const int id, const int ability, ComplexNode<T>* currentNode);

    /*
     * Helper function for get_all_players in world_cup:
     * Recursively inserts the player ID's of the data of the tree into a given array
     * @param - an array
     * @return - void
     */
    //void get_all_data(int* const array) const;

    /*
     * Helper function for get_closest_player in world_cup:
     * Updates the closest player pointers of the given player
     * @param - PlayerID, goals, cards
     * @return - void
     */
    //void update_closest(const int playerId, const int goals, const int cards);

    ComplexNode<T>* find_index(ComplexNode<T>* node, const int index);

    /*
     * Helper function for unite_teams in world_cup:
     * Creates a tree from a given ordered array
     * @param - a pointer to the array and its final index
     * @return - void
     */
    //void insertInorder(T* data, const int end);

    /*
     * Helper function for testing:
     * Prints the tree, node by node
     * @param - none
     * @return - void
     */
    void print_tree();

private:

    /*
     * Helper function for unite_teams in world_cup:
     * Recursively inserts the data from the array into a tree
     * @param - a pointer to the array, its starting index, and its final index
     * @return - a pointer to the root node at the end of the insertions
     */
   // ComplexNode<T>* insertInorderRecursive(T* data, const int start, const int end);

    /*
     * Helper functions for update_closest:
     * Finds the right and left closest players
     * @param - ComplexNode of the current player
     * @return - ComplexNode of the closest player
     */
    typename ComplexNode<T>::ComplexNode* findLeftClosest(ComplexNode<T>* currentPlayerNode);
    typename ComplexNode<T>::ComplexNode* findRightClosest(ComplexNode<T>* currentPlayerNode);

    /*
    * Helper function for updating the rank during remove:
    * Starts at a given node and goes up until reaching the root, updating the number of children for each node
    * @param - ComplexNode* of the node to start the update at
    * @return - void
    */
    void upwardUpdate(ComplexNode<T>* starting);
};


//-----------------------------------------Constructor--------------------------

template<class T>
MultiTree<T>::MultiTree() :
    Tree<ComplexNode<T>, T>()
{}


//----------------------------------Insert and Remove---------------------------------

template<class T>
void MultiTree<T>::insert(T data, const int id, const int ability) {
    //If this is the first node in the tree:
    if (this->m_node->m_height == -1) {
        this->m_node->m_data = data;
        this->m_node->m_ability = ability;
        this->m_node->m_id = id;
        this->m_node->m_height++;
        this->m_node->m_numChildren++;
        return;
    }
    //Find the proper location of the new node (when it's not the first):
    ComplexNode<T>* x = this->m_node;
    ComplexNode<T>* parent = nullptr;
    while (x != nullptr) {
        parent = x;
        if (x->m_id == id) {
            //A node with that id already exists - invalid operation
            throw InvalidID(); 
        }
        if (ability < x->m_ability) {
            x = x->m_left;
        }
        else if (ability == x->m_ability) {
            if (id < x->m_id) {
                x = x->m_left;
            }
            else {
                x = x->m_right;
            }
        }
        else {
            x = x->m_right;
        }
    }
    //Create the new node and add it to the tree:
    ComplexNode<T>* node = nullptr;
    try{
        node = new ComplexNode<T>();
    }
    catch(const std::bad_alloc& e)
    {
        delete node;
        throw e;
    }
    node->m_parent = parent;
    node->m_left = nullptr;
    node->m_right = nullptr;
    node->m_data = data;
    node->m_ability = ability;
    node->m_id = id;
    node->m_height = 0;
    node->m_numChildren = 1;
    if (node->m_ability < parent->m_ability) {
        parent->m_left = node;
    }
    else if (node->m_ability == parent->m_ability) {
        if (node->m_id < parent->m_id) {
            parent->m_left = node;
        }
        else {
            parent->m_right = node;
        }
    }
    else {
        parent->m_right = node;
    }
    //Go back up the tree to update the number of nodes in each of the parents' subtrees:
    upwardUpdate(parent);
    this->rebalance_tree(node->m_parent);
    this->m_node->update_children();
}


template<class T>
void MultiTree<T>::remove(const int id, const int ability) {
    if (this->m_node->m_id == id && this->m_node->m_right == nullptr && this->m_node->m_left == nullptr 
                                                                                && this->m_node->m_parent == nullptr) {
        this->m_node->m_data = nullptr;
        this->m_node->m_height = -1;
        this->m_node->m_bf = 0;
        this->m_node->m_id = 0;
        this->m_node->m_ability = 0;
        this->m_node->m_numChildren = 0;
        return;
    }
    ComplexNode<T>* toRemove = &(search_specific_id(id, ability));
    ComplexNode<T>* nodeToFix = make_node_leaf(toRemove);
    delete toRemove;
    //Go up the tree and check the balance factors and complete needed rotations
    Tree<ComplexNode<T>, T>::rebalance_tree(nodeToFix);
}

template <class T>
ComplexNode<T>* MultiTree<T>::make_node_leaf(ComplexNode<T>* node)
{
    ComplexNode<T>* toUpdate = node->m_parent;
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
        node->m_numChildren = 0;
        upwardUpdate(toUpdate);
        return node->m_parent;
    }
    //Node to be deleted has one child
    if (node->m_left == nullptr || node->m_right == nullptr) {
        ComplexNode<T>* tmpChild;
        if (node->m_left != nullptr) {
            tmpChild = node->m_left;
        }
        else {
            tmpChild = node->m_right;
        }
        toUpdate = tmpChild;
        //Connect child to parent
        tmpChild->m_parent = node->m_parent;
        node->m_numChildren = 0;
        if (node->m_parent != nullptr) {
            if (node->m_parent->m_left == node) {
                node->m_parent->m_left = tmpChild;
            }
            else {
                node->m_parent->m_right = tmpChild;
            }
            upwardUpdate(toUpdate);
        }
        else {
            this->m_node = tmpChild;
            upwardUpdate(toUpdate);
            return this->m_node;
        }
        return node->m_parent;
    }
    //Node to be deleted has two children
    ComplexNode<T>* successor = node->m_right;
    while (successor->m_left != nullptr) {
        successor = successor->m_left;
    }
    ComplexNode<T>* parentToReturn;
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
    toUpdate = parentToReturn;
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
        this->m_node = successor;
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
    node->m_numChildren = 0;
    upwardUpdate(toUpdate);
    return parentToReturn;
}



//-----------------------------------------Search Functions-----------------------------------------

/*template<class T>
T& MultiTree<T>::search_and_return_max() {
    ComplexNode<T>* node = this->m_node;
    while(node->m_right != nullptr) {
        node = node->m_right;
    }
    return node->m_data;
}*/


template<class T>
T& MultiTree<T>::search_and_return_data(const int id, const int ability) {
    return search_recursively(id, ability, this->m_node).m_data;
}


template<class T>
ComplexNode<T>& MultiTree<T>::search_specific_id(const int id, const int ability) {
    return search_recursively(id, ability, this->m_node);
}


template<class T>
ComplexNode<T>& MultiTree<T>::search_recursively(const int id, const int ability, ComplexNode<T>* currentNode) {
    if (currentNode == nullptr) {
        throw NodeNotFound();
    }
    if (currentNode->m_id == id) {
        return *currentNode;
    }
    if (currentNode->m_ability < ability) {
        return search_recursively(id, ability, currentNode->m_right);
    }
    if (currentNode->m_id < id && currentNode->m_ability == ability) {
        return search_recursively(id, ability, currentNode->m_right);
    }
    return search_recursively(id, ability, currentNode->m_left);
}


//-----------------------------------------Helper Functions for world_cup-----------------------------------------

/*template <class T>
void MultiTree<T>::get_all_data(int* const array) const
{
    if (this != nullptr) {
        this->m_node->get_data_inorder(array, 0);
    }
}*/

template <class T>
ComplexNode<T>* MultiTree<T>::find_index(ComplexNode<T>* node, const int index) {
    int leftChildren = 0;
    if (node->m_left != nullptr) {
        leftChildren = node->m_left->m_numChildren;
    }
    if (leftChildren > index) {
        return find_index(node->m_left, index);
    }
    else if (index == leftChildren) {
        return node;
    }
    else {
        if (node->m_right != nullptr) {
            return find_index(node->m_right, index-leftChildren-1);
        }
        else {
            return node;
        }
    }
}


/*template <class T>
void MultiTree<T>::insertInorder(T* data, const int end) {
    ComplexNode<T>* tmp = this->m_node;
    this->m_node = insertInorderRecursive(data, 0, end);
    delete tmp;
}

template <class T>
ComplexNode<T>* MultiTree<T>::insertInorderRecursive(T* data, const int start, const int end) {
    //Stop recursion
    if (start > end)
        return nullptr;
    //Get the middle player and make root
    int mid = (start + end)/2;
    ComplexNode<T>* root = new ComplexNode<T>(data[mid]);
    //Construct left subtree
    root->m_left =  insertInorderRecursive(data, start, mid-1);
    if (root->m_left != nullptr) {
        root->m_left->m_parent = root;
    }
    //Construct right subtree
    root->m_right = insertInorderRecursive(data, mid+1, end);
    if (root->m_right != nullptr) {
        root->m_right->m_parent = root;
    }
    root->update_bf();
    root->update_height();
    return root;
}*/

template<class T>
void MultiTree<T>::upwardUpdate(ComplexNode<T>* starting) {
    while (starting != nullptr) {
        starting->update_children();
        starting = starting->m_parent;
    }
}

template<class T>
void MultiTree<T>::print_tree() {
    this->m_node->inorderWalkNode(1);
}

//----------------------------------------------------------------------------------------------

#endif //AVLMULTITREE_H