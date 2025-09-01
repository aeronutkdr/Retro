'''
Usage: bevent [options] eventfile...
options:
  -h        print this help
  -i id     only process game given by id
  -q        ask whether to process each game
  -y year   Year to process (for teamyyyy and aaayyyy.ros).
  -s start  Earliest date to process (mmdd).
  -e end    Last date to process (mmdd).
  -a        generate Ascii-delimited format files (default)
  -ft       generate Fortran format files
  -m        use master player file instead of local roster files
  -f flist  give list of fields to output
              Default is 0-6,8-9,12-13,16-17,26-40,43-45,51,58-61
  -d        print list of field numbers and descriptions
bevent.exe -f 0-20,26-29,58-61 -i ANA202304090 -y 2023 2023ANA.EVA > out.txt
bevent.exe -f 0-20,26-29,58-61 -y 2023 2023ANA.EVA > out.txt
bevent.exe -f 0-30,58-61 -y 2023 2023ANA.EVA > out.txt
'''
import re
import os
import csv
import evProcess

Rosters = {}
# OO32 1HBB BSSF FFFF
class Game:
    def __init__(self, name : str):
        self.Name = name
        self.Version = 0
        self.Info = {}
        self.Lineup = [[None for _ in range(9)] for _ in range(2)]
        self.Position = [[None for _ in range(10)] for _ in range(2)]
        self.Bases = [None for _ in range(4)]
        self.Inning = 0
        self.Half = 0
        self.Out = 0
        self.Score= [0, 0]
        self.Leadoff = True
    def __str__(self) -> str:
        return "Game name = " + self.Name\
               + "\nVersion = " + str(self.Version)\
               + "\nInning = " + str(self.Inning)\
               + "\nHalf = " + str(self.Half)\
               + "\nOut = " + str(self.Out)\
               + "\nBases = " + str(self.Bases)\

    def Process(self,r: str):
        match r[0]:
            case "version": self.Version = int(r[1])
            case "info":
                self.Info[r[1]] = r[2]
                if r[1] == 'visteam' or r[1] == 'hometeam':
                    if not r[2] in Rosters:
                        Rosters[r[2]] = {}
                        fname = r[2]+self.Name[3:7]+'.ros'
                        with open(fname, mode='r') as file:
                                csv_reader = csv.reader(file)
                                for row in csv_reader:
                                    Rosters[r[2]][row[0]] = {}
                                    Rosters[r[2]][row[0]]['lastname'] = row[1]
                                    Rosters[r[2]][row[0]]['firstname'] = row[2]
                                    Rosters[r[2]][row[0]]['bats'] = row[3]
                                    Rosters[r[2]][row[0]]['throws'] = row[4]
                                    Rosters[r[2]][row[0]]['team'] = row[5]
                                    Rosters[r[2]][row[0]]['pos'] = row[6]
            case "start" | "sub":
                team = int(r[3])
                ord = int(r[4])-1
                pos = int(r[5])-1
                repl = self.Lineup[team][ord]
                ros = Rosters[self.Info['visteam'] if team==0 else self.Info['hometeam']]
                assert r[1] in ros
                if (ord >= 0):
                    self.Lineup[team][ord]=r[1]
                if (pos < 10):
                    self.Position[team][pos]=r[1]
                if repl != None:
                    self.Bases = [r[1] if x==repl else x for x in self.Bases]
            case "play":
                States = []
                self.Inning = int(r[1])
                self.Leadoff |= (self.Half != int(r[2]))
                if (self.Half != int(r[2])):
                    assert self.Out == 3
                    self.Out = 0
                self.Half = int(r[2])
                self.Bases[0] = r[3]
                runners = [-1 if self.Bases[v]==None else v for v in range(4)]
                States = evProcess.GenSequence (self.Out,
                                                runners,
                                                r[5],
                                                r[6])
                if (r[6] != "NP"):
                    Offros = Rosters[self.Info['visteam'] if self.Half==0 else self.Info['hometeam']]
                    Defros = Rosters[self.Info['visteam'] if self.Half==1 else self.Info['hometeam']]
                    BatHand = Offros[self.Bases[0]]['bats']
                    if BatHand=='B':
                        if Defros[self.Position[1-self.Half][0]]['throws'] == 'L':
                            BatHand = 'R'
                        else:
                            BatHand = 'L'
                    outStr = '\"' + str(self.Name) + '\"'                                     # 0  game id*
                    outStr += ',\"' + self.Info['visteam'] + '\"'                             # 1  visiting team*
                    outStr += ',' + str(self.Inning)                                          # 2  inning*
                    outStr += ',' + str(self.Half)                                            # 3  batting team*
                    outStr += ',' + str(self.Out)                                             # 4  outs*
                    outStr += ',' + str(r[4][0])                                              # 5  balls*
                    outStr += ',' + str(r[4][1])                                              # 6  strikes*
                    outStr += ',\"' + str(r[5]) + '\"'                                        # 7  pitch sequence
                    outStr += ',' + str(self.Score[0])                                        # 8  vis score*
                    outStr += ',' + str(self.Score[1])                                        # 9  home score*
                    outStr += ',\"' + self.Bases[0] + '\"'                                    # 10 batter
                    outStr += ',\"' + BatHand + '\"'                                          # 11 batter hand
                    outStr += ',\"' + self.Bases[0] + '\"'                                    # 12 res batter*
                    outStr += ',\"' + BatHand + '\"'                                          # 13 res batter hand*
                    outStr += ',\"' + self.Position[1-self.Half][0] +'\"'                     # 14 pitcher
                    outStr += ',\"' + Defros[self.Position[1-self.Half][0]]['throws'] + '\"'  # 15 pitcher hand
                    outStr += ',\"' + self.Position[1-self.Half][0] +'\"'                     # 16 res pitcher*
                    outStr += ',\"' + Defros[self.Position[1-self.Half][0]]['throws'] + '\"'  # 17 res pitcher hand*
                    outStr += ',\"' + self.Position[1-self.Half][1] +'\"'                     # 18 catcher
                    outStr += ',\"' + self.Position[1-self.Half][2] +'\"'                     # 19 first base
                    outStr += ',\"' + self.Position[1-self.Half][3] +'\"'                     # 20 second base
                    outStr += ',\"' + self.Position[1-self.Half][4] +'\"'                     # 21 third base
                    outStr += ',\"' + self.Position[1-self.Half][5] +'\"'                     # 22 shortstop
                    outStr += ',\"' + self.Position[1-self.Half][6] +'\"'                     # 23 left field
                    outStr += ',\"' + self.Position[1-self.Half][7] +'\"'                     # 24 center field
                    outStr += ',\"' + self.Position[1-self.Half][8] +'\"'                     # 25 right field
                    outStr += ',\"' + ("" if self.Bases[1]==None else self.Bases[1]) + '\"'   # 26 first runner*
                    outStr += ',\"' + ("" if self.Bases[2]==None else self.Bases[2]) + '\"'   # 27 second runner*
                    outStr += ',\"' + ("" if self.Bases[3]==None else self.Bases[3]) + '\"'   # 28 third runner*
                    outStr += ',\"' + r[6] + '\"'                                             # 29 event text*
                    outStr += ',\"' + ('T' if self.Leadoff else 'F') + '\"'                     # 30 leadoff flag*
                    outStr += ',' + str(0 if runners[0] == -1 else runners[0])                # 58 batter dest* (5 if scores and unearned, 6 if team unearned)
                    outStr += ',' + str(0 if runners[1] == -1 else runners[1])                # 59 runner on 1st dest* (5 if scores and unearned, 6 if team unearned)
                    outStr += ',' + str(0 if runners[2] == -1 else runners[2])                # 60 runner on 2nd dest* (5 if scores and unearned, 6 if team unearned)
                    outStr += ',' + str(0 if runners[3] == -1 else runners[3])                # 61 runner on 3rd dest* (5 if scores and unearned, 6 if team unearned)
                    print (outStr)
                for i in reversed(range(4)):
                    if i != runners[i] and runners[i] in range(4):
                        self.Bases[runners[i]] = self.Bases[i]
                        self.Bases[i] = None
                    elif runners[i] > 3:
                        self.Score[self.Half]+=1
                        self.Bases[i] = None
                    elif runners[i] == -1:
                        self.Bases[i] = None
                self.Out = States[-1] >> 14
                if (self.Out == 3):
                    self.Bases = [None] * 4
                self.Leadoff &= (r[6] == "NP")
            case "radj":
                self.Bases[int(r[2])] = r[1]
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

files = [f for f in os.listdir('.') if re.match('.*\\.ev.$', f, re.IGNORECASE)]
for f in files:
    #for i in range(5):
        #None
    #print (str(i))
    games = ProcessFile(f)
    print (*games)