//******************************************************************************
// File Name:  Param.h
// Copyright:  SENCORE, Inc. 2009. All rights reserved.
//******************************************************************************

#ifndef  PARAM_H
#define  PARAM_H

#include <cstdio>
#include <iostream>
#include <algorithm>

#include <map>
#include <string>
#include <vector>
#include <sstream>

#include "DataTypeDef.h"

//! @ingroup UTIL_MODULE
class CParam
{
public:
   // NOTE: We distinguish between INT32 & INT64 for the sake of the SNMP agent.  //! @todo IS THIS STILL TRUE?
   enum ParamType_e
   {
      PT_NULL   = 0,  //!< Nothing is Set
      PT_BOOL   = 1,  //!< true or false
      PT_INT32  = 2,  //!< 32-bit Signed Integer
      PT_INT64  = 3,  //!< 64-bit Signed Integer
      PT_STR    = 4,  //!< null-Terminated String
      PT_OBJ    = 5,  //!< Key/Value Pairs
      PT_ARRAY  = 6   //!< Array of CParam
   };

   //! Construct an empty CParam of specific type.
   explicit CParam( ParamType_e type = PT_NULL );

   //! Copy Constructor
   CParam( const CParam& param );

   template<typename T> explicit CParam( const T& val )
   {
      Initialize();
      *this = val;
   }

   //! Destructor
   virtual ~CParam();

   //! Get the type of CParam.
   //! @sa ParamType_e
   ParamType_e GetType() const;

   //! Convenience function which returns true if type is NULL.
   bool IsNull() const;

   bool IsNumber() const;
   bool IsString() const;
   bool IsBool() const;
   bool IsObj() const;
   bool IsArray() const;


   //! Clears the CParam to NULL.
   //! Equivalent to constructing a CParam with no parameters.
   virtual void Clear( ParamType_e type = PT_NULL );

   //! Assignment Operators
   //! @sa Assign
   CParam& operator=( const CParam& rhs )
   {
      return Assign( rhs );
   }

   template<typename T> CParam& operator=( const T& rhs )
   {
      return Assign( rhs );
   }

   CParam& operator+=( const CParam& rhs );

   // NOTE: Do not define the cast operators because they are too dangerous.
   // Force users to make explicit "casts" using the fcns below:
   bool ToBool( bool onNull = false ) const;
   int32_t ToInt32( int32_t onNull = 0 ) const;
   uint32_t ToUInt32( uint32_t onNull = 0 ) const;
   int64_t ToInt64( int64_t onNull = 0 ) const;
   uint64_t ToUInt64( uint64_t onNull = 0) const;
   std::string ToString( const std::string& onNull = "" ) const;
   const char* ToChar( const char* onNull = NULL ) const;

   // Subscript Operators (const and non-const versions)

   //! Applies To: Obj
   //! If key does not exist, it will be created.
   //! @warning If type != OBJ, the CParam will be converted to a OBJ.
   //! @sa KeyExists()
   CParam& operator[]( const std::string& key );

   //! Applies To: Obj
   //! @warning Fails if type != NULL or OBJ.
   //! @warning Fails if key doesn't exist.
   //! @sa KeyExists()
   const CParam& operator[]( const std::string& key ) const;

   //! Applies To: Obj
   //! If key does not exist, it will be created.
   //! @warning If type != OBJ, the CParam will be converted to a OBJ.
   //! @sa KeyExists()
   CParam& operator[]( const char* key );

   //! Applies To: Obj
   //! @warning Fails if type != NULL or OBJ.
   //! @warning Fails if key doesn't exist.
   //! @sa KeyExists()
   const CParam& operator[]( const char* key ) const;

   //! Applies To: Obj, Array
   //! @warning Fails if index doesn't exist.
   //! @sa GetLength()
   CParam& operator[]( int index );

   //! Applies To: Obj, Array
   //! @warning Fails if index doesn't exist.
   //! @sa GetLength()
   const CParam& operator[]( int index ) const;

   //! Applies To: Obj
   //! @warning Fails if index doesn't exist.
   std::string GetKey( size_t index ) const;

   //! Applies To: Map
   bool KeyExists( const std::string& key ) const;

   //! Applies To: Map
   size_t KeyLowerBound( const std::string& key ) const;

