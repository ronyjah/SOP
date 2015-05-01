#!/bin/bash

make -C .. clean
make -C .. all

make -C .. TEST=Queue_Test test
make -C .. TEST=Task_Test test
make -C .. TEST=Scheduler_Test test
make -C .. TEST=Prio_Scheduler_Test test
make -C .. TEST=Prio_Scheduler_Join_Test test

./Queue_Test
./Task_Test
./Scheduler_Test
./Prio_Scheduler_Test
./Prio_Scheduler_Join_Test

