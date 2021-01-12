if [ $# -eq 3 ]
then
    echo "simulating" $2 "events with energy" $1 " and tau " $tau
#    sed -e 's/EEEE/'$1'/g' -e 's/NNNN/'$2'/g' -e 's/TTTT/'$3'/g' < input/p3pbase.mac > input/p3p_$1keV_$2evt_$3ps.mac
#    sed -e 's/165./'$3'./g' < input/ni78_$1keV.lvldata > input/ni78_$1keV_$3ps.lvldata
#    UCHiCARI input/p3p_$1keV_$2evt_$3ps.mac
    SimCalculate -i data/p3p_$1keV_$2evt_$3ps.dat -o root/p3p_$1keV_$2evt_$3ps.root -s input/p3pana.set 
    Sim_histos -i root/p3p_$1keV_$2evt_$3ps.root -o hist/p3p_$1keV_$2evt_$3ps.root
else
    echo "usage ./sim.sh"
fi
