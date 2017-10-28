import ServoController
import pygame
import pygame.camera
from pygame.locals import *
import time

import sys

import MultiNEAT as NEAT
import MultiNEAT.viz as viz
import random as rnd
import pickle


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
    
    pygame.init()
    pygame.camera.init()
    display = pygame.display.set_mode(SIZE, 0)
    camera = pygame.camera.Camera(DEVICE, SIZE)
    camera.start()
    
    
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
    
    
    g = NEAT.Genome(0, visualWidth * visualHieght ,visualWidth * visualHieght, 12, False,
         NEAT.ActivationFunction.TANH, NEAT.ActivationFunction.TANH, 0, params)
    pop = NEAT.Population(g, params, True, 1.0, rnd.randint(0, 1000))
    
    # Robot stuff
    controller = ServoController.ServoController()
    controller.connectToAddress(0x40)
    controller.setupServoController()
    
    maxf_ever = 0
    generation = 0
    while True:
        generation += 1
        fitnesses = []
        for _, genome in enumerate(NEAT.GetGenomeList(pop)):
            fitness = evaluate_genome(genome, camera, controller)
            fitnesses.append(fitness)
        for genome, fitness in zip(NEAT.GetGenomeList(pop), fitnesses):
            genome.SetFitness(fitness)
        maxf = max([x.GetFitness() for x in NEAT.GetGenomeList(pop)])
        print('Generation: {}, max fitness: {}'.format(generation, maxf))
        if maxf > maxf_ever:
            maxf_ever = maxf
        pop.Epoch()
    
    camera.stop()
    pygame.quit()

def evaluate_genome(genome, camera, controller):
    net = NEAT.NeuralNetwork()
    genome.BuildPhenotype(net)
    fitness = 0
    startTime = time.clock()
    while(time.clock() - startTime < 9):
        scaleImage = pygame.transform.scale(camera.get_image(), (40,30))
        rawImage = scaleImage.get_buffer().raw
        arrayImage = list(rawImage)
        inputList = [ord(x) / 255.0 for x in arrayImage]
        # If time is near the end reward for finding red
        if time.clock() - startTime < 8:
            slice = inputList[::3]
            fitness += sum(slice)
            
        
        net.Input(inputList)
        net.Activate()
        out = net.Output()
        for x in range(0,12):            
            controller.setServoAmount(out[x] * 90 + 90, x)
    print fitness
    return fitness



main()

