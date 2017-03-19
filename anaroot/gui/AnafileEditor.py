#!/usr/bin/env python

import sys
import re
import os
from PyQt4.QtCore import *
from PyQt4.QtGui import *

# pyuic4.bat myform.ui > ui_myform.py
from ui_AnafileEditor import *

class AnafileValue:
    def __init__(self):
        self.analyzer = {}
        self.analyzername = {}
        self.wname = {}
        self.wnum = {}
        self.comment = {}

        self.npattern = re.compile(r'\w+\s*=\s*[0-9]+\s*,')
        self.pattern = re.compile(r'\w+[\s,]*')
        self.wordpattern = re.compile(r'\w+')
        self.numpattern = re.compile(r'[0-9]+')
        self.anapattern = re.compile(r'namespace[a-zA-Z\s]+{')
        self.enumpattern = re.compile(r'enum\s+{')
        self.endpattern = re.compile(r'};')
        self.compattern = re.compile(r'\s*//')
        self.defpattern = re.compile(r'^#')

        eanalyzerfile = os.environ["TARTSYS"] + "/include/EAnalyser.hh"
        ewnumfile = os.environ["TARTSYS"] + "/include/EWNum.hh"
        self.readEAnalyzer(eanalyzerfile)
        self.readEWNum(ewnumfile)

    def readEAnalyzer(self, filename):    
        enum = 0
        for line in open(filename, 'r'):
            anamatch = self.anapattern.search(line)
            enummatch = self.enumpattern.search(line)
            endmatch = self.endpattern.search(line)
            commatch = self.compattern.match(line)
            defmatch = self.defpattern.match(line)
            inimatch = self.npattern.search(line)
            match = self.pattern.search(line)
            if anamatch or enummatch or endmatch or commatch or defmatch:
                i = 1
            elif inimatch:
                #print inimatch.group()
                wmatch = self.wordpattern.search(inimatch.group())
                nmatch = self.numpattern.findall(inimatch.group())
                enum = int(nmatch[len(nmatch)-1])
                self.analyzer[wmatch.group()] = enum
                self.analyzername[enum] = wmatch.group()
            elif match:    
                #print match.group()
                wmatch = self.wordpattern.search(match.group())
                enum += 1
                self.analyzer[wmatch.group()] = enum
                self.analyzername[enum] = wmatch.group()
#        for k, v in sorted(self.analyzer.items(), key=lambda x:x[1]):
#            print k, v
#        print self.analyzername        

    def readEWNum(self, filename):    
        mynames = []
        mywnums = []
        mycoms = []
        com = ''
        before_is_com = False
        enum = 0

        for line in open(filename, 'r'):
            anamatch = self.anapattern.search(line)
            enummatch = self.enumpattern.search(line)
            endmatch = self.endpattern.search(line)
            commatch = self.compattern.match(line)
            defmatch = self.defpattern.match(line)
            inimatch = self.npattern.search(line)
            match = self.pattern.search(line)
            if anamatch:
                words = re.search(r'(\w+)\s+(\w+)',anamatch.group())
                cword = words.group(2)
                myanalyzer = words.group(2)
                #print myanalyzer
            elif enummatch:
                mynames = []
                mywnums = []
                mycoms = []
                enum = 0
                before_is_com = False
            elif endmatch:
                #print mynames, len(mynames)
                self.wname[myanalyzer] = mynames
                #print mywnums, len(mywnums)
                self.wnum[myanalyzer] = mywnums
                #print mycoms, len(mycoms)
                self.comment[myanalyzer] = mycoms
            elif commatch:
                if not before_is_com:
                    com = ''
