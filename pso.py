#!/usr/bin/env python
#    This file is part of DEAP.
#
#    DEAP is free software: you can redistribute it and/or modify
#    it under the terms of the GNU Lesser General Public License as
#    published by the Free Software Foundation, either version 3 of
#    the License, or (at your option) any later version.
#
#    DEAP is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public
#    License along with DEAP. If not, see <http://www.gnu.org/licenses/>.

import operator
import random
import os
import threading
import numpy

from deap import base
from deap import benchmarks
from deap import creator
from deap import tools

work_dir = "/home/yinly/testlsdyna"

def calcu(individual):
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
    return obj, abs(rate - 30)*10

creator.create("FitnessMax", base.Fitness, weights=(1.0, -1.0))
creator.create("Particle", list, fitness=creator.FitnessMax, speed=list, 
    smin=None, smax=None, best=None)

def generate(size, pmin, pmax, smin, smax):
    part = creator.Particle(random.uniform(pmin, pmax) for _ in range(size)) 
    part.speed = [random.uniform(smin, smax) for _ in range(size)]
    part.smin = smin
    part.smax = smax
    return part

def updateParticle(part, best, phi1, phi2):
    u1 = (random.uniform(0, phi1) for _ in range(len(part)))
    u2 = (random.uniform(0, phi2) for _ in range(len(part)))
    v_u1 = map(operator.mul, u1, map(operator.sub, part.best, part))
    v_u2 = map(operator.mul, u2, map(operator.sub, best, part))
    part.speed = list(map(operator.add, part.speed, map(operator.add, v_u1, v_u2)))
    for i, speed in enumerate(part.speed):
        if speed < part.smin:
            part.speed[i] = part.smin
        elif speed > part.smax:
            part.speed[i] = part.smax
    part[:] = list(map(operator.add, part, part.speed))

toolbox = base.Toolbox()
toolbox.register("particle", generate, size=16, pmin=0, pmax=0.2, smin=-0.1, smax=0.1)
toolbox.register("population", tools.initRepeat, list, toolbox.particle)
toolbox.register("update", updateParticle, phi1=0.01, phi2=0.01)
toolbox.register("evaluate", calcu)

def main():
    pop = toolbox.population(n=5)
    stats = tools.Statistics(lambda ind: ind.fitness.values)
    stats.register("avg", numpy.mean)
    stats.register("std", numpy.std)
    stats.register("min", numpy.min)
    stats.register("max", numpy.max)

    logbook = tools.Logbook()
    logbook.header = ["gen", "evals"] + stats.fields

    GEN = 1000
    best = None

    for g in range(GEN):
        for part in pop:
            part.fitness.values = toolbox.evaluate(part)
            if not part.best or part.best.fitness < part.fitness:
                part.best = creator.Particle(part)
                part.best.fitness.values = part.fitness.values
            if not best or best.fitness < part.fitness:
                best = creator.Particle(part)
                best.fitness.values = part.fitness.values
        for part in pop:
            toolbox.update(part, best)

        # Gather all the fitnesses in one list and print the stats
        logbook.record(gen=g, evals=len(pop), **stats.compile(pop))
        print(logbook.stream)
    
    return pop, logbook, best

if __name__ == "__main__":
    main()