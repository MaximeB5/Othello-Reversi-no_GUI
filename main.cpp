/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/
/* Includes */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "configuration.h"
#include "othellier.h"
#include "player.h"
#include "slot.h"
#include "enum_game.h"

#include <iostream>
#include <string>
#include <tuple>
#include <Windows.h>

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/
/* Utility Functions */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

void Setup_Windows_Terminal(void);
void Clear_Windows_Terminal_Screen(char fill = ' ');
void Display_Result(const E_Game_Result & game_result);
void Test_Game_Logic(void);

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/
/* Main */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

int main()
{
    Setup_Windows_Terminal();

    //Test_Game_Logic(); // FOR TESTING PURPOSE ONLY

    // Create the othellier (the board game) and the players
    std::shared_ptr<Othellier> othellier{ std::make_shared<Othellier>() };
    Player player_1{ E_Pawn_Color::BLACK, othellier }; // X
    Player player_2{ E_Pawn_Color::WHITE, othellier }; // O

    // Result of the game
    E_Game_Result game_result{E_Game_Result::NO_RESULT};

    // Game Loop - The player using the black pawns always begin the game
    while (Slot::Get_Number_of_Occupied_Slots() < MAX_PAWNS || game_result != E_Game_Result::NO_RESULT)
    {
        // --------------------------------------------------------------------------------------------------------------------------------------------

        // Data structures which hold both game command and position to place a pawn for the two players
        std::tuple<bool, E_Game_Command, unsigned int, unsigned int> player_1_result{false, E_Game_Command::NO_COMMAND, 0, 0};
        std::tuple<bool, E_Game_Command, unsigned int, unsigned int> player_2_result{false, E_Game_Command::NO_COMMAND, 0, 0};

        // --------------------------------------------------------------------------------------------------------------------------------------------

        if (player_1.Can_Play())
        {
            // Blocking point in order to have a correct answer from the player. The player plays by default here.
            while (!std::get<bool>(player_1_result)) { player_1_result = player_1.Try_Place_Pawn(); }

            // We enter in this case only if the player didn't or can't play -> e.g. he requested a command instead of placing a pawn on the othellier
            if (std::get<E_Game_Command>(player_1_result) == E_Game_Command::QUIT_GAME)
            {
                game_result = E_Game_Result::PLAYER_1_QUIT;
                break;
            }
        }
        else { std::get<E_Game_Command>(player_1_result) = E_Game_Command::PASS_TURN; }

        // Update the data displayed on the screen
        Clear_Windows_Terminal_Screen();
        othellier->Draw_Othellier();

        // --------------------------------------------------------------------------------------------------------------------------------------------

        if (player_2.Can_Play())
        {
            // Blocking point in order to have a correct answer from the player. The player plays by default here.
            while (!std::get<bool>(player_2_result)) { player_2_result = player_2.Try_Place_Pawn(); }

            // We enter in this case only if the player didn't or can't play -> e.g. he requested a command instead of placing a pawn on the othellier
            if (std::get<E_Game_Command>(player_2_result) == E_Game_Command::QUIT_GAME)
            {
                game_result = E_Game_Result::PLAYER_2_QUIT;
                break;
            }
        }
        else { std::get<E_Game_Command>(player_2_result) = E_Game_Command::PASS_TURN; }

        // --------------------------------------------------------------------------------------------------------------------------------------------

        // Case where the game could be blocked due to the lack of positions available for both players to place a pawn
        if (std::get<E_Game_Command>(player_1_result) == E_Game_Command::PASS_TURN
         && std::get<E_Game_Command>(player_2_result) == E_Game_Command::PASS_TURN)
        {
            game_result = E_Game_Result::PLAYERS_BLOCKED;
        }

        // --------------------------------------------------------------------------------------------------------------------------------------------

        // Update the data displayed on the screen
        Clear_Windows_Terminal_Screen();
        othellier->Draw_Othellier();
    } // Game Loop

    Display_Result(game_result);

    return 1;
} // main

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

