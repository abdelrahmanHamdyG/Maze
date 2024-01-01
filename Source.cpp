#include <SFML/Graphics.hpp>
#include <set>
#include<random>
#include"Node.h";
#include<map>
#include "ButtonView.h"
#include<ctime>


int NodesPerColumn;
int NodesPerRow;



typedef pair<int, int> Pair;
typedef pair<double, pair<int, int> > pPair;

int cX = 0;
int cY = 0;
stack<pair<int, int>>s;

//dfs
map<Node, Node>path;
stack<Node*>st;
map<Node, Node>path1;
//
vector <wall> Vwalls;

set<pPair> openList;
vector<int>perm = { 0,1,2,3 };
int moveX[] = { -1,0,1,0 };
int moveY[] = { 0,-1,0,1 };




void joinNodes(int i, int j, int newI, int newJ, int r, vector<vector<Node>>& nodes) {


    nodes[newI][newJ].visited = true;
    nodes[i][j].walls[r] = 0;
    nodes[i][j].visited = true;
    nodes[newI][newJ].walls[(r + 2) % 4] = 0;



}

int getDirection(int x, int y) {

    if (x == -1 && y == 0)
        return 0;
    if (x == 0 && y == -1)
        return 1;
    if (x == 1 && y == 0)
        return 2;
    if (x == 0 && y == 1)
        return 3;


}

bool possible(int i, int j) {

    if (i < 0 || j < 0 || i >= NodesPerColumn || j >= NodesPerRow)
        return 0;
    return 1;


}

void mergeGroups(vector<vector<Node>>& nodes, int g1, int g2) {

    for (int i = 0; i < NodesPerColumn; i++) {
        for (int j = 0; j < NodesPerRow; j++) {
            if (nodes[i][j].group == g2) {
                nodes[i][j].group = g1;
            }
        }
    }

}



bool isUnBlocked(vector<vector<Node>>& nodes, int row, int col, int l)
{
    // Returns true if the cell is not blocked else false
    //0 = up
    //1 = left
    //2 = bottom 
    //3 = right
    if (l == 0) {

        if (nodes[row][col].walls[3] == 1)
            return (false);
        else
            return (true);
    }
    else if (l == 1) {

        if (nodes[row][col].walls[2] == 1)
            return (false);
        else
            return (true);

    }
    else if (l == 2) {
        if (nodes[row][col].walls[1] == 1)
            return (false);
        else
            return (true);
    }
    else if (l == 3) {
        if (nodes[row][col].walls[0] == 1)
            return (false);
        else
            return (true);
    }
}

bool isDestination(int row, int col, Pair dest)
{
    if (row == dest.first && col == dest.second)
        return (true);
    else
        return (false);
}


// A Utility Function to calculate the 'h' heuristics using the Manhattan distance
double calculateHValue(int row, int col, Pair dest)
{
    // Return using the distance formula
    return ((double)sqrt(
        (row - dest.first) * (row - dest.first)
        + (col - dest.second) * (col - dest.second)));
}

void tracePath(vector<vector<Node>>& nodes, Pair dest)
{
    printf("\nThe Path is ");
    int row = dest.first;
    int col = dest.second;

    stack<Pair> Path;

    while (!(nodes[row][col].parent_i == row
        && nodes[row][col].parent_j == col)) {
        Path.push(make_pair(row, col));
        int temp_row = nodes[row][col].parent_i;
        int temp_col = nodes[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }

    Path.push(make_pair(row, col));
    while (!Path.empty()) {
        pair<int, int> p = Path.top();
        Path.pop();
        printf("-> (%d,%d) ", p.first, p.second);
        nodes[p.first][p.second].color = sf::Color::Red;
        nodes[p.first][p.second].wallColor = sf::Color::Red;
    }

    return;
}


bool  aStarSearch(vector<vector<Node>>& nodes, Pair src, Pair dest, vector<vector<bool>>& closedList)
{
    // If the source is out of range






    // We set this boolean value as false as initially
    // the destination is not reached.
    bool foundDest = false;


    pPair p = *openList.begin();

    // Remove this vertex from the open list
    openList.erase(openList.begin());

    // Add this vertex to the closed list
    int i, j;

    i = p.second.first;
    j = p.second.second;

    closedList[i][j] = true;

    double gNew, hNew, fNew;


    // Only process this cell if this is a valid one
    for (int k = 0; k < 4; k++)
    {

        if (possible(i + moveX[k], j + moveY[k]) == true && isUnBlocked(nodes, i + moveX[k], j + moveY[k], k)) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i + moveX[k], j + moveY[k], dest) == true) {
                // Set the Parent of the destination cell
                nodes[i + moveX[k]][j + moveY[k]].parent_i = i;
                nodes[i + moveX[k]][j + moveY[k]].parent_j = j;
                printf("The destination cell is found\n");

                for (int i = 0; i < NodesPerColumn; i++) {
                    for (int j = 0; j < NodesPerRow; j++) {
                        nodes[i][j].color = sf::Color::White;
                        nodes[i][j].wallColor = sf::Color::White;


                    }
                }


                nodes[i + moveX[k]][j + moveY[k]].color = sf::Color::Red;

                tracePath(nodes, dest);
                foundDest = true;
                openList.clear();

                return foundDest;
            }
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i + moveX[k]][j + moveY[k]] == false
                && isUnBlocked(nodes, i + moveX[k], j + moveY[k], k)
                == true) {
                gNew = nodes[i][j].g + 1.0;
                hNew = calculateHValue(i + moveX[k], k + moveY[k], dest);
                fNew = gNew + hNew;
                if (nodes[i + moveX[k]][j + moveY[k]].f == FLT_MAX
                    || nodes[i + moveX[k]][j + moveY[k]].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i + moveX[k], j + moveY[k])));


                    // Update the details of this cell
                    nodes[i + moveX[k]][j + moveY[k]].f = fNew;
                    nodes[i + moveX[k]][j + moveY[k]].g = gNew;
                    nodes[i + moveX[k]][j + moveY[k]].h = hNew;
                    nodes[i + moveX[k]][j + moveY[k]].parent_i = i;
                    nodes[i + moveX[k]][j + moveY[k]].parent_j = j;
                    nodes[i + moveX[k]][j + moveY[k]].color = sf::Color::Green;
                    nodes[i + moveX[k]][j + moveY[k]].wallColor = sf::Color::Green;
                }
            }
        }

    }

    sf::sleep(sf::milliseconds(60));



    return foundDest;
}

