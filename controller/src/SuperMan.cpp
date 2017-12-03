// *****************************************************************************
// File Name: IBasicInfo.h
// Auth: Tian Zhou
// *****************************************************************************

#include "SuperMan.h"

// *****************************************************************************
// Function Name: IBasicInfo.cpp
// Auth: Tian Zhou
// *****************************************************************************
CSuperMan::CSuperMan(const GeneralInfo_t & generalInfo, std :: string skill) :
   CGeneralInfo( generalInfo ),
   m_PersonalSkill( skill )
{

}

// *****************************************************************************
// Function Name: IBasicInfo.cpp
// Auth: Tian Zhou
// *****************************************************************************
void CSuperMan::PrintGeneralInfo()
{
   std::cout<< "\nI'm " << GetName() << ", I'm a super man!\n\n";
}

// *****************************************************************************
// Function Name: IBasicInfo.cpp
// Auth: Tian Zhou
// *****************************************************************************
bool CSuperMan::GetPersonalSkill( std::string& skill )
{ 
   skill = m_PersonalSkill;
   std::cout<< "\nGetPersonalSkill: " << skill << "\n\n";
   return true; 
}
