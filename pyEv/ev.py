import re
import os
import csv

'''
class Team:
    def __init__(self):
        self.Name = ""
        self.Players = []
        self.Batting = [0] * 9
        self.Defense = [0] * 9
    def __str__(self):
        s = " " * self.level + self.number + ": " + self.partName
        for q in self.subParts:
            s += "\n" + str(q)
        return s
'''
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
                self.Inning = int(r[1])
                self.Bottom = r[2] == '1'
                #assert self.Inning == inning
                self.Bases[0] = r[3]
                self.Count = r[4]
                self.Pitches = r[5]
                self.Event = r[6]
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