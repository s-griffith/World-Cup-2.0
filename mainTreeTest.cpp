#include <iostream>
#include "AVLMultiTree.h"
#include "Teams.h"

int main() {
    MultiTree<Team*> t1;
    Team* team5 = new Team(5);
    Team* team6 = new Team(6);
    t1.insert(team5, 5, 0);
    t1.insert(team6, 6, 1);
    t1.print_tree();
    std::cout << "end print\n";
    Team* team7 = new Team(7);
    Team* team8 = new Team(8);
    t1.insert(team7, 7, 2);
    t1.insert(team8, 8, 2);
    t1.print_tree();
    std::cout << "Index 0 is " << t1.find_index(t1.m_node, 0)->getID() << std::endl;
    std::cout << "Index 1 is " << t1.find_index(t1.m_node, 1)->getID() << std::endl;
    std::cout << "Index 2 is " << t1.find_index(t1.m_node, 2)->getID() << std::endl;
    std::cout << "Index 3 is " << t1.find_index(t1.m_node, 3)->getID() << std::endl;
    std::cout << "end print\n";
    t1.remove(5, 0);
    t1.print_tree();
    std::cout << "end print\n";

    MultiTree<Team*> t2;
    Team* team3 = new Team(3);
    Team* team10 = new Team(10);
    Team* team18 = new Team(18);
    Team* team13 = new Team(13);
    Team* team4 = new Team(4);
    Team* team11 = new Team(11);
    Team* team12 = new Team(12);

    t2.insert(team3, 3, 1);
    t2.insert(team4, 4, 1);
    t2.insert(team5, 5, 1);
    t2.insert(team7, 7, 1);
    t2.insert(team10, 10, 1);
    t2.insert(team18, 18, 1);
    t2.insert(team13, 13, 1);
    t2.insert(team11, 11, 1);
    t2.insert(team12, 12, 1);
    t2.print_tree();
    std::cout << "end print\n";
    for(int i = 0; i <9; i++)
    {
        ComplexNode<Team*>* team = t2.find_index(t2.m_node, i);
        int ID;
        if (team == nullptr) {
            ID = -1;
        }
        else {
            ID = team->getID();
        }
        std::cout << "Index " << i << " is " << ID << std::endl;
    }
    std::cout << "end print\n";
    return 0;
}