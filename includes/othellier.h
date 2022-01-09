#ifndef OTHELLIER_H
#define OTHELLIER_H

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/
/* Includes */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "pawn.h"
#include "slot.h"

#include <array>
#include <utility>

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/
/* Defines */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#define MIN_INDEX       0
#define NUMBER_ROWS     8
#define NUMBER_COLUMNS  8

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/
/* Class Definition */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

class Othellier
{
    public:
        Othellier();
        ~Othellier();

        bool Place_Pawn(const unsigned int position_x, const unsigned int position_y, Pawn pawn) noexcept;
        bool Can_Play(const Pawn & pawn) const noexcept;
        void Reset_Othellier(void) noexcept;
        void Draw_Othellier(void) const noexcept;
        std::pair<unsigned int, unsigned int> Count_Pawns(void) const noexcept;
        std::pair<unsigned int, unsigned int> Count_All_Pawns(void) const noexcept;


    protected:
        bool Check_Pawn_Position_is_Valid(const unsigned int position_x, const unsigned int position_y) const noexcept;
        bool Switch_Possible_Opponent_Pawns(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) noexcept;

        // Functions used by Othellier::Switch_Possible_Opponent_Pawns :
        bool Try_to_Switch_in_Up_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) noexcept;
        bool Try_to_Switch_in_Down_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) noexcept;
        bool Try_to_Switch_in_Left_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) noexcept;
        bool Try_to_Switch_in_Right_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) noexcept;
        bool Try_to_Switch_in_Up_Left_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) noexcept;
        bool Try_to_Switch_in_Up_Right_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) noexcept;
        bool Try_to_Switch_in_Down_Left_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) noexcept;
        bool Try_to_Switch_in_Down_Right_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) noexcept;

        // Functions used by Othellier::Can_Play :
        bool Try_to_Play_in_Up_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) const noexcept;
        bool Try_to_Play_in_Down_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) const noexcept;
        bool Try_to_Play_in_Left_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) const noexcept;
        bool Try_to_Play_in_Right_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) const noexcept;
        bool Try_to_Play_in_Up_Left_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) const noexcept;
        bool Try_to_Play_in_Up_Right_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) const noexcept;
        bool Try_to_Play_in_Down_Left_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) const noexcept;
        bool Try_to_Play_in_Down_Right_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) const noexcept;

    protected:
        std::array< std::array<Slot, NUMBER_COLUMNS>, NUMBER_ROWS> _othellier;
};

#endif /* OTHELLIER_H */
