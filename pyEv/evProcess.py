import csv
import re
import sys
'''
https://www.retrosheet.org/datause.html */
FEDCBA9876543210 */
OO321HBBBFFFFFSS */
 3 outs    : 2 bit 15..14 */
 4 runners : 4 bit 13..10 */
 7 balls   : 3 bit  9.. 7 */
31 fouls   : 5 bit  6.. 2 */
 3 strikes : 2 bit  1.. 0 */
total      : 16 bit */
'''
def DEST(x: int) -> int:
    return (1<<(10+(x&3))) if (x>0 and x<4) else 0

def strval (s: str, v: int) -> str:
    return s + str(v)

def SplitMods(mods : str, runners : list[int], flags : dict) -> int:
    if (mods == 'FLE5'):
        None
    rbi = 0
    token_specification = [('DELIM'         , r'[\/;]'),
                           ('CATCHERINTER'  , r'C\/E\d'),
                           ('SACRIFICE'     , r'S[HF]'),
                           ('PITCHOUT'      , r'PO.\(\d+\)'),
                           ('PITCHOUTERROR' , r'PO.\(E\d(?:\/TH)?\)'),
                           ('PITCHOUT_CS'   , r'POCS.\(\d+\)'),
                           ('WILDPITCH'     , r'WP'),
                           ('DOUBLEPLAY'    , r'G?DP'),
                           ('TRIPLEPLAY'    , r'TP'),
                           ('NOPITCH'       , r'NP'),
                           ('DEFENSEINDIFF' , r'DI'),
                           ('WALK'          , r'I?W\+?'),
                           ('PLAY_ON_RUNNER', r'\d+(?:\([\dB]\))?'),
                           ('STOLENBASE'    , r'SB.'),
                           ('GROUNDRULE2B'  , r'DGR'),
                           ('SINGLE'        , r'S\d+'),
                           ('DOUBLE'        , r'D\d+'),
                           ('TRIPLE'        , r'T\d+'),
                           ('FOUL'          , r'FL(?:E\d)?'),
                           ('FORCEOUT'      , r'FO'),
                           ('FIELDERSCHOICE', r'FC\d?'),
                           ('PASSEDBALL'    , r'PB$'),
                           ('LOCATION'      , r'(?:BG|BP|BL|L|G|F|P)\w+(?:\+|-)?'),
                           ('CAUGHTSTEALING', r'CS.\(\d+\)'),
                           ('STRIKEOUT'     , r'(?:K|C)\+?'),
                           ('BALK'          , r'BK\+?'),
                           ('HITBYPITCH'    , r'HP'),
                           ('HOMERUN'       , r'HR'),
                           ('ERROR'         , r'E\d(?:\/TH)?'),
                           ('OTHERADVANCE'  , r'OA'),
                           ('UNKNOWN'       , r'.+')]
    tok_regex = '|'.join('(?P<%s>%s)' % pair for pair in token_specification)
    play = ''
    idx = 0
    for mo in re.finditer(tok_regex, mods):
        match mo.lastgroup:
            case 'DELIM': None
            case 'NOPITCH': None
            case 'SACRIFICE':
                flags['eventtype'] = 2
                flags['sachit'] = mo.group()[1]=='H'
                flags['sacfly'] = mo.group()[1]=='F'
                flags['ab'] = False
            case 'PITCHOUT':
                m = re.match('(PO\\d)(\\(\\d+\\))?', mo.group())
                #print (m.groups(), file=sys.stderr)
                r = int(m.groups()[0][2])
                runners[r] = -2
                flags['eventtype'] =  8
                flags['playonrunner' + m.groups()[0][2]] = m.groups()[1][1:-1]
                flags['porunner' + m.groups()[0][2]] = True
                flags['putouts'] += m.groups()[1][-2]
                flags['assists'] += m.groups()[1][1:-2]
            case 'PITCHOUTERROR':
                flags['eventtype'] =  8
                flags['errorplayers'] += mo.group()[5]
                flags['errortypes'] += 'D'
                flags['playonrunner'+mo.group()[2]] = mo.group()[4:6]
                flags['porunner'+mo.group()[2]] = True
            case 'PITCHOUT_CS':
                r = int(mo.group()[4])-1
                runners[r] = -2
                flags['eventtype'] =  8
                flags['playonrunner'+str(r)] = mo.group()[6:-1]
                flags['csrunner'+str(r)] = True
                flags['porunner'+str(r)] = True
                flags['putouts']+= mo.group()[-2]
                flags['assists']+= mo.group()[6:-2]
            case 'WILDPITCH':
                flags['eventtype'] =  9 if flags['eventtype'] == 0 else flags['eventtype']
                flags['wildpitch']=True
                #flags['playonbatter'] = ''
                #flags['putouts'] = flags['putouts'][:-1]
            case 'DOUBLEPLAY':
                flags['doubleplay'] = True
                flags['batterevent'] = True
            case 'TRIPLEPLAY':      assert False
            case 'DEFENSEINDIFF':
                flags['eventtype'] = 5
            case 'WALK':
                runners[0] =  1
                flags['eventtype'] = 14
                flags['eventtype'] += 1 if mo.group()[0]=='I' else 0
                flags['batterevent'] = True
            case 'PLAY_ON_RUNNER':
                flags['batterevent'] = True
                flags['ab'] = True
                flags['eventtype'] =  2
                m = re.match('(\\d+)(\\(\\d\\))?', mo.group())
                play += m[1]
                #print (m[1], file=sys.stderr)
                if m[2] != None:
                    flags['playonrunner' + m[2][1]] = play
                    runners[int(m[2][1])] = -2
                    idx = len(play) - 1
                else:
                    #flags['playonbatter'] = play[-2:]
                    flags['playonbatter'] = play[idx:]
                    runners[0] = -2
                if flags['fieldedby'] == 0:
                    flags['fieldedby'] = int(m[1][0])
                flags['putouts'] += m[1][-1]
            case 'STOLENBASE':
                r = 3 if mo.group()[2]=='H' else int(mo.group()[2])-1
                if (flags['eventtype'] == 0): flags['eventtype'] =  4
                runners[r] = r+1
                flags['sbrunner' + str(r)] = True
            case 'GROUNDRULE2B':
                flags['batterevent'] = True
                flags['ab'] = True
                runners[0] =  2
                flags['eventtype'] = 21
                flags['hitvalue'] = 2
            case 'SINGLE':
                flags['batterevent'] = True
                flags['ab'] = True
                runners[0] = 1
                flags['eventtype'] = 20
                flags['fieldedby'] = mo.group()[1]
                flags['hitvalue'] = 1
            case 'DOUBLE':
                flags['batterevent'] = True
                flags['ab'] = True
                runners[0] =  2
                flags['eventtype'] = 21
                flags['fieldedby'] = int(mo.group()[1])
                flags['hitvalue'] = 2
            case 'TRIPLE':
                flags['batterevent'] = True
                flags['ab'] = True
                runners[0] =  3
                flags['eventtype'] = 22
                flags['fieldedby'] = int(mo.group()[1])
                flags['hitvalue'] = 3
            case 'FOUL':
                flags['foul'] = True
                if len(mo.group())>2:
                    flags['eventtype'] = 13
                    flags['fieldedby'] = int (mo.group()[3])
                    flags['battedballtype'] = 'P'
                    flags['errorplayers'] += mo.group()[3]
                    flags['errortypes'] += 'F'
            case 'LOCATION':
                flags['bunt'] = mo.group()[0] == 'B'
                i = 1 if flags['bunt'] else 0
                flags['battedballtype'] = mo.group()[i]
                flags['hitlocation'] = mo.group()[i+1:]
                if flags['hitlocation'][-1] in ['+','-']:
                    flags['hitlocation'] = flags['hitlocation'][:-1]
            case 'CAUGHTSTEALING':
                flags['eventtype'] =  6 if flags['eventtype'] == 0 else flags['eventtype']
                m = re.match('(CS.)(\\(\\d+\\))?', mo.group())
                #print (m[1], file=sys.stderr)
                r   = 3 if m[1][2] == 'H' else int(m[1][2])-1
                assert r>0
                pfld = 'playonrunner' + str(r)
                csfld = 'csrunner' + str(r)
                runners[r] = -2
                #print (m[2], file=sys.stderr)
                for c in m[2][1:-2]:
                    if c not in flags['assists']:
                        flags['assists'] += c
                flags['putouts'] += m[2][-2]
                flags[pfld] = m[2][1:len(m[2])-1]
                flags[csfld] = True
            case 'STRIKEOUT':
                flags['batterevent'] = True
                flags['ab'] = True
                runners[0] = -2
                flags['eventtype'] =  3
                flags['playonbatter'] = '2'
                flags['putouts'] += '2'
            case 'BALK':
                flags['eventtype'] = 11
            case 'HITBYPITCH':
                flags['batterevent'] = True
                runners[0] =  1
                flags['eventtype'] = 16
                flags['responsible'] = False
            case 'HOMERUN':
                flags['batterevent'] = True
                flags['ab'] = True
                runners[0] =  4
                flags['eventtype'] = 23
                flags['hitvalue'] = 4
            case 'ERROR':
                flags['batterevent'] = True
                flags['ab'] = True
                flags['eventtype'] = 18 if flags['eventtype'] == 0 else flags['eventtype']
                flags['errorplayers'] += mo.group()[1]
                flags['errortypes'] += ('T' if mo.group().find('/TH')>-1 else 'F')
                flags['fieldedby'] = int(mo.group()[1])
            case 'OTHERADVANCE':    assert False
            case 'FORCEOUT':
                flags['fielderschoice'] = True
                #for i in range(len(runners)):
                    #if (runners[i] == -2):
                        #runners[i] = -3
            case 'FIELDERSCHOICE':
                flags['fielderschoice'] = True
                flags['batterevent'] = True
                flags['ab'] = True
                flags['eventtype'] = 19
                if len(mo.group())>2:
                    flags['fieldedby'] = int(mo.group()[2])
            case 'CATCHERINTER':
                flags['eventtype'] = 17
                flags['batterevent'] = True
                flags['errorplayers'] += mo.group()[3]
                flags['errortypes'] += 'F'
            case 'PASSEDBALL':
                flags['eventtype'] = 10 if flags['eventtype'] == 0 else flags['eventtype']
                flags['passedball'] = True
                flags['playonbatter'] = ''
                flags['putouts'] = flags['putouts'][:-1]
            case 'UNKNOWN': assert False
    if play != '':
        flags['assists'] = play[:-1]
    return rbi

