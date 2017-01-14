cmake_minimum_required(VERSION 2.8)
PROJECT(findBook) 
 
FIND_PACKAGE( OpenCV REQUIRED )   
ADD_EXECUTABLE(findBook main.cpp)  
TARGET_LINK_LIBRARIES (findBook ${OpenCV_LIBS} )   #这两行的次序也不能变!
