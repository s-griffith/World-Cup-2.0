#ifndef WORLD_CUP_SYSTEM_EXCEPTION_H
#define WORLD_CUP_SYSTEM_EXCEPTION_H

#include <exception>

/*
* Class NodeNotFound : std::exception
* This class represents an exception thrown when a requested node is not found.
*/
class NodeNotFound : public std::exception {

    /*
    * Provide information regarding the error - the node that was searched was not found.
    * @param - none
    * @return - const char* - description of exception
    */
    const char* what() const noexcept override
    {
        return "Node Not Found Error: The following node does not exist in the tree";
    }

};


/*
* Class InvalidID : std::exception
* This class represents an exception thrown when an ID number given is invalid.
*/
class InvalidID : public std::exception {

    /*
    * Provide information regarding the error - the ID given already exists in the tree.
    * @param - none
    * @return - const char* - description of exception
    */
    const char* what() const noexcept override {
        return "Invalid ID Error: ID already exists in the tree.";
    }
    
};

#endif //WORLD_CUP_SYSTEM_EXCEPTION_H
