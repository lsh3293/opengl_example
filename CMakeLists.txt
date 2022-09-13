cmake_minimum_required(VERSION 3.13) #cmake minimum version

set(PROJECT_NAME opengl_example)
# CMAKE로 시작하는 변수는 글로벌 변수들
# C++ 17 사용한다는 의미
set(CMAKE_CXX_STANDARD 17) # CMAKE_CXX_STANDARD = 17

# main.cpp 에서 사용할 환경 변수 설정
set(WINDOW_NAME "OpenGL Example")
set(WINDOW_WIDTH 960)
set(WINDOW_HEIGHT 540)

project(${PROJECT_NAME}) # 프로젝트 선언
add_executable(${PROJECT_NAME} src/main.cpp) # 실행파일을 만들 때 컴파일할 파일

include(Dependency.cmake) # Dependency.cmake 파일 불러오기

# 우리 프로젝트에 include / lib 관련 옵션 추가
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR}) # target(PROJECT_NAME) 을 컴파일 할 때 DEP_INCLUDE_DIR 이 필요하다.
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR}) # ./build/install/lib 디렉토리 링크
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS}) # 실제로 어떤 라이브러리를 사용할 것인지 지정

# Dependency들이 먼저 build 될 수 있게 관계 설정
add_dependencies(${PROJECT_NAME} ${DEP_LIST}) # 의존성 리스트 프로젝트를 먼저 컴파일하고 우리 프로젝트를 컴파일해라.

# 환경 변수를 predefined macro로 프로젝트에 추가
target_compile_definitions(${PROJECT_NAME} PUBLIC
  WINDOW_NAME="${WINDOW_NAME}"
  WINDOW_WIDTH=${WINDOW_WIDTH}
  WINDOW_HEIGHT=${WINDOW_HEIGHT}
)