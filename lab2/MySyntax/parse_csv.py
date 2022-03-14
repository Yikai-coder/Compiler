# coding:utf-8
# 脚本功能：
# 将旧LR1分析表转换为序号表示的分析表
import csv
import re

def list_find(list, pattern):
    for i in range(0, len(list)):
        if(list[i]==pattern):
            return i
    return -1

def parse_csv(file_name_read, file_name_write):
    csv_file_read = open(file_name_read, mode='r')
    csv_file_write = open(file_name_write, mode= 'w', newline="")
    reader=csv.reader(csv_file_read)
    writer = csv.writer(csv_file_write, delimiter = "\t")
    pattern_shift = r"shift"
    pattern_reduce = r"reduce"
    generators = []


    for item in reader:
        if(item[0]=='状态' or len(item[0])==0):
            # print(item)
            writer.writerow(item)    
            continue
        # print(item)
        for i in range(1, len(item)):
            if(len(item[i])==0):
                continue
            if((position:=re.match(pattern_shift, item[i]))!=None):
                item[i] = item[i].replace("shift", "s")
                item[i] = item[i].replace("?", " ")
                # print(item[i])
            elif((position:=re.match(pattern_reduce, item[i]))!=None):
                item[i] = item[i].replace("reduce", "r")
                item[i] = item[i].replace("?", " ")
                # generator = item[i].split("r")[1]
                generator = item[i][2:]
                generator = generator.split("->")
                # generator = (generator[0].strip(), generator[1].strip())
                # generator = generator[0].replace(" ", "")+":"+generator[1].replace(" ", "")
                generator[1] = "" if generator[1]==" ε" else generator[1]
                generator = generator[0].strip()+":"+generator[1].strip();
                idx = list_find(generators, generator)
                if(idx==-1):
                    generators.append(generator)
                    item[i] = item[i][0] + " " + str(len(generators)-1)
                else:
                    item[i] = item[i][0] + " " + str(idx)
                # print(item[i])
            elif(item[i]=="accept"):
                item[i] = 'a'
            elif(item[i].isdigit()):
                item[i] = "j "+item[i]
        # print(item)
        writer.writerow(item)
    # print(generators)
    writer.writerow(generators)
    csv_file_read.close()
    csv_file_write.close()

if __name__ == '__main__':
    parse_csv(".\LR1.csv", "SimpleSyntax_LR1_new.tsv")

