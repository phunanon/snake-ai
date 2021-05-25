#pragma once
#include <inttypes.h>
#include <cmath>
#include <cstring>
#include <random>
#include "global.hpp"
using namespace std;

float sigmoid(float x);
float max(float, float);

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
    float result = 0;
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
                sum += from.neurons[i].result * neuron->weights[i];
            }
            neuron->result = sigmoid(sum + neuron->bias);
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
class Brain
{
    static const uint8_t _numLayers = 4;
    Layer<0, NumInputs> _inputLayer;
    Layer<NumInputs, NumHidden> _hiddenLayerA;
    Layer<NumHidden, NumHidden> _hiddenLayerB;
    Layer<NumHidden, NumOutputs> _outputLayer;
public:
    Outputs<NumOutputs> next(Inputs<NumInputs> inputs)
    {
        for (int i = 0; i < NumInputs; ++i) {
            _inputLayer.neurons[i].result = inputs.inputs[i];
        }
        _hiddenLayerA.next(_inputLayer);
        _hiddenLayerB.next(_hiddenLayerA);
        _outputLayer.next(_hiddenLayerB);
        return Outputs<NumOutputs>{
            .output = {
                _outputLayer.neurons[0].result,
                _outputLayer.neurons[1].result,
                _outputLayer.neurons[2].result,
                _outputLayer.neurons[3].result,
            }
        };
    }

    Brain mutant()
    {
        Brain copy;
        memcpy(&copy, this, sizeof(Brain));
        copy._inputLayer.mutate();
        copy._hiddenLayerA.mutate();
        copy._hiddenLayerB.mutate();
        copy._outputLayer.mutate();
        return copy;
    }
};