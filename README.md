Hardware Neural Processing Unit
===

A Hardware Neural Network Processing Unit written in SystemC

## Instructions

The instruction bus is 32 bits wide.

| Data | Layer N length | Layer N+1 length | Activation function ID |
|------|----------------|------------------|------------------------|
| Bits | 15             | 15               | 2                      |

## Activation function nomenclature

| ID | Function name | Equation                                                  |
|----|---------------|-----------------------------------------------------------|
| 0  | Linear        | f(x) = x                                                  |
| 1  | Sigmoid       | f(x) = 1 / (1 - exp(-x))                                  |
| 2  | Relu          | f(x) = max(x, 0)                                          |
| 3  | Softmax       | f(x) = exp(x) / sum(exp(x)) *where x and f(x) are vectors |