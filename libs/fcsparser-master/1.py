import fcsparser
path = 'C:\\Users\\MrArl\\OneDrive\\Рабочий стол\\Данные Цитометр\\Александров\\Alexandrov 280916_control_002.fcs'
meta, data = fcsparser.parse(path, reformat_meta=True)
print(data)
