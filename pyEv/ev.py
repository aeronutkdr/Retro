'''
https://www.retrosheet.org/datause.html */
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
.\bevent.exe -f 0-96 -y 2023 2023ANA.EVA > out.txt
'''
import re
import os
import csv
import evProcess

Rosters = {}
# OO32 1HBB BSSF FFFF

def FindByField(r: dict, fld: str, val: int) -> str:
    v = None
    for k,item in r.items():
        if item[fld] == val:
            v = k
            break
    return v

class Game:
    def __init__(self, name : str):
        self.Name = name
        self.Version = 0
        self.Info = {}
        self.Position = [[None for _ in range(21)] for _ in range(2)]
        self.Bases = [None for _ in range(4)]
        self.Inning = 0
        self.Half = 0
        self.Out = 0
        self.Score= [0, 0]
        self.Leadoff = True
        self.Sub0 = None
        self.Sub1 = None
        self.Sub2 = None
        self.Sub3 = None
        self.NewGame = True
        self.EventNum = 0
        self.Rosters = [None, None]
    def __str__(self) -> str:
        return "\nGame name = " + self.Name\
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
                # assign rosters and clean up
                if r[1] == 'visteam' :
                    self.Rosters[0] = Rosters[r[2]]
                    for rent in self.Rosters[0].values():
                        rent['ord'] = 0; rent['pos'] = 0
                if r[1] == 'hometeam':
                    self.Rosters[1] = Rosters[r[2]]
                    for rent in self.Rosters[1].values():
                        rent['ord'] = 0; rent['pos'] = 0
            case "start" | "sub":
                team = int(r[3])
                ros = self.Rosters[team]
                ord = int(r[4])
                pos = int(r[5])
                repl = None
                ordent = None
                Positions = self.Position[team]
                # if not the pitcher
                if ord > 0:
                    ordent = FindByField(ros, 'ord', ord)
                    if (ordent):
                        ros[ordent]['ord'] *= -1
                        # if replacing the DH
                        #if ros[ordent]['pos'] == 10: pos = 10
                        pos = Positions.index(ordent)
                        repl = ordent
                #posent = FindByField(ros, 'pos', pos)
                posent = Positions[pos]
                if (posent):
                    Positions[pos+10]=posent
                    ros[posent]['pos'] *= -1
                    assert repl == None or repl == ordent
                    repl = ordent
                ros[r[1]]['ord'] = ord
                ros[r[1]]['pos'] = pos
                if (pos < 11):
                    Positions[pos]=r[1]

                self.Sub0 = repl if repl != None and self.Bases[0] == repl else self.Sub0
                self.Sub1 = repl if repl != None and self.Bases[1] == repl else self.Sub1
                self.Sub2 = repl if repl != None and self.Bases[2] == repl else self.Sub2
                self.Sub3 = repl if repl != None and self.Bases[3] == repl else self.Sub3
                if repl != None:
                    self.Bases = [r[1] if x==repl else x for x in self.Bases]
                    ros[r[1]]['resp'] = ros[repl]['resp']
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
                flags = {'eventtype'      : 0,
                         'hitvalue'       : 0,
                         'outsonplay'     : 0,
                         'batterevent'    : True,
                         'ab'             : True,
                         'doubleplay'     : False,
                         'tripleplay'     : False,
                         'rbi'            : 0,
                         'wildpitch'      : False,
                         'passedball'     : False,
                         'fieldedby'      : 0,
                         'battedballtype' : '',
                         'bunt'           : False,
                         'foul'           : False,
                         'hitlocation'    : '',
                         'numerrors'      : 0,
                         'error1player'   : 0,
                         'error1type'     : 'N',
                         'error2player'   : 0,
                         'error2type'     : 'N',
                         'error3player'   : 0,
                         'error3type'     : 'N',
                         'playonbatter'   : '',
                         'playonrunner1'  : '',
                         'playonrunner2'  : '',
                         'playonrunner3'  : '',
                         'sbrunner1'      : False,
                         'sbrunner2'      : False,
                         'sbrunner3'      : False,
                         'csrunner1'      : False,
                         'csrunner2'      : False,
                         'csrunner3'      : False,
                         'porunner1'      : False,
                         'porunner2'      : False,
                         'porunner3'      : False,
                         'endgame'        : False,
                         'pinchrunner1'   : False,
                         'pinchrunner2'   : False,
                         'pinchrunner3'   : False,
                         'putout1'        : 0,
                         'putout2'        : 0,
                         'putout3'        : 0,
                         'assist1'        : 0,
                         'assist2'        : 0,
                         'assist3'        : 0,
                         'assist4'        : 0,
                         'assist5'        : 0,
                         'sachit'         : False,
                         'sacfly'         : False
                    }
                States = evProcess.GenSequence (self.Out,
                                                runners,
                                                r[5],
                                                r[6],
                                                flags)
                if (r[6] != "NP"):
                    self.EventNum += 1
                    Offros = self.Rosters[self.Half]
                    Defros = self.Rosters[1-self.Half]
                    BatHand = Offros[self.Bases[0]]['bats']
                    Defpos = self.Position[1-self.Half]
                    Offpos = self.Position[self.Half]
                    #pitcher = FindByField(Defros, 'pos', 1)
                    pitcher = Defpos[1]
                    if BatHand=='B':
                        if Defros[pitcher]['throws'] == 'L':
                            BatHand = 'R'
                        else:
                            BatHand = 'L'
                    Offros[r[3]]['resp'] = pitcher
                    EndGame = False
                    if (self.Inning > 8):
                        if ((self.Half == 1) or ((States[-1] >> 14)==3)) and (self.Score[1] > self.Score[0]):
                            EndGame = True
                        if ((self.Half == 1) and ((States[-1] >> 14)==3)) and (self.Score[1] != self.Score[0]):
                            EndGame = True
                    outStr = '\"' + str(self.Name) + '\"'                                                   # 0  game id*
                    outStr += ',\"' + self.Info['visteam'] + '\"'                                           # 1  visiting team*
                    outStr += ',' + str(self.Inning)                                                        # 2  inning*
                    outStr += ',' + str(self.Half)                                                          # 3  batting team*
                    outStr += ',' + str(self.Out)                                                           # 4  outs*
                    outStr += ',' + str(r[4][0])                                                            # 5  balls*
                    outStr += ',' + str(r[4][1])                                                            # 6  strikes*
                    outStr += ',\"' + str(r[5]) + '\"'                                                      # 7  pitch sequence
                    outStr += ',' + str(self.Score[0])                                                      # 8  vis score*
                    outStr += ',' + str(self.Score[1])                                                      # 9  home score*
                    outStr += ',\"' + self.Bases[0] + '\"'                                                  # 10 batter
                    outStr += ',\"' + BatHand + '\"'                                                        # 11 batter hand
                    outStr += ',\"' + self.Bases[0] + '\"'                                                  # 12 res batter*
                    outStr += ',\"' + BatHand + '\"'                                                        # 13 res batter hand*
                    outStr += ',\"' + pitcher +'\"'                                                         # 14 pitcher
                    outStr += ',\"' + Defros[pitcher]['throws'] + '\"'                                      # 15 pitcher hand
                    outStr += ',\"' + pitcher +'\"'                                                         # 16 res pitcher*
                    outStr += ',\"' + Defros[pitcher]['throws'] + '\"'                                      # 17 res pitcher hand*
                    '''
                    outStr += ',\"' + FindByField(Defros, 'pos', 2) +'\"'                                   # 18 catcher
                    outStr += ',\"' + FindByField(Defros, 'pos', 3) +'\"'                                   # 19 first base
                    outStr += ',\"' + FindByField(Defros, 'pos', 4) +'\"'                                   # 20 second base
                    outStr += ',\"' + FindByField(Defros, 'pos', 5) +'\"'                                   # 21 third base
                    outStr += ',\"' + FindByField(Defros, 'pos', 6) +'\"'                                   # 22 shortstop
                    outStr += ',\"' + FindByField(Defros, 'pos', 7) +'\"'                                   # 23 left field
                    outStr += ',\"' + FindByField(Defros, 'pos', 8) +'\"'                                   # 24 center field
                    outStr += ',\"' + FindByField(Defros, 'pos', 9) +'\"'                                   # 25 right field
                    '''
                    outStr += ',\"' + Defpos[2] +'\"'                                                       # 18 catcher
                    outStr += ',\"' + Defpos[3] +'\"'                                                       # 19 first base
                    outStr += ',\"' + Defpos[4] +'\"'                                                       # 20 second base
                    outStr += ',\"' + Defpos[5] +'\"'                                                       # 21 third base
                    outStr += ',\"' + Defpos[6] +'\"'                                                       # 22 shortstop
                    outStr += ',\"' + Defpos[7] +'\"'                                                       # 23 left field
                    outStr += ',\"' + Defpos[8] +'\"'                                                       # 24 center field
                    outStr += ',\"' + Defpos[9] +'\"'                                                       # 25 right field
                    outStr += ',\"' + ("" if self.Bases[1]==None else self.Bases[1]) + '\"'                 # 26 first runner*
                    outStr += ',\"' + ("" if self.Bases[2]==None else self.Bases[2]) + '\"'                 # 27 second runner*
                    outStr += ',\"' + ("" if self.Bases[3]==None else self.Bases[3]) + '\"'                 # 28 third runner*
                    outStr += ',\"' + r[6] + '\"'                                                           # 29 event text*
                    outStr += ',\"' + ('T' if self.Leadoff else 'F') + '\"'                                 # 30 leadoff flag*
                    ph = FindByField(Offros, 'ord', -Offros[self.Bases[0]]['ord'])
                    #dh = FindByField(Offros, 'pos', -10)
                    dh = Defpos[20]
                    outStr += ',\"' + ('F' if (dh != None or ph == None) else 'T') + '\"'                   # 31 pinchhit flag*
                    #outStr += ',' + str(Offros[self.Bases[0]]['pos'])                                      # 32 defensive position*
                    pos = Offpos.index(self.Bases[0])
                    outStr += ',' + str(pos)                                                                # 32 defensive position*
                    outStr += ',' + str(Offros[self.Bases[0]]['ord'])                                       # 33 lineup position*
                    outStr += ',' + str(flags['eventtype'])                                                 # 34 event type*
                    outStr += ',\"' + ('T' if flags['batterevent'] else 'F') + '\"'                         # 35 batter event flag*
                    outStr += ',\"' + ('T' if flags['ab'] else 'F') + '\"'                                  # 36 ab flag*
                    outStr += ',' + str(flags['hitvalue'])                                                  # 37 hit value*
                    outStr += ',\"' + ('T' if flags['sachit'] else 'F') + '\"'                              # 38 SH flag*
                    outStr += ',\"' + ('T' if flags['sacfly'] else 'F') + '\"'                              # 39 SF flag*
                    outStr += ','  + str(flags['outsonplay'])                                               # 40 outs on play*
                    outStr += ',\"' + ('T' if flags['doubleplay'] else 'F')  + '\"'                         # 41 double play flag
                    outStr += ',\"' + ('T' if flags['tripleplay'] else 'F')  + '\"'                         # 42 triple play flag
                    outStr += ',' + str(flags['rbi'])                                                       # 43 RBI on play*
                    outStr += ',\"' + ('T' if flags['wildpitch'] else 'F') + '\"'                           # 44 wild pitch flag*
                    outStr += ',\"' + ('T' if flags['passedball'] else 'F') + '\"'                          # 45 passed ball flag*
                    outStr += ',' + str(flags['fieldedby'])                                                 # 46 fielded by
                    outStr += ',\"' + str(flags['battedballtype']) + '\"'                                   # 47 batted ball type
                    outStr += ',\"' + ('T' if flags['bunt'] else 'F') + '\"'                                # 48 bunt flag
                    outStr += ',\"' + ('T' if flags['foul'] else 'F') + '\"'                                # 49 foul flag
                    outStr += ',\"' + str(flags['hitlocation']) + '\"'                                      # 50 hit location
                    outStr += ',' + str(flags['numerrors'])                                                 # 51 num errors*
                    outStr += ',' + str(flags['error1player'])                                              # 52 1st error player
                    outStr += ',\"' + str(flags['error1type']) + '\"'                                       # 53 1st error type
                    outStr += ',' + str(flags['error2player'])                                              # 54 2nd error player
                    outStr += ',\"' + str(flags['error2type']) + '\"'                                       # 55 2nd error type
                    outStr += ',' + str(flags['error3player'])                                              # 56 3rd error player
                    outStr += ',\"' + str(flags['error3type']) + '\"'                                       # 57 3rd error type
                    outStr += ',' + str(0 if runners[0] == -1 else runners[0])                              # 58 batter dest* (5 if scores and unearned, 6 if team unearned)
                    outStr += ',' + str(0 if runners[1] == -1 else runners[1])                              # 59 runner on 1st dest* (5 if scores and unearned, 6 if team unearned)
                    outStr += ',' + str(0 if runners[2] == -1 else runners[2])                              # 60 runner on 2nd dest* (5 if scores and unearned, 6 if team unearned)
                    outStr += ',' + str(0 if runners[3] == -1 else runners[3])                              # 61 runner on 3rd dest* (5 if scores and unearned, 6 if team unearned)
                    outStr += ',\"' + flags['playonbatter'] + '\"'                                          # 62 play on batter
                    outStr += ',\"' + flags['playonrunner1'] + '\"'                                         # 63 play on runner on 1st
                    outStr += ',\"' + flags['playonrunner2'] + '\"'                                         # 64 play on runner on 2nd
                    outStr += ',\"' + flags['playonrunner3'] + '\"'                                         # 65 play on runner on 3rd
                    outStr += ',\"' + ('T' if flags['sbrunner1'] else 'F') + '\"'                           # 66 SB for runner on 1st flag
                    outStr += ',\"' + ('T' if flags['sbrunner2'] else 'F') + '\"'                           # 67 SB for runner on 2nd flag
                    outStr += ',\"' + ('T' if flags['sbrunner3'] else 'F') + '\"'                           # 68 SB for runner on 3rd flag
                    outStr += ',\"' + ('T' if flags['csrunner1'] else 'F') + '\"'                           # 69 CS for runner on 1st flag
                    outStr += ',\"' + ('T' if flags['csrunner2'] else 'F') + '\"'                           # 70 CS for runner on 2nd flag
                    outStr += ',\"' + ('T' if flags['csrunner3'] else 'F') + '\"'                           # 71 CS for runner on 3rd flag
                    outStr += ',\"' + ('T' if flags['porunner1'] else 'F') + '\"'                           # 72 PO for runner on 1st flag
                    outStr += ',\"' + ('T' if flags['porunner2'] else 'F') + '\"'                           # 73 PO for runner on 2nd flag
                    outStr += ',\"' + ('T' if flags['porunner3'] else 'F') + '\"'                           # 74 PO for runner on 3rd flag
                    outStr += ',\"' + ('' if self.Bases[1]==None else Offros[self.Bases[1]]['resp']) + '\"' # 75 Responsible pitcher for runner on 1st
                    outStr += ',\"' + ('' if self.Bases[2]==None else Offros[self.Bases[2]]['resp']) + '\"' # 76 Responsible pitcher for runner on 2nd
                    outStr += ',\"' + ('' if self.Bases[3]==None else Offros[self.Bases[3]]['resp']) + '\"' # 77 Responsible pitcher for runner on 3rd
                    outStr += ',\"' + ('T' if self.NewGame else 'F') + '\"'                                 # 78 New Game Flag
                    outStr += ',\"' + ('T' if EndGame else 'F') + '\"'                                      # 79 End Game Flag
                    outStr += ',\"' + ('F' if self.Sub1==None else 'T') + '\"'                              # 80 Pinch-runner on 1st? (T/F)
                    outStr += ',\"' + ('F' if self.Sub2==None else 'T') + '\"'                              # 81 Pinch-runner on 2nd? (T/F)
                    outStr += ',\"' + ('F' if self.Sub3==None else 'T') + '\"'                              # 82 Pinch-runner on 3rd? (T/F)
                    outStr += ',\"' + ('' if self.Sub1==None else self.Sub1)+ '\"'                          # 83 ID of Runner removed for pinch-runner on 1st
                    outStr += ',\"' + ('' if self.Sub2==None else self.Sub2)+ '\"'                          # 84 ID of Runner removed for pinch-runner on 2nd
                    outStr += ',\"' + ('' if self.Sub3==None else self.Sub3)+ '\"'                          # 85 ID of Runner removed for pinch-runner on 3rd
                    sub = FindByField(Offros, 'ord', -Offros[self.Bases[0]]['ord'])
                    subpos = '0'
                    if sub == None: sub = ''
                    #else:           subpos = str(abs(Offros[sub]['pos']))
                    else:           subpos = str(Offpos.index(sub)-10)
                    outStr += ',\"' + sub + '\"'                                                            # 86 ID of Batter removed for pinch-hitter
                    outStr += ',' + subpos                                                                  # 87 Fielding position of batter removed for pinch-hitter
                    outStr += ',' + str(flags['putout1'])                                                   # 88 Fielder with First Putout (0 if none)
                    outStr += ',' + str(flags['putout2'])                                                   # 89 Fielder with Second Putout (0 if none)
                    outStr += ',' + str(flags['putout3'])                                                   # 90 Fielder with Third Putout (0 if none)
                    outStr += ',' + str(flags['assist1'])                                                   # 91 Fielder with First Assist (0 if none)
                    outStr += ',' + str(flags['assist2'])                                                   # 92 Fielder with Second Assist (0 if none)
                    outStr += ',' + str(flags['assist3'])                                                   # 93 Fielder with Third Assist (0 if none)
                    outStr += ',' + str(flags['assist4'])                                                   # 94 Fielder with Fourth Assist (0 if none)
                    outStr += ',' + str(flags['assist5'])                                                   # 95 Fielder with Fifth Assist (0 if none)
                    outStr += ',' + str(self.EventNum)                                                      # 96 event num
                    print (outStr)
                    self.NewGame = False
                    for it in Offros.values():
                        if it['pos'] < 0:
                            it['pos'] = 0
                    for it in Offros.values():
                        if it['ord'] < 0:
                            it['ord'] = 0
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
                self.Sub0 = None
                self.Sub1 = None
                self.Sub2 = None
                self.Sub3 = None

            case "radj":
                self.Bases[int(r[2])] = r[1]
                Offros = self.Rosters[self.Half]
                #Offros[r[1]]['resp'] = FindByField(self.Rosters[1-self.Half], 'pos', 1)
                Offros[r[1]]['resp'] = self.Position[1-self.Half][1]
            case "com": None
            case "data": None
            case _: print(r[0])

