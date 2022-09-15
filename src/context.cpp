#include "context.h"

ContextUPtr Context::Create() {
    auto context = ContextUPtr(new Context());
    if (!context->Init())
        return nullptr;
    return std::move(context);
}

bool Context::Init() {
    float vertices[] = {
        0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, // top left
    };

    /*
        정점의 인덱스를 저장할 수 있는 버퍼
        인덱스 버퍼라고도 부름
        정점 정보와 별개로 정점의 인덱스로만 구성된 삼각형 정보를 전달 가능
        indexed drawing
    */
    uint32_t indices[] = { // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    /*
        순서 주의
        1. VAO binding
        2. VBO binding
        3. vertex attribute setting
        ※ vertex attribute을 설정하기 전에 VBO가 바인딩 되어있을 것
    */
    m_vertexLayout = VertexLayout::Create();

    m_vertexBuffer = Buffer::CreateWithData(
        GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 12);

	m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    m_indexBuffer = Buffer::CreateWithData(
        GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);

    ShaderPtr vertShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
    if (!vertShader || !fragShader)
        return false;
    SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());

    m_program = Program::Create({fragShader, vertShader});
    if (!m_program)
        return false;
    SPDLOG_INFO("program id: {}", m_program->Get());

    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

    return true;
}

void Context::Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->Use();
    /*
        현재 바인딩된 VAO, VBO, EBO를 바탕으로 그리기
        primitive: 그려낼 기본 primitive 타입
        count: 그리고자 하는 EBO 내 index의 개수
        type: index의 데이터형
        pointer/offset: 그리고자 하는 EBO의 첫 데이터로부터의 오프셋
    */
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
