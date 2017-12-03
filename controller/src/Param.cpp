//******************************************************************************
// File Name:  Param.cpp
// Copyright:  SENCORE, Inc. 2009. All rights reserved.
//******************************************************************************

// OPTIMIZE: This class would probably benefit tremendously from copy-on-write
//           and other optimizations.
// See: http://en.wikipedia.org/wiki/Copy-on-write
//      http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Copy-on-write
//      http://www.tantalon.com/pete/cppopt/final.htm

#include <cassert>
#include <fstream>

#include "Param.h"

const char* const CParam::m_TRUE = "true";
const char* const CParam::m_FALSE = "false";

const CParam m_ConstCParam( "ERROR CParam(): You tried to access an element with [] that doesn't exist." );

CParam::CParam( ParamType_e type )
{
   Initialize();
   Clear( type );
}

CParam::CParam( const CParam& param )
{
   Initialize();
   Assign( param );
}

void CParam::Initialize()
{
   m_IntValue = 0;
   m_pStrValue = NULL;
   m_pMap = NULL;
   m_pArray = NULL;
   m_MapCachePos = -1;

   m_Type = PT_NULL;
}

CParam::~CParam()
{
   CParam::Clear( PT_NULL );
}

void CParam::Clear( CParam::ParamType_e type )
{
   m_Type = type;

   m_IntValue = 0;

   delete m_pStrValue;
   m_pStrValue = NULL;

   m_MapCachePos = -1;
   delete m_pMap;
   m_pMap = NULL;

   delete m_pArray;
   m_pArray = NULL;

   switch( type )
   {
      case PT_STR:
         m_pStrValue = new std::string;
         break;

      case PT_OBJ:
         m_pMap = new CParamMap_t;
         break;

      case PT_ARRAY:
         m_pArray = new CParamVector_t;
         break;

      default:
         break;
   }
}

CParam& CParam::Assign( const bool& rhs )
{
   CParam::Clear( PT_BOOL );
   m_IntValue = (rhs == true ? 1 : 0);
   return *this;
}

CParam& CParam::Assign( const int32_t& rhs )
{
   CParam::Clear( PT_INT32 );
   m_IntValue = static_cast<int64_t>( rhs );
   return *this;
}

CParam& CParam::Assign( const uint32_t& rhs )
{
   CParam::Clear( PT_INT32 );
   m_IntValue = static_cast<int64_t>( rhs );
   return *this;
}

CParam& CParam::Assign( const int64_t& rhs )
{
   CParam::Clear( PT_INT64 );
   m_IntValue = static_cast<int64_t>( rhs );
   return *this;
}

CParam& CParam::Assign( const uint64_t& rhs )
{
   CParam::Clear( PT_INT64 );
   m_IntValue = static_cast<int64_t>( rhs );
   return *this;
}

CParam& CParam::Assign( const std::string& rhs )
{
   CParam::Clear( PT_STR );
   *m_pStrValue = rhs;
   return *this;
}

CParam& CParam::Assign( const char* rhs )
{
   CParam::Clear( PT_STR );
   *m_pStrValue = rhs;
   return *this;
}

CParam& CParam::Assign( const CParam& rhs )
{
   if( &rhs != this )
   {
      CParam::Clear( rhs.m_Type );

      switch( m_Type )
      {
         case PT_BOOL:
         case PT_INT32:
         case PT_INT64:
            m_IntValue = rhs.m_IntValue;
            break;

         case PT_STR:
            *m_pStrValue = *(rhs.m_pStrValue);
            break;

         case PT_OBJ:
            *m_pMap = *(rhs.m_pMap);
            break;

         case PT_ARRAY:
            *m_pArray = *(rhs.m_pArray);
            break;

         case PT_NULL:
         default:
            break;
      }
   }
   return *this;
}

CParam& CParam::operator+=( const CParam& rhs )
{
   if( m_Type == PT_NULL )
   {
      this->m_Type = PT_ARRAY;
      delete m_pArray;
      m_pArray = new CParamVector_t;
   }

   switch( m_Type )
   {
      case PT_ARRAY:
         if( rhs.m_Type == PT_ARRAY )
         {
            this->m_pArray->insert( this->m_pArray->end(), rhs.m_pArray->begin(), rhs.m_pArray->end() );
         }
         else
         {
            this->m_pArray->push_back( rhs );
         }
         break;

      case PT_INT32:
      case PT_INT64:
         this->m_IntValue += rhs.m_IntValue;
         break;

      case PT_STR:
         *(this->m_pStrValue) += *(rhs.m_pStrValue);
         break;

      case PT_OBJ:
         m_MapCachePos = -1;
         m_pMap->insert( rhs.m_pMap->begin(), rhs.m_pMap->end() );
         break;

      case PT_BOOL:
         // What should be the behavior?  Maybe AND?
         break;

      case PT_NULL:
      default:
         break;

   }

   return *this;
}

