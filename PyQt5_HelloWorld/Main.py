# -*- coding: utf-8 -*-

import sys

from PyQt5.QtGui import QIcon
from PyQt5.QtWidgets import QApplication
from GUI import HelloWorldMainWindow

if __name__ == '__main__':

    app = QApplication(sys.argv)

    mainWin = HelloWorldMainWindow()

    mainWin.setWindowIcon(QIcon(':/HelloWorld.ico'))
    mainWin.resize(400, 300)
    mainWin.setWindowTitle('Simple')
    mainWin.show()

    status = app.exec()
    sys.exit(status)
