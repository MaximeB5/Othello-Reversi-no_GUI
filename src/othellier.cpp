/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/
/* Includes */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "othellier.h"
#include "configuration.h"
#include "templates.h"

#include <utility>
#include <iostream>
#include <vector>
#include <string>

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/
/* Static Functions */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

bool Security_Check_Failed(const unsigned int x, const unsigned int y)
{
    if (x >= NUMBER_ROWS || y >= NUMBER_COLUMNS) { return true; }
    return false;
} // Security_Check_Failed

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/
/* Classe Implementation */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

Othellier::Othellier()
{
    Reset_Othellier();
    Draw_Othellier();
} // Othellier

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

Othellier::~Othellier()
{
} // ~Othellier

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

bool Othellier::Place_Pawn(const unsigned int position_x, const unsigned int position_y, Pawn pawn) noexcept
{
    if (Check_Pawn_Position_is_Valid(position_x, position_y))
    {
        const unsigned int internal_position_x{position_x - 1};
        const unsigned int internal_position_y{position_y - 1};

        if (Switch_Possible_Opponent_Pawns(internal_position_x, internal_position_y, pawn))
        {
            _othellier[internal_position_x][internal_position_y].Occupy_Slot(pawn);
            return true;
        }
    }

    return false;
} // Place_Pawn

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

bool Othellier::Check_Pawn_Position_is_Valid(const unsigned int position_x, const unsigned int position_y) const noexcept
{
    const unsigned int internal_position_x{position_x - 1};
    const unsigned int internal_position_y{position_y - 1};

    if ((position_x > NUMBER_ROWS) || (position_y > NUMBER_COLUMNS))        { return false; }   // Check values aren't out of bonds (even in case of overflow, the unsigned int will overflow at its max value)
    if ((position_x == MIN_INDEX)  || (position_y == MIN_INDEX))            { return false; }   // Trap case not covered by the test above
    if (!_othellier[internal_position_x][internal_position_y].Is_Empty())   { return false; }   // Check the slot is empty

    return true;
} // Check_Pawn_Position_is_Valid

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

bool Othellier::Switch_Possible_Opponent_Pawns(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) noexcept
{
    bool result{false};

    // Overflow security due to what happen during the several try to switch -> it is easier to regroup this at this level and not everywhere
    const unsigned int x_minus_1{position_x == MIN_INDEX ?          position_x : position_x - 1};
    const unsigned int x_plus_1 {position_x == NUMBER_ROWS - 1 ?    position_x : position_x + 1};
    const unsigned int y_minus_1{position_y == MIN_INDEX ?          position_y : position_y - 1};
    const unsigned int y_plus_1 {position_y == NUMBER_COLUMNS - 1 ? position_y : position_y + 1};

    // Normal directions
    result |= Try_to_Switch_in_Up_Direction(position_x, y_minus_1, pawn);
    result |= Try_to_Switch_in_Down_Direction(position_x, y_plus_1, pawn);
    result |= Try_to_Switch_in_Left_Direction(x_minus_1, position_y, pawn);
    result |= Try_to_Switch_in_Right_Direction(x_plus_1, position_y, pawn);

    // Diagonal directions
    result |= Try_to_Switch_in_Up_Left_Direction(x_minus_1, y_minus_1, pawn);
    result |= Try_to_Switch_in_Up_Right_Direction(x_plus_1, y_minus_1, pawn);
    result |= Try_to_Switch_in_Down_Left_Direction(x_minus_1, y_plus_1, pawn);
    result |= Try_to_Switch_in_Down_Right_Direction(x_plus_1, y_plus_1, pawn);

    return result;
} // Switch_Possible_Opponent_Pawns

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

bool Othellier::Try_to_Switch_in_Up_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) noexcept
{
    bool must_return_slot{false};
    std::vector<Slot *> slots; // Addresses of the slots

    for (auto y{position_y}; y > MIN_INDEX; --y)
    {
        // I don't know where the overflow is so I suppress it
        if (Security_Check_Failed(position_x, y)) { break; }

        // Empty slot
        if (_othellier[position_x][y].Is_Empty())
        {
            break;
        }

        // Same color
        else if (_othellier[position_x][y].Get_Pawn_Color() == pawn.Get_Color())
        {
            must_return_slot = true;
            break;
        }

        // Opponent color
        else if (_othellier[position_x][y].Get_Pawn_Color() != pawn.Get_Color())
        {
            slots.push_back( &_othellier[position_x][y] );
        }
    }

    // Case where we must return the pawn on the slot
    if (must_return_slot && !slots.empty())
    {
        for (auto slot : slots) { slot->Return_Pawn(); }
        return true;
    }

    // No actions
    return false;
} // Try_to_Switch_in_Up_Direction

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