void Kruskal(vector<wall>& Vwalls, vector<vector<Node>>& nodes) {
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(Vwalls.begin(), Vwalls.end(), g);

    int random = 0;
    wall* WW = &Vwalls[random];
    Node* node1 = WW->node1;
    Node* node2 = WW->node2;
    int node1Row = (WW->node1)->row;
    int node2Row = (WW->node2)->row;
    int node1Column = (WW->node1)->column;
    int node2Column = (WW->node2)->column;
    int r = getDirection(node2Column - node1Column, node2Row - node1Row);

    if (node1->group != node2->group) {

        joinNodes(node1Row, node1Column, node2Row, node2Column, r, nodes);
        mergeGroups(nodes, node1->group, node2->group);
        cout << node1->group << " " << node2->group << endl;
    }
    Vwalls.erase(Vwalls.begin() + random);
}








void drawNodes(sf::RenderWindow& window, vector<vector<Node>>v, int cX, int cY) {

    if (cX != -1 && cY != -1) {
        v[cX][cY].color = sf::Color::Red;

    }



    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[0].size(); j++) {

            v[i][j].drawNode(window);
        }

    }
    if (cX != -1 && cY != -1) {
        v[cX][cY].color = sf::Color::White;

    }

}







void addWall(vector<wall>& Vwall, int row, int column, vector<vector<Node>>& v, int algo) {
    int i = 0;
    if (algo == 3)
        i = 2;

    for (; i < 4; i++) {
        wall w;
        w.node1 = &(v[row][column]);
        int neiRow = row + moveX[i];
        int neiColumn = column + moveY[i];
        if (possible(neiRow, neiColumn)) {
            w.node2 = &(v[neiRow][neiColumn]);
            Vwall.push_back(w);
        }
    }


}



void backTracking(stack<pair<int, int>>& s, vector<vector<Node>>& nodes) {

    if (!s.empty()) {


        int i = s.top().first;
        int j = s.top().second;


        bool AllVisited = true;
        for (int k = 0; k < 4; k++) {


            int newJ = j + moveX[perm[k]];
            int newI = i + moveY[perm[k]];

            if (possible(newI, newJ)) {

                if (!nodes[newI][newJ].visited) {


                    AllVisited = false;
                    break;


                }

            }
        }
        bool foundNotVisited = false;


        if (AllVisited) {
            s.pop();

        }
        else {
            while (!foundNotVisited) {

                int r = rand() % 4;
                int newJ = j + moveX[r];

                int newI = i + moveY[r];

                if (possible(newI, newJ)) {

                    if (!nodes[newI][newJ].visited) {

                        cX = newI;
                        cY = newJ;
                        foundNotVisited = true;
                        joinNodes(i, j, newI, newJ, r, nodes);

                        s.push({ newI ,newJ });

                    }

                }

            }

        }

    }

}



int h = 0;

