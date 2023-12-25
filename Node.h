#pragma once
#include<vector>
#include<queue>
#include <SFML/Graphics.hpp>
#include<string>
#include<iostream>
#include<stack>


using namespace std;
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 400;
const int NODE_SIZE = 20;
class Node
{	

public:
	vector<bool>walls;
	int row;
	int column;
	bool visited;
	vector<int>unVisited = { 0,1,2,3};
	sf::Color color;

	void drawNode(sf::RenderWindow& window);
	
	Node(int row,int column);	
	Node();

	
	
};

