// *****************************************************************************
// File Name: IBasicInfo.h
// Auth: Tian Zhou
// *****************************************************************************

#ifndef SUPERMAN_H
#define SUPERMAN_H

#include <iostream>
#include <string>

#include "IBasicInfo.h"
#include "GeneralInfo.h"

class CSuperMan : public CGeneralInfo
{
   public:
      CSuperMan( const GeneralInfo_t& generalInfo, std::string skill );
      ~CSuperMan(){}

      void PrintGeneralInfo();
      inline void AddPersonalSkill( std::string& skill ){ m_PersonalSkill = skill; }
      bool GetPersonalSkill( std::string& skill );
      
      virtual void TellTianz(){};
      
   private:
      std::string m_PersonalSkill;
};

#endif
