/*=========================================================================
*
*  Copyright Insight Software Consortium
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*         http://www.apache.org/licenses/LICENSE-2.0.txt
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
*=========================================================================*/

#ifndef itkafDataManager_hxx
#define itkafDataManager_hxx

#include "itkafDataManager.h"

namespace itk
{

namespace af
{

template< typename TBuffer >
DataManager< TBuffer >
::DataManager():
  m_IsArrayDirty( false ),
  m_IsHostBufferDirty( false )
{
  m_Array      = ITK_NULLPTR;
  m_HostBuffer = ITK_NULLPTR;

  this->Initialize();
}


template< typename TBuffer >
DataManager< TBuffer >
::~DataManager()
{
  delete m_Array;
}


template< typename TBuffer >
void
DataManager< TBuffer >
::SetArrayDimensions( const ::af::dim4 & dims )
{
  if( dims != m_ArrayDimensions )
    {
    this->m_ArrayDimensions = dims;
    this->Modified();
    }
}


template< typename TBuffer >
::af::dim4
DataManager< TBuffer >
::GetArrayDimensions() const
{
  return this->m_ArrayDimensions;
}


template< typename TBuffer >
void
DataManager< TBuffer >
::Allocate()
{
  delete m_Array;
  m_Array = new ::af::array( this->m_ArrayDimensions, (::af::dtype)::af::dtype_traits< TBuffer >::af_type );
  this->m_IsArrayDirty = true;
}


template< typename TBuffer >
void
DataManager< TBuffer >
::SetHostBufferPointer( TBuffer* ptr )
{
  m_HostBuffer = ptr;
}


template< typename TBuffer >
void
DataManager< TBuffer >
::SetHostDirtyFlag( bool isDirty )
{
  m_IsHostBufferDirty = isDirty;
}


template< typename TBuffer >
void
DataManager< TBuffer >
::SetArrayDirtyFlag( bool isDirty )
{
  m_IsArrayDirty = isDirty;
}


template< typename TBuffer >
void
DataManager< TBuffer >
::SetArrayDirty()
{
  this->UpdateHostBuffer();
  m_IsArrayDirty = true;
}


template< typename TBuffer >
void
DataManager< TBuffer >
::SetHostBufferDirty()
{
  this->UpdateArray();
  m_IsHostBufferDirty = true;
}


template< typename TBuffer >
void
DataManager< TBuffer >
::UpdateHostBuffer()
{
  MutexHolderType holder(m_Mutex);

  if( m_IsHostBufferDirty && m_Array != ITK_NULLPTR && m_HostBuffer != ITK_NULLPTR )
    {

    itkDebugMacro( this << "::UpdateHostBuffer GPU->CPU data copy " << m_HostBuffer << "->" << m_Array );
    this->m_Array->host( this->m_HostBuffer );
    m_IsHostBufferDirty = false;
    }
}


template< typename TBuffer >
void
DataManager< TBuffer >
::UpdateArray()
{
  MutexHolderType mutexHolder(m_Mutex);

  if( m_IsArrayDirty && m_HostBuffer != ITK_NULLPTR && m_Array != ITK_NULLPTR )
    {
    itkDebugMacro( this << "::UpdateArray CPU->GPU data copy " << m_HostBuffer << "->" << m_Array );
    this->m_Array->write( this->m_HostBuffer, this->m_ArrayDimensions.elements() );
    this->m_IsArrayDirty = true;
    }
}


template< typename TBuffer >
::af::array *
DataManager< TBuffer >
::GetModifiableArray()
{
  SetHostBufferDirty();
  return m_Array;
}


template< typename TBuffer >
TBuffer*
DataManager< TBuffer >
::GetHostBufferPointer()
{
  SetArrayDirty();
  return m_HostBuffer;
}


template< typename TBuffer >
bool
DataManager< TBuffer >
::Update()
{
  if( m_IsArrayDirty && m_IsHostBufferDirty )
    {
    itkExceptionMacro("Cannot make up-to-date buffer because both CPU and GPU buffers are dirty");
    }

  this->UpdateArray();
  this->UpdateHostBuffer();

  m_IsArrayDirty = m_IsHostBufferDirty = false;

  return true;
}


template< typename TBuffer >
void
DataManager< TBuffer >
::Graft(DataManager* data)
{
  if( data )
    {
    m_ArrayDimensions = data->GetArrayDimensions();

    m_Array = data->GetModifiableArray();

    m_HostBuffer = data->GetHostBufferPointer();

    m_IsHostBufferDirty = data->m_IsHostBufferDirty;
    m_IsArrayDirty = data->m_IsArrayDirty;
    }
}


template< typename TBuffer >
void
DataManager< TBuffer >
::Initialize()
{
  for( unsigned int dim = 0; dim < 4; ++dim )
    {
    m_ArrayDimensions[dim] = 0;
    }
  m_Array = ITK_NULLPTR;
  m_HostBuffer = ITK_NULLPTR;
  m_IsArrayDirty = false;
  m_IsHostBufferDirty = false;
}


template< typename TBuffer >
void
DataManager< TBuffer >
::PrintSelf(std::ostream & os, Indent indent) const
{
  os << indent << "DataManager (" << this << ")" << std::endl;
  for( unsigned int dim = 0; dim < 4; ++dim )
    {
    os << indent << "m_ArrayDimensions[" << dim << "]: " << m_ArrayDimensions[dim] << std::endl;
    }
  os << indent << "m_IsArrayDirty: " << m_IsArrayDirty << std::endl;
  os << indent << "m_Array: " << m_Array << std::endl;
  os << indent << "m_IsHostBufferDirty: " << m_IsHostBufferDirty << std::endl;
  os << indent << "m_HostBuffer: " << m_HostBuffer << std::endl;
}

} // namespace af
} // namespace itk

#endif
