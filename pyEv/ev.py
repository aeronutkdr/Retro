'''
..\\RetroEventMaestro\\2021eve\\bevent.exe -f 7,10,26-29,58-61 -i ANA202304090 2023ANA.EVA -y 2023 > out.txt
..\\RetroEventMaestro\\2021eve\\bevent.exe -f 7,10,26-29 -i ANA202304090 2023ANA.EVA -y 2023 > out.txt
number	field
------	-----
7	pitch sequence
10	batter
26	first runner*
27	second runner*
28	third runner*
29	event text*
58	batter dest* (5 if scores and unearned, 6 if team unearned)
59	runner on 1st dest* (5 if scores and unearned, 6 if team unearned)
60	runner on 2nd dest* (5 if scores and unearned, 6 if team unearned)
61	runner on 3rd dest* (5 if socres and uneanred, 6 if team unearned)
'''
import re
import os
import csv
import evProcess

# OO32 1HBB BSSF FFFF
class Game:
    def __init__(self, name):
        self.Name = name
        #self.Team = [Team()] * 2
        self.Version = 0
        self.Info = {}
        self.Lineup = [["" for _ in range(9)] for _ in range(2)]
        self.Position = [["" for _ in range(9)] for _ in range(2)]
        self.Bases = ["" for _ in range(4)]
        self.Inning = 0
        self.Bottom = False
        self.Out = 0
    def __str__(self):
               #+ "\nInfo = " + str(self.Info)\
               #+ "\nLineup = " + str(self.Lineup)\
               #+ "\nPosition = " + str(self.Position)\
        return "Game name = " + self.Name\
               + "\nVersion = " + str(self.Version)\
               + "\nInning = " + str(self.Inning)\
               + "\nBottom = " + str(self.Bottom)\
               + "\nOut = " + str(self.Out)\
               + "\nBases = " + str(self.Bases)\

    def Process(self,r):
        match r[0]:
            case "version": self.Version = int(r[1])
            case "info": self.Info[r[1]] = r[2]
            case "start" | "sub":
                team = int(r[3])
                ord = int(r[4])
                if (ord > 0) and (ord < 10):
                    for i in range (4):
                        if self.Lineup[team][ord-1] != '' and\
                           self.Bases[i] == self.Lineup[team][ord-1]:
                            self.Bases[i] = r[1]
                    self.Lineup[team][ord-1] = r[1]
                pos = int(r[5])
                if (pos < 10):
                    self.Position[team][pos-1] = r[1]
            case "play":
                States = []
                self.Inning = int(r[1])
                self.Bottom = r[2] == '1'
                #assert self.Inning == inning
                self.Bases[0] = r[3]
                #self.Count = r[4]
                #self.Pitches = r[5]
                #self.Event = r[6]
                b = self.Bases.copy()
                States = evProcess.GenSequence (self.Out,
                                                self.Bases,
                                                r[5],
                                                r[6])
                '''
                print ('\"{}\",{},\"{}\",{},[{}]'.format(r[5],
                                                     ','.join('\"'+B+'\"' for B in b),
                                                     r[6],
                                                     ','.join(str(s) for s in self.Bases),
                                                     ', '.join(hex(x) for x in States)))
                '''
                if (r[6] != "NP"):
                    print ('\"{}\",{},\"{}\"'.format(r[5],
                                                    ','.join('\"'+B+'\"' for B in b),
                                                    r[6]))
                self.Out = States[-1] >> 14
                if (self.Out == 3):
                    self.Out = 0
                    self.Bases = ["", "", "", ""]

            case "radj": self.Bases[int(r[2])] = r[1]
            case "com": None
            case "data": None
            case _: print(r[0])

def ProcessFile(s):
    g = []
    with open(s, mode='r') as file:
        csv_reader = csv.reader(file)
        for row in csv_reader:
            if (row[0] == "id"):
                g.append(Game(row[1]))
            else:
                g[-1].Process(row)
    return g

files = [f for f in os.listdir('.') if re.match('.*\\.ev.', f, re.IGNORECASE)]
for f in files:
    #for i in range(5):
        #None
    #print (str(i))
    games = ProcessFile(f)
    print (*games)