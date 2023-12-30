#include <SFML/Graphics.hpp>

#include"Node.h";
#include "ButtonView.h"
#include<ctime>


int NodesPerColumn;
int NodesPerRow;
int cX = 0;
int cY = 0;
stack<pair<int, int>>s;
vector <wall> Vwalls;







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


vector<int>perm = { 0,1,2,3 };
int moveX[] = { -1,0,1,0 };
int moveY[] = { 0,-1,0,1 };

bool possible(int i, int j) {

    if (i < 0 || j < 0 || i >= NodesPerColumn || j >= NodesPerRow)
        return 0;
    return 1;


}

int getDirection(int x, int y) {

    if (x == 1 && y == 0)
        return 2;
    if (x == -1 && y == 0)
        return 0;
    if (x == 0 && y == 1)
        return 3;
    if (x == 0 && y == -1)
        return 1;


}

void joinNodes(int i, int j, int newI, int newJ, int r, vector<vector<Node>>& nodes) {


    nodes[newI][newJ].visited = true;
    nodes[i][j].walls[r] = 0;
    nodes[newI][newJ].walls[(r + 2) % 4] = 0;



}

void addWall(vector<wall>& Vwall, int row, int column, vector<vector<Node>>& v) {
    for (int i = 0; i < 4; i++) {
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
void solveWithDfs() {



}


void prim(vector<wall>&Vwalls,vector<vector<Node>>&nodes) {
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
            addWall(Vwalls, (randwall.node2)->row, (randwall.node2)->column, nodes);
        }
        else {
            (randwall.node1)->visited = true;
            addWall(Vwalls, (randwall.node1)->row, (randwall.node1)->column, nodes);
        }
    }
    Vwalls.erase(Vwalls.begin() + randwallIndex);
}


void nodesInitialization(vector<vector<Node>>&nodes) {

    for (int i = 0; i < NodesPerColumn; i++) {
        for (int j = 0; j < NodesPerRow; j++) {
            nodes[i][j] = Node(i, j);
            
            }
    }

}


void clearStack(stack<pair<int,int>>&s) {

    while (!s.empty()) {
        s.pop();
    }

}

void initializeAlgorithms(int usedAlgorithm,vector<vector<Node>>&nodes) {

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
        addWall(Vwalls, randRow, randColumn, nodes);
        break;
    }
    case 3:{
        // initalize kruskal here if you want something outside the loop 
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


    int usedAlgorithm = 1;
    vector<vector<Node>>nodes(NodesPerColumn, vector<Node>(NodesPerRow));

    initializeAlgorithms(usedAlgorithm, nodes);

    bool isFinishedCreatingMaze = false;


    
    srand(static_cast<unsigned>(time(0)));

    
    


    
    
    

    ButtonView buttonBackTracking(10,50,73,25,1,"Backtracking");
    ButtonView buttonPrim(90, 50, 70, 25, 2, "Prim");

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
     
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            
            int x1 =buttonBackTracking.handleEvent(event);
            int x2 = buttonPrim.handleEvent(event);
            if (x1||x2) {
                currentX = 0;
                currentY = 0;
                usedAlgorithm = x1|x2;
                isFinishedCreatingMaze = false;
                initializeAlgorithms(usedAlgorithm,nodes);
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
        window.clear(sf::Color::Black);

        buttonBackTracking.draw(window);
        buttonPrim.draw(window);

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
                    prim(Vwalls,nodes);
                }
                else {
                    isFinishedCreatingMaze = true;
                }
            }else {

                if (usedAlgorithm == 3) {
                    // kurskal 


                }


            }

        }



        drawNodes(window, nodes, cX, cY);


        if (isFinishedCreatingMaze) {
            sprite.setScale((NODE_SIZE * nodesToWall) / 512.0, (NODE_SIZE * nodesToWall) / 512.0);
            sprite.setPosition(sf::Vector2f(currentY * NODE_SIZE + wallWidth + (SCREEN_WIDTH / 3.0), currentX * NODE_SIZE + wallWidth));

            window.draw(sprite);
        }



        window.display();

    }

    return 0;
}