def SplitAdvs(advs: str, runners : list[int], flags : dict):
    rbi = 0
    ad = advs.split(';')
    for a in ad:
        #mo = re.match('(.-.)(\\(E\\d.*\\))?', a)
        r1 = 0 if a[0]=='B' else int(a[0])
        r2 = 4 if a[2]=='H' else int(a[2])
        match a[1]:
            case '-':
                if (r1==0) and flags['wildpitch']:
                    flags['playonbatter'] = ''
                    flags['putouts'] = flags['putouts'][:-1]
                #https://regex101.com/
                mo = re.match(r'(?:.-.)((?:\(.+?\))*)', a)
                #2-H(E3)(NR)(UR)
                #Group1: (E3)(NR)(UR)
                runners[r1] = r2
                sp = mo.group(1).split('(')
                for s in sp[1:]:
                    match s[0]:
                        case 'E':
                            flags['errorplayers'] += s[1]
                            flags['errortypes'] += 'T' if '/TH' in s else 'F'
                        case 'N':
                            assert s[1]=='R'
                            rbi -= 1
                        case 'U':
                            assert s[1]=='R'
                            runners[r1] += 1
                        case 'T':
                            assert s[1:3]=='UR'
                            runners[r1] += 2
            case 'X':
                runners[r1] = -2
                m = re.match(r'(.X.)(\(\d+\))?', a)
                if len(flags['putouts']) > 0 and flags['putouts'][-1] == '2' and m.group(2)[1] == '2':
                    flags['putouts'] = ''
                if (r1==0): fld = 'playonbatter'
                else: fld = 'playonrunner'+str(r1)
                flags[fld] = m.group(2)[1:-1]
                flags['assists'] += m.group(2)[1:-2]
                flags['putouts'] += m.group(2)[-2]
            case _:
                assert False
    return rbi

