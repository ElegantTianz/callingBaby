// *****************************************************************************
// File Name: IBasicInfo.h
// Auth: Tian Zhou
// *****************************************************************************
#ifndef IBASICINFO_H
#define IBASICINFO_H

#include <iostream>
#include <string>

class CIBasicInfo
{
   public:
      CIBasicInfo( std::string sex, unsigned char age );
      ~CIBasicInfo(){}


      inline std::string GetSex(){ return m_Sex; }

      virtual void SetAge( unsigned char age ){ m_Age = age; }
      inline unsigned char GetAge(){ return m_Age; }

      virtual void SetName( const std::string& name ) = 0;
      virtual std::string GetName() = 0;
      virtual void SetID( const std::string id ) = 0;
      virtual std::string GetID() = 0;

      //virtual void AddPersonalSkill( std::string& skill ) = 0;
      
      virtual void TellTianz() = 0;

   private:
      std::string m_Sex;
      unsigned char m_Age;
   
};

#endif
