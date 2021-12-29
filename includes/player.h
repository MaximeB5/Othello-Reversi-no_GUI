#ifndef PLAYER_H
#define PLAYER_H

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/
/* Includes */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "othellier.h"
#include "pawn.h"
#include "enum_game.h"

#include <memory>
#include <tuple>

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/
/* Class Definition */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

class Player
{
    public:
        explicit Player(const E_Pawn_Color color, std::shared_ptr<Othellier> othellier);
        ~Player();

        E_Pawn_Color Get_Color(void) const noexcept;
        bool Can_Play(void) const noexcept;
        std::tuple<bool, E_Game_Command, unsigned int, unsigned int> Try_Place_Pawn(void) noexcept;

    protected:
        unsigned int Ask_Position_X(void) const noexcept;
        unsigned int Ask_Position_Y(void) const noexcept;
        bool Ask_Quit_Game(void) const noexcept;

    protected:
        E_Pawn_Color _color;
        std::shared_ptr<Othellier> _othellier;
};

#endif // PLAYER_H