#                com += commatch.group()
                com += line
                before_is_com = True
            elif inimatch:
                #print inimatch.group()
                wmatch = self.wordpattern.search(inimatch.group())
                nmatch = self.numpattern.findall(inimatch.group())
                mynames.append(wmatch.group())
                #print nmatch[len(nmatch)-1]
                #print nmatch[len(nmatch)-1].group()
                mywnums.append(int(nmatch[len(nmatch)-1]))
                mycoms.append(com)
                enum = int(nmatch[len(nmatch)-1])
                before_is_com = False
            elif match:   
                #print match.group()
                wmatch = self.wordpattern.search(match.group())
                mynames.append(wmatch.group())
                enum += 1
                mywnums.append(enum)
                mycoms.append(com)
                before_is_com = False

class AnaTreeWidgetItem(QTreeWidgetItem):
    def __init__(self):
        QTreeWidgetItem.__init__(self)
        self.wnumid = -1
        self.analyzerid = -1

class MyMainWindow(QMainWindow):
    def __init__(self):
        QMainWindow.__init__(self)

        # Set up the user interface from Designer.
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.ui.htableWidget.resizeColumnsToContents()
        self.ui.ctableWidget.resizeColumnsToContents()
        self.avalue = AnafileValue()
        self.treeitems = []
        self.setuptree()

        self.chistoid = 0
        self.ccutid = 0

        # Connect up the buttons.
        self.ui.openButton.clicked.connect(self.readfile)
        self.ui.saveButton.clicked.connect(self.savefile)
        self.ui.saveasButton.clicked.connect(self.saveasfile)
        self.ui.quitButton.clicked.connect(self.quit)
        self.ui.lvWidget.itemDoubleClicked.connect(self.showwnumcom)

        self.ui.xysetButton.clicked.connect(self.setxy)
        self.ui.make1dhButton.clicked.connect(self.make1dhisto)
        self.ui.make2dhButton.clicked.connect(self.make2dhisto)
        self.ui.deletehButton.clicked.connect(self.deletehisto)

        self.ui.csetButton.clicked.connect(self.setcut)
        self.ui.makecButton.clicked.connect(self.makecut)
        self.ui.deletecButton.clicked.connect(self.deletecut)

    def setuptree(self):
        for k, v in sorted(self.avalue.analyzer.items(), key=lambda x:x[1]):
            aitem = QTreeWidgetItem()
            aitem.setText(0,"%s = %d" % (k, v))
            aitem.setData(0,Qt.CheckStateRole,Qt.Unchecked);
            aitem.setFlags(aitem.flags() & ~Qt.ItemIsSelectable);
            self.treeitems.append(aitem)
            treewnums = []
            cn = 0
            for wn in self.avalue.wname[k]:
                citem = AnaTreeWidgetItem()
                citem.analyzerid = v
                citem.wnumid = self.avalue.wnum[k][cn]
                citem.setText(0,"%s = %d" % (wn, self.avalue.wnum[k][cn]))
                citem.setToolTip(0,self.avalue.comment[k][cn])
                treewnums.append(citem)
                cn += 1
            aitem.insertChildren(0,treewnums)    
        self.ui.lvWidget.addTopLevelItems(self.treeitems) 

    def showwnumcom(self,item,column):
        ret = QMessageBox.information(self, item.text(column), item.toolTip(column), QMessageBox.Close);

####### functions for histo ##########

    def setxy(self):
        self.setv(self.ui.htableWidget)

    def is2d(self):
        return self.isthisrow2d(self.ui.htableWidget.currentRow())

    def isthisrow2d(self, row):
        return self.ui.htableWidget.rowSpan(row,0) == 2

    def deletehisto(self):
        ret = QMessageBox.question(self, "Delete Histogram", "Do you want to delete Histogram: %s?" % self.ui.htableWidget.item(self.ui.htableWidget.currentRow(),1).text(), QMessageBox.Yes | QMessageBox.No)
        if ret == QMessageBox.Yes :
            if self.is2d():
                self.ui.htableWidget.removeRow(self.ui.htableWidget.currentRow()+1)
                self.ui.htableWidget.removeRow(self.ui.htableWidget.currentRow())
            else:
                self.ui.htableWidget.removeRow(self.ui.htableWidget.currentRow())

    def make1dhisto(self):
        self.ui.htableWidget.setRowCount(self.ui.htableWidget.rowCount() + 1)
        rowid = self.ui.htableWidget.rowCount() - 1
        self.ui.htableWidget.selectRow(rowid)
        self.ui.htableWidget.setItem(rowid,0,QTableWidgetItem("0"))
        self.setxy()

    def make2dhisto(self):
        self.ui.htableWidget.setRowCount(self.ui.htableWidget.rowCount() + 2)
        rowid = self.ui.htableWidget.rowCount() - 2
        self.ui.htableWidget.selectRow(rowid)
        self.ui.htableWidget.setItem(rowid,0,QTableWidgetItem("0"))
        self.ui.htableWidget.setSpan(rowid,0,2,1)
        self.ui.htableWidget.setSpan(rowid,1,2,1)
        self.setxy()
        self.ui.htableWidget.selectRow(rowid+1)
        self.setxy()

