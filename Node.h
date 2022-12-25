#ifndef NODE_H
#define NODE_H

/*
* Class Node
* This class is used as a base class for the specific kinds of nodes in the tree.
*/
template <class T>
class Node {
public:

    /*
    * Constructor of Node class
    * @param - none
    * @return - A new instance of Node
    */     
    Node();

    /*
    * Constructor of Node class
    * Used in unite_teams to make a new node containing the desired player
    * @param - data the node holds
    * @return - A new instance of Node
    */     
    Node(T data);

    /*
    * Copy Constructor and Assignment Operator of Node class
    * world_cup does not allow two of the same player or team (repeating ID's).
    * Therefore the system does not allow a copy constructor or assignment operator.
    */    
    Node(const Node&) = delete;
    Node& operator=(const Node& other) = delete;

    /*
    * Destructor of Node class
    * @param - none
    * @return - void
    */
    virtual ~Node();

    /*
    * Returns the ID of a node
    * @param - none
    * @return - ID
    */
    int getID() const;

protected:

    /*
     * The internal fields of Node:
     *   The data of the node of type T
     *   The height of the subtree of the node
     *   The balance factor of the node
     *   The key by which the tree is sorted (id)
     */
    T m_data;
    int m_height;
    int m_bf;
    int m_id;

    /*
     * The following classes are friend classes in order to allow full access to private fields and functions of
     * Node.
     * This allows Node to be a mostly protected class, while allowing the system to run smoothly.
    */    
    template <class K>
    friend class TreeExtraPointer;

    template <class S>
    friend class MultiTree;

    template <class N, class M>
    friend class Tree;
};


template <class T>
Node<T>::Node() :
    m_data(nullptr),
    m_height(-1),
    m_bf(0),
    m_id(0)
{}

template <class T>
Node<T>::Node(T data) :
    m_data(data),
    m_height(-1),
    m_bf(0),
    m_id(data->get_playerId())
{}

template <class T>
Node<T>::~Node()
{
    m_data = nullptr;
}

template <class T>
int Node<T>::getID() const {
    return m_id;
}


#endif //NODE_H