set(ITKArrayFire_SRCS
  #itkafDataManager.cxx
  )

#itk_module_add_library(ITKArrayFire ${ITKArrayFire_SRCS})
#target_link_libraries(ITKArrayFire LINK_PUBLIC ${ArrayFire_LIBRARIES})

#if(ArrayFire_CPU_FOUND)
  #target_link_libraries(ITKArrayFire LINK_PUBLIC ${ArrayFire_CPU_LIBRARIES}
    #${CMAKE_THREAD_LIBS_INIT})
#endif()

#find_package(OpenCL)
#if(ArrayFire_OpenCL_FOUND AND OpenCL_FOUND)
  #target_link_libraries(ITKArrayFire LINK_PUBLIC ${ArrayFire_OpenCL_LIBRARIES}
    #${CMAKE_THREAD_LIBS_INIT})
#endif()

#find_package(CUDA)
#if(ArrayFire_CUDA_FOUND AND CUDA_FOUND)
  #find_package(CUDA REQUIRED)
  #find_package(NVVM REQUIRED)
  #target_link_libraries(ITKArrayFire LINK_PUBLIC ${ArrayFire_CUDA_LIBRARIES}
    #${NVVM_LIB}
    #${CMAKE_THREAD_LIBS_INIT})
#endif()