   //! Applies To: Map
   size_t KeyUpperBound( const std::string& key ) const;

   //! Returns 0 for NULL.\n
   //! Returns 0 for BOOL.\n
   //! Returns # of bytes (4) for INT32\n
   //! Returns # of bytes (8) for INT64\n
   //! Returns strlen for STR\n
   //! Returns # of entries for OBJ, ARRAY\n
   //! Returns # of entries for ARRAY\n
   size_t Length() const;

   //! Applies To: Obj
   void RemoveParam( const std::string& key );

   //! Applies To: Obj, Array
   void RemoveParam( size_t index );

   //! Applies To: Array
   void AddParam( const CParam& param = CParam() );


   template<typename Type_t, typename Parseable_t >
   Type_t ParseType( Parseable_t, const std::string& str ) const
   {
      std::istringstream is( str );
      Parseable_t value;
      is >> value;
      return (Type_t)value;
   }

   template<typename Numeric_t>
   bool ParseBoolean( Numeric_t& target, const std::string& str ) const
   {
      std::string lowerStr = str;
      std::transform( lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower );
      if( lowerStr == "true" )
      {
         target = (Numeric_t)1;
         return true;
      }
      if( lowerStr == "false" )
      {
         target = (Numeric_t)0;
         return true;
      }

      return false;
   }

   //! Converts a number to a string.
   //! @param[in] value     The number to convert to a string.
   //! @param[in] precision If applicable, the number of digits after the decimal.
   //! @return The string equivalent of value. e.g. int 123 becomes string "123"
   template<typename Type_t>
   inline std::string ToStr( const Type_t& value, std::streamsize precision = 6 ) const
   {
      std::ostringstream os;

      os.precision( precision );
      os.setf( std::ios::fixed );

      // Unary + promotes types (e.g. char) to ensure output as integers.
      os << +value;

      return os.str();
   }

   //! Converts a string to a number.
   //! @param[out] target The value of the converted string will be returned
   //!                    in this parameter. It must be a numeric type.
   //! @param[in]  str    The string representation of a number to convert.
   //! @return true if the conversion was successful, false otherwise.
   //! @note Special cases "false" and "true" return 0 and 1, respectively.
   //! @warning If this function returns false, the value of "target" is unspecified.
   template<typename Numeric_t>
   inline bool ToNum( Numeric_t& target, const std::string& str ) const
   {
      // This abstraction permits using unary + to specify the type via overloading.
      target = ParseType<Numeric_t>( +target, str );

      if( str != ToStr( target ) )
      {
         // If the converted number doesn't match, try boolean literals.
         return ParseBoolean( target, str );
      }

      return true;
   }

private:
   typedef std::map<std::string, CParam> CParamMap_t;
   typedef std::vector<CParam> CParamVector_t;

   //! Retrieves iterator for the specified index, with caching for increasing indices.
   CParamMap_t::iterator GetIterator( size_t index ) const;

   ParamType_e m_Type;
   int64_t m_IntValue;
   std::string* m_pStrValue;

   // These are mutable because when using the const
   // version of the [] operator we need to be able to
   // add the key to the param so we have something to
   // return rather than crashing.
   mutable CParamMap_t* m_pMap;
   mutable CParamVector_t* m_pArray;

   mutable CParamMap_t::iterator m_MapCache;
   mutable size_t m_MapCachePos;

   CParam& Assign( const bool& rhs );
   CParam& Assign( const int32_t& rhs );
   CParam& Assign( const uint32_t& rhs );
   CParam& Assign( const int64_t& rhs );
   CParam& Assign( const uint64_t& rhs );
   CParam& Assign( const std::string& rhs );
   //! @attention Assumes a C string.
   CParam& Assign( const char* rhs );

   CParam& Assign( const CParam& rhs );
   void Initialize();

   static const char* const m_TRUE;
   static const char* const m_FALSE;
};

// These specializations will accept constants designated as LL/ULL and
// convert to int64_t and uint64_t (which are not long long on 64-bit targets).
template<> inline CParam& CParam::operator=( const long long& rhs )
{
   return Assign( static_cast<int64_t>(rhs) );
}

template<> inline CParam& CParam::operator=( const unsigned long long& rhs )
{
   return Assign( static_cast<uint64_t>(rhs) );
}

#endif