bool solveWithDfs(vector<vector<Node>>& nodes) {

        cout<<h++<<"\n";
    
        bool found = false;
    
        //0 = up
        //1 = left
        //2 = bottom 
        //3 = right
        Node*u = st.top();
        Node *child;
        u->color = sf::Color::Green;
        u->wallColor = sf::Color::Green;

        st.pop();
        if (u->column == NodesPerRow - 1 && u->row == NodesPerRow - 1) {
            found = true;
            tracePath(nodes, { NodesPerColumn - 1,NodesPerRow - 1 });
            for (int i = 0; i < NodesPerColumn; i++) {
                for (int j = 0; j < NodesPerRow; j++) {

                    nodes[i][j].color = sf::Color::White;
                    nodes[i][j].wallColor= sf::Color::White;
                }


            }
            return true;
        }
        for (int i = 0; i < 4; i++)
        {
            if (possible(u->row + moveX[i], u->column + moveY[i]) && isUnBlocked(nodes, u->row + moveX[i], u->column + moveY[i], i)) {
                child = &nodes[u->row + moveX[i]][u->column + moveY[i]];
                if (child->visited1)continue;
                st.push(child);
                child->visited1 = true;
                child->parent_i = u->row;
                child->parent_j = u->column;



            }

        }
        
        Node start, end;
        start.row = 0, start.column = 0;
        end.row = NodesPerColumn - 1, end.column = NodesPerRow - 1;
        while ((start.row != end.row) && (start.column != end.column)) {
            path1[path[end]] = end;
            end = path[end];
        }



   
        if (found) {
            tracePath(nodes, { NodesPerColumn - 1,NodesPerRow - 1 });
            return true;
        }

        sf::sleep(sf::milliseconds(50));
    return false;

}

void prim(vector<wall>& Vwalls, vector<vector<Node>>& nodes) {
    int randwallIndex = rand() % Vwalls.size();
    wall randwall = Vwalls[randwallIndex];
    bool opt1 = (randwall.node1)->visited && !(randwall.node2)->visited;
    bool opt2 = !(randwall.node1)->visited && (randwall.node2)->visited;
    if (opt1 || opt2) {
        int node1Row = (randwall.node1)->row;
        int node2Row = (randwall.node2)->row;
        int node1Column = (randwall.node1)->column;
        int node2Column = (randwall.node2)->column;
        int r = getDirection(node2Column - node1Column, node2Row - node1Row);
        joinNodes((randwall.node1)->row, (randwall.node1)->column, (randwall.node2)->row, (randwall.node2)->column, r, nodes);
        if (opt1) {
            (randwall.node2)->visited = true;
            addWall(Vwalls, (randwall.node2)->row, (randwall.node2)->column, nodes, 2);
        }
        else {
            (randwall.node1)->visited = true;
            addWall(Vwalls, (randwall.node1)->row, (randwall.node1)->column, nodes, 2);
        }
    }
    Vwalls.erase(Vwalls.begin() + randwallIndex);
}



void nodesInitialization(vector<vector<Node>>& nodes) {

    for (int i = 0; i < NodesPerColumn; i++) {
        for (int j = 0; j < NodesPerRow; j++) {
            nodes[i][j] = Node(i, j);
            
            nodes[i][j].f = FLT_MAX;
            nodes[i][j].g = FLT_MAX;
            nodes[i][j].h = FLT_MAX;
            nodes[i][j].parent_i = -1;
            nodes[i][j].parent_j = -1;
        }
    }

}


void clearStack(stack<pair<int, int>>& s) {

    while (!s.empty()) {
        s.pop();
    }

}
void initializeAlgorithms(int usedAlgorithm, vector<vector<Node>>& nodes) {

    nodesInitialization(nodes);

    switch (usedAlgorithm) {
    case 1: {

        nodes[0][0].visited = 1;
        clearStack(s);
        s.push({ 0,0 });
        break;
    }
    case 2: {
        Vwalls.clear();
        int randRow = rand() % NodesPerColumn;
        int randColumn = rand() % NodesPerRow;
        nodes[randRow][randColumn].visited = true;
        addWall(Vwalls, randRow, randColumn, nodes, 2);
        break;
    }
    case 3: {
        Vwalls.clear();
        int c = 0;
        for (int i = 0; i < NodesPerColumn; i++) {
            for (int j = 0; j < NodesPerRow; j++) {
                nodes[i][j].group = c++;
            }
        }
        for (int row = 0; row < NodesPerColumn; row++) {
            for (int col = 0; col < NodesPerRow; col++) {   
                addWall(Vwalls, row, col, nodes, 3);
            }
        }
        break;

    }
    default:
        break;

    }

}



