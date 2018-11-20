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
    , m_scale{1.0f}
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

void CRenderer::rotate(const QQuaternion &q)
{
    m_rotation *= q;
    m_rotation.normalize();
}

QQuaternion CRenderer::getRotation() const
{
    return m_rotation;
}

void CRenderer::translate(const QVector3D &delta)
{
    m_translation += delta;
}

QVector3D CRenderer::getTranslation() const
{
    return m_translation;
}

void CRenderer::scale(float factor)
{
    m_scale *= factor;
}

float CRenderer::getScale() const
{
    return m_scale;
}

QMatrix4x4 CRenderer::getViewProjection() const
{
    QMatrix4x4 view_projection;

    static constexpr float c_fov = 45.0f;
    static constexpr float c_z_near = 1e-5f;
    static constexpr float c_z_far = 1e5f;
    static constexpr float c_camera_height = 10.0f;

    view_projection.perspective(
        c_fov, (float)m_view->width() / m_view->height(), c_z_near, c_z_far);
    view_projection.lookAt(
        QVector3D{0, 0, c_camera_height / m_scale},
        QVector3D{0, 0, 0},
        QVector3D{0, 1, 0});

    return view_projection;
}

QMatrix4x4 CRenderer::getMVP() const
{
    QMatrix4x4 mvp = getViewProjection();

    mvp.rotate(m_rotation);
    mvp.translate(m_translation);

    return mvp;
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

    m_program->enableAttributeArray(m_vertexAttr);
    m_program->setAttributeBuffer(m_vertexAttr, GL_FLOAT, 0, 3);

    m_program->setUniformValue(m_matrixUniform, getMVP());

    static constexpr float c_ref_scale = 1.0f;

    m_program->setUniformValue(m_colorUniform, QColor{Qt::red});
    f->glLineWidth(2);
    f->glBegin(GL_LINES);
    f->glVertex3f(0.0f, 0.0f, 0.0f);
    f->glVertex3f(c_ref_scale, 0.0f, 0.0f);
    f->glEnd();

    m_program->setUniformValue(m_colorUniform, QColor{Qt::green});
    f->glLineWidth(2);
    f->glBegin(GL_LINES);
    f->glVertex3f(0.0f, 0.0f, 0.0f);
    f->glVertex3f(0.0f, c_ref_scale, 0.0f);
    f->glEnd();

    m_program->setUniformValue(m_colorUniform, QColor{Qt::blue});
    f->glLineWidth(2);
    f->glBegin(GL_LINES);
    f->glVertex3f(0.0f, 0.0f, 0.0f);
    f->glVertex3f(0.0f, 0.0f, c_ref_scale);
    f->glEnd();

    m_program->setUniformValue(m_colorUniform, QColor{Qt::white});
    f->glLineWidth(2);
    {
        std::unique_lock<CDataProvider> guard{m_data_provider};

        for (const auto &segment : m_data_provider)
        {
            f->glBegin(GL_LINE_STRIP);
            for (const auto &point : segment)
            {
                f->glVertex3f(point.x(), point.y(), point.z());
            }
            f->glEnd();
        }
    }

    QMatrix4x4 view_projection = getViewProjection();
    view_projection.scale(1.0f / m_scale);
    m_program->setUniformValue(m_matrixUniform, view_projection);

    static constexpr float c_crosshair_scale = 0.5f;

    m_program->setUniformValue(m_colorUniform, QColor{Qt::yellow});
    f->glLineWidth(2);
    f->glBegin(GL_LINES);
    f->glVertex3f(0.0f, 0.0f, 0.0f);
    f->glVertex3f(c_crosshair_scale, 0.0f, 0.0f);
    f->glVertex3f(0.0f, 0.0f, 0.0f);
    f->glVertex3f(0.0f, c_crosshair_scale, 0.0f);
    f->glEnd();

    m_context->swapBuffers(m_view);

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
}

} // namespace viz

