#include "texturerenderer.hpp"

#include "moduleq.hpp"

#include <nmlib/codegeneration/glslgenerator.hpp>

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLContext>
#include <sstream>

namespace nmgui {

TextureRenderer::TextureRenderer(QQuickItem *the_parent) :
    QQuickItem(the_parent),
    m_program(0),
    m_generatorDirty(true),
    m_thread_generatorFunctionSource("void test(in vec2 coords, out float height){height = 0.5;}"),
    m_thread_sourceDirty(true),
    m_inputLink(nullptr),
    m_outputLink(nullptr),
    m_t(0),
    m_thread_t(0),
    m_domain(0,0,1,1),
    m_thread_domain(m_domain)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
}

void TextureRenderer::setT(qreal new_t)
{
    if (new_t == m_t)
        return;
    m_t = new_t;
    emit tChanged();
    if (window())
        window()->update();
}

void TextureRenderer::setInputLink(InputLinkQ *newLink)
{
    if(newLink==m_inputLink){
        return;
    }
    m_inputLink = newLink;
    emit inputLinkChanged();
    if(window()){
        window()->update();
    }
}

void TextureRenderer::setOutputLink(OutputLinkQ *newLink)
{
    if(newLink==m_outputLink){
        return;
    }
    //dicsonnect all signals, since we are about to forget about this input
    if(m_outputLink!=nullptr)disconnect(m_outputLink, 0, this, 0);
    m_outputLink = newLink;
    connect(m_outputLink->owner(), SIGNAL(dependenciesChanged()), this, SLOT(handleModelChanged()));
    emit outputLinkChanged();
    if(window()){
        window()->update();
    }
}

void TextureRenderer::handleWindowChanged(QQuickWindow *win)
{
    //This event is sent when we are first attached to a window
    if(win){
        // Connect the beforeRendering signal to our paint function.
        // Since this call is executed on the rendering thread it must be
        // a Qt::DirectConnection
        connect(win, SIGNAL(afterRendering()), this, SLOT(paint()), Qt::DirectConnection);
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
        win->setClearBeforeRendering(false);
    }
}

void TextureRenderer::handleModelChanged()
{
    m_generatorDirty = true;
}

void TextureRenderer::compileProgram()
{
    if(m_program){
        delete m_program;
        m_program = 0;
    } else {
        //if its the first time compiling
        connect(window()->openglContext(), SIGNAL(aboutToBeDestroyed()),
                this, SLOT(cleanup()), Qt::DirectConnection);
    }
    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                       "#version 130\n"
                                       "uniform vec4 domain;\n"
                                       "attribute highp vec4 vertices;\n"
                                       "varying highp vec2 coords;\n"
                                       "void main() {\n"
                                       "    gl_Position = vertices;\n"
                                       "    coords = vertices.xy*vec2(0.5,0.5)*domain.zw+vec2(0.5,0.5)+domain.xy;\n"
//                                       "    coords = vec2(domain.x,10);\n"
                                       "}\n");
    std::stringstream fs;
    fs << "#version 130\n";

//    fs << "float test(vec2 coords){return 0.5*coords.x;}";
    fs << m_thread_generatorFunctionSource;

    fs << "uniform lowp float t;\n"
          "varying highp vec2 coords;\n"
          "void main() {\n"
          "    float height;\n"
          "    test(coords, height);\n"
          "    gl_FragColor = vec4(height, height, height, 1);\n"
//          "    gl_FragColor = vec4(coords.x, coords.y, 1, 1);\n"
          "}\n";
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fs.str().c_str());

    m_program->bindAttributeLocation("vertices", 0);
    m_program->link();

    m_thread_sourceDirty = false;
}

void TextureRenderer::paint()
{
    if(!m_program || m_thread_sourceDirty){
        compileProgram();
    }
    m_program->bind();

    m_program->enableAttributeArray(0);

   float values[] = {
//        left, top,
//        right, top,
//        left,  bottom,
//        right, bottom
        -1,-1,
         1,-1,
        -1, 1,
         1, 1
    };
    m_program->setAttributeArray(0, GL_FLOAT, values, 2);
    m_program->setUniformValue("t", static_cast<float>(m_thread_t));

    float l = m_thread_domain.left();
    float t = m_thread_domain.top();
    float w = m_thread_domain.width();
    float h = m_thread_domain.height();
    QVector4D domain{l, t, w, h};
    m_program->setUniformValue("domain", domain);


    auto globalPos = mapToScene(position());
    glViewport(globalPos.x(), window()->height()-height()-globalPos.y(), width(), height());

    glDisable(GL_DEPTH_TEST);

    //        glClearColor(0, 0, 0, 1);
    //        glClear(GL_COLOR_BUFFER_BIT);

    //this looked fancy, but don't need it
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    //unbind everything
    m_program->disableAttributeArray(0);
    m_program->release();
}

void TextureRenderer::cleanup()
{
    if (m_program) {
        delete m_program;
        m_program = 0;
    }
}

void TextureRenderer::sync()
{
    m_thread_t = m_t;
    m_thread_domain = m_domain;
    if(m_generatorDirty){
        if(m_inputLink == nullptr || m_outputLink == nullptr){return;}
        auto source = nm::GlslGenerator::compileToGlslFunction(m_inputLink->inputLink(), m_outputLink->outputLink(), "test");
        m_thread_generatorFunctionSource = source;
        m_generatorDirty = false;
        m_thread_sourceDirty = true;
    }
}

} // namespace nmgui
