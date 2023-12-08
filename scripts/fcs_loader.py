import fcsparser
import sys
def loadFCS(path):
    meta, data = fcsparser.parse(path, meta_data_only=False, reformat_meta=True)
    return data 

def saveCSV(path ,columns, data):
    data = data[columns].astype('str')
    temp = ''
    with open(path, 'w') as fl:
        # Names of columns 
        temp += columns[0]
        l_col = len(columns)
        for a in range(1,l_col):
            temp += ',' + columns[a]
        temp += '\n'
        # Data
        l_dt = len(data.index)
        for a in range(l_dt):
            temp += data.iloc[a,0]
            for i in range(1,l_col):
                 temp += ',' + data.iloc[a,i]
            temp += '\n'
        fl.write(temp)

data = loadFCS(sys.argv[1])

saveCSV(sys.argv[2], data.columns[1:],data)

