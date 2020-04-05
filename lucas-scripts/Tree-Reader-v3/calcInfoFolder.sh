# =================================================================
# Calcula a informacao de todas as arvores em um diretorio
# =================================================================
#!/bin/bash

make clean; make;
if [ $# -ne 1 ]; then
    echo "=============================================="
    echo "Usage:> ./calcInfoFolder.sh <folder_path>"
    echo "=============================================="
    exit 1
fi

# Ler todas as 10 arvores do diretorio passado na entrada
FOLDER=$1
for i in $(seq 0 9); do
    echo "[!] Tree number $i"
    echo $FOLDER/$i/Tree_Nterm.vtk
    ./treeReader $FOLDER/$i/Tree_Nterm.vtk
    echo 
done

# Executar o codigo que calcula a media e o desvio padrao
# Considerando todas as arvores
if [ ! -f MeanStd ]; then
    g++ -o MeanStd MeanStd.cpp
fi
./MeanStd Output/edges.txt Output/angles.txt Output/segments.txt





