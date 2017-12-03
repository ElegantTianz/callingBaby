// *****************************************************************************
// File Name: IBasicInfo.h
// Auth: Tian Zhou
// *****************************************************************************

#include "GeneralInfo.h"

// *****************************************************************************
// Function Name: IBasicInfo.cpp
// Auth: Tian Zhou
// *****************************************************************************
CGeneralInfo::CGeneralInfo( const GeneralInfo_t & generalInfo ) :
   CIBasicInfo( generalInfo.sex, generalInfo.age ),
   m_Name( generalInfo.name ),
   m_ID( generalInfo.id )
{
}

// *****************************************************************************
// Function Name: IBasicInfo.cpp
// Auth: Tian Zhou
// *****************************************************************************
void CGeneralInfo::PrintGeneralInfo()
{
   std::cout << "\n Name:\t" << m_Name << "\n";
   std::cout << " ID:\t" << m_ID << "\n";
   std::cout << " Sex:\t" << this->GetSex() << "\n";
   std::cout << " Age:\t" << static_cast<unsigned int>( this->GetAge() ) << "\n\n";
}
