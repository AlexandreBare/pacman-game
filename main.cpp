#include "Gameplay.hpp"
#include "constants.hpp"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <iostream>

using namespace sf;
using namespace std;

int main(int, char const**)
{
    // Create the main window
    RenderWindow window(VideoMode(CELL_SIZE * GRID_SIZE[0], CELL_SIZE * GRID_SIZE[1]), "PACoronam");
    window.setFramerateLimit(FPS);
    
    while(window.isOpen()){
        Gameplay gameplay;
        
        // Start the game loop
        while (window.isOpen())
        {
            
            if(Keyboard::isKeyPressed(Keyboard::R))
                break; //Reset
            
            // Process events
            sf::Event event;
            while (window.pollEvent(event))
            {
                // Close window: exit
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }
            
            try{
                // Update game
                gameplay.update();
            
                // Clear screen
                window.clear();
                
                // Draw
                gameplay.drawOnWindow(window);
                
                // Update the window
                window.display();
            }catch(const exception& e){
                cerr << e.what() << endl;
                return 1;
            }
        }
    }

    return EXIT_SUCCESS;
}
