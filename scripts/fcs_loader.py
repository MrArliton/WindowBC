import fcsparser

meta = 0
data = 0

def loadFCS(path):
    global meta
    global data

    meta, data = fcsparser.parse(path, meta_data_only=False, reformat_meta=True)
    
def getColumnByName(col_name):
    
    if(meta != 0 and data != 0):
        return data[col_name] 
    else:
        return -1  