def ProcessRoster(s: str):
    with open(s, mode='r') as file:
        Rosters[s[:3]] = {}
        csv_reader = csv.reader(file)
        for row in csv_reader:
            Rosters[s[:3]][row[0]] = {}
            Rosters[s[:3]][row[0]]['bats'] = row[3]
            Rosters[s[:3]][row[0]]['throws'] = row[4]
            Rosters[s[:3]][row[0]]['resp'] = None
            Rosters[s[:3]][row[0]]['pos'] = 0
            Rosters[s[:3]][row[0]]['ord'] = 0

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

print("game id,visiting team,inning,batting team,outs,balls,strikes,pitch sequence,vis score,home score,batter,batter hand,res batter,res batter hand,pitcher,pitcher hand,res pitcher,res pitcher hand,catcher,first base,second base,third base,shortstop,left field,center field,right field,first runner,second runner,third runner,event text,leadoff flag,pinchhit flag,defensive position,lineup position,event type,batter event flag,ab flag,hit value,SH flag,SF flag,outs on play,double play flag,triple play flag,RBI on play,wild pitch flag,passed ball flag,fielded by,batted ball type,bunt flag,foul flag,hit location,num errors,1st error player,1st error type,2nd error player,2nd error type,3rd error player,3rd error type,batter dest,runner on 1st dest,runner on 2nd dest,runner on 3rd dest,play on batter,play on runner on 1st,play on runner on 2nd,play on runner on 3rd,SB for runner on 1st flag,SB for runner on 2nd flag,SB for runner on 3rd flag,CS for runner on 1st flag,CS for runner on 2nd flag,CS for runner on 3rd flag,PO for runner on 1st flag,PO for runner on 2nd flag,PO for runner on 3rd flag,Responsible pitcher for runner on 1st,Responsible pitcher for runner on 2nd,Responsible pitcher for runner on 3rd,New Game Flag,End Game Flag,Pinch-runner on 1st,Pinch-runner on 2nd,Pinch-runner on 3rd,ID of Runner removed for pinch-runner on 1st,ID of Runner removed for pinch-runner on 2nd,ID of Runner removed for pinch-runner on 3rd,ID of Batter removed for pinch-hitter,Fielding position of batter removed for pinch-hitter,Fielder with First Putout,Fielder with Second Putout,Fielder with Third Putout,Fielder with First Assist,Fielder with Second Assist,Fielder with Third Assist,Fielder with Fourth Assist,Fielder with Fifth Assist,event num")
files = [f for f in os.listdir('.') if re.match('.*\\.ros$', f, re.IGNORECASE)]
for f in files: ProcessRoster(f)
files = [f for f in os.listdir('.') if re.match('.*\\.ev.$', f, re.IGNORECASE)]
for f in files: games = ProcessFile(f); print (*games)