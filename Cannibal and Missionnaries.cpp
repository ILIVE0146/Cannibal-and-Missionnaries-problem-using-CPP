// * Cannibal and Missionnaries
// *cannibals shouldnt out number the missionnaries
/*

 This code uses Breadth First Search algorithm to solve the Cannibal and Missionnaries
 using BFS we discover new nodes where each node represents a posibility of occuring a change of number of Cannibal and/or Missionnaries from it's parent node.

 */
//TODO: Find the shortest path to goal
#include <iostream>
#include <vector>
#include <set>
int nextEmptyPositionInQueue = 0;
enum List{ zero , one , two , three };
enum Boatposition{ left = bool(false) , right = bool(true) };


class Node{
public:
    int position;
    Boatposition bp;
    std::pair<List/*cannibals*/,List/*missionaries*/> left,right; // each node in graph has 2, each representing number of cannicals and massionaries on each side respectively
    bool isConditionSatisfied = true;
    bool checkCondition(){
        //checking condition on left side of the bank
        if(this->left.first > this->left.second && this->left.second != 0 ){
            return false;
        }
        //checking condition on right side of the bank
        if(this->right.first > this->right.second && this->right.second != 0){
            return false;
        }
        return true;
    }
};

///\brief creates a new node and if the data of the given node exists in graphNodes, the newly created now will have same position else, it initializes a unique position to it and returns it
///\brief List is an enum {zero, one, two, three}
///\param boatPosition Boatposition enum
///\param left std::pair<List,List>
///\param right std::pair<List,List>
///\param graphNodes std::vector<Node>&
///\return Node class's object
Node createNode(Boatposition bp, std::pair<List,List> left,std::pair<List,List> right, std::vector<Node>& graphNodes ){
    Node tempNode;
    bool isMatched = false;
    int tempPos=0;
    for (auto &&vec : graphNodes)
    {
        if(vec.bp == bp && vec.left == left && vec.right == right){
            isMatched = true;
            tempPos = vec.position;
            break;
        }
    }
    
    if(isMatched == false){
        tempNode.position = nextEmptyPositionInQueue;
        nextEmptyPositionInQueue++;
    }else{
        tempNode.position = tempPos;
    }
    tempNode.bp = bp;
    tempNode.left = left;
    tempNode.right = right;
    tempNode.isConditionSatisfied = tempNode.checkCondition();
    return tempNode;
}
class Graph{
public:
    std::vector<Node> nodes;
    std::set<std::pair<int,int>> edgesList; // example {0,1} defines a edge between 0 node to 1 node

    /// \brief function to push a new node
    /// \param ver Node class
    /// \param edglist std::vector<std::pair<int,int>> it's connected edge lists
    void pushNode(Node ver, std::set<std::pair<int,int>> edglist){
        this->nodes.push_back(ver);
        this->edgesList.insert(edglist.begin(), edglist.end());
    }

    ///\brief adds a edge between the given nodes into edgesList, if the source node doesnt exist, it will add it into the graph
    ///\param from Node class
    ///\param to Node class
    void addEdge(Node from, Node to){
        bool doesFromExist = false;
        bool doesToExist = false;
        for(auto i = this->nodes.begin(); i != this->nodes.end(); i++){
            if(i->position == from.position){
                doesFromExist = true;
            }
            if(i->position == to.position){
                doesToExist = true;
            }
            if(doesFromExist == true && doesToExist == true ){
                break;
            }
        }
        
        if(doesFromExist == false){
            this->nodes.push_back(createNode(from.bp,from.left,from.right , this->nodes ));
        }
        if(doesToExist == false){
            this->nodes.push_back(to);
        }
        this->edgesList.insert( {from.position,to.position} );
    }
};

