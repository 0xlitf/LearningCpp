// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "WindowWithOpenGL.h"

#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QPainter>

WindowWithOpenGL::WindowWithOpenGL(QWindow* parent)
    : QWindow(parent) {
    setSurfaceType(QWindow::OpenGLSurface);
}

WindowWithOpenGL::~WindowWithOpenGL() {
    delete m_device;
}

void WindowWithOpenGL::render(QPainter* painter) {
    Q_UNUSED(painter);
}

void WindowWithOpenGL::initialize() {
}

void WindowWithOpenGL::render() {
    if (!m_device)
        m_device = new QOpenGLPaintDevice;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_device->setSize(size() * devicePixelRatio());
    m_device->setDevicePixelRatio(devicePixelRatio());

    QPainter painter(m_device);
    render(&painter);
}


void WindowWithOpenGL::renderLater() {
    requestUpdate();
}

bool WindowWithOpenGL::event(QEvent* event) {
    switch (event->type()) {
        case QEvent::UpdateRequest:
            renderNow();
            return true;
        default:
            return QWindow::event(event);
    }
}

void WindowWithOpenGL::exposeEvent(QExposeEvent* event) {
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}


void WindowWithOpenGL::renderNow() {
    if (!isExposed())
        return;

    bool needsInitialize = false;

    if (!m_context) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();

        needsInitialize = true;
    }

    m_context->makeCurrent(this);

    if (needsInitialize) {
        initializeOpenGLFunctions();
        initialize();
    }

    render();

    m_context->swapBuffers(this);

    if (m_animating) {
        renderLater();
    }
}


void WindowWithOpenGL::setAnimating(bool animating) {
    m_animating = animating;

    if (animating) {
        renderLater();
    }
}