####### functions for cut ##########

    def setcut(self):
        self.setv(self.ui.ctableWidget)

    def deletecut(self):
        ret = QMessageBox.question(self, "Delete Cut", "Do you want to delete Cut:%s ?" % self.ui.ctableWidget.item(self.ui.ctableWidget.currentRow(),0).text(), QMessageBox.Yes | QMessageBox.No)
        if ret == QMessageBox.Yes :
            self.ui.ctableWidget.removeRow(self.ui.ctableWidget.currentRow())

    def makecut(self):
        self.ui.ctableWidget.setRowCount(self.ui.ctableWidget.rowCount() + 1)
        rowid = self.ui.ctableWidget.rowCount() - 1
        self.ui.ctableWidget.selectRow(rowid)
        self.ui.ctableWidget.setItem(rowid,0,QTableWidgetItem("%d" % self.ccutid))
        self.ccutid += 1
        self.setcut()

####### other functions ##########

    def setv(self, table):
        items = self.ui.lvWidget.selectedItems()
        if not items:
            rowid = table.currentRow()
            for cc in range(1,table.columnCount()):
                item = table.item(rowid,cc)
                if not item:
                    table.setItem(rowid,cc,QTableWidgetItem("-1"))
            return
        rowid = table.currentRow()
        table.setItem(rowid,2,QTableWidgetItem("%s" % (items[0].analyzerid)))
        table.setItem(rowid,5,QTableWidgetItem("%s" % (items[0].wnumid)))

        item = table.item(rowid,1)
        if not item:
            title = "%s(WNum:%s)" % (self.avalue.analyzername[items[0].analyzerid], items[0].wnumid)
            table.setItem(rowid,1,QTableWidgetItem(title))

        item = table.item(rowid,3)
        if not item:
            table.setItem(rowid,3,QTableWidgetItem("0"))

        item = table.item(rowid,4)
        if not item:
            table.setItem(rowid,4,QTableWidgetItem("99"))

        for cc in range(5,table.columnCount()):
            item = table.item(rowid,cc)
            if not item:
                table.setItem(rowid,cc,QTableWidgetItem("-1"))

        self.ui.htableWidget.resizeColumnToContents(1)

    def readfile(self):
        fname = QFileDialog.getOpenFileName(self,"Open Anafile","")
        readfile_fromarg(fname)

    def readfile_fromarg(self, fname):
        analyspattern = re.compile(r'^<analys>')
        cutpattern = re.compile(r'^<gate>')
        histopattern = re.compile(r'^[0-9]')
        self.ui.anafileLabel.setText(fname)
        try:
            f = open(fname, 'r')
        except IOError:
            QMessageBox.information(self, "File open", 'except: Cannot open "%s"' % fname, QMessageBox.Close)
            return
        for n in range(0,self.ui.htableWidget.rowCount()):
            self.ui.htableWidget.removeRow(0)
        for n in range(0,self.ui.ctableWidget.rowCount()):
            self.ui.ctableWidget.removeRow(0)
        for line in f.readlines():
            analysmatch = analyspattern.match(line)
            cutmatch = cutpattern.match(line)
            histomatch = histopattern.match(line)
            ### analyzer line
            if analysmatch:
                line = line.replace("<analys>","")
                line = line.replace(" ","")
                ids = line.split(",")
                for aid in ids:
                    for item in self.treeitems:
                        if "%d" % item.child(0).analyzerid == aid:
                            item.setCheckState(0,Qt.Checked)
            ### read cut line
            if cutmatch:
                line = line.replace("<gate>","")
                line = line.replace(" ","")
                line = line.replace("\"","")
                line = line.rstrip()
                ids = line.split(",")
                self.makecut()
                rowid = self.ui.ctableWidget.currentRow()
                self.ui.ctableWidget.setItem(rowid,0,QTableWidgetItem(ids[0]))
                self.ui.ctableWidget.setItem(rowid,1,QTableWidgetItem(ids[len(ids)-1]))
                for i in range(1,7):
                    self.ui.ctableWidget.setItem(rowid,i+1,QTableWidgetItem(ids[i]))
                self.ui.ctableWidget.resizeColumnToContents(1)
            ### read histogram line
            if histomatch:
                line = line.replace(" ","")
                line = line.replace("\"","")
                line = line.rstrip()
                ids = line.split(",")
                rowid = 0
                if len(ids) == 9: # id histo
                    self.make1dhisto()
                    rowid = self.ui.htableWidget.currentRow()
                elif len(ids) == 16: # 2d histo
                    self.make2dhisto()
                    rowid = self.ui.htableWidget.currentRow()
                    for i in range(8,15):
                        self.ui.htableWidget.setItem(rowid,i-6,QTableWidgetItem(ids[i]))
                    rowid = rowid - 1    
                self.ui.htableWidget.setItem(rowid,0,QTableWidgetItem(ids[0]))
                self.ui.htableWidget.setItem(rowid,1,QTableWidgetItem(ids[len(ids)-1]))
                for i in range(1,8):
                    self.ui.htableWidget.setItem(rowid,i+1,QTableWidgetItem(ids[i]))
                self.ui.htableWidget.resizeColumnToContents(1)
        f.close()    

    def saveasfile(self):
        fname = QFileDialog.getSaveFileName(self,"Save Anafile","")
        self.ui.anafileLabel.setText(fname)
        self.savefile()

    def savefile(self):
        fname = self.ui.anafileLabel.text()
        if len(fname) == 0:
            fname = QFileDialog.getSaveFileName(self,"Save Anafile","")
            self.ui.anafileLabel.setText(fname)
        f = open(fname, 'w')
        com = "<analys> "
        for item in self.treeitems:
            if item.checkState(0):
                com += "%s, " % item.child(0).analyzerid
        com += "\n"        
        if len(com) > 10:
            f.write(com)
        for row in range(self.ui.ctableWidget.rowCount()):
            com = "<gate> %s, " % self.ui.ctableWidget.item(row,0).text()
            for cc in range(2,8):
                com += "%s, " % self.ui.ctableWidget.item(row,cc).text()
            com += "\"%s\"\n" % self.ui.ctableWidget.item(row,1).text()
            f.write(com)
        for row in range(self.ui.htableWidget.rowCount()):
            hitem = self.ui.htableWidget.item(row,0)
            if hitem:
                com = "%s, " % hitem.text()
                for cc in range(2,9):
                    com += "%s, " % self.ui.htableWidget.item(row,cc).text()
                if self.isthisrow2d(row):
                    for cc in range(2,9):
                        com += "%s, " % self.ui.htableWidget.item(row+1,cc).text()
                com += "\"%s\"\n" % self.ui.htableWidget.item(row,1).text()
                f.write(com)
        f.close()

    def quit(self):
        QtGui.QApplication.quit()

if __name__ == '__main__':

    app = QApplication(sys.argv)
    mainwindow = MyMainWindow()
    mainwindow.show()
    if len(sys.argv) > 1:
        mainwindow.readfile_fromarg(sys.argv[1])
    sys.exit(app.exec_())
