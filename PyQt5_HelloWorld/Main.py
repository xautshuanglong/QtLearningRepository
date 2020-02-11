# -*- coding: utf-8 -*-

# 打包单个 exe : (适用于在 Release 文件夹中运行)
# pyinstaller -i E:\VS2017\QtLearningRepository\PyQt5_HelloWorld\Resources\HelloWorld.ico -F ../Main.py -w --clean
#

import sys

from PyQt5.QtGui import QIcon
from PyQt5.QtWidgets import QApplication
from GUI import HelloWorldMainWindow

if __name__ == '__main__':

    app = QApplication(sys.argv)

    mainWin = HelloWorldMainWindow()

    mainWin.setWindowIcon(QIcon(':/HelloWorld.ico'))
    mainWin.setWindowTitle('Simple')
    mainWin.show()

    status = app.exec()
    sys.exit(status)
