#ifndef SLOT_H
#define SLOT_H

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/
/* Includes */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "pawn.h"

#include <memory>

/********************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************/
/* Class Definition */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

class Slot
{
    public:
        explicit Slot();
        ~Slot();

        bool Is_Empty(void) const noexcept;
        void Set_Empty(void) noexcept;
        void Occupy_Slot(Pawn pawn) noexcept;
        void Return_Pawn(void) const noexcept;
        E_Pawn_Color Get_Pawn_Color(void) const;
        static unsigned int Get_Number_of_Slots(void) noexcept;
        static unsigned int Get_Number_of_Occupied_Slots(void) noexcept;
        static unsigned int Get_Number_of_Free_Slots(void) noexcept;

    protected:
        static unsigned int _number_of_slots;
        static unsigned int _number_of_slots_occupied;

    protected:
        bool _occupation;
        std::unique_ptr<Pawn> _pawn;
};

#endif /* SLOT_H */
