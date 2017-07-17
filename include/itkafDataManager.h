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

#ifndef itkafDataManager_h
#define itkafDataManager_h

#include "arrayfire.h"

#include "itkObject.h"
#include "itkDataObject.h"
#include "itkObjectFactory.h"
#include "itkSimpleFastMutexLock.h"
#include "itkMutexLockHolder.h"

namespace itk
{
namespace af
{

/** \class DataManager
 * \brief Memory manager implemented for ArrayFire. Required by itk::af::Image class.
 *
 * This class serves as a base class for GPU data container for itk::af::Image class,
 * which is similar to ImageBase class for Image class. However, all the image-related
 * meta data will be already stored in image class (parent of af::Image), therefore
 * we did not name it af::ImageBase. Rather, this class is a GPU-specific data manager
 * that provides functionalties for CPU-GPU data synchronization and grafting GPU data.
 *
 * \ingroup ITKArrayFire
 */
template< typename TBuffer >
class DataManager: public Object
{
public:
  typedef DataManager              Self;
  typedef Object                   Superclass;
  typedef SmartPointer<Self>       Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);
  itkTypeMacro(DataManager, Object);

  typedef MutexLockHolder<SimpleFastMutexLock> MutexHolderType;

  void SetArrayDimensions( const ::af::dim4 & dims );
  ::af::dim4 GetArrayDimensions() const;

  void SetHostBufferPointer( TBuffer* ptr );

  void SetHostDirtyFlag( bool isDirty );

  void SetArrayDirtyFlag( bool isDirty );

  /** Make af::array up-to-date and mark host as dirty.
   * Call this function when you want to modify host data */
  void SetHostBufferDirty();

  /** Make host up-to-date and mark af::array as dirty.
   * Call this function when you want to modify af::array data */
  void SetArrayDirty();

  bool IsHostBufferDirty() {
    return m_IsHostBufferDirty;
  }

  bool IsArrayDirty() {
    return m_IsArrayDirty;
  }

  /** actual af::array->host memory copy takes place here */
  virtual void UpdateHostBuffer();

  /** actual af::array->host memory copy takes place here */
  virtual void UpdateArray();

  void Allocate();

  /** Synchronize host and af::array buffers (using dirty flags) */
  bool Update();

  /** Method for grafting the content of one DataManager into another one */
  virtual void Graft(DataManager* data);

  /** Initialize DataManager */
  virtual void Initialize();

  /** Get af::array */
  ::af::array* GetModifiableArray();

  /** Get host buffer pointer */
  TBuffer* GetHostBufferPointer();

protected:

  DataManager();
  virtual ~DataManager();
  virtual void PrintSelf(std::ostream & os, Indent indent) const ITK_OVERRIDE;

protected:
  /* NOTE: ivars are protected instead of private to improve performance access in child classes*/

  ::af::dim4     m_ArrayDimensions;

  /** buffer pointers */
  ::af::array* m_Array;
  TBuffer*     m_HostBuffer;

  /** checks if buffer needs to be updated */
  bool m_IsArrayDirty;
  bool m_IsHostBufferDirty;

  /** Mutex lock to prevent r/w hazard for multithreaded code */
  SimpleFastMutexLock m_Mutex;

private:
  ITK_DISALLOW_COPY_AND_ASSIGN(DataManager);
};

} // namespace af
} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkafDataManager.hxx"
#endif

#endif
