import sys
import numpy as np
import statistics as st
from scipy import stats

def getInfoSample (filename):
    data = np.genfromtxt(open(filename,"r"))
    mean = st.mean(data)
    std = st.stdev(data)
    return data, mean, std

def main ():
    
    if (len(sys.argv) != 3):
        print("=====================================================")
        print("Usage:> python %s <sample_1> <sample_2>" % sys.argv[0])
        print("=====================================================")
        sys.exit()

    g1, g1_mean, g1_std = getInfoSample(sys.argv[1])
    g2, g2_mean, g2_std = getInfoSample(sys.argv[2])
    ttest = stats.ttest_ind(g1,g2)
    
    print("Mean Sample 1 = %f" % g1_mean)
    print("Standard Deviation Sample 1 = %f" % g1_std)
    print("\nMean Sample 2 = %f" % g2_mean)
    print("Standard Deviation Sample 2 = %f" % g2_std)
    print("\nTvalue = %f" % ttest[0])

if __name__ == "__main__":
    main()

# O T-Test eh um teste estatistico utilizado para medir o nivel de significancia entre uma ou duas amostras.
# Muito utilizado no ramo da biologia, aonde se esta interessado em medir o qual efetivo uma droga foi em um
# tratamento contra alguma doenca. 
#
# A comparacao eh feita utilizando a media e o desvio padrao das duas amostras e verificando se existe alguma
# diference significativa entre ambas. Isto eh feito mediante ao uso de uma tabela.
# 
# Calculado o valor do Tvalue, checa-se através de uma triangularizacao entre o numero de graus de liberdade,
# a porcentagem de significancia e o valor do pvalue na tabela de referencia do T-Test.
# 
# Se o Tvalue for menor ou igual que o pvalue de referencia, significa que nao existiu diferenca significativa
# entre as duas amostras.

# Exemplo:
# 
# Supondo que o estamos utilizando os valores de referencia para o T-Test como:
#
# DF = 18 (graus de liberdade), p <= 0.05, pvalue = 2.101
#
# Apos realizar o T-Test entre duas amostras obtemos o resultado do Tvalue igual a 3.987.
# Isto significa que existe uma diferenca significativa entre as duas amostras, de tal forma que aceitamos 
# a hipotese alternativa e dizemos que a probabilidade de obtermos os mesmos resultados em um outro
# experimento eh de 95 %.  

# Fonte: http://www.indiana.edu/~educy520/sec6342/week_10/ttest_exp.pdf
