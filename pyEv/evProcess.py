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
    for mo in re.finditer(tok_regex, mods):
        match mo.lastgroup:
            case 'DELIM':           None
            case 'SACRIFICE':
                flags['eventtype'] = 2
                flags['sachit'] = mo.group()[1]=='H'
                flags['sacfly'] = mo.group()[1]=='F'
                flags['ab'] = False
            case 'PITCHOUT':
                m = re.match('(PO\\d)(\\(\\d+\\))?', mo.group())
                #print (m.groups(), file=sys.stderr)
                r = int(m.groups()[0][2])
                runners[r] = -1
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
                runners[r] = -1
                flags['eventtype'] =  8
                flags['playonrunner'+str(r)] = mo.group()[6:-1]
                flags['csrunner'+str(r)] = True
                flags['porunner'+str(r)] = True
                flags['putouts']+= mo.group()[-2]
                flags['assists']+= mo.group()[6:-2]
            case 'WILDPITCH':
                flags['eventtype'] =  9 if flags['eventtype'] == 0 else flags['eventtype']
                flags['wildpitch']=True
                flags['playonbatter'] = ''
                flags['putouts'] = flags['putouts'][:-1]
            case 'DOUBLEPLAY':
                flags['doubleplay'] = True
                flags['batterevent'] = True
            case 'TRIPLEPLAY':      assert False
            case 'NOPITCH':
                None
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
                    runners[int(m[2][1])] = -1
                else:
                    flags['playonbatter'] = play[-2:]
                    runners[0] = -1
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
                runners[r] = -1
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
                runners[0] = -1
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
            case 'FIELDERSCHOICE':
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
            case 'X':
                runners[r1] = -1
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

