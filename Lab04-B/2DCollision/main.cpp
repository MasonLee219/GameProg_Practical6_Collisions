#include <iostream>
#include <SFML/Graphics.hpp>
#define TINYC2_IMPL
#include <tinyc2.h>
#include <AnimatedSprite.h>
#include <GameObject.h>
#include <Player.h>
#include <NPC.h>
#include <Input.h>
#include <Debug.h>

using namespace std;

//declaring the player shape state enum
enum ShapeState {AABB, CIRCLE, RAY };

int main()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

	//Making an instance of the player shape state enum
	ShapeState playerState;
	playerState = AABB;
	
	// Load a NPC's sprites to display
	sf::Texture npc_texture;
	if (!npc_texture.loadFromFile("assets\\grid.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}

	// Load a mouse texture to display
	sf::Texture player_texture;
	if (!player_texture.loadFromFile("assets\\player.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}

	

	// Setup NPC's Default Animated Sprite
	AnimatedSprite npc_animated_sprite(npc_texture);
	npc_animated_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(428, 3, 84, 84));

	// Setup Players Default Animated Sprite
	AnimatedSprite player_animated_sprite(player_texture);
	player_animated_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(428, 3, 84, 84));

	// Setup the NPC
	GameObject& npc = NPC(npc_animated_sprite);

	// Setup the Player
	GameObject& player = Player(player_animated_sprite);

	//Setting Collision Bounding Rectangle
	sf::RectangleShape playerCollider;
	playerCollider.setOutlineColor(sf::Color(0, 255, 0));
	playerCollider.setSize(sf::Vector2f(84, 84));
	playerCollider.setOutlineThickness(3.0f);

	sf::RectangleShape npcCollider;
	npcCollider.setOutlineColor(sf::Color(0, 255, 0));
	npcCollider.setSize(sf::Vector2f(84, 84));
	npcCollider.setOutlineThickness(3.0f);

	//setting up npc capsule
	sf::RectangleShape npc_capsuleRect;
	npc_capsuleRect.setSize(sf::Vector2f(84, 84));
	npc_capsuleRect.setPosition(sf::Vector2f(160, 300));

	sf::CircleShape npc_capsuleCircleLeft;
	npc_capsuleCircleLeft.setRadius(42);
	npc_capsuleCircleLeft.setPosition(sf::Vector2f(119, 300));

	sf::CircleShape npc_capsuleCircleRight;
	npc_capsuleCircleRight.setRadius(42);
	npc_capsuleCircleRight.setPosition(sf::Vector2f(191, 300));

	//setting Polygons
	sf::VertexArray npc_polygon(sf::Triangles, 3);
	npc_polygon[0] = sf::Vector2f(200.0, 200.0);
	npc_polygon[1] = sf::Vector2f(300.0, 200.0);
	npc_polygon[2] = sf::Vector2f(250.0, 100.0);

	//setting up rays
	sf::Vector2f player_ray_start (300, 300);
	sf::Vector2f player_ray_end(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
	sf::Vector2f player_ray_dv = player_ray_end - player_ray_start;
	float player_ray_magnitude = sqrt(player_ray_dv.x * player_ray_dv.x + player_ray_dv.y * player_ray_dv.y);
	sf::Vector2f player_ray_unitVector = player_ray_dv / player_ray_magnitude;

	sf::Vector2f npc_ray_start(500, 500);
	sf::Vector2f npc_ray_end(700, 500);
	sf::Vector2f npc_ray_dv = npc_ray_end - npc_ray_start;
	float npc_ray_magnitude = sqrt(npc_ray_dv.x * npc_ray_dv.x + npc_ray_dv.y * npc_ray_dv.y);
	sf::Vector2f npc_ray_unitVector = npc_ray_dv / npc_ray_magnitude;

	sf::Vertex npc_ray[] =
	{
		sf::Vertex(npc_ray_start),
		sf::Vertex(npc_ray_end)
	};

	sf::Vertex player_ray[] =
	{
		sf::Vertex(player_ray_start),
		sf::Vertex(player_ray_end)
	};

	//setting up circles
	sf::CircleShape npc_circle;
	npc_circle.setRadius(50);
	npc_circle.setPosition(sf::Vector2f(500,300));

	sf::CircleShape player_circle;
	player_circle.setRadius(50);
	player_circle.setPosition(sf::Vector2f(200, 300));

//Setup Player shapes
	//setting up player aabb
	c2AABB aabb_player;
	aabb_player.min = c2V(player.getAnimatedSprite().getPosition().x, player.getAnimatedSprite().getPosition().y);
	aabb_player.max = c2V(player.getAnimatedSprite().getGlobalBounds().width / 6, player.getAnimatedSprite().getGlobalBounds().width / 6);

	//setting up player ray
	c2Ray ray_player;
	ray_player.p = { player_ray_start.x, player_ray_start.y };
	ray_player.d = { player_ray_unitVector.x,player_ray_unitVector.y };
	ray_player.t = player_ray_magnitude;

	//setting up playercircle
	c2Circle circle_player;
	circle_player.p = c2V(player_circle.getPosition().x + player_circle.getRadius(), player_circle.getPosition().y + player_circle.getRadius());
	circle_player.r = player_circle.getRadius();
	
//setting up NPC shapes
	//setting up npc AABB
	c2AABB aabb_npc;
	aabb_npc.min = c2V(npc.getAnimatedSprite().getPosition().x, npc.getAnimatedSprite().getPosition().y);
	aabb_npc.max = c2V(
		npc.getAnimatedSprite().getPosition().x +
		npc.getAnimatedSprite().getGlobalBounds().width,
		npc.getAnimatedSprite().getPosition().y +
		npc.getAnimatedSprite().getGlobalBounds().height);

	//setting up npc capsule
	c2Capsule capsule_npc;
	capsule_npc.a = c2V(npc_capsuleRect.getPosition().x, npc_capsuleRect.getPosition().y + npc_capsuleCircleLeft.getRadius());
	capsule_npc.b = c2V(npc_capsuleRect.getPosition().x + 84, npc_capsuleRect.getPosition().y + npc_capsuleCircleRight.getRadius());
	capsule_npc.r = 43;

	//setting up npc poly
	c2Poly poly_npc;
	poly_npc.count = 3;
	poly_npc.verts[0] = c2V(200.0, 200.0);
	poly_npc.verts[1] = c2V(300.0, 200.0);
	poly_npc.verts[2] = c2V(250.0, 100.0);

	//setting up npc ray
	c2Ray ray_npc;
	ray_npc.p = { npc_ray_start.x,npc_ray_start.y };
	ray_npc.d = {npc_ray_unitVector.x, npc_ray_unitVector.y};
	ray_npc.t = npc_ray_magnitude;
	
	c2Raycast ray_npc_cast;
	
	//setting up npc circle
	c2Circle circle_npc;
	circle_npc.p = c2V(npc_circle.getPosition().x, npc_circle.getPosition().y);
	circle_npc.r = npc_circle.getRadius();

	// Initialize Input
	Input input;

	//Collisions checks for player aabb
	int aabb_to_aabb = 0;
	int aabb_to_capsule = 0;
	int aabb_to_poly = 0;

	//Collision checks for player circle
	int circle_to_aabb = 0;
	int circle_to_capsule = 0;
	int circle_to_poly = 0;
	int circle_to_ray = 0;
	int circle_to_circle = 0;
	
	//Collision checks for player ray
	int ray_to_capsule = 0;
	int ray_to_poly = 0;
	int ray_to_circle = 0;

	// Direction of movement of NPC
	sf::Vector2f direction(0.1f, 0.2f);
	
	// Start the game loop
	while (window.isOpen())
	{
		// Move Sprite Follow Mouse
		player.getAnimatedSprite().setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		playerCollider.setPosition(player.getAnimatedSprite().getPosition());
		npcCollider.setPosition(npc.getAnimatedSprite().getPosition());
		player_circle.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		circle_player.p = c2V(player_circle.getPosition().x, player_circle.getPosition().y);

		player_ray_end = sf::Vector2f(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		player_ray[1].position = player_ray_end;
		

		sf::Vector2f player_ray_dv = player_ray_end - player_ray_start;
		float player_ray_magnitude = sqrt(player_ray_dv.x * player_ray_dv.x + player_ray_dv.y * player_ray_dv.y);
		sf::Vector2f player_ray_unitVector = player_ray_dv / player_ray_magnitude;

		ray_player.p = c2V(player_ray_start.x, player_ray_start.y);
		ray_player.d = { player_ray_unitVector.x,player_ray_unitVector.y };
		ray_player.t = player_ray_magnitude;
	
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			cout << "Player AABB Drawing" << endl;
			playerState = AABB;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			cout << "Player Circle Drawing" << endl;
			playerState = CIRCLE;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		{
			cout << "Player Ray Drawing" << endl;
			playerState = RAY;
		}

		// Move The NPC
		sf::Vector2f move_to(npc.getAnimatedSprite().getPosition().x + direction.x, npc.getAnimatedSprite().getPosition().y + direction.y);

		if (move_to.x < 0) {
			direction.x *= -1;
			move_to.x = 0 + npc.getAnimatedSprite().getGlobalBounds().width;
		}
		else if (move_to.x + npc.getAnimatedSprite().getGlobalBounds().width >= 800) { 
			direction.x *= -1;
			move_to.x = 800 - npc.getAnimatedSprite().getGlobalBounds().width;
		}
		else if (move_to.y < 0) { 
			direction.y *= -1;
			move_to.y = 0 + npc.getAnimatedSprite().getGlobalBounds().height;
		}
		else if (move_to.y + npc.getAnimatedSprite().getGlobalBounds().height >= 600) {
			direction.y *= -1;
			move_to.y = 600 - npc.getAnimatedSprite().getGlobalBounds().height;
		}
		
		npc.getAnimatedSprite().setPosition(move_to);

		// Update NPC AABB set x and y
		aabb_npc.min = c2V(
			npc.getAnimatedSprite().getPosition().x, 
			npc.getAnimatedSprite().getPosition().y
		);

		aabb_npc.max = c2V(
			npc.getAnimatedSprite().getPosition().x +
			npc.getAnimatedSprite().getGlobalBounds().width,
			npc.getAnimatedSprite().getPosition().y +
			npc.getAnimatedSprite().getGlobalBounds().height
		);

		// Update Player AABB
		aabb_player.min = c2V(
			player.getAnimatedSprite().getPosition().x, 
			player.getAnimatedSprite().getPosition().y
		);
		aabb_player.max = c2V(
			player.getAnimatedSprite().getPosition().x +
			player.getAnimatedSprite().getGlobalBounds().width, 
			player.getAnimatedSprite().getPosition().y + 
			player.getAnimatedSprite().getGlobalBounds().height
		);

		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				// Close window : exit
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					input.setCurrent(Input::Action::LEFT);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					input.setCurrent(Input::Action::RIGHT);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					input.setCurrent(Input::Action::UP);
				}
				break;
			default:
				input.setCurrent(Input::Action::IDLE);
				break;
			}

							
												
		}

		// Handle input to Player
		player.handleInput(input);

		// Update the Player
		player.update();

		// Update the Player
		npc.update();

// Check for collisions
	//AABB Collisions
		if (playerState == AABB)
		{
			//AABB to AABB
			aabb_to_aabb = c2AABBtoAABB(aabb_player, aabb_npc);
			std::cout << ((aabb_to_aabb != 0) ? ("AABB to AABB Collision") : "") << endl;
			if (aabb_to_aabb) {
				playerCollider.setOutlineThickness(3);
				playerCollider.setOutlineColor(sf::Color(255, 0, 0));
				npcCollider.setOutlineColor(sf::Color(255, 0, 0));
			}
			else {
				playerCollider.setOutlineColor(sf::Color(0, 255, 0));
				npcCollider.setOutlineColor(sf::Color(0, 255, 0));
			}
			//Player AABB to NPC_Capsule
			aabb_to_capsule = c2AABBtoCapsule(aabb_player, capsule_npc);
			std::cout << ((aabb_to_capsule != 0) ? ("AABB to Capsule Collision") : "") << endl;
			if (aabb_to_capsule)
			{
				playerCollider.setOutlineColor(sf::Color(255, 0, 0));
				npc_capsuleRect.setFillColor(sf::Color(0, 255, 255));
				npc_capsuleCircleLeft.setFillColor(sf::Color(0, 255, 255));
				npc_capsuleCircleRight.setFillColor(sf::Color(0, 255, 255));
			}
			else
			{
				playerCollider.setOutlineColor(sf::Color(0, 255, 0));
				npc_capsuleRect.setFillColor(sf::Color(255, 255, 255));
				npc_capsuleCircleLeft.setFillColor(sf::Color(255, 255, 255));
				npc_capsuleCircleRight.setFillColor(sf::Color(255, 255, 255));
			}

			aabb_to_poly = c2AABBtoPoly(aabb_player, &poly_npc, NULL);
			std::cout << ((aabb_to_poly != 0) ? ("AABB to Poly Collision") : "") << endl;
			if (aabb_to_poly)
			{
				playerCollider.setOutlineColor(sf::Color(255, 0, 0));

				npc_polygon[0].color = sf::Color(255, 0, 255);
				npc_polygon[1].color = sf::Color(255, 0, 255);
				npc_polygon[2].color = sf::Color(255, 0, 255);
			}
			else
			{
				npc_polygon[0].color = sf::Color(255, 255, 255);
				npc_polygon[1].color = sf::Color(255, 255, 255);
				npc_polygon[2].color = sf::Color(255, 255, 255);
			}

			int hit = c2RaytoAABB(ray_npc, aabb_player, &ray_npc_cast);
			std::cout << ((hit != 0) ? ("AABB to Ray Collision") : "") << endl;
			if (hit)
			{
				playerCollider.setOutlineColor(sf::Color(255, 0, 0));
				npc_ray->color = sf::Color::Blue;
			}
			else
			{
				npc_ray->color = sf::Color::White;
			}
		}
	//Circle collisions
		if (playerState == CIRCLE)
		{
			circle_to_aabb = c2CircletoAABB(circle_player, aabb_npc);
			std::cout << ((circle_to_aabb != 0) ? ("Circle to AABB Collision") : "") << endl;
			if (circle_to_aabb) 
			{				
				player_circle.setFillColor(sf::Color(255, 0, 0));
				npcCollider.setOutlineColor(sf::Color(255, 0, 0));
			}
			else {
				player_circle.setFillColor(sf::Color(255, 255, 255));
				npcCollider.setOutlineColor(sf::Color(255, 0, 0));
			}

			circle_to_capsule = c2CircletoCapsule(circle_player, capsule_npc);
			std::cout << ((circle_to_capsule != 0) ? ("Circle to Capsule Collision") : "") << endl;
			if (circle_to_capsule)
			{
				player_circle.setFillColor(sf::Color(255, 0, 0));
				npc_capsuleRect.setFillColor(sf::Color(0, 255, 255));
				npc_capsuleCircleLeft.setFillColor(sf::Color(0, 255, 255));
				npc_capsuleCircleRight.setFillColor(sf::Color(0, 255, 255));
				
			}
			else {
				player_circle.setFillColor(sf::Color(255, 255, 255));
				npc_capsuleRect.setFillColor(sf::Color(255, 255, 255));
				npc_capsuleCircleLeft.setFillColor(sf::Color(255, 255, 255));
				npc_capsuleCircleRight.setFillColor(sf::Color(255, 255, 255));
			}

			circle_to_poly = c2CircletoPoly(circle_player, &poly_npc, NULL);
			std::cout << ((circle_to_poly != 0) ? ("Circle to Capsule Collision") : "") << endl;
			if (circle_to_poly)
			{
				player_circle.setFillColor(sf::Color(255, 0, 0));
				npc_polygon[0].color = sf::Color(255, 0, 255);
				npc_polygon[1].color = sf::Color(255, 0, 255);
				npc_polygon[2].color = sf::Color(255, 0, 255);

			}
			else {
				player_circle.setFillColor(sf::Color(255, 255, 255));
				npc_polygon[0].color = sf::Color(255, 255, 255);
				npc_polygon[1].color = sf::Color(255, 255, 255);
				npc_polygon[2].color = sf::Color(255, 255, 255);
			}


		}

	
		

		
		
		// Clear screen
		window.clear();

		if (playerState == AABB)
		{
			window.draw(playerCollider);
		}
		if (playerState == CIRCLE)
		{
			window.draw(player_circle);
		}
		if (playerState == RAY)
		{
			window.draw(player_ray, 2, sf::Lines);
		}
		////Draw the Players Current Animated Sprite
		//window.draw(player.getAnimatedSprite());
		
		window.draw(npcCollider);
		//drawing NPC capsule
		window.draw(npc_capsuleRect);
		window.draw(npc_capsuleCircleLeft);
		window.draw(npc_capsuleCircleRight);
		//drawing NPC Polygon
		window.draw(npc_polygon);
		//drawing NPC ray
		window.draw(npc_ray, 2, sf::Lines);
		//drawing NPC circle
		window.draw(npc_circle);
		//Draw the NPC's Current Animated Sprite
		window.draw(npc.getAnimatedSprite());

		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
};