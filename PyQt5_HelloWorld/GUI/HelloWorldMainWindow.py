# -*- coding: utf-8 -*-

from Generated.Ui_HelloWorldMainWindow import Ui_HelloWorldMainWindow
from .AboutWidget import AboutWidget
from PyQt5.QtWidgets import QMainWindow


class HelloWorldMainWindow(QMainWindow):

    def __init__(self):
        super().__init__()

        self.ui = Ui_HelloWorldMainWindow()
        self.ui.setupUi(self)

        self.about_widget = AboutWidget()
        self.about_widget.setParent(self)
        self.about_widget.setVisible(False)

        self.count = 0
        self.le_input = None
        self.init_ui()

    def init_ui(self):
        self.ui.btnTest.clicked.connect(self.on_btn_test_clicked)

    def resizeEvent(self, event):
        self.about_widget.resize(event.size())

    def on_btn_test_clicked(self):
        self.count += 1
        self.ui.leInputTest.setText("test %d" % self.count)
        self.about_widget.setVisible(True)