bool Othellier::Try_to_Switch_in_Down_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) noexcept
{
    bool must_return_slot{false};
    std::vector<Slot *> slots; // Addresses of the slots

    for (auto y{position_y}; y < NUMBER_COLUMNS; ++y)
    {
        // I don't know where the overflow is so I suppress it
        if (Security_Check_Failed(position_x, y)) { break; }

        // Empty slot
        if (_othellier[position_x][y].Is_Empty())
        {
            break;
        }

        // Same color
        else if (_othellier[position_x][y].Get_Pawn_Color() == pawn.Get_Color())
        {
            must_return_slot = true;
            break;
        }

        // Opponent color
        else if (_othellier[position_x][y].Get_Pawn_Color() != pawn.Get_Color())
        {
            slots.push_back( &_othellier[position_x][y] );
        }
    }

    // Case where we must return the pawn on the slot
    if (must_return_slot && !slots.empty())
    {
        for (auto slot : slots) { slot->Return_Pawn(); }
        return true;
    }

    // No actions
    return false;
} // Try_to_Switch_in_Down_Direction

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

bool Othellier::Try_to_Switch_in_Left_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) noexcept
{
    bool must_return_slot{false};
    std::vector<Slot *> slots; // Addresses of the slots

    for (auto x{position_x}; x > MIN_INDEX; --x)
    {
        // I don't know where the overflow is so I suppress it
        if (Security_Check_Failed(x, position_y)) { break; }

        // Empty slot
        if (_othellier[x][position_y].Is_Empty())
        {
            break;
        }

        // Same color
        else if (_othellier[x][position_y].Get_Pawn_Color() == pawn.Get_Color())
        {
            must_return_slot = true;
            break;
        }

        // Opponent color
        else if (_othellier[x][position_y].Get_Pawn_Color() != pawn.Get_Color())
        {
            slots.push_back( &_othellier[x][position_y] );
        }
    }

    // Case where we must return the pawn on the slot
    if (must_return_slot && !slots.empty())
    {
        for (auto slot : slots) { slot->Return_Pawn(); }
        return true;
    }

    // No actions
    return false;
} // Try_to_Switch_in_Left_Direction

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

bool Othellier::Try_to_Switch_in_Right_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) noexcept
{
    bool must_return_slot{false};
    std::vector<Slot *> slots; // Addresses of the slots

    for (auto x{position_x}; x < NUMBER_ROWS; ++x)
    {
        // I don't know where the overflow is so I suppress it
        if (Security_Check_Failed(x, position_y)) { break; }

        // Empty slot
        if (_othellier[x][position_y].Is_Empty())
        {
            break;
        }

        // Same color
        else if (_othellier[x][position_y].Get_Pawn_Color() == pawn.Get_Color())
        {
            must_return_slot = true;
            break;
        }

        // Opponent color
        else if (_othellier[x][position_y].Get_Pawn_Color() != pawn.Get_Color())
        {
            slots.push_back( &_othellier[x][position_y] );
        }
    }

    // Case where we must return the pawn on the slot
    if (must_return_slot && !slots.empty())
    {
        for (auto slot : slots) { slot->Return_Pawn(); }
        return true;
    }

    // No actions
    return false;
} // Try_to_Switch_in_Right_Direction

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

