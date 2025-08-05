import re
import os
import csv

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

class Game:
    def __init__(self, name):
        self.Name = name
        #self.Team = [Team()] * 2
        self.Version = 0
        self.Info = {}
        self.Lineup = [[0 for _ in range(9)] for _ in range(2)]
        self.Position = [[0 for _ in range(9)] for _ in range(2)]
    def __str__(self):
        return "Game name = " + self.Name +\
               "\nVersion = " + str(self.Version) +\
               "\nInfo = " + str(self.Info) +\
               "\nLineup = " + str(self.Lineup) +\
               "\nPosition = " + str(self.Position)
    def Process(self,r):
        match r[0]:
            case "version": self.Version = int(r[1])
            case "info": self.Info[r[1]] = r[2]
            case "start":
                team = int(r[3])
                ord = int(r[4])
                if (ord > 0) and (ord < 10):
                    self.Lineup[team][ord-1] = r[1]
                pos = int(r[5])
                if (pos < 10):
                    self.Position[team][pos-1] = r[1]
            case _: print("other")

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

#print (os.listdir('.'))
files = [f for f in os.listdir('.') if re.match('.*\\.EV.', f)]
for f in files:
    games = ProcessFile(f)
print (*games)