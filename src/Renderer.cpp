#include "Renderer.hpp"
#include <cmath>
#include <mutex>
#include <QtCore/QTimer>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFunctions_2_0>
#include "View.hpp"

namespace viz {

CRenderer::CRenderer(CDataProvider &data_provider)
    : m_data_provider{data_provider}
    , m_initialized(false)
{
    m_format.setDepthBufferSize(16);

    m_context = new QOpenGLContext(this);
    m_context->setFormat(m_format);
    m_context->create();
}

void CRenderer::setView(CView *view)
{
    m_view = view;
}

void CRenderer::render()
{
    if (!m_context->makeCurrent(m_view))
        return;

    QSize viewSize = m_view->size();

    if (!m_initialized)
    {
        initialize();
        m_initialized = true;
    }

    QOpenGLFunctions_2_0 *f =
        m_context->versionFunctions<QOpenGLFunctions_2_0>();
    f->glViewport(
        0,
        0,
        viewSize.width() * m_view->devicePixelRatio(),
        viewSize.height() * m_view->devicePixelRatio());
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    f->glClearColor(0, 0, 0, 0);
    f->glFrontFace(GL_CW);
    f->glCullFace(GL_FRONT);
    f->glEnable(GL_CULL_FACE);
    f->glEnable(GL_DEPTH_TEST);
    f->glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    m_program->bind();
    m_vbo.bind();

    m_program->enableAttributeArray(m_vertexAttr);
    m_program->setAttributeBuffer(m_vertexAttr, GL_FLOAT, 0, 3);

    QMatrix4x4 mvp;

    mvp.perspective(45, (float)m_view->width() / m_view->height(), 1, 100);

    mvp.lookAt(QVector3D{0, 0, -3}, QVector3D{0, 0, 0}, QVector3D{0, 1, 0});

    mvp.rotate(m_fAngle, 0.0f, 1.0f, 0.0f);
    mvp.rotate(m_fAngle, 1.0f, 0.0f, 0.0f);
    mvp.rotate(m_fAngle, 0.0f, 0.0f, 1.0f);
    mvp.translate(0.0f, -0.2f, 0.0f);

    m_program->setUniformValue(m_matrixUniform, mvp);
    m_program->setUniformValue(m_colorUniform, QColor{Qt::white});

    f->glLineWidth(2);
    // f->glDrawArrays(GL_POINTS, 0, m_vertices.size());

    f->glBegin(GL_LINE_STRIP);

    {
        std::unique_lock<CDataProvider> guard{m_data_provider};

        for (const auto &point : m_data_provider)
        {
            f->glVertex2f(point.x(), point.y());
        }
    }

    f->glEnd();

    m_context->swapBuffers(m_view);

    m_fAngle += 1.0f;

    QTimer::singleShot(16, this, &CRenderer::render);
}

void CRenderer::initialize()
{
    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vshader->compileSourceFile(":/shaders/vertex.glsl");

    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fshader->compileSourceFile(":/shaders/fragment.glsl");

    m_program = new QOpenGLShaderProgram(this);
    m_program->addShader(vshader);
    m_program->addShader(fshader);
    m_program->link();
    m_program->bind();

    m_vertexAttr = m_program->attributeLocation("vertex");
    m_matrixUniform = m_program->uniformLocation("mvp");
    m_colorUniform = m_program->uniformLocation("color");

    m_fAngle = 0;

    m_vbo.create();
    m_vbo.bind();
    const int verticesSize = m_vertices.count() * 3 * sizeof(GLfloat);
    m_vbo.allocate(verticesSize * 2);
    m_vbo.write(0, m_vertices.constData(), verticesSize);
}

} // namespace viz

