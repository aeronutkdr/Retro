import math
import typing

def stdev(arr : list):
    SSQ = 0.
    SQS = 0.
    n = 0
    for a in arr:
        SSQ += a['Value'] * a['Value'] * a['Frequency']
        SQS += a['Value'] * a['Frequency']
        n += a['Frequency']
    if (n>0):
        SSQ /= n
        SQS *= SQS
        SQS /= n*n
    return 0. if SQS > SSQ else math.sqrt(SSQ - SQS)

def ProcessFile (f : str, v : typing.List[float], n : typing.List[int]):
    with open(f, 'rb') as binary_file:
        numStates = int.from_bytes(binary_file.read(4),byteorder='little')
        numTransitions = int.from_bytes(binary_file.read(4),byteorder='little')
        #print ('NumStates = ', numStates)
        #print ('NumTransitions = ', numTransitions)
        States = []
        for i in range(numStates):
            States.append({'ID'        : int.from_bytes(binary_file.read(2), byteorder='little'),
                           'EndInning' : int.from_bytes(binary_file.read(2), byteorder='little'),
                           'Frequency' : int.from_bytes(binary_file.read(4), byteorder='little'),
                           'Value'     : int.from_bytes(binary_file.read(4), byteorder='little')/(1<<30),
                           'Transitions' : []})
        for i in range(numTransitions):
            FromIdx = int.from_bytes(binary_file.read(2), byteorder='little')
            ToIdx = int.from_bytes(binary_file.read(2), byteorder='little')
            Freq = int.from_bytes(binary_file.read(4), byteorder='little')
            States[FromIdx]['Transitions'].append({'Value' : States[ToIdx]['Value'],
                                                'Frequency' : Freq})
        #print ('ID', 'N', 'Value', 'stdev(Transitions)')
        '''
        for s in States:
            # FEDCBA9876543210
            # OO321HBBBFFFFFSS
            if ((s['ID'] & 0x7FF) == 0x400) and (s['ID'] & 0xC000 != 0xC000):
                print (s['ID'], s['Frequency'], s['Value'], stdev(s['Transitions']))
            #print (s)
            #print (stdev(s['Transitions']))
            v = s['Frequency']-s['EndInning']
            for t in s['Transitions']:
                v -= t['Frequency']
            assert v == 0
        '''
        '''
        print (stdev(States[1]['Transitions']))
        for t in States[1]['Transitions']:
            print (t['Value'], t['Frequency'])
        '''
        '''
        parsing for 00321H/Count:
        '''
        for s in States:
            # FEDCBA 9876543210
            # OO321H BBBFFFFFSS
            outs    = (s['ID'] >> 14) & 3
            batter  = (s['ID'] >> 10) & 1
            balls   = (s['ID'] >> 7 ) & 0x07
            strikes = (s['ID']      ) & 0x03
            strikes += (s['ID'] >> 2) & 0x1F
            if outs < 3 and batter == 1 and balls < 4 and strikes < 3:
                # OO321BBSS
                runners = ((s['ID'] >> 11) & 7)
                id = (outs << 7) + (runners << 4) + (balls << 2) + strikes
                v[id] += s['Value'] * s['Frequency']
                n[id] += s['Frequency']
                #print (outs, ((s['ID'] >> 11) & 7) , balls, strikes, s['Frequency'], s['Value'], stdev(s['Transitions']))
            #print (s)
            #print (stdev(s['Transitions']))
            #v = s['Frequency']-s['EndInning']
            #for t in s['Transitions']:
                #v -= t['Frequency']
            #assert v == 0

v = [0.0 for i in range(1<<9)]
n = [0 for i in range(1<<9)]
for i in range(2010,2019):
    fname = '../data/' + str(i) + 'eve/States.bin'
    #print (fname)
    ProcessFile(fname, v, n)
#print ('O 3 2 1 B S Value N')
print ('ID N Value stddev(Transitions)')
for i in range(1<<9):
    if (n[i] > 0):
        # OO321BBSS
        outs = i>>7
        r3 = (i>>6) & 1
        r2 = (i>>5) & 1
        r1 = (i>>4) & 1
        balls = (i>>2) & 3
        strikes = (i>>0) & 3
        print ((outs<<12) + (r3<<10) + (r2<<9) + (r1<<8) + (balls<<4) + (strikes<<0), n[i], v[i]/float(n[i]), 0)