bool Othellier::Try_to_Switch_in_Up_Left_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) noexcept
{
    bool must_return_slot{false};
    std::vector<Slot *> slots; // Addresses of the slots

    auto x{position_x};
    auto y{position_y};

    for (; x > MIN_INDEX || y > MIN_INDEX; --x, --y)
    {
        // I don't know where the overflow is so I suppress it
        if (Security_Check_Failed(x, y)) { break; }

        // Empty slot
        if (_othellier[x][y].Is_Empty())
        {
            break;
        }

        // Same color
        else if (_othellier[x][y].Get_Pawn_Color() == pawn.Get_Color())
        {
            must_return_slot = true;
            break;
        }

        // Opponent color
        else if (_othellier[x][y].Get_Pawn_Color() != pawn.Get_Color())
        {
            slots.push_back( &_othellier[x][y] );
        }
    }

    // Case where we must return the pawn on the slot
    if (must_return_slot && !slots.empty())
    {
        for (auto slot : slots) { slot->Return_Pawn(); }
        return true;
    }

    // No actions
    return false;
} // Try_to_Switch_in_Up_Left_Direction

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

bool Othellier::Try_to_Switch_in_Up_Right_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) noexcept
{
    bool must_return_slot{false};
    std::vector<Slot *> slots; // Addresses of the slots

    auto x{position_x};
    auto y{position_y};

    for (; x < NUMBER_ROWS || y > MIN_INDEX; ++x, --y)
    {
        // I don't know where the overflow is so I suppress it
        if (Security_Check_Failed(x, y)) { break; }

        // Empty slot
        if (_othellier[x][y].Is_Empty())
        {
            break;
        }

        // Same color
        else if (_othellier[x][y].Get_Pawn_Color() == pawn.Get_Color())
        {
            must_return_slot = true;
            break;
        }

        // Opponent color
        else if (_othellier[x][y].Get_Pawn_Color() != pawn.Get_Color())
        {
            slots.push_back( &_othellier[x][y] );
        }
    }

    // Case where we must return the pawn on the slot
    if (must_return_slot && !slots.empty())
    {
        for (auto slot : slots) { slot->Return_Pawn(); }
        return true;
    }

    // No actions
    return false;
} // Try_to_Switch_in_Up_Right_Direction

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

bool Othellier::Try_to_Switch_in_Down_Left_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) noexcept
{
    bool must_return_slot{false};
    std::vector<Slot *> slots; // Addresses of the slots

    auto x{position_x};
    auto y{position_y};

    for (; x > MIN_INDEX || y < NUMBER_COLUMNS; --x, ++y)
    {
        // I don't know where the overflow is so I suppress it
        if (Security_Check_Failed(x, y)) { break; }

        // Empty slot
        if (_othellier[x][y].Is_Empty())
        {
            break;
        }

        // Same color
        else if (_othellier[x][y].Get_Pawn_Color() == pawn.Get_Color())
        {
            must_return_slot = true;
            break;
        }

        // Opponent color
        else if (_othellier[x][y].Get_Pawn_Color() != pawn.Get_Color())
        {
            slots.push_back( &_othellier[x][y] );
        }
    }

    // Case where we must return the pawn on the slot
    if (must_return_slot && !slots.empty())
    {
        for (auto slot : slots) { slot->Return_Pawn(); }
        return true;
    }

    // No actions
    return false;
} // Try_to_Switch_in_Down_Left_Direction

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

bool Othellier::Try_to_Switch_in_Down_Right_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) noexcept
{
    bool must_return_slot{false};
    std::vector<Slot *> slots; // Addresses of the slots

    auto x{position_x};
    auto y{position_y};

    for (; x < NUMBER_ROWS || y < NUMBER_COLUMNS; ++x, ++y)
    {
        // I don't know where the overflow is so I suppress it
        if (Security_Check_Failed(x, y)) { break; }

        // Empty slot
        if (_othellier[x][y].Is_Empty())
        {
            break;
        }

        // Same color
        else if (_othellier[x][y].Get_Pawn_Color() == pawn.Get_Color())
        {
            must_return_slot = true;
            break;
        }

        // Opponent color
        else if (_othellier[x][y].Get_Pawn_Color() != pawn.Get_Color())
        {
            slots.push_back( &_othellier[x][y] );
        }
    }

    // Case where we must return the pawn on the slot
    if (must_return_slot && !slots.empty())
    {
        for (auto slot : slots) { slot->Return_Pawn(); }
        return true;
    }

    // No actions
    return false;
} // Try_to_Switch_in_Down_Right_Direction

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

