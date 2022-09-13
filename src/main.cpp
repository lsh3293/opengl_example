#include "common.h"
#include "shader.h"
#include <spdlog/spdlog.h>
#include <glad/glad.h> // 반드시 GLFW 라이브러리 이전에 추가할 것
#include <GLFW/glfw3.h>

// #define WINDOW_NAME "Hello, OpenGL"
// #define WINDOW_WIDTH 960
// #define WINDOW_HEIGHT 540

void OnFramebufferSizeChange(GLFWwindow* window, int width, int height) {
    SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
    // OpenGL이 그림을 그릴 화면의 위치 및 크기 설정 (x, y, width, height)
    glViewport(0, 0, width, height);
}

void OnKeyEvent(GLFWwindow* window,
    int key, int scancode, int action, int mods) {
    SPDLOG_INFO("key: {}, scancode: {}, action: {}, mods: {}{}{}",
        key, scancode,
        action == GLFW_PRESS ? "Pressed" :
        action == GLFW_RELEASE ? "Released" :
        action == GLFW_REPEAT ? "Repeat" : "Unknown",
        mods & GLFW_MOD_CONTROL ? "C" : "-",
        mods & GLFW_MOD_SHIFT ? "S" : "-",
        mods & GLFW_MOD_ALT ? "A" : "-");
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int argc, const char** argv) {
    SPDLOG_INFO("Start program");

    // glfw 라이브러리 초기화, 실패하면 에러 출력 후 종료
    SPDLOG_INFO("Initialize glfw");
    if (!glfwInit()) {
        const char* description = nullptr;
        glfwGetError(&description);
        SPDLOG_ERROR("failed to initialize glfw: {}", description);
        return -1;
    }
    
    // GLFW 윈도우를 생성하기 전 만들기 희망하는 OpenGL 버전 추가 (3.3.0)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw 윈도우 생성, 실패하면 에러 출력 후 종료
    SPDLOG_INFO("Create glfw window");
    auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (!window) {
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return -1;
    }
    
    /**
     * 여러 개의 윈도우가 동시에 OpenGL 사용할 수 있기 때문에
     * 명시적으로 윈도우 생성과 함께 생성된 OpenGL Context를 사용하는 작업
    */
	glfwMakeContextCurrent(window);

    // glad를 활용한 OpenGL 함수 로딩
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        SPDLOG_ERROR("failed to initialize glad");
        glfwTerminate();
        return -1;
    }

    // OpenGL Function test
    auto glVersion = glGetString(GL_VERSION);
    SPDLOG_INFO("OpenGL context version: {}", glVersion);

    auto vertexShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
    auto fragmentShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
    SPDLOG_INFO("vertex shader id: {}", vertexShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragmentShader->Get());

    OnFramebufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
    glfwSetKeyCallback(window, OnKeyEvent);

    // 컬러 프레임버퍼 화면을 클리어할 색상 지정
    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
    
    // glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료
    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        // 프레임버퍼 클리어
        glClear(GL_COLOR_BUFFER_BIT);

        /**
         * FRAMEBUFFER SWAP
          - 화면에 그림을 그리는 과정
          - 프레임버퍼 2개를 준비 (front / back)
          - back buffer에 그림 그리기
          - front와 back을 바꿔치기
          - 위의 과정을 반복
        */
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
