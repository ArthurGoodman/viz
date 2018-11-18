#pragma once

#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QtGui/QSurfaceFormat>
#include "DataProvider.hpp"

namespace viz {

class CView;

class CRenderer final : public QObject
{
    Q_OBJECT

public: // methods
    explicit CRenderer(CDataProvider &data_provider);

    QSurfaceFormat format() const
    {
        return m_format;
    }

    void setView(CView *view);

    void rotate(const QQuaternion &q);
    QQuaternion getRotation() const;

    void translate(const QVector3D &delta);
    QVector3D getTranslation() const;

    void scale(float factor);
    float getScale() const;

    QMatrix4x4 getMVP() const;

public slots:
    void render();

private: // methods
    void initialize();

private: // fields
    CDataProvider &m_data_provider;

    QVector<QVector3D> m_vertices;
    int m_vertexAttr;
    int m_matrixUniform;
    int m_colorUniform;

    bool m_initialized;
    QSurfaceFormat m_format;
    QOpenGLContext *m_context;
    QOpenGLShaderProgram *m_program;

    CView *m_view;

    QQuaternion m_rotation;
    QVector3D m_translation;
    float m_scale;
};

} // namespace viz
