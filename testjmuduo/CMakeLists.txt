INCLUDE_DIRECTORIES(BEFORE "/home/hqwu/jmuduo/src")
LINK_DIRECTORIES("/home/hqwu/jmuduo/build/lib")
add_compile_options(-std=c++11)
ADD_EXECUTABLE(testjmuduo testCountDownLatch.cc)
TARGET_LINK_LIBRARIES(testjmuduo jmuduo pthread)

ADD_EXECUTABLE(test1 test1.cc)
TARGET_LINK_LIBRARIES(test1 jmuduo pthread)


ADD_EXECUTABLE(test02 test02.cc)
TARGET_LINK_LIBRARIES(test02 jmuduo pthread)

ADD_EXECUTABLE(test3 test3.cc)
TARGET_LINK_LIBRARIES(test3 jmuduo pthread)

ADD_EXECUTABLE(test4 test4.cc)
TARGET_LINK_LIBRARIES(test4 jmuduo pthread)

ADD_EXECUTABLE(test7 test7.cc)
TARGET_LINK_LIBRARIES(test7 jmuduo pthread)

ADD_EXECUTABLE(test8 test8.cc)
TARGET_LINK_LIBRARIES(test8 jmuduo pthread)

ADD_EXECUTABLE(test10 test10.cc)
TARGET_LINK_LIBRARIES(test10 jmuduo pthread)