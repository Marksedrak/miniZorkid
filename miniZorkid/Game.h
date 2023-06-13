#include "World.h"
#include "Player.h"
#include "Item.h"
#include "Direction.h"
#include<iostream>
#pragma once

class Game
{
	private:
		World gameWorld;
		Player gamePlayer;

	public:
		string playerInput;

        // Game Constructor
        Game(World world = World(), Player player = Player(), string input = "") : gameWorld(world), gamePlayer(player), playerInput(input) {}
		
       /*
        *
        *   Function Name:  takeUserInput
        *   Parameters:     None
        *   Description:    reads user's input from console
        *                   and transforms all text to lower-case
        *                   to allow for case-insensitive inputs.
        *
        */
        void takeUserInput()
        {
            string command;
            cout << "\t>";
            getline(cin, command);

            if (command == "")
                playerInput = "";
            else
            {
                int i;
                for (i = 0; i < command.length(); i++)
                {
                    command[i] = ::tolower(command[i]);
                }
                playerInput = command;
            }
        }

};

