import ServoController
import pygame
import pygame.camera
from pygame.locals import *
import time

import sys
import math

import MultiNEAT as NEAT
import MultiNEAT.viz as viz
import random as rnd
import pickle
from mpu6050 import mpu6050


def createArray( length):
    inputArray = []
    for x in range(0,length):
        inputArray += (-1,x + 1 / length)
    return inputArray


visualWidth = 40
visualHieght = 30


    
    
def main():
    
    # Camera stuff
    DEVICE = '/dev/video0'
    SIZE = (visualWidth, visualHieght)
    
    
    
    rng = NEAT.RNG()
    rng.TimeSeed()

    params = NEAT.Parameters()
    params.PopulationSize = 40
    params.DynamicCompatibility = True
    params.WeightDiffCoeff = 1.0
    params.CompatTreshold = 2.0
    params.YoungAgeTreshold = 15
    params.SpeciesMaxStagnation = 15
    params.OldAgeTreshold = 35
    params.MinSpecies = 5
    params.MaxSpecies = 10
    params.RouletteWheelSelection = False
    params.Elitism = True
    params.RecurrentProb = 0.5
    params.OverallMutationRate = 0.2

    params.MutateWeightsProb = 0.8
    # params.MutateNeuronTimeConstantsProb = 0.1
    # params.MutateNeuronBiasesProb = 0.1

    params.WeightMutationMaxPower = 0.5
    params.WeightReplacementMaxPower = 1.0
    params.MutateWeightsSevereProb = 0.5
    params.WeightMutationRate = 0.25

    params.TimeConstantMutationMaxPower = 0.1
    params.BiasMutationMaxPower = params.WeightMutationMaxPower

    params.MaxWeight = 8

    params.MutateAddNeuronProb = 0.1
    params.MutateAddLinkProb = 0.2
    params.MutateRemLinkProb = 0.0

    params.MutateActivationAProb = 0.0;
    params.ActivationAMutationMaxPower = 0.5;
    params.MinActivationA = 1.1
    params.MaxActivationA = 6.9

    params.MinNeuronTimeConstant = 0.04
    params.MaxNeuronTimeConstant = 0.24

    params.MinNeuronBias = -params.MaxWeight
    params.MaxNeuronBias = params.MaxWeight

    params.MutateNeuronActivationTypeProb = 0.0

    params.ActivationFunction_SignedSigmoid_Prob = 0
    params.ActivationFunction_UnsignedSigmoid_Prob = 0
    params.ActivationFunction_Tanh_Prob = 1
    params.ActivationFunction_TanhCubic_Prob = 0
    params.ActivationFunction_SignedStep_Prob = 0
    params.ActivationFunction_UnsignedStep_Prob = 0
    params.ActivationFunction_SignedGauss_Prob = 0
    params.ActivationFunction_UnsignedGauss_Prob = 0
    params.ActivationFunction_Abs_Prob = 0
    params.ActivationFunction_SignedSine_Prob = 0
    params.ActivationFunction_UnsignedSine_Prob = 0
    params.ActivationFunction_Linear_Prob = 0

    params.CrossoverRate = 0.75  # mutate only 0.25
    params.MultipointCrossoverRate = 0.4
    params.SurvivalRate = 0.2
    
    params.MutateNeuronTraitsProb = 0.1
    params.MutateLinkTraitsProb = 0.1
    
    
    g = NEAT.Genome("latestGenome38")
    pop = NEAT.Population("latestPop38")
    
    # Robot stuff
    controller = ServoController.ServoController()
    controller.connectToAddress(0x40)
    controller.setupServoController()
    
    #sensor = mpu6050(0x68)
    sensor = None
    
    maxf_ever = 0
    generation = 0
    while True:
        generation += 1
        fitnesses = []
        for _, genome in enumerate(NEAT.GetGenomeList(pop)):
            fitness = evaluate_genome(genome, controller, sensor)
            fitnesses.append(fitness)
        for genome, fitness in zip(NEAT.GetGenomeList(pop), fitnesses):
            genome.SetFitness(fitness)
        maxf = max([x.GetFitness() for x in NEAT.GetGenomeList(pop)])
        print('Generation: {}, max fitness: {}'.format(generation, maxf))
        if maxf > maxf_ever:
            maxf_ever = maxf
        pop.Epoch()
    


def evaluate_genome(genome, controller, sensor):
    servoInversionList = [1,-1,-1,-1,1,1,-1,1,1,1,-1,-1]
    net = NEAT.NeuralNetwork()
    genome.BuildPhenotype(net)
    fitness = 5
    startTime = time.clock()
    runs = 0
    while(time.clock() - startTime < 20):
        runs += 1
        #inputList = [ord(x) / 255.0 for x in arrayImage]
        #If time is near the end reward for finding red
        #force = None
#         while force is None:
#             try:
#                 force = list( sensor.get_accel_data().values())
#             except:
#                 pass
#         force = [int( x * 10)/ 100.0 * -1 for x in force]
        force = [0,0,-.9]
        force.append(math.sin(runs /20.0))
        print force
        net.Input(force)
        net.Activate()
        out = net.Output()
        print [x for x in out]
        for x in range(0,12):            
            controller.setServoAmount(out[x] * 90 * servoInversionList[x] + 90, x)
        time.sleep(.1)
    print fitness
    return fitness



main()

