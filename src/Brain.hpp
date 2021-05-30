#pragma once
#include <inttypes.h>
#include <cmath>
#include <cstring>
#include <random>
#include "global.hpp"
using namespace std;

template <uint8_t Count>
struct Inputs
{
    float inputs[Count];
    static const uint8_t count = Count;
};

template <uint8_t Count>
struct Outputs
{
    float output[Count];
    static const uint8_t count = Count;
};

template <uint8_t NumConnections>
struct Neuron
{
    float bias = 1;
    float weights[NumConnections];
    float value = 0;
    static const uint8_t numConnections = NumConnections;
    Neuron()
    {
        for (int i = 0; i < NumConnections; ++i) {
            weights[i] = 1;
        }
    }
    void mutate()
    {
        rb(BiasMutateChance) && (bias = rf(BiasMin, BiasMax));
        for (int i = 0; i < NumConnections; ++i) {
            rb(WeightMutateChance) && (weights[i] = rf(WeightMin, WeightMax));
        }
    }
};

template <uint8_t NumConnect, uint8_t Count>
struct Layer
{
    Neuron<NumConnect> neurons[Count];
    static const uint8_t connectCount = NumConnect;
    static const uint8_t count = Count;

    template <uint8_t FromConnect>
    void next(Layer<FromConnect, NumConnect>& from)
    {
        for (int h = 0; h < Count; ++h) {
            float sum = 0;
            auto neuron = neurons + h;
            for (int i = 0; i < NumConnect; ++i) {
                sum += from.neurons[i].value * neuron->weights[i];
            }
            auto x = sum + neuron->bias;
            neuron->value = x / (1 + fabsf(x));
        }
    }

    void mutate()
    {
        for (int i = 0; i < Count; ++i) {
            neurons[i].mutate();
        }
    }
};

template <uint8_t NumInputs, uint8_t NumHidden, uint8_t NumOutputs>
struct Brain
{
    Layer<0, NumInputs> InputLayer;
    Layer<NumInputs, NumHidden> HiddenLayerA;
    Layer<NumHidden, NumHidden> HiddenLayerB;
    Layer<NumHidden, NumOutputs> OutputLayer;

    Outputs<NumOutputs> next(Inputs<NumInputs> inputs)
    {
        for (int i = 0; i < NumInputs; ++i) {
            InputLayer.neurons[i].value = inputs.inputs[i];
        }
        HiddenLayerA.next(InputLayer);
        HiddenLayerB.next(HiddenLayerA);
        OutputLayer.next(HiddenLayerB);
        return Outputs<NumOutputs>{
            .output = {
                OutputLayer.neurons[0].value,
                OutputLayer.neurons[1].value,
                OutputLayer.neurons[2].value,
                OutputLayer.neurons[3].value,
            }
        };
    }

    Brain mutant()
    {
        Brain copy;
        memcpy(&copy, this, sizeof(Brain));
        copy.InputLayer.mutate();
        copy.HiddenLayerA.mutate();
        copy.HiddenLayerB.mutate();
        copy.OutputLayer.mutate();
        return copy;
    }
};