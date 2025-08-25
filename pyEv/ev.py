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
    def __init__(self, name : str):
        self.Name = name
        #self.Team = [Team()] * 2
        self.Version = 0
        self.Info = {}
        self.Rosters = [[] for _ in range(2)]
        self.Lineup = [[-1 for _ in range(9)] for _ in range(2)]
        self.Position = [[-1 for _ in range(10)] for _ in range(2)]
        self.Bases = [-1 for _ in range(4)]
        self.Inning = 0
        self.Half = 0
        self.Out = 0
    def __str__(self) -> str:
               #+ "\nInfo = " + str(self.Info)\
               #+ "\nLineup = " + str(self.Lineup)\
               #+ "\nPosition = " + str(self.Position)\
        return "Game name = " + self.Name\
               + "\nVersion = " + str(self.Version)\
               + "\nInning = " + str(self.Inning)\
               + "\nHalf = " + str(self.Half)\
               + "\nOut = " + str(self.Out)\
               + "\nBases = " + str(self.Bases)\

    def Process(self,r: str):
        match r[0]:
            case "version": self.Version = int(r[1])
            case "info": self.Info[r[1]] = r[2]
            case "start" | "sub":
                team = int(r[3])
                ord = int(r[4])-1
                pos = int(r[5])-1
                repl = self.Lineup[team][ord]
                if not r[1] in self.Rosters[team]:
                    self.Rosters[team].append(r[1])
                idx = self.Rosters[team].index(r[1])
                if (ord >= 0):
                    self.Lineup[team][ord]=idx
                if (pos < 10):
                    self.Position[team][pos]=idx
                if repl>=0:
                    # if player to be replaced is on base:
                    for i in range(4):
                        if self.Bases[i] == repl:
                            self.Bases[i] = idx
            case "play":
                States = []
                self.Inning = int(r[1])
                if (self.Half != int(r[2])):
                    assert self.Out == 3
                    self.Out = 0
                self.Half = int(r[2])
                #assert self.Inning == inning
                self.Bases[0] = self.Rosters[self.Half].index(r[3])
                Bases = [[-1] * 2 for _ in range(4)]
                for i in range(4):
                    Bases[i][0] = self.Bases[i]
                b = self.Bases.copy()
                States = evProcess.GenSequence (self.Out,
                                                Bases,
                                                r[5],
                                                r[6])
                if (r[6] != "NP"):
                    R = ""
                    for i in range(4):
                        R += "," + str(0 if Bases[i][1]<0 else Bases[i][1])
                    print ('\"{}\",{},\"{}\"{}'.format(r[5],
                                                    ','.join('\"'+("" if (B == -1) else (self.Rosters[self.Half][B]))+'\"' for B in b),
                                                    r[6],
                                                    R))
                for i in range(4):
                    self.Bases[i] = -1
                for i in range(4):
                    if Bases[i][1] in range(0,4):
                        self.Bases[Bases[i][1]] = Bases[i][0]
                self.Out = States[-1] >> 14
                if (self.Out == 3):
                    #self.Out = 0
                    self.Bases = [-1, -1, -1, -1]
            case "radj":
                if not r[1] in self.Rosters[self.Half]:
                    self.Rosters[self.Half].append(r[1])
                idx = self.Rosters[self.Half].index(r[1])
                self.Bases[int(r[2])] = idx
            case "com": None
            case "data": None
            case _: print(r[0])

def ProcessFile(s: str) -> list[Game]:
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