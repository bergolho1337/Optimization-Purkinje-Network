# =================================================================
# Calcula a informacao de todas as arvores em um diretorio
# =================================================================
#!/bin/bash

make clean; make;
if [ $# -ne 2 ]; then
    echo "=========================================================================="
    echo "Usage:> ./calcInfoFolder.sh <analit_folder_path> <aprox_folder_path>"
    echo "Example:> ./calcInfoFolder.sh Benjamin CCO-T130-angulo63"
    echo "=========================================================================="
    exit 1
fi

# Ler todas as 10 arvores do diretorio passado na entrada
ANALIT_FOLDER=$1
APROX_FOLDER=$2
for i in $(seq 0 9); do
    echo "[!] Tree number $i"
    echo $APROX_FOLDER/$i/FMM_130.vtk
    ./errorCalculator $ANALIT_FOLDER/FMM_130.vtk $APROX_FOLDER/$i/FMM_130.vtk laranja melancia 
    echo 
done

# Executar o codigo que calcula a media e o desvio padrao
# Considerando todas as arvores
if [ ! -f MeanStd ]; then
    g++ -o MeanStd MeanStd.cpp
fi
./MeanStd Output/globalError.txt





