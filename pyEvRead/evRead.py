import math

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

with open('../_data/2023/out/States.bin', 'rb') as binary_file:
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
    print ('ID', 'N', 'Value', 'stdev(Transitions)')
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
    print (stdev(States[1]['Transitions']))
    for t in States[1]['Transitions']:
        print (t['Value'], t['Frequency'])
    '''