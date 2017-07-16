set(ITKArrayFire_SRCS
  itkMinimalStandardRandomVariateGenerator.cxx
  )

add_library(ITKArrayFire ${ITKArrayFire_SRCS})
itk_module_link_dependencies()
itk_module_target(ITKArrayFire)
