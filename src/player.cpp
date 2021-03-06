/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/
/* Includes */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "player.h"

#include <string>
#include <iostream>
#include <algorithm>

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/
/* Classe Implementation */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

Player::Player(const E_Pawn_Color color, std::shared_ptr<Othellier> othellier)
    : _color(color), _othellier(othellier)
{
} // Player

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

Player::~Player()
{
} // ~Player

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

E_Pawn_Color Player::Get_Color(void) const noexcept
{
    return _color;
} // Get_Color

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

bool Player::Can_Play(void) const noexcept
{
    return _othellier->Can_Play( Pawn{this->_color} );
} // Can_Play

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

std::tuple<bool, E_Game_Command, unsigned int, unsigned int> Player::Try_Place_Pawn(void) noexcept
{
    std::tuple<bool, E_Game_Command, unsigned int, unsigned int> result{false, E_Game_Command::NO_COMMAND, 0, 0};

    // --------------------------------------------------------------------------------------------------------------------------------------------

    // Quit the game
    if (Ask_Quit_Game())
    {
        std::get<bool>(result) = true;
        std::get<E_Game_Command>(result) = E_Game_Command::QUIT_GAME;
        return result;
    }

    // --------------------------------------------------------------------------------------------------------------------------------------------

    // Play : try to place a pawn
    unsigned int column = Ask_Position_X();
    unsigned int line   = Ask_Position_Y();

    std::get<bool>(result) = _othellier->Place_Pawn(column, line, Pawn{ _color } );

    return result;
} // Try_Place_Pawns

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

unsigned int Player::Ask_Position_X(void) const noexcept
{
    // Ask to the player
    std::cout << std::endl << (_color == E_Pawn_Color::BLACK ? "[Player 1] " : "[Player 2] ");
    std::cout << "Please enter the X position to place your pawn [Range : 1 to 8]." << std::endl;

    // Get the answer
    std::string answer;
    std::getline(std::cin, answer);

    // Analyze the answer : check the string only contains digits and is not empty
    if (std::all_of(answer.begin(), answer.end(), ::isdigit) && !answer.empty())
    {
        unsigned long ul_answer = std::stoul(answer, nullptr, 0);
        return (unsigned int)ul_answer;
    }

    // Error
    return 0;
} // Ask_Position_X

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

unsigned int Player::Ask_Position_Y(void) const noexcept
{
    // Ask to the player
    std::cout << std::endl << (_color == E_Pawn_Color::BLACK ? "[Player 1] " : "[Player 2] ");
    std::cout << "Please enter the Y position to place your pawn [Range : 1 to 8]." << std::endl;

    // Get the answer
    std::string answer;
    std::getline(std::cin, answer);

    // Analyze the answer : check the string only contains digits and is not empty
    if (std::all_of(answer.begin(), answer.end(), ::isdigit) && !answer.empty())
    {
        unsigned long ul_answer = std::stoul(answer, nullptr, 0);
        return (unsigned int)ul_answer;
    }

    // Error
    return 0;
} // Ask_Position_Y

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

bool Player::Ask_Quit_Game(void) const noexcept
{
    // Ask to the player
    std::cout << std::endl << (_color == E_Pawn_Color::BLACK ? "[Player 1] " : "[Player 2] ");
    std::cout << "Would you like to quit the game ? (Y/N)" << std::endl;

    // Get the answer
    std::string answer;
    std::getline(std::cin, answer);

    // Formate the answer
    std::transform(answer.begin(), answer.end(), answer.begin(), ::toupper);

    // Analyze the answer
    if (answer.find("Y") != std::string::npos)
    {
        return true;
    }

    return false;
} // Ask_Position_Y

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/
