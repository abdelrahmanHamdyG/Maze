#include "Node.h"

//0 left 
//1 top
//2 right
//3 down
void Node::drawNode(sf::RenderWindow& window) {


	if (visited) {	

		sf::RectangleShape inner;

		float innerSize = nodesToWall * NODE_SIZE;
		float wallWidth = (NODE_SIZE - innerSize) / 2;
			

		inner.setFillColor(color);
		inner.setPosition(sf::Vector2f(column * NODE_SIZE + wallWidth+(SCREEN_WIDTH/3.0), row * NODE_SIZE + wallWidth));


		inner.setSize(sf::Vector2f(innerSize, innerSize));

		window.draw(inner);

		if (!walls[0]) {

			sf::RectangleShape wall;
			wall.setPosition(sf::Vector2f(column * NODE_SIZE+ (SCREEN_WIDTH / 3.0), row * NODE_SIZE + wallWidth));
			wall.setSize(sf::Vector2f(wallWidth, innerSize));
			wall.setFillColor(wallColor);
			window.draw(wall);
		}

		if (!walls[1]) {

			sf::RectangleShape wall;
			wall.setPosition(sf::Vector2f(column * NODE_SIZE + wallWidth+ (SCREEN_WIDTH / 3.0), row * NODE_SIZE));
			wall.setSize(sf::Vector2f(innerSize, wallWidth));
			wall.setFillColor(wallColor);
			window.draw(wall);
		}

		if (!walls[2]) {

			sf::RectangleShape wall;
			wall.setPosition(sf::Vector2f(column * NODE_SIZE + innerSize + wallWidth+ (SCREEN_WIDTH / 3.0), row * NODE_SIZE + wallWidth));
			wall.setSize(sf::Vector2f(wallWidth, innerSize));
			wall.setFillColor(wallColor);
			window.draw(wall);
		}

		if (!walls[3]) {

			sf::RectangleShape wall;
			wall.setPosition(sf::Vector2f(column * NODE_SIZE + wallWidth+ (SCREEN_WIDTH / 3.0), row * NODE_SIZE + innerSize + wallWidth));
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

	color = sf::Color::White;
	wallColor = sf::Color::White;

}

Node::Node()
{

	
	row = 0;
	column = 0;
	walls = vector<bool>(4, true);
	visited = false;

	color = sf::Color::White;
	wallColor = sf::Color::White;
}
