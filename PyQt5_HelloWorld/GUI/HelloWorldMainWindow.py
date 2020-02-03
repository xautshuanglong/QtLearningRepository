# -*- coding: utf-8 -*-

from PyQt5.QtWidgets import QMainWindow, QLabel, QLineEdit, QPushButton


class HelloWorldMainWindow(QMainWindow):

    def __init__(self):
        super().__init__()

        self.count = 0
        self.le_input = None
        self.init_ui()

    def init_ui(self):
        lbl1 = QLabel('First QLabel', self)
        lbl1.move(15, 10)

        lbl2 = QLabel('Second QLabel', self)
        lbl2.move(35, 40)

        lbl3 = QLabel('Third QLabel', self)
        lbl3.move(55, 70)

        lb_input = QLabel('Input Test:', self)
        lb_input.move(15, 100)

        self.le_input = QLineEdit(self)
        self.le_input.move(90, 100)

        btn_test = QPushButton("Test Button", self)
        btn_test.move(200, 100)
        btn_test.clicked.connect(self.on_btn_test_clicked)

    def on_btn_test_clicked(self):
        self.count += 1
        self.le_input.setText("test %d" % self.count)
