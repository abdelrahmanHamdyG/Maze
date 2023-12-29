#pragma once
#include<vector>
#include<queue>
#include <SFML/Graphics.hpp>
#include<string>
#include<iostream>
#include<stack>


using namespace std;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int NODE_SIZE = 30;
const float nodesToWall = 0.8;
class Node
{	

public:
	vector<bool>walls;
	int row;
	int column;
	bool visited;
	
	sf::Color color;

	void drawNode(sf::RenderWindow& window);
	
	Node(int row,int column);	
	Node();

	
	
};

