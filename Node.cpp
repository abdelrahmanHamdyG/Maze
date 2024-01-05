#include "Node.h"


bool Node::operator<(const Node& other)const
{
	return row < other.row || (row == other.row && column < other.column);
}

//0 left 
//1 top
//2 right
//3 down
void Node::drawNode(sf::RenderWindow& window, int NODE_SIZE) {


	if (visited) {

		sf::RectangleShape inner;

		float innerSize = nodesToWall * NODE_SIZE;
		float wallWidth = (NODE_SIZE - innerSize) / 2;


		inner.setFillColor(color);
		inner.setPosition(sf::Vector2f(column * NODE_SIZE + wallWidth, row * NODE_SIZE + wallWidth + 108));


		inner.setSize(sf::Vector2f(innerSize, innerSize));

		window.draw(inner);

		if (!walls[0]) {

			sf::RectangleShape wall;
			wall.setPosition(sf::Vector2f(column * NODE_SIZE, row * NODE_SIZE + wallWidth + 108));
			wall.setSize(sf::Vector2f(wallWidth, innerSize));
			wall.setFillColor(wallColor);
			window.draw(wall);
		}

		if (!walls[1]) {

			sf::RectangleShape wall;
			wall.setPosition(sf::Vector2f(column * NODE_SIZE + wallWidth, row * NODE_SIZE + 108));
			wall.setSize(sf::Vector2f(innerSize, wallWidth));
			wall.setFillColor(wallColor);
			window.draw(wall);
		}

		if (!walls[2]) {

			sf::RectangleShape wall;
			wall.setPosition(sf::Vector2f(column * NODE_SIZE + innerSize + wallWidth, row * NODE_SIZE + wallWidth + 108));
			wall.setSize(sf::Vector2f(wallWidth, innerSize));
			wall.setFillColor(wallColor);
			window.draw(wall);
		}

		if (!walls[3]) {

			sf::RectangleShape wall;
			wall.setPosition(sf::Vector2f(column * NODE_SIZE + wallWidth, row * NODE_SIZE + innerSize + wallWidth + 108));
			wall.setSize(sf::Vector2f(innerSize, wallWidth));
			wall.setFillColor(wallColor);
			window.draw(wall);
		}

	}

}

Node::Node(int i, int j) {

	this->column = j;
	this->row = i;
	walls = vector<bool>(4, true);
	visited = false;
	visited1 = false;
	group = 0;
	color = sf::Color::White;
	wallColor = sf::Color::White;

}

Node::Node()
{

	group = 0;
	row = 0;
	column = 0;
	walls = vector<bool>(4, true);
	visited = false;
	visited1 = false;
	color = sf::Color::White;
	wallColor = sf::Color::White;
}