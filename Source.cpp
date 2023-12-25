#include <SFML/Graphics.hpp>

#include"Node.h";
#include<ctime>


int NodesPerColumn;
int NodesPerRow;




void drawNodes(sf::RenderWindow &window,vector<vector<Node>>v,int cX,int cY) {

    v[cX][cY].color = sf::Color::Red;

    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[0].size(); j++) {
            
            v[i][j].drawNode(window);
        }

    }
    v[cX][cY].color = sf::Color::White;

}


vector<int>perm = { 0,1,2,3 };
int moveX[] = { -1,0,1,0 };
int moveY[] = { 0,-1,0,1 };

bool possible(int i,int j) {

    if (i < 0 || j < 0 || i >= NodesPerColumn || j >= NodesPerRow)
        return 0;
    return 1;


}

int getDirection(int x,int y) {

    if (x == 1 && y == 0)
        return 2;
    if (x == -1 && y == 0)
        return 0;
    if (x == 0 && y == 1)
        return 3;
    if (x == 0 && y == -1)
        return 1;


}

void recurse(int i,int j,vector<vector<Node>>&v) {
    
    next_permutation(perm.begin(), perm.end());
    swap(perm[rand() % 4], perm[rand() % 4]);
    for (int k = 0; k < 4; k++) {

        int newJ = j + moveX[perm[k]];
        int newI = i + moveY[perm[k]];

        if (possible(newI, newJ)) {

            if (!v[newI][newJ].visited) {

                
                int direction = getDirection(moveX[perm[k]], moveY[perm[k]]);

                v[i][j].walls[direction] = 0;
                v[newI][newJ].walls[(direction + 2) % 4]=0;

                v[newI][newJ].visited = 1;
                recurse(newI, newJ, v);

            }
                    
        }
    }

}

void ff() {



}
int cX = 0;
int cY = 0;

int main()
{


    int usedAlgorithm = 1;
    srand(static_cast<unsigned>(time(0)));


    NodesPerColumn = SCREEN_HEIGHT / NODE_SIZE;
    
    NodesPerRow = SCREEN_WIDTH / NODE_SIZE;
    cout << NodesPerColumn << "\n" << NodesPerRow << '\n';
    vector<vector<Node>>nodes(NodesPerColumn,vector<Node>(NodesPerRow));

    for (int i = 0; i < NodesPerColumn; i++) {
        for (int j = 0; j < NodesPerRow; j++) {

            
            nodes[i][j] = Node(i,j);
            


        }

    }
    
    stack<pair<int, int>>s;

    s.push({ 0, 0 });

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML Rectangle Example");


    nodes[0][0].visited = 1;
    //recurse(0, 0, nodes);
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);
        if (usedAlgorithm == 1) {
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
                                nodes[newI][newJ].visited = true;
                                nodes[newI][newJ].walls[(r + 2) % 4] = 0;
                                nodes[i][j].walls[r] = 0;

                                s.push({ newI ,newJ });

                            }

                        }

                    }

                }

            }
        }
        else {
            if (usedAlgorithm == 2) {


            }

        }
        sf::sleep(sf::milliseconds(1));
        
        drawNodes(window, nodes,cX,cY);
        window.display();
        
        

    }

    return 0;
}



