# ExternalProject 관련 명령어 셋 추가
include(ExternalProject)

# Dependency 관련 변수 설정
set(DEP_INSTALL_DIR ${PROJECT_BINARY_DIR}/install) # PROJECT_BINARY_DIR 은 상수
set(DEP_INCLUDE_DIR ${DEP_INSTALL_DIR}/include)
set(DEP_LIB_DIR ${DEP_INSTALL_DIR}/lib)

# spdlog: fast logger library
# 다운로드 받아서 CMAKE 실행
ExternalProject_Add(
    dep_spdlog # 프로젝트 컴파일할 때 대표 이름 선언
    GIT_REPOSITORY "https://github.com/gabime/spdlog.git" # 사용하려고 하는 라이브러리의 Git 주소
    GIT_TAG "v1.x" # 버전 세팅 (branch 이름 또는 Tag 이름)
    GIT_SHALLOW 1 # 최신의 것만 다운로드 받을지 여부 0 - False, 1 - True
    UPDATE_COMMAND "" # for no update step, 업데이트 필요 없이 안정화된 버전 사용
    PATCH_COMMAND "" # for no patch step, 코드를 고치고 싶을 때 사용
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR} # CMAKE Configure 인자, -D: Definition, 글로벌 변수 CMAKE_INSTALL_PREFIX 할당
    TEST_COMMAND "" # for no test step
)
# Dependency 리스트 및 라이브러리 파일 리스트 추가
set(DEP_LIST ${DEP_LIST} dep_spdlog)
 # $<>: generator 문법, CONFIG 가 Debug 모드면 d 를 붙이라는 의미
 # Debug 환경에서 빌드 시 set(DEP_LIBS ${DEP_LIBS} spdlogd) 
 # <- DEP_LIBS = ["spdlogd.lib"]
set(DEP_LIBS ${DEP_LIBS} spdlog$<$<CONFIG:Debug>:d>)

ExternalProject_Add(
    dep_glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw.git"
    GIT_TAG "3.3.3"
    GIT_SHALLOW 1
    UPDATE_COMMAND "" 
    PATCH_COMMAND "" 
    TEST_COMMAND ""
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
        # GLFW 의 CMakeLists.txt > option 확인
        -DGLFW_BUILD_EXAMPLES=OFF 
        -DGLFW_BUILD_TESTS=OFF
        -DGLFW_BUILD_DOCS=OFF
    )
set(DEP_LIST ${DEP_LIST} dep_glfw)
set(DEP_LIBS ${DEP_LIBS} glfw3)

# glad
ExternalProject_Add(
    dep_glad
    GIT_REPOSITORY "https://github.com/Dav1dde/glad"
    GIT_TAG "v0.1.34"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
        -DGLAD_INSTALL=ON
    TEST_COMMAND ""
    )
set(DEP_LIST ${DEP_LIST} dep_glad)
set(DEP_LIBS ${DEP_LIBS} glad)
