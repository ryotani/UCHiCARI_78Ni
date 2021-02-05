#!/bin/bash

runsim() {
    ./simp2p.sh 5293 765 0 > log/ni_5293_0ps.log &
    ./simp2p.sh 3183 364 0 > log/ni_3183_0ps.log &
    ./simp2p.sh 5243 655 0 > log/ni_5243_0ps.log &
    ./simp2p.sh 3703 473 0 > log/ni_3703_0ps.log &
    #		     0		    _0ps
    ./simp3p.sh 5293 242 0 > log/ni_5293_0psp3p.log &
    ./simp3p.sh 3977 307 0 > log/ni_3977_0ps.log &
    ./simp3p.sh 2910 557 0 > log/ni_2910_0ps.log 
}


for i in {1..20}
do
    echo "Sim of $i times"
    time runsim
    sleep 1
    echo "Analysis"
    time root -b -q plot_ATC_study.C 2> /dev/null 1> /dev/null
    echo "Done"
done

cat fig/Nsigma-Summary.csv 