bool Othellier::Can_Play(const Pawn & pawn) const noexcept
{
    bool result{false};

    // Check there is at least one pawn
    bool no_more_pawn{false};
    unsigned int i{0};
    unsigned int j{0};

    for (; i < NUMBER_ROWS && j < NUMBER_COLUMNS; ++i, ++j)
    {
        if (_othellier[i][j].Get_Pawn_Color() == pawn.Get_Color())
        {
            no_more_pawn = true;
        }
    }

    if (!no_more_pawn)
    {
        return false;
    }

    // Check possibilities to play
    for (unsigned int position_x{MIN_INDEX}; position_x < NUMBER_ROWS; ++position_x)
    {
        for (unsigned int position_y{MIN_INDEX}; position_y < NUMBER_COLUMNS; ++position_y)
        {
            // Normal directions
            result |= Try_to_Play_in_Up_Direction(position_x, position_y, pawn);
            result |= Try_to_Play_in_Down_Direction(position_x, position_y, pawn);
            result |= Try_to_Play_in_Left_Direction(position_x, position_y, pawn);
            result |= Try_to_Play_in_Right_Direction(position_x, position_y, pawn);

            // Diagonal directions
            result |= Try_to_Play_in_Up_Left_Direction(position_x, position_y, pawn);
            result |= Try_to_Play_in_Up_Right_Direction(position_x, position_y, pawn);
            result |= Try_to_Play_in_Down_Left_Direction(position_x, position_y, pawn);
            result |= Try_to_Play_in_Down_Right_Direction(position_x, position_y, pawn);
        }
    }

    return result;
} // Can_Play

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

bool Othellier::Try_to_Play_in_Up_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) const noexcept
{
    bool must_return_slot{false}, can_play{false};

    for (auto y{position_y}; y > MIN_INDEX; --y)
    {
        // I don't know where the overflow is so I suppress it
        if (Security_Check_Failed(position_x, y)) { break; }

        // Empty slot
        if (_othellier[position_x][y].Is_Empty())
        {
            break;
        }

        // Same color
        else if (_othellier[position_x][y].Get_Pawn_Color() == pawn.Get_Color())
        {
            must_return_slot = true;
            break;
        }

        // Opponent color
        else if (_othellier[position_x][y].Get_Pawn_Color() != pawn.Get_Color())
        {
            can_play = true;
        }
    }

    // Case where we must return the pawn on the slot
    if (must_return_slot && can_play)
    {
        return true;
    }

    // No actions
    return false;
} // Try_to_Play_in_Up_Direction

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

bool Othellier::Try_to_Play_in_Down_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) const noexcept
{
    bool must_return_slot{false}, can_play{false};

    for (auto y{position_y}; y < NUMBER_COLUMNS; ++y)
    {
        // I don't know where the overflow is so I suppress it
        if (Security_Check_Failed(position_x, y)) { break; }

        // Empty slot
        if (_othellier[position_x][y].Is_Empty())
        {
            break;
        }

        // Same color
        else if (_othellier[position_x][y].Get_Pawn_Color() == pawn.Get_Color())
        {
            must_return_slot = true;
            break;
        }

        // Opponent color
        else if (_othellier[position_x][y].Get_Pawn_Color() != pawn.Get_Color())
        {
            can_play = true;
        }
    }

    // Case where we must return the pawn on the slot
    if (must_return_slot && can_play)
    {
        return true;
    }

    // No actions
    return false;
} // Try_to_Play_in_Down_Direction

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

bool Othellier::Try_to_Play_in_Left_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) const noexcept
{
    bool must_return_slot{false}, can_play{false};

    for (auto x{position_x}; x > MIN_INDEX; --x)
    {
        // I don't know where the overflow is so I suppress it
        if (Security_Check_Failed(x, position_y)) { break; }

        // Empty slot
        if (_othellier[x][position_y].Is_Empty())
        {
            break;
        }

        // Same color
        else if (_othellier[x][position_y].Get_Pawn_Color() == pawn.Get_Color())
        {
            must_return_slot = true;
            break;
        }

        // Opponent color
        else if (_othellier[x][position_y].Get_Pawn_Color() != pawn.Get_Color())
        {
            can_play = true;
        }
    }

    // Case where we must return the pawn on the slot
    if (must_return_slot && can_play)
    {
        return true;
    }

    // No actions
    return false;
} // Try_to_Play_in_Left_Direction

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

