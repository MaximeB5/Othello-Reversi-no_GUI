/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/
/* Includes */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "pawn.h"

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/
/* Classe Implementation */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

Pawn::Pawn(const E_Pawn_Color color) : _color(color)
{
} // Pawn

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

Pawn::~Pawn()
{
} // ~Pawn

E_Pawn_Color Pawn::Get_Color() const noexcept
{
    return _color;
} // Get_Color

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/

void Pawn::Return_Pawn() noexcept
{
    _color = (_color == E_Pawn_Color::BLACK ? E_Pawn_Color::WHITE : E_Pawn_Color::BLACK);
} // Return_Pawn

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/