bool CParam::ToBool( bool onNull ) const
{
   bool val = false;

   switch( m_Type )
   {
      case PT_BOOL:
      case PT_INT32:
      case PT_INT64:
         val = (m_IntValue != 0);
         break;

      case PT_STR:
         {
            int boolean = 0;
            if( ToNum( boolean, *m_pStrValue ) == true )
            {
               val = !!boolean;
            }
            else
            {
               val = onNull;
            }
         }
         break;

      case PT_OBJ:
      case PT_ARRAY:
         val = false;
         break;

      case PT_NULL:
         val = onNull;
         break;
   }

   return val;
}

int32_t CParam::ToInt32( int32_t onNull ) const
{
   int32_t val = 0;

   switch( m_Type )
   {
      case PT_BOOL:
      case PT_INT32:
      case PT_INT64:
         val = static_cast<int32_t>( m_IntValue );
         break;

      case PT_STR:
         if( ToNum( val, *m_pStrValue ) == false )
         {
            val = onNull;
         }
         break;

      case PT_OBJ:
      case PT_ARRAY:
         val = 0;
         break;

      case PT_NULL:
         val = onNull;
         break;
   }

   return val;
}

uint32_t CParam::ToUInt32( uint32_t onNull ) const
{
   uint32_t val = 0;

   switch( m_Type )
   {
      case PT_BOOL:
      case PT_INT32:
      case PT_INT64:
         val = static_cast<uint32_t>( m_IntValue );
         break;

      case PT_STR:
         if( ToNum( val, *m_pStrValue ) == false )
         {
            val = onNull;
         }
         break;

      case PT_OBJ:
      case PT_ARRAY:
         val = 0;
         break;

      case PT_NULL:
         val = onNull;
         break;
   }

   return val;
}

int64_t CParam::ToInt64( int64_t onNull ) const
{
   int64_t val = 0;

   switch( m_Type )
   {
      case PT_BOOL:
      case PT_INT32:
      case PT_INT64:
         val = static_cast<int64_t>( m_IntValue );
         break;

      case PT_STR:
         if( ToNum( val, *m_pStrValue ) == false )
         {
            val = onNull;
         }
         break;

      case PT_OBJ:
      case PT_ARRAY:
         val = 0;
         break;

      case PT_NULL:
         val = onNull;
         break;
   }

   return val;
}

uint64_t CParam::ToUInt64( uint64_t onNull ) const
{
   uint64_t val = 0;

   switch( m_Type )
   {
      case PT_BOOL:
      case PT_INT32:
      case PT_INT64:
         val = static_cast<uint64_t>( m_IntValue );
         break;

      case PT_STR:
         if( ToNum( val, *m_pStrValue ) == false )
         {
            val = onNull;
         }
         break;

      case PT_OBJ:
      case PT_ARRAY:
         val = 0;
         break;

      case PT_NULL:
         val = onNull;
         break;
   }

   return val;
}

std::string CParam::ToString( const std::string& onNull ) const
{
   std::string val;

   switch( m_Type )
   {
      case PT_INT32:
      case PT_INT64:
         val = ToStr( m_IntValue );
         break;

      case PT_BOOL:
         val = (m_IntValue == 0 ? "false" : "true");
         break;

      case PT_STR:
         val = *m_pStrValue;
         break;

      case PT_OBJ:
      case PT_ARRAY:
      default:
         // val = Serialize();
         break;

      case PT_NULL:
         val = onNull;
         break;
   }

   return val;
}

const char* CParam::ToChar( const char* onNull ) const
{
   const char* val = NULL;

   switch( m_Type )
   {
      case PT_INT32:
      case PT_INT64:
         val = reinterpret_cast<const char*>( &m_IntValue );
         break;

      case PT_BOOL:
         val = (m_IntValue == 0 ? m_FALSE : m_TRUE );
         break;

      case PT_STR:
         val = m_pStrValue->data();
         break;

      case PT_OBJ:
      case PT_ARRAY:
      default:
         val = onNull;
         break;

      case PT_NULL:
         val = onNull;
         break;
   }

   return val;
}

CParam& CParam::operator[]( const std::string& key )
{
   m_MapCachePos = -1;
   if( m_Type != PT_OBJ )
   {
      CParam::Clear( PT_OBJ );
   }

   return (*m_pMap)[key];
}

const CParam& CParam::operator[]( const std::string& key ) const
{
   if( m_pMap != NULL )
   {
      CParamMap_t::iterator it;
      it = m_pMap->find( key );
      if( it != m_pMap->end() )
      {
         return it->second;
      }
   }

   return m_ConstCParam;
}

CParam& CParam::operator[]( const char* key )
{
   m_MapCachePos = -1;
   if( m_Type != PT_OBJ )
   {
      CParam::Clear( PT_OBJ );
   }

   return (*m_pMap)[key];
}

const CParam& CParam::operator[]( const char* key ) const
{
   if( m_pMap != NULL )
   {
      CParamMap_t::iterator it;
      it = m_pMap->find( key );
      if( it != m_pMap->end() )
      {
         return it->second;
      }
   }

   return m_ConstCParam;
}

