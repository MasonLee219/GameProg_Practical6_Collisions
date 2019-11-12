#include <iostream>
#include <math.h>
#include <time.h>

using namespace std;

class Point {
public:
	int x;
	int y;

	void print() {
		std::cout << "x:" << x << " y:" << y << std::endl;
	}
};

class Circle {
public:
	Point p;
	int r;

	void print() {
		std::cout << "x:" << p.x << " y:" << p.y << " r:" << r << std::endl;
	}
};

class Box {
public:
	Point p;
	int w;
	int h;
	void print()
	{
		std::cout << "x:" << p.x << " y:" << p.y << " w:" << w << " h:" << h << endl;
	}

};
int calculate_hypotenuse(Point a, Point b) {
	int result = 0;

	// Find the distance (Length) of X and Y  

	int x_Distance = b.x - a.x;
	int y_Distance = b.y - a.y;

	// Now that we have the distance (Length)
	// of x and y, use Pythagoras theorem 
	// to get the distance (length of hypotenuse) 
	// between Point a and Point b 
	result = sqrt(pow(x_Distance, 2) + pow(y_Distance, 2));

	return result;
}


int main() {

	srand(time(NULL));

	// Point Collision
	Point player_point;
	player_point.x = rand() % 10 + 1;
	player_point.y = rand() % 10 + 1;

	Point npc_point;
	npc_point.x = rand() % 10 + 1;
	npc_point.y = rand() % 10 + 1;

	// Circle Collision
	Circle player_circle;
	player_circle.p.x = rand() % 10 + 1;
	player_circle.p.y = rand() % 10 + 1;
	player_circle.r = rand() % 2 + 1;
	
	Circle npc_circle;
	npc_circle.p.x = rand() % 10 + 1;
	npc_circle.p.y = rand() % 10 + 1;
	npc_circle.r = rand() % 2 + 1;
	
	//Circle Point Collision
	Circle positivecase_circle;
	Circle negativecase_circle;

	positivecase_circle.p.x = 8;
	positivecase_circle.p.y = 8;
	positivecase_circle.r = 8;
	
	negativecase_circle.p.x = 5;
	negativecase_circle.p.y = 5;
	negativecase_circle.r = 1;

	Point positivecase_point;
	Point negativecase_point;

	positivecase_point.x = 10;
	positivecase_point.y = 10;
	negativecase_point.x = 0;
	negativecase_point.y = 0;

	//Box to Box collision
	Box positivecase_box1;
	Box positivecase_box2;
	Box negativecase_box1;
	Box negativecase_box2;

	positivecase_box1.p.x = 0;
	positivecase_box1.p.y = 2;
	positivecase_box1.w = 3;
	positivecase_box1.h = 3;

	positivecase_box2.p.x = 1;
	positivecase_box2.p.y = 4;
	positivecase_box2.w = 1;
	positivecase_box2.h = 2;


	negativecase_box1.p.x = 1;
	negativecase_box1.p.y = 4;
	negativecase_box1.w = 1;
	negativecase_box1.h = 2;

	negativecase_box2.p.x = 7;
	negativecase_box2.p.y = 9;
	negativecase_box2.w = 1;
	negativecase_box2.h = 2;

	while (true) {

		// Point Collision check
		if (calculate_hypotenuse(player_point, npc_point) == 0)
		{
			std::cout << "Point Collision" << std::endl;
			player_point.print();
			npc_point.print();
			std::cin.get();
		}
		else
		{
			std::cout << "No Collision" << std::endl;
			player_point.print();
			npc_point.print();
		}

		player_point.x = rand() % 10 + 1;
		player_point.y = rand() % 10 + 1;

		// Circle Collision check
		if (calculate_hypotenuse(player_circle.p, npc_circle.p) < (player_circle.r + npc_circle.r))
		{
			std::cout << "Circle Collision" << std::endl;
			player_circle.print();
			npc_circle.print();
			std::cin.get();
		}
		else
		{
			std::cout << "No Collision" << std::endl;
			player_circle.print();
			npc_circle.print();
		}

		player_circle.p.x = rand() % 10 + 1;
		player_circle.p.y = rand() % 10 + 1;

		//Circle to Point collision check, positive case
		if (calculate_hypotenuse(positivecase_point, positivecase_circle.p) < positivecase_circle.r)
		{
			cout << "Circle to Point collision" << endl;
			positivecase_circle.print();
			positivecase_point.print();
		}
		else
		{
			std::cout << "No Collision" << std::endl;
			positivecase_circle.print();
			positivecase_point.print();
		}

		
		//Circle to Point collision check, negative case
		if (calculate_hypotenuse(negativecase_point, negativecase_circle.p) < negativecase_circle.r)
		{
			cout << "Circle to Point collision" << endl;
			negativecase_circle.print();
			negativecase_point.print();
		}
		else
		{
			std::cout << "No Collision" << std::endl;
			negativecase_circle.print();
			negativecase_point.print();
		}
		
		//Box to box collision, positive case
		if (((positivecase_box1.p.x + positivecase_box1.w) > positivecase_box2.p.x && 
			(positivecase_box2.p.x + positivecase_box2.w) > positivecase_box1.p.x) && 
			(positivecase_box1.p.y + positivecase_box1.h) > positivecase_box2.p.y &&
			(positivecase_box2.p.y + positivecase_box2.h) > positivecase_box1.p.y)
		{
			cout << "Box to box collision" << endl;
			positivecase_box1.print();
			positivecase_box2.print();
		}
		else
		{
			cout << "No Collision" << endl;
			positivecase_box1.print();
			positivecase_box2.print();
		}

		////Box to box collision, negative case
		if (((negativecase_box1.p.x + negativecase_box1.w) > negativecase_box2.p.x &&
			(negativecase_box2.p.x + negativecase_box2.w) > negativecase_box1.p.x) &&
			(negativecase_box1.p.y + negativecase_box1.h) > negativecase_box2.p.y &&
			(negativecase_box2.p.y + negativecase_box2.h) > negativecase_box1.p.y)
		{
			cout << "Box to box collision" << endl;
			negativecase_box1.print();
			negativecase_box2.print();
		}
		else
		{
			cout << "No Collision" << endl;
			negativecase_box1.print();
			negativecase_box2.print();
		}
	}

	return 0;
}