int main()
{

    float innerSize = nodesToWall * NODE_SIZE;
    float wallWidth = (NODE_SIZE - innerSize) / 2;

    NodesPerColumn = SCREEN_HEIGHT / NODE_SIZE;
    NodesPerRow = 2 * (SCREEN_WIDTH / 3.0) / NODE_SIZE;

    int usedAlgorithm = 3;
    vector<vector<Node>>nodes(NodesPerColumn, vector<Node>(NodesPerRow));

    initializeAlgorithms(usedAlgorithm, nodes);

    bool isFinishedCreatingMaze = false;



    srand(static_cast<unsigned>(time(0)));


    Node *v = &nodes[0][0];

    st.push(v);


    // Create a closed list and initialise it to false which
    // means that no cell has been included yet This closed
    // list is implemented as a boolean 2D array
    vector<vector<bool>>closedList(NodesPerColumn, vector<bool>(NodesPerRow, false));


    // Declare a 2D array of structure to hold the details
    // of that cell
    //vector<vector<Node>>nodes1(NodesPerColumn,vector<Node>(NodesPerRow));





    // Put the starting cell on the open list and set its
    // 'f' as 0
    openList.insert(make_pair(0.0, make_pair(0, 0)));

    
    Pair src = make_pair(0, 0);
    Pair dest = make_pair(NodesPerColumn - 1, NodesPerRow - 1);

    nodes[0][0].f = 0.0;
    nodes[0][0].g = 0.0;
    nodes[0][0].h = 0.0;
    nodes[0][0].parent_i = 0;
    nodes[0][0].parent_j = 0;







    ButtonView buttonBackTracking(10, 50, 73, 25, 1, "Backtracking");
    ButtonView buttonPrim(90, 50, 70, 25, 2, "Prim");
    ButtonView buttonKruskal(70, 80, 70, 25, 3, "Kruskal");


    sf::Image redCircle;
    redCircle.loadFromFile("red_circle.png");










    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML Rectangle Example");

    sf::Sprite sprite;
    sf::Texture texture;
    texture.loadFromImage(redCircle);
    sprite.setTexture(texture);
    int currentX = 0, currentY = 0;

    //for prim algorithm 
    ///

    bool startingSolving = false;
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            int x1 = buttonBackTracking.handleEvent(event);
            int x2 = buttonPrim.handleEvent(event);
            int x3 = buttonKruskal.handleEvent(event);
            if (x1 || x2 || x3) {
                currentX = 0;
                currentY = 0;
                usedAlgorithm = x1 | x2 | x3;
                isFinishedCreatingMaze = false;
                initializeAlgorithms(usedAlgorithm, nodes);
            }

            if (isFinishedCreatingMaze) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {

                    if (!nodes[currentX][currentY].walls[1] && possible(currentX - 1, currentY)) {

                        currentX -= 1;
                    }
                    cout << currentX << " " << currentY << '\n';
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    if (!nodes[currentX][currentY].walls[3] && possible(currentX + 1, currentY)) {

                        currentX += 1;
                    }

                    cout << currentX << " " << currentY << '\n';
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    if (!nodes[currentX][currentY].walls[0] && possible(currentX, currentY - 1)) {

                        currentY -= 1;
                    }
                    cout << currentX << " " << currentY << '\n';
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    if (!nodes[currentX][currentY].walls[2] && possible(currentX, currentY + 1)) {

                        currentY += 1;
                    }
                    cout << currentX << " " << currentY << '\n';
                }

            }
        }
        buttonBackTracking.update();
        buttonPrim.update();
        buttonKruskal.update();
        window.clear(sf::Color::Black);

        buttonBackTracking.draw(window);
        buttonPrim.draw(window);
        buttonKruskal.draw(window);
        if (usedAlgorithm == 1) {


            if (s.empty()) {
                cX = cY = -1;
                isFinishedCreatingMaze = true;

            }
            else {
                backTracking(s, nodes);
            }
        }
        else {
            if (usedAlgorithm == 2) {
                cX = -1;
                cY = -1;
                if (!Vwalls.empty()) {
                    prim(Vwalls, nodes);
                }
                else {
                    isFinishedCreatingMaze = true;
                }
            }
            else {

                if (usedAlgorithm == 3) {
                    // kurskal 
                    cX = -1;
                    cY = -1;
                    if (!Vwalls.empty()) {
                        Kruskal(Vwalls, nodes);
                    }
                    else {
                        isFinishedCreatingMaze = true;
                    }

                }


            }

        }



        drawNodes(window, nodes, cX, cY);


        if (isFinishedCreatingMaze) {
            sprite.setScale((NODE_SIZE * nodesToWall) / 512.0, (NODE_SIZE * nodesToWall) / 512.0);
            sprite.setPosition(sf::Vector2f(currentY * NODE_SIZE + wallWidth + (SCREEN_WIDTH / 3.0), currentX * NODE_SIZE + wallWidth));

            window.draw(sprite);

            if (!startingSolving) {

                
                v->visited1 = true;
               if(!st.empty()) {
                    bool found=solveWithDfs(nodes);
                    if (found)
                        startingSolving = true;
                }
                else {
                    startingSolving = true;
                }
                
            }

        }



        window.display();

    }

    return 0;
}