// Copyright 2017 Patrick Flynn
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//	this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this
//	list of conditions and the following disclaimer in the documentation and/or
//	other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may
//	be used to endorse or promote products derived from this software
//	without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include <QKeyEvent>
#include <QApplication>
#include <QSplitter>
#include <QPixmap>
#include <QVariant>
#include <settings.hh>

#include "window.hh"
#include "videopane.hh"
#include "control.hh"
#include "seekbar.hh"
#include "tray.hh"
#include "filemenu.hh"
#include "editmenu.hh"
#include "viewmenu.hh"
#include "helpmenu.hh"
#include "actions.hh"

Window::Window() {
    this->setWindowTitle("Media Player");
    this->resize(800,700);
    this->setAttribute(Qt::WA_QuitOnClose);

    QPixmap winIcon(":/icons/cpp-media-player.png");
#ifdef NO_THEME_ICONS
    this->setWindowIcon(winIcon);
#else
    this->setWindowIcon(QIcon::fromTheme("applications-multimedia",winIcon));
#endif

    FileMenu *fileMenu = new FileMenu;
    EditMenu *editMenu = new EditMenu;
    ViewMenu *viewMenu = new ViewMenu;
    HelpMenu *helpMenu = new HelpMenu;

    this->menuBar()->addMenu(fileMenu);
    this->menuBar()->addMenu(editMenu);
    this->menuBar()->addMenu(viewMenu);
    this->menuBar()->addMenu(helpMenu);
    
    QSplitter *splitter = new QSplitter;
    this->setCentralWidget(splitter);
    
    playlist = new PlayList;
    splitter->addWidget(playlist);

    QWidget *videopanel = new QWidget;
    videopanel->setStyleSheet("background-color:black;");
    
    QVBoxLayout *videolayout = new QVBoxLayout;
    videopanel->setLayout(videolayout);
    splitter->addWidget(videopanel);

    videopane = new VideoPane;
    videolayout->addWidget(videopane);

    seek = new SeekBar;
    videolayout->addWidget(seek);

    controller = new ControlBar;
    this->addToolBar(Qt::BottomToolBarArea,controller);

    systray = new SysTray;
    if (QVariant(Settings::getSetting("taskbar/icon","true")).toBool()) {
        systray->show();
    } else {
        systray->hide();
    }
    
    isHeadless = false;
}

Window::~Window() {
    delete videopane;
    delete controller;
    delete seek;
}

void Window::setHeadless(bool headless) {
	if (headless) {
		menubar->hide();
		playlist->hide();
		controller->hide();
		seek->hide();
	} else {
		menubar->show();
		playlist->show();
		controller->show();
		seek->show();
	}
}

void Window::keyPressEvent(QKeyEvent *event) {
	if ((event->modifiers()==Qt::ControlModifier)and(event->key()==Qt::Key_Q)) {
		qApp->exit();
	} else if ((event->modifiers()==Qt::ControlModifier)and(event->key()==Qt::Key_O)) {
		Actions::open();
	} else if (event->key()==Qt::Key_F11) {
		Actions::setWindowFullscreen();
	} else if ((event->modifiers()==Qt::ControlModifier)and(event->key()==Qt::Key_H)) {
		if (isHeadless) {
			isHeadless = false;
		} else {
			isHeadless = true;
		}
		this->setHeadless(isHeadless);
	}
}

void Window::closeEvent(QCloseEvent *event) {
    delete systray;
    event->accept();
}