def ProcessEvent (runners : list[int], event : str, flags : dict) -> int:
    Outs = 1 +\
           (1 if runners[1]>0 else 0) +\
           (1 if runners[2]>0 else 0) +\
           (1 if runners[3]>0 else 0)
    rbi = 0
    evt = event.split('.')
    assert len(evt) < 3
    rbi += SplitMods(evt[0], runners, flags)
    if len(evt) > 1:
        rbi += SplitAdvs(evt[1], runners, flags)
    Outs -= (1 if runners[0]>-1 else 0) +\
            (1 if runners[1]>-1 else 0) +\
            (1 if runners[2]>-1 else 0) +\
            (1 if runners[3]>-1 else 0)
    flags['rbi'] = (1 if runners[0]>3 else 0) +\
                   (1 if runners[1]>3 else 0) +\
                   (1 if runners[2]>3 else 0) +\
                   (1 if runners[3]>3 else 0) + rbi
    return Outs

def GenSequence (Outs : int, runners : list[int], pitches : str, event : str, flags : dict) -> list[int]:
    to = [(Outs<<14) |\
          (0 if runners[3] < 0 else (1<<13)) |\
          (0 if runners[2] < 0 else (1<<12)) |\
          (0 if runners[1] < 0 else (1<<11)) |\
          (0 if runners[0] < 0 else (1<<10))]
    if ((to[-1] & (1<<10)) == 0): # no batter at home
        to.append (to[-1] | (1<<10))
    Strikes = 0
    Balls   = 0
    Fouls   = 0
    Pitch   = 0
    for p in pitches:
        Pitch = 1
        match p:
            case 'F' | 'L' | 'O' | 'R' | 'T':                                     Fouls  +=1
            case 'A' | 'C' | 'K' | 'M' | 'Q' | 'S':                               Strikes+=1
            case 'B' | 'I' | 'P' | 'V':                                           Balls  +=1
            case 'H' | 'N' | 'U' | '+' | '*' | '1' | '2' | '3' | '>' | 'X' | 'Y': Pitch   =0
            case '.':                                                             Pitch   =0; to=[to[-1]]
            case _ : assert False

        if Pitch:
            assert Balls   <  8
            assert Fouls   < 32
            assert Strikes <  4
            to.append(to[-1] & 0xFC00)
            to[-1] |= (Balls   << 7)
            to[-1] |= (Fouls   << 2)
            to[-1] |= (Strikes << 0)
    OutsOnPlay = ProcessEvent(runners, event, flags)
    flags['outsonplay'] = OutsOnPlay
    assert OutsOnPlay in range(0,4)
    BatterEvent = runners[0] != 0
    if not BatterEvent:
        if (Pitch): # take into account event on the pitch
            to.pop(-1)
        to[-1] = 1<<10
    else: to.append(0)
    to[-1] |= ((Outs+OutsOnPlay)<<14)
    to[-1] |= DEST(runners[0])
    to[-1] |= DEST(runners[1])
    to[-1] |= DEST(runners[2])
    to[-1] |= DEST(runners[3])
    if (not BatterEvent):
        to[-1] |= (Balls   << 7)
        to[-1] |= (Fouls   << 2)
        to[-1] |= (Strikes << 0)
    if (Outs+OutsOnPlay == 3):
        to.append(0xFFFF)
    return to