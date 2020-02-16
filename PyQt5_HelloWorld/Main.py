# -*- coding: utf-8 -*-

# 打包单个 exe : (适用于在 Release 文件夹中运行)
# pyinstaller -i E:\VS2017\QtLearningRepository\PyQt5_HelloWorld\Resources\HelloWorld.ico -F ../Main.py -w --clean
#

import sys

from PyQt5.QtGui import QIcon
from PyQt5.QtWidgets import QApplication
from GUI import HelloWorldMainWindow
from Servers import start_websocket_thread, stop_websocket_thread


if __name__ == '__main__':

    start_websocket_thread('localhost', 7899)

    app = QApplication(sys.argv)

    mainWin = HelloWorldMainWindow()
    mainWin.setWindowIcon(QIcon(':/HelloWorld.ico'))
    mainWin.setWindowTitle('Simple')
    mainWin.show()

    status = app.exec()
    stop_websocket_thread('localhost', 7899)
    sys.exit(status)
