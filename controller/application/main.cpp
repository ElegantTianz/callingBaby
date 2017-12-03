// *****************************************************************************
// File Name: IBasicInfo.h
// Auth: Tian Zhou
// *****************************************************************************

#include <iostream>
#include <string>

#include "IBasicInfo.h"
#include "GeneralInfo.h"
#include "SuperMan.h"
#include "Param.h"

using namespace std;

class TempClass
{
   public:
      TempClass( uint8_t data)
      {
      }
      ~TempClass(){}

      //virtual void TellTianz01(){}
      //virtual void TellTianz02(){}
   private:
      //uint32_t m_Data01;
      //uint32_t m_Data02;
      //uint32_t m_Data03;
      std::string m_Str01;
      std::string m_Str02;

};

// *****************************************************************************
// Function Name: IBasicInfo.cpp
// Auth: Tian Zhou
// *****************************************************************************
int main( int argc, char** argv )
{
	// Create a super man
	GeneralInfo_t generalInfo;
	generalInfo.name = string( "Tianz" );
	generalInfo.id = string( "123456789" );
	generalInfo.sex = "Male";
	generalInfo.age = 24;

	CSuperMan* superMan = new CSuperMan( generalInfo, "I can fly" );

	//CSuperMan superMan1( generalInfo, string( "Good man") );
	TempClass temClass( 8 );
	std::cout << "Sizeof = " << sizeof( temClass ) << "\n";

   CParam personInfo;
   
   personInfo["Name"] = "Tianz";
   personInfo["Age"] = sizeof( temClass );

   std::string info = string( ">>>>>" ) + "Name = " + personInfo["Age"].ToString() + "\n";
   std::cout << info;
	
	return 0;
}