CParam::CParamMap_t::iterator CParam::GetIterator( size_t index ) const
{
   size_t size = m_pMap->size();
   if( index >= size )
   {
      return m_pMap->end();
   }

   if( m_MapCachePos < 0 || index < m_MapCachePos )
   {
      m_MapCache = m_pMap->begin();
      m_MapCachePos = 0;
   }
   advance( m_MapCache, index - m_MapCachePos );
   m_MapCachePos = index;
   return m_MapCache;
}

CParam& CParam::operator[]( int index )
{
   assert( m_Type == PT_ARRAY || m_Type == PT_OBJ );
   if( m_Type == PT_ARRAY )
   {
      assert( (size_t)index < m_pArray->size() ); // NS
      return (*m_pArray)[index];
   }
   else //if( m_Type == PT_OBJ )
   {
      CParamMap_t::iterator it = GetIterator( index );
      assert( it != m_pMap->end() );
      return it->second;
   }
}

const CParam& CParam::operator[]( int index ) const
{
   if( m_Type == PT_ARRAY )
   {
      if( (size_t)index < m_pArray->size() )
      {
         return (*m_pArray)[index];
      }
   }
   else if( m_Type == PT_OBJ )
   {
      CParamMap_t::iterator it = GetIterator( index );
      if( it != m_pMap->end() )
      {
         return it->second;
      }
   }

   return m_ConstCParam;
}

std::string CParam::GetKey( size_t index ) const
{
   if( m_Type == PT_OBJ )
   {
      CParamMap_t::iterator it = GetIterator( index );
      if( it != m_pMap->end() )
      {
         return it->first;
      }
   }
   return "";
}

CParam::ParamType_e CParam::GetType() const
{
   return m_Type;
}

size_t CParam::Length() const
{
   size_t count = 0;

   switch( m_Type )
   {
      case PT_INT32:
         count = 4;
         break;

      case PT_INT64:
         count = 8;
         break;

      case PT_STR:
         count = m_pStrValue->length();
         break;

      case PT_OBJ:
         count = m_pMap->size();
         break;

      case PT_ARRAY:
         count = m_pArray->size();
         break;

      case PT_NULL:
      case PT_BOOL:
      default:
         count = 0;
         break;
   }

   return count;
}

bool CParam::IsNull() const
{
   return (m_Type == PT_NULL);
}

bool CParam::IsNumber() const
{
   return (m_Type == PT_INT32) || (m_Type == PT_INT64);
}

bool CParam::IsString() const
{
   return (m_Type == PT_STR);
}

bool CParam::IsBool() const
{
   return (m_Type == PT_BOOL);
}

bool CParam::IsObj() const
{
   return (m_Type == PT_OBJ);
}

bool CParam::IsArray() const
{
   return (m_Type == PT_ARRAY);
}

bool CParam::KeyExists( const std::string& key ) const
{
   if( m_Type == PT_OBJ )
   {
      return (m_pMap->count( key ) > 0);
   }
   return false;
}

size_t CParam::KeyLowerBound( const std::string& key ) const
{
   if( m_Type == PT_OBJ )
   {
      m_MapCache = m_pMap->lower_bound( key );
      m_MapCachePos = distance( m_pMap->begin(), m_MapCache );
      return m_MapCachePos;
   }
   return 0;
}

size_t CParam::KeyUpperBound( const std::string& key ) const
{
   if( m_Type == PT_OBJ )
   {
      m_MapCache = m_pMap->upper_bound( key );
      m_MapCachePos = distance( m_pMap->begin(), m_MapCache );
      return m_MapCachePos;
   }
   return 0;
}

void CParam::AddParam( const CParam& param )
{
   if( m_Type != PT_ARRAY )
   {
      CParam::Clear( PT_ARRAY );
   }

   m_pArray->push_back( param );
}

void CParam::RemoveParam( const std::string& key )
{
   if( m_Type == PT_OBJ )
   {
      m_MapCachePos = -1;
      m_pMap->erase( key );
   }
}

void CParam::RemoveParam( size_t index )
{
   if( m_Type == PT_ARRAY )
   {
      if( index < m_pArray->size() )
      {
         m_pArray->erase( m_pArray->begin() + index );
      }
   }
   else if( m_Type == PT_OBJ )
   {
      CParamMap_t::iterator it = GetIterator( index );
      if( it != m_pMap->end() )
      {
         m_pMap->erase( it );
         m_MapCachePos = -1;
      }
   }
}

// Allows you to do "cout << param;" without ambiguity.
std::ostream& operator<<( std::ostream& out, CParam& param )
{
   out << param.ToString();
   return out;
}

// Allows you to do "cout << "ABC" + param;".
std::string operator+( const char* pStr, const CParam& param )
{
   std::string s( pStr );
   return s + param.ToString();
}

// Allows you to do "cout << param + "ABC";".
std::string operator+( const CParam& param, const char* pStr )
{
   std::string s( pStr );
   return param.ToString() + s;
}
