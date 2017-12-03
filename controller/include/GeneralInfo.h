// *****************************************************************************
// File Name: IBasicInfo.h
// Auth: Tian Zhou
// *****************************************************************************

#ifndef GENERALINFO_H
#define GENERALINFO_H

#include <iostream>
#include <string>

#include "IBasicInfo.h"

struct GeneralInfo_t
{
   std::string sex;
   unsigned char age;
   std::string name;
   std::string id;
};

class CGeneralInfo : public CIBasicInfo
{
   public:
      CGeneralInfo( const GeneralInfo_t& generalInfo );
      ~CGeneralInfo(){};

      inline void SetName( const std::string& name ){ m_Name = name; }
      inline std::string GetName(){ return m_Name; }
      inline void SetID( const std::string id ){ m_ID = id; }
      inline std::string GetID(){ return m_ID; }

      void PrintGeneralInfo();
      
      virtual bool GetPersonalSkill( std::string& skill ){};

      virtual void TellTianz(){};

   private:
      std::string m_Name;
      std::string m_ID;
};

#endif 