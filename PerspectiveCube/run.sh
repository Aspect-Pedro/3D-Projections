#!/bin/bash

g++ src/game.cpp -o game -Iraylib/include -Lraylib/lib -lraylib -lGL -lm -lpthread -ldl -lrt

./game
