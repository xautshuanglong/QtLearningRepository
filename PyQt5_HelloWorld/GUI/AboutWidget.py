# -*- coding: utf-8 -*-

from Generated.AboutWidget_ui import Ui_AboutWidget
from PyQt5.QtWidgets import QWidget


class AboutWidget(QWidget):

    def __init__(self):
        super().__init__()

        self.ui = Ui_AboutWidget()
        self.ui.setupUi(self)

        self.ui.btnClose.clicked.connect(self.on_btnClose_clicked)

    def resizeEvent(self, event):
        pass

    def on_btnClose_clicked(self):
        self.hide()
