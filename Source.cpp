#include <SFML/Graphics.hpp>
#include <set>
#include<random>
#include"Node.h";
#include<map>
#include<ctime>


int NodesPerColumn;
int NodesPerRow;

int options[3];

typedef pair<int, int> Pair;
typedef pair<double, pair<int, int> > pPair;
vector<vector<bool>>closedList;
queue<Node*>q;

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


pair<int, int> getPos(float posx, float posy) {
    int i = -1, j = -1;
    if (posx > 620 && posx < 1027) {
        i = 0;
    }
    if (posx > 1114 && posx < 1440) {
        i = 1;
    }
    if (posx > 1569 && posx < 1755) {
        i = 2;
    }
    if (posy > 290 && posy < 362) {
        j = 0;
    }
    if (posy > 524 && posy < 610) {
        j = 1;
    }
    if (posy > 760 && posy < 850) {
        j = 2;
    }

    return { j,i };
}

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

    sf::sleep(sf::milliseconds(15));



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
        
    }
    Vwalls.erase(Vwalls.begin() + random);
}








void drawNodes(sf::RenderWindow& window, vector<vector<Node>>v, int cX, int cY, int NODE_SIZE) {

    if (cX != -1 && cY != -1) {
        v[cX][cY].color = sf::Color::Red;

    }



    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[0].size(); j++) {

            v[i][j].drawNode(window, NODE_SIZE);
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

bool solveWithBfs(vector<vector<Node>>& nodes) {

    Node* child;
    bool found = false;



    Node* u = q.front();
    q.pop();

    u->color = sf::Color::Green;
    u->wallColor = sf::Color::Green;
    if (u->column == NodesPerRow - 1 && u->row == NodesPerColumn - 1)
    {
        found = true;
        for (int i = 0; i < NodesPerColumn; i++) {
            for (int j = 0; j < NodesPerRow; j++) {

                nodes[i][j].color = sf::Color::White;
                nodes[i][j].wallColor = sf::Color::White;
                nodes[i][j].visited1 = false;
            }

        }
        tracePath(nodes, { NodesPerColumn - 1,NodesPerRow - 1 });
        return true;
    }

    for (int i = 0; i < 4; i++)
    {
        if (possible(u->row + moveX[i], u->column + moveY[i]) && isUnBlocked(nodes, u->row + moveX[i], u->column + moveY[i], i)) {
            child = &nodes[u->row + moveX[i]][u->column + moveY[i]];
            if (child->visited1)continue;
            q.push(child);
            child->visited1 = true;
            child->parent_i = u->row;
            child->parent_j = u->column;



        }

    }




    if (found) {
        tracePath(nodes, { NodesPerColumn - 1,NodesPerRow - 1 });
        return true;
    }

    sf::sleep(sf::milliseconds(15));
    return false;
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

    

    bool found = false;

    //0 = up
    //1 = left
    //2 = bottom 
    //3 = right
    Node* u = st.top();
    Node* child;
    u->color = sf::Color::Green;
    u->wallColor = sf::Color::Green;

    st.pop();
    if (u->column == NodesPerRow - 1 && u->row == NodesPerColumn - 1) {
        found = true;
        for (int i = 0; i < NodesPerColumn; i++) {
            for (int j = 0; j < NodesPerRow; j++) {

                nodes[i][j].color = sf::Color::White;
                nodes[i][j].wallColor = sf::Color::White;
                nodes[i][j].visited1 = false;
            }

        }
        tracePath(nodes, { NodesPerColumn - 1,NodesPerRow - 1 });
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

    sf::sleep(sf::milliseconds(15));
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
void clearStack(stack<Node*>& s) {

    while (!s.empty()) {
        s.pop();
    }

}
void clearQueue(queue<Node*>& q) {

    while (!q.empty()) {
        q.pop();
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
    case 3: {
        Vwalls.clear();
        int randRow = rand() % NodesPerColumn;
        int randColumn = rand() % NodesPerRow;
        nodes[randRow][randColumn].visited = true;
        addWall(Vwalls, randRow, randColumn, nodes, 2);
        break;
    }
    case 2: {
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

void initializeSolvingAlgorithms(int usedSolvingAlgorithm, vector<vector<Node>>& nodes) {


    switch (usedSolvingAlgorithm) {
    case 1: {


        nodes[0][0].f = 0.0;
        nodes[0][0].g = 0.0;
        nodes[0][0].h = 0.0;
        nodes[0][0].parent_i = 0;
        nodes[0][0].parent_j = 0;
        openList.clear();
        openList.insert(make_pair(0.0, make_pair(0, 0)));
        closedList = vector<vector<bool>>(NodesPerColumn, vector<bool>(NodesPerRow, false));

        break;
    }

    case 2: {
        //bfs

        clearQueue(q);
        nodes[0][0].f = 0.0;
        nodes[0][0].g = 0.0;
        nodes[0][0].h = 0.0;
        nodes[0][0].parent_i = 0;
        nodes[0][0].parent_j = 0;
        nodes[0][0].visited1 = true;
        q.push(&nodes[0][0]);
        path.clear();
        path1.clear();
        break;


    }
    case 3: {
        //dfs
        // temporary
        clearStack(st);
        nodes[0][0].f = 0.0;
        nodes[0][0].g = 0.0;
        nodes[0][0].h = 0.0;
        nodes[0][0].parent_i = 0;
        nodes[0][0].parent_j = 0;
        nodes[0][0].visited1 = true;
        st.push(&nodes[0][0]);
        path.clear();
        path1.clear();
        break;
    }



    default:
        break;

    }



}



int main()
{



    int NODE_SIZE = 60;
    int windowIndex = 0;


    options[0] = 0;
    options[1] = 1;
    options[2] = 1;

    if (options[1] == 0) {
        NODE_SIZE = 120;
    }
    if (options[1] == 1) {
        NODE_SIZE = 60;
    }
    if (options[1] == 2) {
        NODE_SIZE = 30;
    }


    sf::Texture backgroundTexture;
    sf::Image imageBackground;
    imageBackground.loadFromFile("startingScreen6.png");
    backgroundTexture.loadFromImage(imageBackground);
    sf::Sprite background(backgroundTexture);

    sf::Texture regenerateTexture;
    sf::Image imageRegenerate;
    imageRegenerate.loadFromFile("regenerate.png");
    regenerateTexture.loadFromImage(imageRegenerate);
    sf::Sprite regenerate(regenerateTexture);
    regenerate.setPosition(sf::Vector2f(10, 9));


    sf::Texture settingsTexture;
    sf::Image imageSettings;
    imageSettings.loadFromFile("settings.png");
    settingsTexture.loadFromImage(imageSettings);
    sf::Sprite settings(settingsTexture);
    settings.setPosition(sf::Vector2f(1800, 9));

    sf::Texture giveupTexture;
    sf::Image imageGiveup;
    imageGiveup.loadFromFile("giveup2.png");
    giveupTexture.loadFromImage(imageGiveup);
    sf::Sprite giveup(giveupTexture);
    giveup.setPosition(sf::Vector2f(915, 9));


    sf::Texture backgroundSettingTexture;
    sf::Image imageSettingsBackground;
    imageSettingsBackground.loadFromFile("settingBackground.png");
    backgroundSettingTexture.loadFromImage(imageSettingsBackground);
    sf::Sprite settingBackground(backgroundSettingTexture);

    sf::Texture htTexture;
    sf::Image imageht;
    imageht.loadFromFile("ht.png");
    htTexture.loadFromImage(imageht);


    sf::Sprite ht(htTexture);
    sf::Sprite htArr[3] = { ht,ht,ht };

    sf::Texture YWTexture;
    sf::Image imageYW;
    imageYW.loadFromFile("YouWon.png");
    YWTexture.loadFromImage(imageYW);
    sf::Sprite YW(YWTexture);







    float innerSize = nodesToWall * NODE_SIZE;
    float wallWidth = (NODE_SIZE - innerSize) / 2;

    NodesPerColumn = (SCREEN_HEIGHT - 108) / NODE_SIZE;
    NodesPerRow = SCREEN_WIDTH / NODE_SIZE;

    int usedAlgorithm = options[0] + 1;
    int usedSolvingAlgorithm = options[2] + 1;
    vector<vector<Node>>nodes(NodesPerColumn, vector<Node>(NodesPerRow));



    initializeAlgorithms(usedAlgorithm, nodes);
    initializeSolvingAlgorithms(usedSolvingAlgorithm, nodes);

    bool giveUpClicked = false;
    bool isFinishedGenerating = false;



    srand(static_cast<unsigned>(time(0)));





    Pair src = make_pair(0, 0);
    Pair dest = make_pair(NodesPerColumn - 1, NodesPerRow - 1);








    sf::Image redCircle;
    redCircle.loadFromFile("red_circle.png");



    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Maze App", sf::Style::Fullscreen);


    sf::Sprite sprite;
    sf::Texture texture;
    texture.loadFromImage(redCircle);
    sprite.setTexture(texture);
    int currentX = 0, currentY = 0;


    bool pathIsFounded = false;
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }


            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Check if the mouse click is within the bounds of the clickable area

                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    


                    if (windowIndex == 0) {
                        if (mousePosition.x > 800.3 && mousePosition.x < 1119.7 && mousePosition.y>771.4 && mousePosition.y < 907.7) {
                            windowIndex = 1;

                        }
                    }
                    else {
                        if (windowIndex == 1) {
                            // regenerate
                            if (mousePosition.x > 10 && mousePosition.x < 100 && mousePosition.y>9 && mousePosition.y < 99) {
                                currentX = 0;
                                currentY = 0;

                                giveUpClicked = false;
                                isFinishedGenerating = false;
                                initializeAlgorithms(options[0] + 1, nodes);
                                initializeSolvingAlgorithms(options[2] + 1, nodes);


                                pathIsFounded = false;

                            }

                            // settings
                            if (mousePosition.x > 1800 && mousePosition.x < 1890 && mousePosition.y>9 && mousePosition.y < 99) {
                                windowIndex = 2;
                                currentX = 0;
                                currentY = 0;

                                giveUpClicked = false;
                                isFinishedGenerating = false;
                                pathIsFounded = false;


                            }
                            // giveup
                            if (mousePosition.x > 915 && mousePosition.x < 1005 && mousePosition.y>9 && mousePosition.y < 99) {


                                giveUpClicked = true;
                            }
                        }
                        else {
                            if (windowIndex == 2) {


                                //confirm button 
                                if (mousePosition.x > 1450.9 && mousePosition.x < 1821 && mousePosition.y>928.6 && mousePosition.y < 1016) {

                                    int optionssarr[3] = { 120,60,30 };
                                    NODE_SIZE = optionssarr[options[1]];
                                    innerSize = nodesToWall * NODE_SIZE;
                                    wallWidth = (NODE_SIZE - innerSize) / 2;

                                    NodesPerColumn = (SCREEN_HEIGHT - 108) / NODE_SIZE;
                                    NodesPerRow = SCREEN_WIDTH / NODE_SIZE;

                                    nodes = vector<vector<Node>>(NodesPerColumn, vector<Node>(NodesPerRow));


                                    windowIndex = 1;
                                    initializeAlgorithms(options[0] + 1, nodes);
                                    initializeSolvingAlgorithms(options[2] + 1, nodes);

                                    
                                }
                                else {


                                    pair<int, int>pos = getPos(mousePosition.x, mousePosition.y);
                                    if (pos.first != -1 && pos.second != -1) {

                                        options[pos.first] = pos.second;

                                    }
                                }
                            }
                            else {
                                if (mousePosition.x > 650 && mousePosition.x < 1310 && mousePosition.y>911 && mousePosition.y < 1031) {
                                    windowIndex = 0;
                                }
                            }
                        }
                    }

                }
            }





            if (isFinishedGenerating && !giveUpClicked) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {

                    if (!nodes[currentX][currentY].walls[1] && possible(currentX - 1, currentY)) {

                        currentX -= 1;
                    }
                    
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    if (!nodes[currentX][currentY].walls[3] && possible(currentX + 1, currentY)) {

                        currentX += 1;
                    }

                    
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    if (!nodes[currentX][currentY].walls[0] && possible(currentX, currentY - 1)) {

                        currentY -= 1;
                    }
                    
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    if (!nodes[currentX][currentY].walls[2] && possible(currentX, currentY + 1)) {

                        currentY += 1;
                    }
                    
                }

                if (currentX == NodesPerColumn - 1 && currentY == NodesPerRow - 1) {
                    windowIndex = 3;
                    currentX = 0;
                    currentY = 0;
                    giveUpClicked = false;
                    isFinishedGenerating = false;
                    pathIsFounded = false;
                    initializeAlgorithms(options[0] + 1, nodes);
                    initializeSolvingAlgorithms(options[2] + 1, nodes);



                }

            }
        }

        window.clear(sf::Color::Black);
        if (windowIndex == 0) {

            window.draw(background);


        }
        else {
            if (windowIndex == 1) {
                window.clear(sf::Color::Black);

                if (options[0] + 1 == 1) {


                    if (s.empty()) {
                        cX = cY = -1;
                        isFinishedGenerating = true;

                    }
                    else {
                        backTracking(s, nodes);
                    }
                }
                else {
                    if (options[0] + 1 == 3) {
                        cX = -1;
                        cY = -1;
                        if (!Vwalls.empty()) {
                            prim(Vwalls, nodes);
                        }
                        else {
                            isFinishedGenerating = true;
                        }
                    }
                    else {

                        if (options[0] + 1 == 2) {
                            // kurskal 
                            cX = -1;
                            cY = -1;
                            if (!Vwalls.empty()) {
                                Kruskal(Vwalls, nodes);
                            }
                            else {
                                isFinishedGenerating = true;
                            }

                        }


                    }

                }





                drawNodes(window, nodes, cX, cY, NODE_SIZE);

                window.draw(regenerate);
                window.draw(settings);
                window.draw(giveup);

                if (isFinishedGenerating && !giveUpClicked) {
                    sprite.setScale((NODE_SIZE * nodesToWall) / 512.0, (NODE_SIZE * nodesToWall) / 512.0);
                    sprite.setPosition(sf::Vector2f(currentY * NODE_SIZE + wallWidth, currentX * NODE_SIZE + wallWidth + 108));

                    window.draw(sprite);


                }

                if (giveUpClicked) {

                    if (!pathIsFounded) {



                        if (options[2] + 1 == 3) {
                            if (!st.empty()) {
                                bool found = solveWithDfs(nodes);
                                if (found) {
                                    clearStack(st);
                                    pathIsFounded = true;
                                }
                            }
                            else {

                                pathIsFounded = true;
                            }

                        }


                        else {

                            if (options[2] + 1 == 1) {  
                                if (!openList.empty()) {

                                    bool found = aStarSearch(nodes, src, { NodesPerColumn - 1,NodesPerRow - 1 }, closedList);
                                    if (found) {
                                        clearStack(s);
                                        pathIsFounded = true;
                                    }

                                }
                                else {
                                    pathIsFounded = true;
                                }



                            }
                            else {
                                if (options[2] + 1 == 2) {

                                    if (!q.empty()) {
                                        bool found = solveWithBfs(nodes);
                                        if (found) {
                                            clearQueue(q);
                                            pathIsFounded = true;
                                        }
                                    }
                                    else {

                                        pathIsFounded = true;
                                    }



                                }


                            }
                        }
                    }
                }







            }
            else {
                if (windowIndex == 2) {

                    window.draw(settingBackground);

                    int height[3] = { 292,526,762 };

                    for (int i = 0; i < 3; i++) {

                        int x = 620;

                        if (options[i] == 1) {

                            x = 1090;
                        }
                        if (options[i] == 2) {

                            x = 1480;
                        }

                        htArr[i].setPosition(sf::Vector2f(x, height[i]));
                        window.draw(htArr[i]);
                    }


                }
                else {
                    window.draw(YW);
                }



            }
        }

        window.display();

    }

    return 0;
}