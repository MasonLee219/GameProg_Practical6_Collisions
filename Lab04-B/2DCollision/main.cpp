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

int main()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

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
	GameObject &npc = NPC(npc_animated_sprite);

	// Setup the Player
	GameObject &player = Player(player_animated_sprite);


	//Setting Collision Bounding Rectangle
	sf::RectangleShape playerCollider;
	playerCollider.setOutlineColor(sf::Color(0, 255, 0));
	playerCollider.setSize(sf::Vector2f(84, 84));
	playerCollider.setOutlineThickness(3.0f);

	sf::RectangleShape npcCollider;
	npcCollider.setOutlineColor(sf::Color(0, 255, 0));
	npcCollider.setSize(sf::Vector2f(84, 84));
	npcCollider.setOutlineThickness(3.0f);

	//setting up capsules
	sf::RectangleShape npc_capsuleRect;
	npc_capsuleRect.setSize(sf::Vector2f(84, 84));
	npc_capsuleRect.setPosition(sf::Vector2f(160, 300));

	sf::CircleShape npc_capsuleCircleLeft;
	npc_capsuleCircleLeft.setRadius(42);
	npc_capsuleCircleLeft.setPosition(sf::Vector2f(119, 300));

	sf::CircleShape npc_capsuleCircleRight;
	npc_capsuleCircleRight.setRadius(42);
	npc_capsuleCircleRight.setPosition(sf::Vector2f(191, 300));

	//setting up rays
	sf::Vertex npc_ray_start = { 500,500 };
	sf::Vertex npc_ray_end = sf::Vertex(sf::Vector2f(700, 500));
	sf::Vertex npc_ray[] =
	{
		sf::Vertex(npc_ray_start),
		sf::Vertex(npc_ray_end)
	};

	

	//setting Polygons
	sf::VertexArray npc_polygon(sf::Triangles, 3);
	npc_polygon[0] = sf::Vector2f(200.0, 200.0);
	npc_polygon[1] = sf::Vector2f(300.0, 200.0);
	npc_polygon[2] = sf::Vector2f(250.0, 100.0);

	//Setup NPC AABB
	c2AABB aabb_npc;
	aabb_npc.min = c2V(npc.getAnimatedSprite().getPosition().x, npc.getAnimatedSprite().getPosition().y);
	aabb_npc.max = c2V(
		npc.getAnimatedSprite().getPosition().x +
		npc.getAnimatedSprite().getGlobalBounds().width, 
		npc.getAnimatedSprite().getPosition().y +
		npc.getAnimatedSprite().getGlobalBounds().height);

	//Setup Player AABB
	c2AABB aabb_player;
	aabb_player.min = c2V(player.getAnimatedSprite().getPosition().x, player.getAnimatedSprite().getPosition().y);
	aabb_player.max = c2V(player.getAnimatedSprite().getGlobalBounds().width / 6, player.getAnimatedSprite().getGlobalBounds().width / 6);
	
	//setting up NPC shapes
	c2Capsule capsule_npc;
	capsule_npc.a = c2V(npc_capsuleRect.getPosition().x, npc_capsuleRect.getPosition().y + npc_capsuleCircleLeft.getRadius());
	capsule_npc.b = c2V(npc_capsuleRect.getPosition().x + 84, npc_capsuleRect.getPosition().y + npc_capsuleCircleRight.getRadius());
	capsule_npc.r = 43;
	
	c2Poly poly_npc;
	poly_npc.count = 3;	
	poly_npc.verts[0] = c2V(200.0, 200.0);
	poly_npc.verts[1] = c2V(300.0, 200.0);
	poly_npc.verts[2] = c2V(250.0, 100.0);

	c2Ray ray_npc;
	ray_npc.p = c2V(npc_ray_start);
	ray_npc.d = c2Norm(c2V(1, 0));
	ray_npc.t = 200;
	

	// Initialize Input
	Input input;

	// Collision aabb_to_aabb
	int aabb_to_aabb = 0;
	int aabb_to_capsule = 0;
	int aabb_to_poly = 0;

	// Direction of movement of NPC
	sf::Vector2f direction(0.1f, 0.2f);
	
	// Start the game loop
	while (window.isOpen())
	{
		// Move Sprite Follow Mouse
		player.getAnimatedSprite().setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		playerCollider.setPosition(player.getAnimatedSprite().getPosition());
		npcCollider.setPosition(npc.getAnimatedSprite().getPosition());
		
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
		//AABB to AABB
		aabb_to_aabb = c2AABBtoAABB(aabb_player, aabb_npc);
		std::cout << ((aabb_to_aabb != 0) ? ("AABB to AABB Collision") : "") << endl;
		if (aabb_to_aabb){
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
		std::cout << ((aabb_to_capsule != 0) ? ("AABB to Capsule	Collision") : "") << endl;
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
		std::cout << ((aabb_to_capsule != 0) ? ("AABB to Poly	Collision") : "") << endl;
		if(aabb_to_poly)
		{
			playerCollider.setOutlineColor(sf::Color(255, 0, 0));
			
			npc_polygon[0].color= sf::Color(255, 0, 255);
			npc_polygon[1].color = sf::Color(255, 0, 255);
			npc_polygon[2].color = sf::Color(255, 0, 255);
		}
		else
		{
			npc_polygon[0].color = sf::Color(255, 255, 255);
			npc_polygon[1].color = sf::Color(255, 255, 255);
			npc_polygon[2].color = sf::Color(255, 255, 255);
		}

		
		// Clear screen
		window.clear();

		//Draw the Players Current Animated Sprite
		window.draw(player.getAnimatedSprite());
		window.draw(playerCollider);
		window.draw(npcCollider);
		//drawing NPC capsule
		window.draw(npc_capsuleRect);
		window.draw(npc_capsuleCircleLeft);
		window.draw(npc_capsuleCircleRight);
		//drawing NPC Polygon
		window.draw(npc_polygon);
		//drawing NPC ray
		window.draw(npc_ray, 2, sf::Lines);
		//Draw the NPC's Current Animated Sprite
		window.draw(npc.getAnimatedSprite());

		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
};