'''
def ProcessMods (mods : str, runners : list[int], assist : list[int], putout : list[int], flags : dict) -> int:
    tok_regex = '|'.join('(?P<%s>%s)' % pair for pair in token_specification)
    rbi = 0
    modarr = mods.split('/')
    for r in modarr:
        for mo in re.finditer(tok_regex, r):
            match r:
                case None: None
                case ''  : None
                case '1-2':         runners[1] =  2
                case '1-3':         runners[1] =  3
                case '1-3(E2/TH)':  runners[1] =  3;         flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 2; flags['error' + str(flags['numerrors']) + 'type'] = 'T'
                case '1-3(E4/TH)':  runners[1] =  3;         flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 4; flags['error' + str(flags['numerrors']) + 'type'] = 'T'
                case '1-3(E5/TH)':  runners[1] =  3;         flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 5; flags['error' + str(flags['numerrors']) + 'type'] = 'T'
                case '1-3(E4)':  runners[1] =  3;         flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 4; flags['error' + str(flags['numerrors']) + 'type'] = 'F'
                case '1-H':         runners[1] =  4
                case '1-H(UR)':     runners[1] =  5
                case '1-H(E7)(NR)(UR)': runners[1] =  5;     flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 7; flags['error' + str(flags['numerrors']) + 'type'] = 'F'; rbi -= 1
                case '13':          runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 1; flags['playonbatter'] = '13'; putout[0] += 1; flags[strval('putout', putout[0])] = 3; assist[0] += 1; flags[strval('assist', assist[0])] = 1
                case '23':          runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 2; flags['playonbatter'] = '23'; putout[0] += 1; flags[strval('putout', putout[0])] = 3; assist[0] += 1; flags[strval('assist', assist[0])] = 2
                case '2':           runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 2; flags['playonbatter'] = '2'; putout[0] += 1; flags[strval('putout', putout[0])] = 2
                case '2-3':         runners[2] =  3
                case '2-3(E6/TH)':  runners[2] =  3;         flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 6; flags['error' + str(flags['numerrors']) + 'type'] = 'T'
                case '2-H':         runners[2] =  4
                case '2-H(NR)':     runners[2] =  4; rbi -= 1
                case '2-H(UR)':     runners[2] =  5
                case '2-H(E2)(NR)': runners[2] =  4;         flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 2; flags['error' + str(flags['numerrors']) + 'type'] = 'F'; rbi -= 1
                case '2-H(E2/TH)(NR)': runners[2] =  4;         flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 2; flags['error' + str(flags['numerrors']) + 'type'] = 'T'; rbi -= 1
                case '2-H(E3)(NR)(UR)': runners[2] =  5;        flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 3; flags['error' + str(flags['numerrors']) + 'type'] = 'F'; rbi -= 1 
                case '3':           runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 3; flags['playonbatter'] = '3'; putout[0] += 1; flags[strval('putout', putout[0])] = 3
                case '3-H':         runners[3] =  4
                case '3-H(UR)':     runners[3] =  5
                case '3-H(NR)(UR)': runners[3] =  5; rbi -= 1
                case '3-H(E5)(NR)(UR)': runners[3] =  5;        flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 5; flags['error' + str(flags['numerrors']) + 'type'] = 'F'; rbi -= 1 
                case '3-H(E2/TH)(NR)(UR)': runners[3] =  5;        flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 2; flags['error' + str(flags['numerrors']) + 'type'] = 'T'; rbi -= 1 
                case '16(1)':       runners[1] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 1; flags['playonrunner1'] = '16'; putout[0] += 1; flags[strval('putout', putout[0])] = 6; assist[0] += 1; flags[strval('assist', assist[0])] = 1
                case '16(1)3':      runners[1] = -1; runners[0] = -1; flags['eventtype'] =  2; flags['fieldedby'] = 1; flags['playonbatter'] = '63'; flags['playonrunner1'] = '16'; putout[0] += 1; flags[strval('putout', putout[0])] = 6; assist[0] += 1; flags[strval('assist', assist[0])] = 1; putout[0] += 1; flags[strval('putout', putout[0])] = 3; assist[0] += 1; flags[strval('assist', assist[0])] = 6
                case '31':          runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 3; flags['playonbatter'] = '31'; putout[0] += 1; flags[strval('putout', putout[0])] = 1; assist[0] += 1; flags[strval('assist', assist[0])] = 3
                case '34(1)':       runners[1] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 3; flags['playonrunner1'] = '34'; putout[0] += 1; flags[strval('putout', putout[0])] = 4; assist[0] += 1; flags[strval('assist', assist[0])] = 3
                case '36(1)':       runners[1] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 3; flags['playonrunner1'] = '36'; putout[0] += 1; flags[strval('putout', putout[0])] = 6; assist[0] += 1; flags[strval('assist', assist[0])] = 3
                case '36(1)3':      runners[1] = -1; runners[0] = -1; flags['eventtype'] =  2; flags['fieldedby'] = 3; flags['playonbatter'] = '63'; flags['playonrunner1'] = '36'; putout[0] += 1; flags[strval('putout', putout[0])] = 6; assist[0] += 1; flags[strval('assist', assist[0])] = 3; putout[0] += 1; flags[strval('putout', putout[0])] = 3; assist[0] += 1; flags[strval('assist', assist[0])] = 6
                case '4':           runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 4; flags['playonbatter'] = '4'; putout[0] += 1; flags[strval('putout', putout[0])] = 4
                case '4(1)3':       runners[0:2] = [-1, -1]; flags['eventtype'] =  2; flags['fieldedby'] = 4; flags['playonbatter'] = '43'; flags['playonrunner1'] = '4'; putout[0] += 1; flags[strval('putout', putout[0])] = 4; putout[0] += 1; flags[strval('putout', putout[0])] = 3; assist[0] += 1; flags[strval('assist', assist[0])] = 4
                case '4(1)3/GDP':   runners[0:2] = [-1, -1]; flags['eventtype'] =  2; flags['fieldedby'] = 4; flags['playonbatter'] = '43'; flags['playonrunner1'] = '4'; putout[0] += 1; flags[strval('putout', putout[0])] = 4; putout[0] += 1; flags[strval('putout', putout[0])] = 3; assist[0] += 1; flags[strval('assist', assist[0])] = 4; flags['battedballtype'] = 'G'; flags['doubleplay'] = True
                case '43':          runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 4; flags['playonbatter'] = '43'; putout[0] += 1; flags[strval('putout', putout[0])] = 3; assist[0] += 1; flags[strval('assist', assist[0])] = 4
                case '46(1)':       runners[1] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 4; flags['playonrunner1'] = '46'; putout[0] += 1; flags[strval('putout', putout[0])] = 6; assist[0] += 1; flags[strval('assist', assist[0])] = 4
                case '46(1)/FO':    runners[1] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 4; flags['playonrunner1'] = '46'; putout[0] += 1; flags[strval('putout', putout[0])] = 6; assist[0] += 1; flags[strval('assist', assist[0])] = 4; flags['battedballtype'] = 'F'
                case '46(1)3':      runners[1] = -1; runners[0] = -1; flags['eventtype'] =  2; flags['fieldedby'] = 4; flags['playonbatter'] = '63'; flags['playonrunner1'] = '46'; putout[0] += 1; flags[strval('putout', putout[0])] = 6; assist[0] += 1; flags[strval('assist', assist[0])] = 4; putout[0] += 1; flags[strval('putout', putout[0])] = 3; assist[0] += 1; flags[strval('assist', assist[0])] = 6
                case '346(1)':      runners[1] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 3; flags['playonrunner1'] = '346'; putout[0] += 1; flags[strval('putout', putout[0])] = 6; assist[0] += 1; flags[strval('assist', assist[0])] = 3; assist[0] += 1; flags[strval('assist', assist[0])] = 4
                case '486(1)':      runners[1] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 4; flags['playonrunner1'] = '486'; putout[0] += 1; flags[strval('putout', putout[0])] = 6; assist[0] += 1; flags[strval('assist', assist[0])] = 4; assist[0] += 1; flags[strval('assist', assist[0])] = 8
                case '486(1)/FO':   runners[1] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 4; flags['playonrunner1'] = '486'; putout[0] += 1; flags[strval('putout', putout[0])] = 6; assist[0] += 1; flags[strval('assist', assist[0])] = 4; assist[0] += 1; flags[strval('assist', assist[0])] = 8; flags['battedballtype'] = 'F'
                case '5':           runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 5; flags['playonbatter'] = '5'; putout[0] += 1; flags[strval('putout', putout[0])] = 5
                case '53':          runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 5; flags['playonbatter'] = '53'; putout[0] += 1; flags[strval('putout', putout[0])] = 3; assist[0] += 1; flags[strval('assist', assist[0])] = 5
                case '54(1)':       runners[1] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 5; flags['playonrunner1'] = '54'; putout[0] += 1; flags[strval('putout', putout[0])] = 4; assist[0] += 1; flags[strval('assist', assist[0])] = 5
                case '54(1)/FO':       runners[1] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 5; flags['playonrunner1'] = '54'; putout[0] += 1; flags[strval('putout', putout[0])] = 4; assist[0] += 1; flags[strval('assist', assist[0])] = 5; flags['battedballtype'] = 'F'
                case '54(1)3':      runners[0:2] = [-1, -1]; flags['eventtype'] =  2; flags['fieldedby'] = 5; flags['playonbatter'] = '43'; flags['playonrunner1'] = '54'; putout[0] += 1; flags[strval('putout', putout[0])] = 4; putout[0] += 1; flags[strval('putout', putout[0])] = 3; assist[0] += 1; flags[strval('assist', assist[0])] = 5; assist[0] += 1; flags[strval('assist', assist[0])] = 4
                case '56(1)':       runners[1] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 5; flags['playonrunner1'] = '56'; putout[0] += 1; flags[strval('putout', putout[0])] = 6; assist[0] += 1; flags[strval('assist', assist[0])] = 5
                case '5(2)':        runners[2] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 5; flags['playonrunner2'] = '5'; putout[0] += 1; flags[strval('putout', putout[0])] = 5
                case '6':           runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 6; flags['playonbatter'] = '6'; putout[0] += 1; flags[strval('putout', putout[0])] = 6
                case '6(1)':        runners[1] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 6; flags['playonrunner1'] = '6'; putout[0] += 1; flags[strval('putout', putout[0])] = 6
                case '6(1)3':       runners[1] = -1; runners[0] = -1; flags['eventtype'] =  2; flags['fieldedby'] = 6; flags['playonrunner1'] = '6'; flags['playonbatter'] = '63'; putout[0] += 1; flags[strval('putout', putout[0])] = 6; assist[0] += 1; flags[strval('assist', assist[0])] = 6; putout[0] += 1; flags[strval('putout', putout[0])] = 3
                case '63':          runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 6; flags['playonbatter'] = '63'; putout[0] += 1; flags[strval('putout', putout[0])] = 3; assist[0] += 1; flags[strval('assist', assist[0])] = 6
                case '64(1)':       runners[1] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 6; flags['playonrunner1'] = '64'; putout[0] += 1; flags[strval('putout', putout[0])] = 4; assist[0] += 1; flags[strval('assist', assist[0])] = 6
                case '64(1)/FO':       runners[1] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 6; flags['playonrunner1'] = '64'; putout[0] += 1; flags[strval('putout', putout[0])] = 4; assist[0] += 1; flags[strval('assist', assist[0])] = 6; flags['battedballtype'] = 'F'
                case '64(1)3':      runners[0:2] = [-1, -1]; flags['eventtype'] =  2; flags['fieldedby'] = 6; flags['playonbatter'] = '43'; flags['playonrunner1'] = '64'; putout[0] += 1; flags[strval('putout', putout[0])] = 4; assist[0] += 1; flags[strval('assist', assist[0])] = 6; putout[0] += 1; flags[strval('putout', putout[0])] = 3; assist[0] += 1; flags[strval('assist', assist[0])] = 4
                case '7':           runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 7; flags['playonbatter'] = '7'; putout[0] += 1; flags[strval('putout', putout[0])] = 7
                case '8':           runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 8; flags['playonbatter'] = '8'; putout[0] += 1;flags[strval('putout', putout[0])] = 8
                case '9':           runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 9; flags['playonbatter'] = '9'; putout[0]+=1; flags[strval('putout', putout[0])] = 9
                case 'B-1':         runners[0] =  1
                case 'B-2' :        runners[0] =  2
                case 'B-2(E5/TH)':  runners[0] =  2;         flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 5; flags['error' + str(flags['numerrors']) + 'type'] = 'T'
                case 'B-3' :        runners[0] =  3
                case 'B-3(E6)':     runners[0] =  3;         flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 6; flags['error' + str(flags['numerrors']) + 'type'] = 'F'
                case 'B-3(E7)':     runners[0] =  3;         flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 7; flags['error' + str(flags['numerrors']) + 'type'] = 'F'
                case 'B-3(E9/TH)':  runners[0] =  3;         flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 9; flags['error' + str(flags['numerrors']) + 'type'] = 'T'
                case 'B-3(E9)':  runners[0] =  3;         flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 9; flags['error' + str(flags['numerrors']) + 'type'] = 'F'
                case 'B-H(E4/TH)(NR)': runners[0] =  4;        flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 4; flags['error' + str(flags['numerrors']) + 'type'] = 'T'; rbi -= 1 
                case '3-H(E2/TH)(NR)(UR)': runners[3] =  5;        flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 2; flags['error' + str(flags['numerrors']) + 'type'] = 'T'; rbi -= 1 
                case 'CSH(242536)': runners[3] = -1;         flags['eventtype'] =  6; flags['batterevent'] = False; flags['ab'] = False; flags['playonrunner3'] = '242536'; flags['csrunner3'] = True; putout[0]+=1; flags[strval('putout', putout[0])] = 6; assist[0] += 1; flags[strval('assist', assist[0])] = 2; assist[0] += 1; flags[strval('assist', assist[0])] = 4; assist[0] += 1; flags[strval('assist', assist[0])] = 5; assist[0] += 1; flags[strval('assist', assist[0])] = 3
                case 'POCS2(136)': runners[1] = -1;         flags['eventtype'] =  8; flags['batterevent'] = False; flags['ab'] = False; flags['playonrunner1'] = '136'; flags['csrunner1'] = True; flags['porunner1'] = True; putout[0]+=1; flags[strval('putout', putout[0])] = 6; assist[0] += 1; flags[strval('assist', assist[0])] = 1; assist[0] += 1; flags[strval('assist', assist[0])] = 3
                case 'CS2(24)':     runners[1] = -1;         flags['eventtype'] =  6; flags['batterevent'] = False; flags['ab'] = False; flags['playonrunner1'] = '24'; flags['csrunner1'] = True; putout[0]+=1; flags[strval('putout', putout[0])] = 4; assist[0] += 1; flags[strval('assist', assist[0])] = 2
                case 'D39':         runners[0] =  2;         flags['eventtype'] = 21; flags['fieldedby'] = 3;                           flags['hitvalue'] = 2
                case 'D57':         runners[0] =  2;         flags['eventtype'] = 21; flags['fieldedby'] = 5;                           flags['hitvalue'] = 2
                case 'D7':          runners[0] =  2;         flags['eventtype'] = 21; flags['fieldedby'] = 7;                           flags['hitvalue'] = 2
                case 'D8':          runners[0] =  2;         flags['eventtype'] = 21; flags['fieldedby'] = 8;                           flags['hitvalue'] = 2
                case 'D9':          runners[0] =  2;         flags['eventtype'] = 21; flags['fieldedby'] = 9;                           flags['hitvalue'] = 2
                case 'DGR':         runners[0] =  2;         flags['eventtype'] = 21;                                                   flags['hitvalue'] = 2
                case 'DI':                                   flags['eventtype'] = 5;  flags['batterevent'] = False; flags['ab'] = False
                case 'FC':                                   flags['eventtype'] = 19
                case 'FC4':                                  flags['eventtype'] = 19; flags['fieldedby'] = 4
                case 'FC5':                                  flags['eventtype'] = 19; flags['fieldedby'] = 5
                case 'FC6':                                  flags['eventtype'] = 19; flags['fieldedby'] = 6
                case 'HP':          runners[0] =  1;         flags['eventtype'] = 16;                               flags['ab'] = False; flags['responsible'] = False
                case 'HR':          runners[0] =  4;         flags['eventtype'] = 23;                                                   flags['hitvalue'] = 4
                case 'IW':          runners[0] =  1;         flags['eventtype'] = 15;                               flags['ab'] = False
                case 'PB':                                   flags['eventtype'] = 10;                               flags['batterevent'] = False; flags['ab'] = False; flags['passedball'] = True
                case 'K':           runners[0] = -1;         flags['eventtype'] =  3; flags['playonbatter'] = '2'; putout[0]+=1; flags[strval('putout', putout[0])] = 2
                case 'K+CS2(24)':       runners[0] = -1; runners[1] =  -1;        flags['eventtype'] =  3; flags['playonbatter'] = '2'; flags['playonrunner1'] = '24'; putout[0]+=1; flags[strval('putout', putout[0])] = 2; flags['csrunner1'] = True; putout[0]+=1; flags[strval('putout', putout[0])] = 4; assist[0] += 1; flags[strval('assist', assist[0])] = 2
                case 'K+SB2':       runners[0] = -1; runners[1] =  2;        flags['eventtype'] =  3; flags['playonbatter'] = '2'; putout[0]+=1; flags[strval('putout', putout[0])] = 2; flags['sbrunner1'] = True
                case 'K+WP':       runners[0] = -1; flags['eventtype'] =  3; flags['wildpitch']=True
                case 'K+PB':       runners[0] = -1; flags['eventtype'] =  3; flags['passedball']=True
                case 'PO1(13)':     runners[1] = -1;         flags['eventtype'] =  8; flags['batterevent'] = False; flags['ab'] = False; flags['playonrunner1'] = '13'; flags['porunner1'] = True; putout[0] += 1; flags[strval('putout', putout[0])] = 3; assist[0] += 1; flags[strval('assist', assist[0])] = 1
                case 'PO1(E1/TH)':     flags['eventtype'] =  8; flags['batterevent'] = False; flags['ab'] = False;         flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 1; flags['error' + str(flags['numerrors']) + 'type'] = 'D'; flags['playonrunner1'] = 'E1'; flags['porunner1'] = True
                case 'S1':          runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 1;                           flags['hitvalue'] = 1
                case 'S14':         runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 1;                           flags['hitvalue'] = 1
                case 'S16':         runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 1;                           flags['hitvalue'] = 1
                case 'S3':          runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 3;                           flags['hitvalue'] = 1
                case 'S39':         runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 3;                           flags['hitvalue'] = 1
                case 'S4':          runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 4;                           flags['hitvalue'] = 1
                case 'S48':         runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 4;                           flags['hitvalue'] = 1
                case 'S5':          runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 5;                           flags['hitvalue'] = 1
                case 'S54':         runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 5;                           flags['hitvalue'] = 1
                case 'S56':         runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 5;                           flags['hitvalue'] = 1
                case 'S6':          runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 6;                           flags['hitvalue'] = 1
                case 'S7':          runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 7;                           flags['hitvalue'] = 1
                case 'S8':          runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 8;                           flags['hitvalue'] = 1
                case 'S9':          runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 9;                           flags['hitvalue'] = 1
                case 'SB2':         runners[1] =  2;         flags['eventtype'] =  4; flags['batterevent'] = False; flags['ab'] = False; flags['sbrunner1'] = True
                case 'SB3':         runners[2] =  3;         flags['eventtype'] =  4; flags['batterevent'] = False; flags['ab'] = False; flags['sbrunner2'] = True
                case 'T8':          runners[0] =  3;         flags['eventtype'] = 22; flags['fieldedby'] = 8;                           flags['hitvalue'] = 3
                case 'T9':          runners[0] =  3;         flags['eventtype'] = 22; flags['fieldedby'] = 9;                           flags['hitvalue'] = 3
                case 'T98':         runners[0] =  3;         flags['eventtype'] = 22; flags['fieldedby'] = 9;                           flags['hitvalue'] = 3
                case 'W':           runners[0] =  1;         flags['eventtype'] = 14;                               flags['ab'] = False
                case 'BX1(23)': runners[0] = -1; flags['playonbatter'] = '23'; flags[strval('putout', putout[0])] = 3; assist[0] += 1; flags[strval('assist', assist[0])] = 2
                case 'BX2(36)':     runners[0] = -1; flags['playonbatter'] = '36'; putout[0] += 1; flags[strval('putout', putout[0])] = 6; assist[0] += 1; flags[strval('assist', assist[0])] = 3
                case 'BX2(74)':     runners[0] = -1; flags['playonbatter'] = '74'; putout[0] += 1; flags[strval('putout', putout[0])] = 4; assist[0] += 1; flags[strval('assist', assist[0])] = 7
                case 'BX2(9346)':   runners[0] = -1; flags['playonbatter'] = '9346'; putout[0] += 1; flags[strval('putout', putout[0])] = 6; assist[0] += 1; flags[strval('assist', assist[0])] = 9; assist[0] += 1; flags[strval('assist', assist[0])] = 3; assist[0] += 1; flags[strval('assist', assist[0])] = 4
                case 'BX2(96)':     runners[0] = -1; flags['playonbatter'] = '96'; putout[0] += 1; flags[strval('putout', putout[0])] = 6; assist[0] += 1; flags[strval('assist', assist[0])] = 9
                case '1X1(3)':      runners[1] = -1; flags['playonrunner1'] = '3'; putout[0] += 1; flags[strval('putout', putout[0])] = 3
                case '1X1(83)':     runners[1] = -1; flags['playonrunner1'] = '83'; putout[0] += 1; flags[strval('putout', putout[0])] = 3; assist[0] += 1; flags[strval('assist', assist[0])] = 8
                case '1X1(834)':    runners[1] = -1; flags['playonrunner1'] = '834'; putout[0] += 1; flags[strval('putout', putout[0])] = 4; assist[0] += 1; flags[strval('assist', assist[0])] = 8; assist[0] += 1; flags[strval('assist', assist[0])] = 3
                case '3X3(65)':     runners[3] = -1; flags['playonrunner3'] = '65'; putout[0] += 1; flags[strval('putout', putout[0])] = 5; assist[0] += 1; flags[strval('assist', assist[0])] = 6
                case '3XH(32)':     runners[3] = -1; flags['playonrunner3'] = '32'; putout[0] += 1; flags[strval('putout', putout[0])] = 2; assist[0] += 1; flags[strval('assist', assist[0])] = 3
                case '3XH(42)':     runners[3] = -1; flags['playonrunner3'] = '42'; putout[0] += 1; flags[strval('putout', putout[0])] = 2; assist[0] += 1; flags[strval('assist', assist[0])] = 4
                case '3XH(52)':     runners[3] = -1; flags['playonrunner3'] = '52'; putout[0] += 1; flags[strval('putout', putout[0])] = 2; assist[0] += 1; flags[strval('assist', assist[0])] = 5
                case '3XH(62)':     runners[3] = -1; flags['playonrunner3'] = '62'; putout[0] += 1; flags[strval('putout', putout[0])] = 2; assist[0] += 1; flags[strval('assist', assist[0])] = 6
                case 'BG13':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '13'; flags['bunt'] = True
                case 'BG1S':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '1S'; flags['bunt'] = True
                case 'BG23':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '23'; flags['bunt'] = True
                case 'BG25':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '25'; flags['bunt'] = True
                case 'DP':                                   flags['doubleplay'] = True
                case 'E1':     flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 1; flags['eventtype'] = 18; flags['fieldedby'] = 1; flags['error' + str(flags['numerrors']) + 'type'] = 'F'
                case 'E2':     flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 2; flags['error' + str(flags['numerrors']) + 'type'] = 'F'
                case 'E3':     flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 3; flags['eventtype'] = 18; flags['fieldedby'] = 3; flags['error' + str(flags['numerrors']) + 'type'] = 'F'
                case 'E4':     flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 4; flags['eventtype'] = 18; flags['fieldedby'] = 4; flags['error' + str(flags['numerrors']) + 'type'] = 'F'
                case 'E5':     flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 5; flags['eventtype'] = 18; flags['fieldedby'] = 5; flags['error' + str(flags['numerrors']) + 'type'] = 'F'
                case 'E6':     flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 6; flags['eventtype'] = 18; flags['fieldedby'] = 6; flags['error' + str(flags['numerrors']) + 'type'] = 'T'
                case 'E6/TH':  flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 6; flags['eventtype'] = 18; flags['fieldedby'] = 6; flags['error' + str(flags['numerrors']) + 'type'] = 'T'; flags['error' + str(flags['numerrors']) + 'type'] = 'T'
                case 'F3D':    flags['battedballtype'] = 'F'; flags['hitlocation'] = '3D'
                case 'F4D':    flags['battedballtype'] = 'F'; flags['hitlocation'] = '4D'
                case 'F6MD':   flags['battedballtype'] = 'F'; flags['hitlocation'] = '6MD'
                case 'F6D':    flags['battedballtype'] = 'F'; flags['hitlocation'] = '6D'
                case 'F7':     flags['battedballtype'] = 'F'; flags['hitlocation'] = '7'
                case 'F7LS':   flags['battedballtype'] = 'F'; flags['hitlocation'] = '7LS'
                case 'F78':    flags['battedballtype'] = 'F'; flags['hitlocation'] = '78'
                case 'F78+':   flags['battedballtype'] = 'F'; flags['hitlocation'] = '78'
                case 'F78S':   flags['battedballtype'] = 'F'; flags['hitlocation'] = '78S'
                case 'F78D':   flags['battedballtype'] = 'F'; flags['hitlocation'] = '78D'
                case 'F78D+':  flags['battedballtype'] = 'F'; flags['hitlocation'] = '78D'
                case 'F78XD':  flags['battedballtype'] = 'F'; flags['hitlocation'] = '78XD'
                case 'F78XD+': flags['battedballtype'] = 'F'; flags['hitlocation'] = '78XD'
                case 'F7L':    flags['battedballtype'] = 'F'; flags['hitlocation'] = '7L'
                case 'F89D':   flags['battedballtype'] = 'F'; flags['hitlocation'] = '89D'
                case 'F8D':    flags['battedballtype'] = 'F'; flags['hitlocation'] = '8D'
                case 'F7D':    flags['battedballtype'] = 'F'; flags['hitlocation'] = '7D'
                case 'F7D+':   flags['battedballtype'] = 'F'; flags['hitlocation'] = '7D'
                case 'F7LD':   flags['battedballtype'] = 'F'; flags['hitlocation'] = '7LD'
                case 'F7LSF':  flags['battedballtype'] = 'F'; flags['hitlocation'] = '7LSF'
                case 'F7S':    flags['battedballtype'] = 'F'; flags['hitlocation'] = '7S'
                case 'F8':     flags['battedballtype'] = 'F'; flags['hitlocation'] = '8'
                case 'F8+':    flags['battedballtype'] = 'F'; flags['hitlocation'] = '8'
                case 'F89':    flags['battedballtype'] = 'F'; flags['hitlocation'] = '89'
                case 'F89S':   flags['battedballtype'] = 'F'; flags['hitlocation'] = '89S'
                case 'F89XD':  flags['battedballtype'] = 'F'; flags['hitlocation'] = '89XD'
                case 'F89XD+': flags['battedballtype'] = 'F'; flags['hitlocation'] = '89XD'
                case 'F8S':    flags['battedballtype'] = 'F'; flags['hitlocation'] = '8S'
                case 'F8XD':   flags['battedballtype'] = 'F'; flags['hitlocation'] = '8XD'
                case 'F8XD+':  flags['battedballtype'] = 'F'; flags['hitlocation'] = '8XD'
                case 'F9D':    flags['battedballtype'] = 'F'; flags['hitlocation'] = '9D'
                case 'F9D+':   flags['battedballtype'] = 'F'; flags['hitlocation'] = '9D'
                case 'F9L':    flags['battedballtype'] = 'F'; flags['hitlocation'] = '9L'
                case 'F9LD':   flags['battedballtype'] = 'F'; flags['hitlocation'] = '9LD'
                case 'F9LD+':  flags['battedballtype'] = 'F'; flags['hitlocation'] = '9LD'
                case 'F9LF':   flags['battedballtype'] = 'F'; flags['hitlocation'] = '9LF'
                case 'F9LS':   flags['battedballtype'] = 'F'; flags['hitlocation'] = '9LS'
                case 'F9LSF':  flags['battedballtype'] = 'F'; flags['hitlocation'] = '9LSF'
                case 'F9S':    flags['battedballtype'] = 'F'; flags['hitlocation'] = '9S'
                case 'F9':     flags['battedballtype'] = 'F'; flags['hitlocation'] = '9'
                case 'F9+':    flags['battedballtype'] = 'F'; flags['hitlocation'] = '9'
                case 'FO':     flags['battedballtype'] = 'F'
                case 'FL':     flags['foul'] = True
                case 'G1':     flags['battedballtype'] = 'G'; flags['hitlocation'] = '1'
                case 'G13':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '13'
                case 'G1-':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '1'
                case 'G15+':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '15'
                case 'G1S':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '1S'
                case 'G1S-':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '1S'
                case 'G23-':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '23'
                case 'G25':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '25'
                case 'G25-':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '25'
                case 'G2':     flags['battedballtype'] = 'G'; flags['hitlocation'] = '2'
                case 'G2-':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '2'
                case 'G3':     flags['battedballtype'] = 'G'; flags['hitlocation'] = '3'
                case 'G3-':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '3'
                case 'G3+':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '3'
                case 'G34':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '34'
                case 'G34-':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '34'
                case 'G34+':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '34'
                case 'G34D':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '34D'
                case 'G34S':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '34S'
                case 'G34S-':  flags['battedballtype'] = 'G'; flags['hitlocation'] = '34S'
                case 'G3S':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '3S'
                case 'G3S-':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '3S'
                case 'G4':     flags['battedballtype'] = 'G'; flags['hitlocation'] = '4'
                case 'G4-':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '4'
                case 'G4S-':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '4S'
                case 'G4+':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '4'
                case 'G4D':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '4D'
                case 'G4D+':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '4D'
                case 'G4M':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '4M'
                case 'G4MD+':  flags['battedballtype'] = 'G'; flags['hitlocation'] = '4MD'
                case 'G4MS-':  flags['battedballtype'] = 'G'; flags['hitlocation'] = '4MS'
                case 'G4M+':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '4M'
                case 'G4S':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '4S'
                case 'G4S-':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '4S'
                case 'G5':     flags['battedballtype'] = 'G'; flags['hitlocation'] = '5'
                case 'G5+':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '5'
                case 'G5S':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '5S'
                case 'G5S-':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '5S'
                case 'G56':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '56'
                case 'G56+':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '56'
                case 'G56D':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '56D'
                case 'G56S':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '56S'
                case 'G56S-':  flags['battedballtype'] = 'G'; flags['hitlocation'] = '56S'
                case 'G56S+':  flags['battedballtype'] = 'G'; flags['hitlocation'] = '56S'
                case 'G6':     flags['battedballtype'] = 'G'; flags['hitlocation'] = '6'
                case 'G6-':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '6'
                case 'G6+':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '6'
                case 'G6D':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '6D'
                case 'G6M':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '6M'
                case 'G6M+':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '6M'
                case 'G6M-':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '6M'
                case 'G6MS':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '6MS'
                case 'G6MS-':  flags['battedballtype'] = 'G'; flags['hitlocation'] = '6MS'
                case 'G6MS+':  flags['battedballtype'] = 'G'; flags['hitlocation'] = '6MS'
                case 'G6S':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '6S'
                case 'G6S+':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '6S'
                case 'G6S-':   flags['battedballtype'] = 'G'; flags['hitlocation'] = '6S'
                case 'G8S':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '8S'
                case 'GDP':    flags['battedballtype'] = 'G'; flags['doubleplay'] = True
                case 'L3':     flags['battedballtype'] = 'L'; flags['hitlocation'] = '3'
                case 'L34':    flags['battedballtype'] = 'L'; flags['hitlocation'] = '34'
                case 'L34-':   flags['battedballtype'] = 'L'; flags['hitlocation'] = '34'
                case 'L3D':    flags['battedballtype'] = 'L'; flags['hitlocation'] = '3D'
                case 'L4':     flags['battedballtype'] = 'L'; flags['hitlocation'] = '4'
                case 'L4M+':   flags['battedballtype'] = 'L'; flags['hitlocation'] = '4M'
                case 'L4MD+':  flags['battedballtype'] = 'L'; flags['hitlocation'] = '4MD'
                case 'L5':     flags['battedballtype'] = 'L'; flags['hitlocation'] = '5'
                case 'L56':    flags['battedballtype'] = 'L'; flags['hitlocation'] = '56'
                case 'L56D+':  flags['battedballtype'] = 'L'; flags['hitlocation'] = '56D'
                case 'L5D':    flags['battedballtype'] = 'L'; flags['hitlocation'] = '5D'
                case 'L5D+':   flags['battedballtype'] = 'L'; flags['hitlocation'] = '5D'
                case 'L6':     flags['battedballtype'] = 'L'; flags['hitlocation'] = '6'
                case 'L6+':    flags['battedballtype'] = 'L'; flags['hitlocation'] = '6'
                case 'L6MD':   flags['battedballtype'] = 'L'; flags['hitlocation'] = '6MD'
                case 'L6D':    flags['battedballtype'] = 'L'; flags['hitlocation'] = '6D'
                case 'L6D+':   flags['battedballtype'] = 'L'; flags['hitlocation'] = '6D'
                case 'L7':     flags['battedballtype'] = 'L'; flags['hitlocation'] = '7'
                case 'L7L':    flags['battedballtype'] = 'L'; flags['hitlocation'] = '7L'
                case 'L7L+':   flags['battedballtype'] = 'L'; flags['hitlocation'] = '7L'
                case 'L7S+':   flags['battedballtype'] = 'L'; flags['hitlocation'] = '7S'
                case 'L7+':    flags['battedballtype'] = 'L'; flags['hitlocation'] = '7'
                case 'L78':    flags['battedballtype'] = 'L'; flags['hitlocation'] = '78'
                case 'L78S':   flags['battedballtype'] = 'L'; flags['hitlocation'] = '78S'
                case 'L78D+':  flags['battedballtype'] = 'L'; flags['hitlocation'] = '78D'
                case 'L78XD':  flags['battedballtype'] = 'L'; flags['hitlocation'] = '78XD'
                case 'L78XD+': flags['battedballtype'] = 'L'; flags['hitlocation'] = '78XD'
                case 'L78D':   flags['battedballtype'] = 'L'; flags['hitlocation'] = '78D'
                case 'L7D':    flags['battedballtype'] = 'L'; flags['hitlocation'] = '7D'
                case 'L7D+':   flags['battedballtype'] = 'L'; flags['hitlocation'] = '7D'
                case 'L7LD':   flags['battedballtype'] = 'L'; flags['hitlocation'] = '7LD'
                case 'L7LD+':  flags['battedballtype'] = 'L'; flags['hitlocation'] = '7LD'
                case 'L7LS':   flags['battedballtype'] = 'L'; flags['hitlocation'] = '7LS'
                case 'L7LS+':  flags['battedballtype'] = 'L'; flags['hitlocation'] = '7LS'
                case 'L7S':    flags['battedballtype'] = 'L'; flags['hitlocation'] = '7S'
                case 'L8':     flags['battedballtype'] = 'L'; flags['hitlocation'] = '8'
                case 'L8+':    flags['battedballtype'] = 'L'; flags['hitlocation'] = '8'
                case 'L8XD':   flags['battedballtype'] = 'L'; flags['hitlocation'] = '8XD'
                case 'L8XD+':  flags['battedballtype'] = 'L'; flags['hitlocation'] = '8XD'
                case 'L89':    flags['battedballtype'] = 'L'; flags['hitlocation'] = '89'
                case 'L89+':   flags['battedballtype'] = 'L'; flags['hitlocation'] = '89'
                case 'L89D':   flags['battedballtype'] = 'L'; flags['hitlocation'] = '89D'
                case 'L89D+':  flags['battedballtype'] = 'L'; flags['hitlocation'] = '89D'
                case 'L89XD':  flags['battedballtype'] = 'L'; flags['hitlocation'] = '89XD'
                case 'L89XD+': flags['battedballtype'] = 'L'; flags['hitlocation'] = '89XD'
                case 'L89S':   flags['battedballtype'] = 'L'; flags['hitlocation'] = '89S'
                case 'L8D':    flags['battedballtype'] = 'L'; flags['hitlocation'] = '8D'
                case 'L8D+':   flags['battedballtype'] = 'L'; flags['hitlocation'] = '8D'
                case 'L8S':    flags['battedballtype'] = 'L'; flags['hitlocation'] = '8S'
                case 'L9':     flags['battedballtype'] = 'L'; flags['hitlocation'] = '9'
                case 'L9+':    flags['battedballtype'] = 'L'; flags['hitlocation'] = '9'
                case 'L9D':    flags['battedballtype'] = 'L'; flags['hitlocation'] = '9D'
                case 'L9D+':   flags['battedballtype'] = 'L'; flags['hitlocation'] = '9D'
                case 'L9L':    flags['battedballtype'] = 'L'; flags['hitlocation'] = '9L'
                case 'L9LD':   flags['battedballtype'] = 'L'; flags['hitlocation'] = '9LD'
                case 'L9LD+':  flags['battedballtype'] = 'L'; flags['hitlocation'] = '9LD'
                case 'L9L+':   flags['battedballtype'] = 'L'; flags['hitlocation'] = '9L'
                case 'L9LS':   flags['battedballtype'] = 'L'; flags['hitlocation'] = '9LS'
                case 'L9S':    flags['battedballtype'] = 'L'; flags['hitlocation'] = '9S'
                case 'L9S+':   flags['battedballtype'] = 'L'; flags['hitlocation'] = '9S'
                case 'NP':     None
                case 'P1S-':   flags['battedballtype'] = 'P'; flags['hitlocation'] = '1S'
                case 'P3':     flags['battedballtype'] = 'P'; flags['hitlocation'] = '3'
                case 'P3DF':   flags['battedballtype'] = 'P'; flags['hitlocation'] = '3DF'
                case 'P3F':    flags['battedballtype'] = 'P'; flags['hitlocation'] = '3F'
                case 'P3SF':   flags['battedballtype'] = 'P'; flags['hitlocation'] = '3SF'
                case 'P3SF-':  flags['battedballtype'] = 'P'; flags['hitlocation'] = '3SF'
                case 'P3F-':   flags['battedballtype'] = 'P'; flags['hitlocation'] = '3F'
                case 'P2F':    flags['battedballtype'] = 'P'; flags['hitlocation'] = '2F'
                case 'P2F-':   flags['battedballtype'] = 'P'; flags['hitlocation'] = '2F'
                case 'P25':    flags['battedballtype'] = 'P'; flags['hitlocation'] = '25'
                case 'P34':    flags['battedballtype'] = 'P'; flags['hitlocation'] = '34'
                case 'P34D':   flags['battedballtype'] = 'P'; flags['hitlocation'] = '34D'
                case 'P34S':   flags['battedballtype'] = 'P'; flags['hitlocation'] = '34S'
                case 'P5':     flags['battedballtype'] = 'P'; flags['hitlocation'] = '5'
                case 'BG1S-':  flags['battedballtype'] = 'G'; flags['hitlocation'] = '1S'; flags['bunt'] = True
                case 'BL5S-':  flags['battedballtype'] = 'L'; flags['hitlocation'] = '5S'; flags['bunt'] = True
                case 'BP2F':   flags['battedballtype'] = 'P'; flags['hitlocation'] = '2F'; flags['bunt'] = True
                case 'BP25F-': flags['battedballtype'] = 'P'; flags['hitlocation'] = '25F'; flags['bunt'] = True
                case 'BP5S-':  flags['battedballtype'] = 'P'; flags['hitlocation'] = '5S'; flags['bunt'] = True
                case 'P4':     flags['battedballtype'] = 'P'; flags['hitlocation'] = '4'
                case 'P4S':    flags['battedballtype'] = 'P'; flags['hitlocation'] = '4S'
                case 'P4MD':   flags['battedballtype'] = 'P'; flags['hitlocation'] = '4MD'
                case 'P56':    flags['battedballtype'] = 'P'; flags['hitlocation'] = '56'
                case 'P56D':   flags['battedballtype'] = 'P'; flags['hitlocation'] = '56D'
                case 'P56S':   flags['battedballtype'] = 'P'; flags['hitlocation'] = '56S'
                case 'P5D':    flags['battedballtype'] = 'P'; flags['hitlocation'] = '5D'
                case 'P5F':    flags['battedballtype'] = 'P'; flags['hitlocation'] = '5F'
                case 'P5S':    flags['battedballtype'] = 'P'; flags['hitlocation'] = '5S'
                case 'P6':     flags['battedballtype'] = 'P'; flags['hitlocation'] = '6'
                case 'P6D':    flags['battedballtype'] = 'P'; flags['hitlocation'] = '6D'
                case 'P6MD':   flags['battedballtype'] = 'P'; flags['hitlocation'] = '6MD'
                case 'P9S':    flags['battedballtype'] = 'P'; flags['hitlocation'] = '9S'
                case 'BK':     flags['eventtype'] = 11; flags['batterevent'] = False; flags['ab'] = False
                case 'C':      flags['eventtype'] = 17; flags['ab'] = False
                case 'SH' :    flags['eventtype'] = 2; flags['ab'] = False; flags['sachit'] = True
                case 'SF' :    flags['eventtype'] = 2; flags['ab'] = False; flags['sacfly'] = True
                case 'TH':     flags['error' + str(flags['numerrors']) + 'type'] = 'T'
                case 'WP':     flags['eventtype'] =  9; flags['batterevent'] = False; flags['ab'] = False; flags['wildpitch']=True
                case '3-3' :   None
                case  _ : print (r, file=sys.stderr)
    return rbi

def SplitEvent (event: str) -> list[str]:
    sp = re.split('[.;\\/\\(\\)]', event)
    paren = False
    closeparen = False
    retval = []
    j = -1
    for i in range(len(sp)):
        paren = paren and not closeparen
        if j>=0 and (event[j] == '('):
            paren = True
            closeparen = False
        if j>=0 and (event[j] == ')'):
            closeparen = True
        if (paren):
            retval[-1] += event[j] + sp[i]
        else:
            retval.append(sp[i])
        j += len(sp[i])+1
    return retval
'''

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
          (0 if runners[3] == -1 else (1<<13)) |\
          (0 if runners[2] == -1 else (1<<12)) |\
          (0 if runners[1] == -1 else (1<<11)) |\
          (0 if runners[0] == -1 else (1<<10))]
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