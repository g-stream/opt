#!/usr/bin/env python
import random
import os
import sys
import threading
work_dir = "/home/yinly/testlsdyna"

def calcu(*argv):
    if len(argv) != 2*8 :
        raise Exception("error! should passed in 16 parameters!!\n")
    individual = [float(x) for x in argv]
    par_list = [individual[:8], individual[8:]]
    #create a unique directory
    lock = threading.Lock()
    lock.acquire()
    maxdir = 0
    if os.listdir(work_dir) != []:
        maxdir = max([int(i) for i in os.listdir(work_dir)])
    new_dir = work_dir + '/' + str(maxdir+1)
    os.mkdir(new_dir)
    lock.release()

    partxt = open(new_dir + "/optpar.txt", 'w')
    for elems in par_list:
        for elem in elems:
            partxt.write(str(elem))
            partxt.write(' ')
        partxt.write('\n')
    partxt.close()
    os.system("./main " + new_dir)
    data = ""
    with open(new_dir + "/obj.txt") as input:
        data = input.read()
    obj = float(data)
    with open(new_dir + "/rate.txt") as input:
        data = input.read()
    rate = float(data)*100
    print("obj: ", obj, " rate: ", rate)
    return obj + abs(rate - 30)*10
def calcu1(*argv):
    if len(argv) != 8 :
        raise Exception("error! should passed in 8 parameters!!\n")
    par_list = [[float(x) for x in argv]]
    #create a unique directory
    lock = threading.Lock()
    lock.acquire()
    maxdir = 0
    if os.listdir(work_dir) != []:
        maxdir = max([int(i) for i in os.listdir(work_dir)])
    new_dir = work_dir + '/' + str(maxdir+1)
    os.mkdir(new_dir)
    lock.release()

    partxt = open(new_dir + "/optpar.txt", 'w')
    for elems in par_list:
        for elem in elems:
            partxt.write(str(elem))
            partxt.write(' ')
        partxt.write('\n')
    partxt.close()
    os.system("./main " + new_dir)
    data = ""
    with open(new_dir + "/obj.txt") as input:
        data = input.read()
    obj = float(data)
    with open(new_dir + "/rate.txt") as input:
        data = input.read()
    rate = float(data)*100
    print("obj: ", obj, " rate: ", rate)
    return obj
def calcu1_with_rate(*argv):
    if len(argv) != 8 :
        raise Exception("error! should passed in 8 parameters!!\n")
    par_list = [[float(x) for x in argv]]
    #create a unique directory
    lock = threading.Lock()
    lock.acquire()
    maxdir = 0
    if os.listdir(work_dir) != []:
        maxdir = max([int(i) for i in os.listdir(work_dir)])
    new_dir = work_dir + '/' + str(maxdir+1)
    os.mkdir(new_dir)
    lock.release()

    partxt = open(new_dir + "/optpar.txt", 'w')
    for elems in par_list:
        for elem in elems:
            partxt.write(str(elem))
            partxt.write(' ')
        partxt.write('\n')
    partxt.close()
    os.system("./main " + new_dir)
    data = ""
    with open(new_dir + "/obj.txt") as input:
        data = input.read()
    obj = float(data)
    with open(new_dir + "/rate.txt") as input:
        data = input.read()
    rate = float(data)*100
    print("obj: ", obj, " rate: ", rate)
    return obj - abs(rate - 40)*10
if __name__ == "__main__":
    calcu1_with_rate(2,3,4,5,3,4,5,3)