void Setup_Windows_Terminal(void)
{
    std::string terminal_title("Othello [no GUI] v" + std::to_string(VERSION_HIGH) + "." + std::to_string(VERSION_LOW));
    std::wstring w_terminal_title(terminal_title.begin(), terminal_title.end());
    SetConsoleTitle(w_terminal_title.data());
}

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

void Clear_Windows_Terminal_Screen(char fill)
{
    COORD coordinate = {0,0};
    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(console, &screen_buffer_info);
    DWORD written, cells = screen_buffer_info.dwSize.X * screen_buffer_info.dwSize.Y;
    FillConsoleOutputCharacter(console, fill, cells, coordinate, &written);
    FillConsoleOutputAttribute(console, screen_buffer_info.wAttributes, cells, coordinate, &written);
    SetConsoleCursorPosition(console, coordinate);
} // Clear_Windodws_Terminal_Screen

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

void Display_Result(const E_Game_Result & game_result)
{
    // Display the game result
    switch (game_result)
    {
        case E_Game_Result::DRAW:               std::cout << "Draw !" << std::endl;                         break;
        case E_Game_Result::PLAYER_1_QUIT:      std::cout << "Player 1 has left the game." << std::endl;
        case E_Game_Result::PLAYER_2_WIN:       std::cout << "Player 2 won !" << std::endl;                 break;
        case E_Game_Result::PLAYER_2_QUIT:      std::cout << "Player 2 has left the game." << std::endl;
        case E_Game_Result::PLAYER_1_WIN:       std::cout << "Player 1 won !" << std::endl;                 break;
        case E_Game_Result::PLAYERS_BLOCKED:    std::cout << "Players are both bloqued *.* !" << std::endl; break;
        default: break;
    }

    // A player who has left the game is considered an abandonment
    if (game_result == E_Game_Result::PLAYER_1_QUIT)        { std::cout << "Player 2 got all "
                                                                        << MAX_PAWNS
                                                                        << " pawns due to player 1's abandonment." << std::endl; }

    else if (game_result == E_Game_Result::PLAYER_2_QUIT)   { std::cout << "Player 1 got all "
                                                                        << MAX_PAWNS
                                                                        << " pawns due to player 2's abandonment." << std::endl; }
    // Count the pawns
    else
    {
        // TODO
    }

    // Wait for any user input before closing the terminal
    std::cin.get();
} // Display_Result

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

void Test_Game_Logic(void)
{
    // Create the othellier (the board game) and the players
    std::shared_ptr<Othellier> othellier{ std::make_shared<Othellier>() };
    Player player_1{ E_Pawn_Color::BLACK, othellier }; // X
    Player player_2{ E_Pawn_Color::WHITE, othellier }; // O

    // Try_Place_Pawn( COLUMN , LINE );
    //player_2.Try_Place_Pawn(4, 6); // Up
    //player_1.Try_Place_Pawn(5, 6); // Up

    //player_2.Try_Place_Pawn(5, 3); // Down
    //player_1.Try_Place_Pawn(4, 3); // Down

    //player_1.Try_Place_Pawn(6, 5); // Left
    //player_2.Try_Place_Pawn(6, 4); // Left

    //player_1.Try_Place_Pawn(3, 4); // Right
    //player_2.Try_Place_Pawn(3, 5); // Right

    //player_1.Try_Place_Pawn(5, 6); // Up Left
    //player_2.Try_Place_Pawn(6, 6); // Up Left

    //player_2.Try_Place_Pawn(4, 6); // Up Right
    //player_1.Try_Place_Pawn(3, 6); // Up Right
    //player_2.Try_Place_Pawn(3, 5); // Up Right - advanced moves
    //player_1.Try_Place_Pawn(3, 4); // Up Right - advanced moves

    //player_2.Try_Place_Pawn(5, 3); // Down Left
    //player_1.Try_Place_Pawn(6, 3); // Down Left

    //player_1.Try_Place_Pawn(4, 3); // Down Right
    //player_2.Try_Place_Pawn(3, 3); // Down Right

    Clear_Windows_Terminal_Screen();
    othellier->Draw_Othellier();

    //std::cin.get(); // do not use this while coding with Qt - it only works for a terminal [DEBUG AND CODING USES ONLY]
} // Test_Game_Logic

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/