bool Othellier::Try_to_Play_in_Right_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) const noexcept
{
    bool must_return_slot{false}, can_play{false};

    for (auto x{position_x}; x < NUMBER_ROWS; ++x)
    {
        // I don't know where the overflow is so I suppress it
        if (Security_Check_Failed(x, position_y)) { break; }

        // Empty slot
        if (_othellier[x][position_y].Is_Empty())
        {
            break;
        }

        // Same color
        else if (_othellier[x][position_y].Get_Pawn_Color() == pawn.Get_Color())
        {
            must_return_slot = true;
            break;
        }

        // Opponent color
        else if (_othellier[x][position_y].Get_Pawn_Color() != pawn.Get_Color())
        {
            can_play = true;
        }
    }

    // Case where we must return the pawn on the slot
    if (must_return_slot && can_play)
    {
        return true;
    }

    // No actions
    return false;
} // Try_to_Play_in_Right_Direction

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

bool Othellier::Try_to_Play_in_Up_Left_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) const noexcept
{
    bool must_return_slot{false}, can_play{false};

    auto x{position_x};
    auto y{position_y};

    for (; x > MIN_INDEX || y > MIN_INDEX; --x, --y)
    {
        // I don't know where the overflow is so I suppress it
        if (Security_Check_Failed(x, y)) { break; }

        // Empty slot
        if (_othellier[x][y].Is_Empty())
        {
            break;
        }

        // Same color
        else if (_othellier[x][y].Get_Pawn_Color() == pawn.Get_Color())
        {
            must_return_slot = true;
            break;
        }

        // Opponent color
        else if (_othellier[x][y].Get_Pawn_Color() != pawn.Get_Color())
        {
            can_play = true;
        }
    }

    // Case where we must return the pawn on the slot
    if (must_return_slot && can_play)
    {
        return true;
    }

    // No actions
    return false;
} // Try_to_Play_in_Up_Left_Direction

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

bool Othellier::Try_to_Play_in_Up_Right_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) const noexcept
{
    bool must_return_slot{false}, can_play{false};

    auto x{position_x};
    auto y{position_y};

    for (; x < NUMBER_ROWS || y > MIN_INDEX; ++x, --y)
    {
        // I don't know where the overflow is so I suppress it
        if (Security_Check_Failed(x, y)) { break; }

        // Empty slot
        if (_othellier[x][y].Is_Empty())
        {
            break;
        }

        // Same color
        else if (_othellier[x][y].Get_Pawn_Color() == pawn.Get_Color())
        {
            must_return_slot = true;
            break;
        }

        // Opponent color
        else if (_othellier[x][y].Get_Pawn_Color() != pawn.Get_Color())
        {
            can_play = true;
        }
    }

    // Case where we must return the pawn on the slot
    if (must_return_slot && can_play)
    {
        return true;
    }

    // No actions
    return false;
} // Try_to_Play_in_Up_Right_Direction

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

bool Othellier::Try_to_Play_in_Down_Left_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) const noexcept
{
    bool must_return_slot{false}, can_play{false};

    auto x{position_x};
    auto y{position_y};

    for (; x > MIN_INDEX || y < NUMBER_COLUMNS; --x, ++y)
    {
        // I don't know where the overflow is so I suppress it
        if (Security_Check_Failed(x, y)) { break; }

        // Empty slot
        if (_othellier[x][y].Is_Empty())
        {
            break;
        }

        // Same color
        else if (_othellier[x][y].Get_Pawn_Color() == pawn.Get_Color())
        {
            must_return_slot = true;
            break;
        }

        // Opponent color
        else if (_othellier[x][y].Get_Pawn_Color() != pawn.Get_Color())
        {
            can_play = true;
        }
    }

    // Case where we must return the pawn on the slot
    if (must_return_slot && can_play)
    {
        return true;
    }

    // No actions
    return false;
} // Try_to_Play_in_Down_Left_Direction

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