int main(){
    Graph myGraph;
    std::vector<int> visited , queue ;
    
    {
        Node startNode = createNode(right,{zero,zero},{three,three} , myGraph.nodes);
        if(startNode.isConditionSatisfied == true){
            // visited.push_back(startNode.position);
            queue.push_back(startNode.position);
            myGraph.nodes.push_back(startNode);
        }else{
            std::cerr<<"ERROR CREATING START NODE, CHECK CONDITION FAILED"<<std::endl;
            return -1;
        }
    }
    while (!queue.empty())
    {
        int pos = queue.front();
        Node n;
        for( auto itr = myGraph.nodes.begin(); itr!= myGraph.nodes.end(); itr++){
            if(itr->position == pos){
                int location = itr - myGraph.nodes.begin();
                n = myGraph.nodes.at(location);
            }
        }
        if(n.isConditionSatisfied == true){
            if(n.bp == right){
                if(n.right.first >= 2){
                    //checking if i can move 2 canibals at once or not
                    List right =List(int(n.right.first) - 2);
                    List left =List(int(n.left.first) + 2);

                    Node newNodes = createNode(Boatposition(!bool(n.bp)), {left, n.left.second },{right , n.right.second} , myGraph.nodes );
                    myGraph.addEdge(n,newNodes);
                    // Checking if the newNode is in visited vector or not, if they match, we dont push the newNode into queue Vector
                    bool ismatched = false;
                    for (auto &&vecitr : visited)
                    {
                        if(vecitr == newNodes.position){
                            ismatched = true;
                            break;
                        }
                    }
                    if(ismatched == false){
                        queue.push_back(newNodes.position);
                    }
                    
                }
                if(n.right.second >= 2){
                    //checking if i can move 2 missionnaries or not
                    List right = List(int(n.right.second) - 2);
                    List left = List(int(n.left.second) + 2);
                    
                    Node newNodes = createNode(Boatposition(!bool(n.bp)), {n.left.first, left },{n.right.first , right}, myGraph.nodes );
                    myGraph.addEdge(n,newNodes);
                    bool ismatched = false;
                    // Checking if the newNode is in visited vector or not, if they match, we dont push the newNode into queue Vector
                    for (auto &&vecitr : visited)
                    {
                        if(vecitr == newNodes.position){
                            ismatched = true;
                            break;
                        }
                    }
                    if(ismatched == false){
                        queue.push_back(newNodes.position);
                    }
                }
                if(n.right.first >= 1 && n.right.second >= 1){
                    //checking if i can move 1 cannibal and 1 missionnary
                    std::pair<List,List>left,right;
                    right = {  List(int(n.right.first) - 1) , List(int(n.right.second) - 1)};
                    left = {  List(int(n.left.first) + 1) , List(int(n.left.second) + 1)};

                    Node newNodes = createNode(Boatposition(!bool(n.bp)), left, right , myGraph.nodes );
                    myGraph.addEdge(n,newNodes);
                    // Checking if the newNode is in visited vector or not, if they match, we dont push the newNode into queue Vector
                    bool ismatched = false;
                    for (auto &&vecitr : visited)
                    {
                        if(vecitr == newNodes.position){
                            ismatched = true;
                            break;
                        }
                    }
                    if(ismatched == false){
                        queue.push_back(newNodes.position);
                    }
                }
                if(n.right.first >= 1){
                    //checking if i can move 1 cannibal
                    std::pair<List,List>left,right;
                    right = {  List(int(n.right.first) - 1) , List(int(n.right.second)) };
                    left = {  List(int(n.left.first) + 1) , List(int(n.left.second)) };

                    Node newNodes = createNode(Boatposition(!bool(n.bp)), left, right, myGraph.nodes );
                    myGraph.addEdge(n,newNodes);
                    // Checking if the newNode is in visited vector or not, if they match, we dont push the newNode into queue Vector
                    bool ismatched = false;
                    for (auto &&vecitr : visited)
                    {
                        if(vecitr == newNodes.position){
                            ismatched = true;
                            break;
                        }
                    }
                    if(ismatched == false){
                        queue.push_back(newNodes.position);
                    }
                }
                if(n.right.second >= 1){
                    //checking if i can move 1 missionnary
                    std::pair<List,List>left,right;
                    right = {  List(int(n.right.first) ) , List(int(n.right.second)- 1) };
                    left = {  List(int(n.left.first) ) , List(int(n.left.second)+ 1) };

                    Node newNodes = createNode(Boatposition(!bool(n.bp)), left, right, myGraph.nodes );
                    myGraph.addEdge(n,newNodes);
                    // Checking if the newNode is in visited vector or not, if they match, we dont push the newNode into queue Vector
                    bool ismatched = false;
                    for (auto &&vecitr : visited)
                    {
                        if(vecitr == newNodes.position){
                            ismatched = true;
                            break;
                        }
                    }
                    if(ismatched == false){
                        queue.push_back(newNodes.position);
                    }

                }
            }else{
                // new node creation condition if the current boat position is on the Left side of the river.
                if(n.left.first >= 2){
                    //checking if i can move 2 canibals at once or not
                    List first =List(int(n.left.first) - 2);
                    List second =List(int(n.right.first) + 2);

                    Node newNodes = createNode(Boatposition(!bool(n.bp)), {first, n.left.second },{second , n.right.second}, myGraph.nodes );
                    myGraph.addEdge(n,newNodes);
                    // Checking if the newNode is in visited vector or not, if they match, we dont push the newNode into queue Vector
                    bool ismatched = false;
                    for (auto &&vecitr : visited)
                    {
                        if(vecitr == newNodes.position){
                            ismatched = true;
                            break;
                        }
                    }
                    if(ismatched == false){
                        queue.push_back(newNodes.position);
                    }
                }
                if(n.left.second >= 2){
                    //checking if i can move 2 missionnaries or not
                    List first = List(int(n.left.second) - 2);
                    List second = List(int(n.right.second) + 2);
                    
                    Node newNodes = createNode(Boatposition(!bool(n.bp)), {n.left.first, first },{n.right.first , second}, myGraph.nodes );
                    myGraph.addEdge(n,newNodes);
                    // Checking if the newNode is in visited vector or not, if they match, we dont push the newNode into queue Vector
                    bool ismatched = false;
                    for (auto &&vecitr : visited)
                    {
                        if(vecitr == newNodes.position){
                            ismatched = true;
                            break;
                        }
                    }
                    if(ismatched == false){
                        queue.push_back(newNodes.position);
                    }
                }
                if(n.left.first >= 1 && n.right.second >= 1){
                    //checking if i can move 1 cannibal and 1 missionnary
                    std::pair<List,List>left,right;
                    left = {  List(int(n.left.first) - 1) , List(int(n.left.second) - 1)};
                    right = {  List(int(n.right.first) + 1) , List(int(n.right.second) + 1)};

                    Node newNodes = createNode(Boatposition(!bool(n.bp)), left, right, myGraph.nodes );
                    myGraph.addEdge(n,newNodes);
                    // Checking if the newNode is in visited vector or not, if they match, we dont push the newNode into queue Vector
                    bool ismatched = false;
                    for (auto &&vecitr : visited)
                    {
                        if(vecitr == newNodes.position){
                            ismatched = true;
                            break;
                        }
                    }
                    if(ismatched == false){
                        queue.push_back(newNodes.position);
                    }
                }
                if(n.left.first >= 1){
                    //checking if i can move 1 cannibal
                    std::pair<List,List>left,right;
                    left = {  List(int(n.left.first) - 1) , List(int(n.left.second)) };
                    right = {  List(int(n.right.first) + 1) , List(int(n.right.second)) };

                    Node newNodes = createNode(Boatposition(!bool(n.bp)), left, right, myGraph.nodes );
                    myGraph.addEdge(n,newNodes);
                    // Checking if the newNode is in visited vector or not, if they match, we dont push the newNode into queue Vector
                    bool ismatched = false;
                    for (auto &&vecitr : visited)
                    {
                        if(vecitr == newNodes.position){
                            ismatched = true;
                            break;
                        }
                    }
                    if(ismatched == false){
                        queue.push_back(newNodes.position);
                    }
                }
                if(n.left.second >= 1){
                    //checking if i can move 1 missionnary
                    std::pair<List,List>left,right;
                    left = {  List(int(n.left.first) ) , List(int(n.left.second)- 1) };
                    right = {  List(int(n.right.first) ) , List(int(n.right.second)+ 1) };

                    Node newNodes = createNode(Boatposition(!bool(n.bp)), left, right, myGraph.nodes );
                    myGraph.addEdge(n,newNodes);
                    // Checking if the newNode is in visited vector or not, if they match, we dont push the newNode into queue Vector
                    bool ismatched = false;
                    for (auto &&vecitr : visited)
                    {
                        if(vecitr == newNodes.position){
                            ismatched = true;
                            break;
                        }
                    }
                    if(ismatched == false){
                        queue.push_back(newNodes.position);
                    }

                }

            }
            // for(auto setitr = myGraph.edgesList.begin() ; setitr != myGraph.edgesList.end(); setitr++){
            //     bool isMatched = false;
            //     for (auto&& vectoritr : visited)
            //     {
            //         if(setitr->second == vectoritr){
            //             isMatched = true;
            //             break;
            //         }
            //     }
            //     if(isMatched == false){
            //         queue.push_back(setitr->second);
            //     }
            // }
        }
        visited.push_back(n.position);
        queue.erase(queue.begin());
    }

    for (auto &&vecitr : myGraph.nodes)
    {
        // Checking if we reached goal state or not, which is cannibals and Missionnaries each 3 on the left side of the bank
        if(vecitr.left.first/*cannibals on left side*/ == List(3) && vecitr.left.second/*Missionnaries on left side*/ == List(3) ){
            //we reached our goal state
            std::cout<<"Goal State of transporting cannibals and Missionnaries each 3 to the left side of the bank reached with Node position in graph "<<vecitr.position<<std::endl;
        }

        //TODO: use A* algorithm with heuristic = 1 or dijkstra's algorithm (same thing) to find the shortest path to reach the goal state.
    }
    

}