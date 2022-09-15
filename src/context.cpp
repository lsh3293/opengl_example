#include "context.h"
#include "image.h"

ContextUPtr Context::Create() {
    auto context = ContextUPtr(new Context());
    if (!context->Init())
        return nullptr;
    return std::move(context);
}

bool Context::Init() {
    float vertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
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
        ※ 순서 주의
        vertex attribute을 설정하기 전에 VBO가 바인딩 되어있을 것
    */
    // 1. VAO binding
    m_vertexLayout = VertexLayout::Create(); 

    // 2. VBO binding
    m_vertexBuffer = Buffer::CreateWithData( 
        GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 32);

    // 3. vertex attribute setting
	m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0); 
	m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3);
    m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 6);

    m_indexBuffer = Buffer::CreateWithData(
        GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);

    ShaderPtr vertShader = Shader::CreateFromFile("./shader/texture.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/texture.fs", GL_FRAGMENT_SHADER);
    if (!vertShader || !fragShader)
        return false;
    SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());

    m_program = Program::Create({fragShader, vertShader});
    if (!m_program)
        return false;
    SPDLOG_INFO("program id: {}", m_program->Get());

    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

    auto image = Image::Load("./image/container.jpg");
    if (!image) 
        return false;
    SPDLOG_INFO("image: {}x{}, {} channels",image->GetWidth(), image->GetHeight(), image->GetChannelCount());

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    /*
        바인딩된 텍스처의 크기 / 픽셀 포맷을 설정하고 GPU에 이미지 데이터를 복사
        target: 대상이 될 바인딩 텍스처
        level: 설정할 텍스처 레벨. 0레벨이 base. 추후 설명
        internalFormat: 텍스처의 픽셀 포맷
        width: 텍스처 / 이미지의 가로 크기
        height: 텍스처 / 이미지의 세로 크기
        border: 텍스처 외곽의 border 크기
        format: 입력하는 이미지의 픽셀 포맷
        type: 입력하는 이미지의 채널별 데이터 타입
        data: 이미지 데이터가 기록된 메모리 주소
    */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
        image->GetWidth(), image->GetHeight(), 0,
        GL_RGB, GL_UNSIGNED_BYTE, image->GetData());

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
