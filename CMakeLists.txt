cmake_minimum_required(VERSION 2.6)

project(GamePlay)

set(GAMEPLAY_VERSION 1.4.0)

# gameplay library
add_subdirectory(gameplay)

# gameplay samples
add_subdirectory(gameplay-samples/sample00-mesh)
#add_subdirectory(gameplay-samples/sample01-longboard)
#add_subdirectory(gameplay-samples/sample02-spaceship)
#add_subdirectory(gameplay-samples/sample03-character)
#add_subdirectory(gameplay-samples/sample04-particles)
#add_subdirectory(gameplay-samples/sample05-lua)

