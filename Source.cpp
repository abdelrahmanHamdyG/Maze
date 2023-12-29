#include <SFML/Graphics.hpp>

#include"Node.h";
#include<ctime>


int NodesPerColumn;
int NodesPerRow;
int cX = 0;
int cY = 0;







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

void backtrackingAlgorithm(int i, int j, vector<vector<Node>>& v) {

    next_permutation(perm.begin(), perm.end());
    swap(perm[rand() % 4], perm[rand() % 4]);
    for (int k = 0; k < 4; k++) {

        int newJ = j + moveX[perm[k]];
        int newI = i + moveY[perm[k]];

        if (possible(newI, newJ)) {

            if (!v[newI][newJ].visited) {


                int direction = getDirection(moveX[perm[k]], moveY[perm[k]]);

                v[i][j].walls[direction] = 0;
                v[newI][newJ].walls[(direction + 2) % 4] = 0;

                v[newI][newJ].visited = 1;
                backtrackingAlgorithm(newI, newJ, v);

            }

        }
    }

}


void backTracking2(stack<pair<int, int>>& s, vector<vector<Node>>& nodes) {

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





void ff() {



}

int main()
{

    sf::Image redCircle;
    redCircle.loadFromFile("red_circle.png");

    float innerSize = nodesToWall * NODE_SIZE;
    float wallWidth = (NODE_SIZE - innerSize) / 2;

    vector <wall> Vwalls;
    sf::Font font;
    if (!font.loadFromFile("fontt.ttf")) {
        // Handle font loading error
        return -1;
    }

    sf::Text youWonText("You Won!", font, 40);
    youWonText.setPosition((SCREEN_WIDTH - youWonText.getLocalBounds().width) / 2.0f,
        (SCREEN_HEIGHT - youWonText.getLocalBounds().height) / 2.0f);
    youWonText.setFillColor(sf::Color::Green);
    youWonText.setStyle(sf::Text::Bold);


    int usedAlgorithm = 2;
    srand(static_cast<unsigned>(time(0)));

    NodesPerColumn = SCREEN_HEIGHT / NODE_SIZE;

    NodesPerRow = SCREEN_WIDTH / NODE_SIZE;

    vector<vector<Node>>nodes(NodesPerColumn, vector<Node>(NodesPerRow));

    for (int i = 0; i < NodesPerColumn; i++) {
        for (int j = 0; j < NodesPerRow; j++) {


            nodes[i][j] = Node(i, j);



        }

    }

    stack<pair<int, int>>s;

    s.push({ 0, 0 });
    bool isFinishedCreatingMaze = false;

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML Rectangle Example");

    sf::Sprite sprite;
    sf::Texture texture;
    texture.loadFromImage(redCircle);
    sprite.setTexture(texture);
    int currentX = 0, currentY = 0;
    int randRow = rand() % NodesPerColumn;
    int randColumn = rand() % NodesPerRow;
    nodes[randRow][randColumn].visited = true;
    addWall(Vwalls, randRow, randColumn, nodes);
    // nodes[0][0].visited = 1;
     //recurse(0, 0, nodes);
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();


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

        window.clear(sf::Color::Black);
        if (usedAlgorithm == 1) {

            if (s.empty()) {
                cX = cY = -1;
                isFinishedCreatingMaze = true;

            }
            else {
                backTracking2(s, nodes);
            }
        }
        else {
            if (usedAlgorithm == 2) {
                cX = -1;
                cY = -1;
                if (!Vwalls.empty()) {
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
                else {
                    isFinishedCreatingMaze = true;
                }
            }

        }




        sprite.setScale((NODE_SIZE * nodesToWall) / 512.0, (NODE_SIZE * nodesToWall) / 512.0);

        // 512X=(nodeSize*ratio)
        sprite.setPosition(sf::Vector2f(currentY * NODE_SIZE + wallWidth, currentX * NODE_SIZE + wallWidth));








        drawNodes(window, nodes, cX, cY);
        if (isFinishedCreatingMaze)
            window.draw(sprite);



        window.display();

    }

    return 0;
}
