#pragma once
#include<vector>
#include<queue>
#include <SFML/Graphics.hpp>
#include<string>
#include<iostream>
#include<stack>


using namespace std;
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

const float nodesToWall = 0.8;
class Node
{

public:
	bool operator<(const Node& other) const;

	vector<bool>walls;
	int row;
	int column;

	int group;
	bool visited;
	bool visited1;
	int parent_i, parent_j;
	double f, g, h;

	sf::Color color;
	sf::Color wallColor;

	void drawNode(sf::RenderWindow& window, int NODE_SIZE);

	Node(int row, int column);
	Node();



};
class wall {
public:
	Node* node1;
	Node* node2;
};