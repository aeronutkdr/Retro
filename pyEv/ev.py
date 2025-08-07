import re
import os
import csv

# OO32 1HBB BSSF FFFF
def CalcState (O, B, Ba, St, F):
    return (O << 14) +\
           (0x2000 if B[3]!="" else 0) +\
           (0x1000 if B[2]!="" else 0) +\
           (0x0800 if B[1]!="" else 0) +\
           (0x0400 if B[0]!="" else 0) +\
           (Ba << 7) +\
           (St << 4) +\
           (F  << 0)

def ProcessPitch(p):
    return 1 if (p != None) and (p=='S') else 0

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
                self.Count = r[4]
                self.Pitches = r[5]
                self.Event = r[6]
                States.append (CalcState(self.Out, self.Bases, 0, 0, 0))
                i = 0
                for p in self.Pitches:
                    BSF = ProcessPitch(self.Pitches[i])
                    i += 1
                    States.append (States[-1] + BSF)
                print ('[{}]'.format(', '.join(hex(x) for x in States)))

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
    games = ProcessFile(f)
    print (*games)