bool Othellier::Try_to_Play_in_Down_Right_Direction(const unsigned int position_x, const unsigned int position_y, const Pawn & pawn) const noexcept
{
    bool must_return_slot{false}, can_play{false};

    auto x{position_x};
    auto y{position_y};

    for (; x < NUMBER_ROWS || y < NUMBER_COLUMNS; ++x, ++y)
    {
        // I don't know where the overflow is so I suppress it
        if (Security_Check_Failed(x, y)) { break; }

        // Empty slot
        if (_othellier[x][y].Is_Empty())
        {
            break;
        }

        // Same color
        else if (_othellier[x][y].Get_Pawn_Color() == pawn.Get_Color())
        {
            must_return_slot = true;
            break;
        }

        // Opponent color
        else if (_othellier[x][y].Get_Pawn_Color() != pawn.Get_Color())
        {
            can_play = true;
        }
    }

    // Case where we must return the pawn on the slot
    if (must_return_slot && can_play)
    {
        return true;
    }

    // No actions
    return false;
} // Try_to_Play_in_Down_Right_Direction

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

void Othellier::Reset_Othellier(void) noexcept
{
    for (unsigned int row{0}; row < NUMBER_ROWS; ++row)
    {
        for (unsigned int column{0}; column < NUMBER_COLUMNS; ++column)
        {
            // White Pawns
            if ( (row == 3 && column == 3) || (row == 4 && column == 4) ) {
                _othellier[column][row].Occupy_Slot( Pawn{ E_Pawn_Color::WHITE } );
            }

            // Black Pawns
            else if ( (row == 3 && column == 4) || (row == 4 && column == 3) ) {
                _othellier[column][row].Occupy_Slot( Pawn{ E_Pawn_Color::BLACK } );
            }

            // Empty slot
            else {
                _othellier[column][row].Set_Empty();
            }
        }
    }
} // Reset_Othellier

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

void Othellier::Draw_Othellier(void) const noexcept
{
    std::cout << std::endl << "  1 2 3 4 5 6 7 8   Axis X" << std::endl;

    for (unsigned int row{0}; row < NUMBER_ROWS; ++row)
    {
        std::cout << row + 1;

        for (unsigned int column{0}; column < NUMBER_COLUMNS; ++column)
        {
            std::cout << "|";

            if (_othellier[column][row].Is_Empty())
            {
                std::cout << " ";
            }
            else
            {
                _othellier[column][row].Get_Pawn_Color() == E_Pawn_Color::BLACK ? std::cout << "X" : std::cout << "O";
            }
        }

         std::cout << "|" << std::endl;
    }

    std::cout << "Axis Y" << std::endl;
} // Draw_Othellier

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

std::pair<unsigned int, unsigned int> Othellier::Count_Pawns(void) const noexcept
{
    // First = black player ; Second = white player
    auto pawns = std::make_pair<unsigned int, unsigned int>(0, 0);

    // Count pawns
    for (unsigned int row{0}; row < NUMBER_ROWS; ++row)
    {
        for (unsigned int column{0}; column < NUMBER_COLUMNS; ++column)
        {
            if (!_othellier[column][row].Is_Empty())
            {
                _othellier[column][row].Get_Pawn_Color() == E_Pawn_Color::BLACK ? ++pawns.first : ++pawns.second;
            }
        }
    }

    return pawns;
} // Count_Pawns

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

std::pair<unsigned int, unsigned int> Othellier::Count_All_Pawns(void) const noexcept
{
    // First = black player ; Second = white player
    auto pawns = std::make_pair<unsigned int, unsigned int>(0, 0);
    unsigned int empty_slots{0};

    // Count pawns
    for (unsigned int row{0}; row < NUMBER_ROWS; ++row)
    {
        for (unsigned int column{0}; column < NUMBER_COLUMNS; ++column)
        {
            if (_othellier[column][row].Is_Empty())
            {
                ++empty_slots;
            }
            else
            {
                _othellier[column][row].Get_Pawn_Color() == E_Pawn_Color::BLACK ? ++pawns.first : ++pawns.second;
            }
        }
    }

    // If no draw
    if (pawns.first != pawns.second)
    {
        // Empty pawns are given to the player who has the greatest number of pawns
        if (pawns.first > pawns.second) { pawns.first  += empty_slots; }
        else                            { pawns.second += empty_slots; }
    }

    return pawns;
} // Count_All_Pawns

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/
