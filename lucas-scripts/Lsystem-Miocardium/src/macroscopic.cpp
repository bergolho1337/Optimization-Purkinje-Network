#include "../include/macroscopic.h"

// Cria os diretorios necessarios
void createDirectories ()
{
    printf("[!] Creating directory ... ");
    fflush(stdout);
    char cmd[50];
    sprintf(cmd,"mkdir Run");
    if (system(cmd)) error("In createDirectories()!"); 
    sprintf(cmd,"mkdir Run/VTK");
    if (system(cmd)) error("In createDirectories()!");
    sprintf(cmd,"mkdir Run/Graphics");
    if (system(cmd)) error("In createDirectories()!");
    printf("ok\n");
}

// Construtor do modelo
MacroscopicModel* newMacroscopicModel (int argc, char *argv[])
{
    printf("[!] Building macrocopic model ... \n");
    createDirectories();
    MacroscopicModel *model = (MacroscopicModel*)malloc(sizeof(MacroscopicModel));
    // Capturar argumentos de entrada
    model->t_max = atof(argv[1]);
    model->dx = atof(argv[2]);
    model->dt = atof(argv[3]);
    strcpy(model->cloudPointFile,argv[4]);
    strcpy(model->terminalPointFile,argv[5]);
    
    //model->y0 = getInitialConditions();
    //model->functions = getFunctions();

    // Criar grafo
    model->g = newGraph(model->dx,model->cloudPointFile,model->terminalPointFile);
    countTerminals(model->g->terminalPoint,model->g->numTerminal);
    writeGraphVTK(model->g);
    writeTerminalsVTK(model->g);
    //printGraph(model->g);

    /*
    setInitialConditions(this->g,this->y0,num_equations);
    // Setar os volumes do plot
    this->plot = setPlotPointers(this->g);
    printGraph(this->g,this->exec_number);
    writeVTK(this->g,0,this->exec_number);
    // OK

    printInfoModel(this->g,this->y0,this->t_max,this->dt,this->dx,exec_number);
    */
    printf("[+] Done\n");
    return model;
}

// Copia os arquivos gerados pelo L-System para o diretorio do FMM da Jesuliana
void copyFilesToFMMDirectory ()
{
  if (!system("cp Run/graph.vtk ../geodesicsonmeshes_purkinje/Tree_Nterm.vtk"))
    printf("[+] Output tree file copy with sucess to FMM folder!\n");
  else
    error("Moving output tree file to FMM folder!\n");

  if (!system("cp Run/terminals.txt ../geodesicsonmeshes_purkinje/dadosTerminais.txt"))
    printf("[+] Output terminal file copy with sucess to FMM folder!\n");
  else
    error("Moving output terminal file to FMM folder!\n");
}

/*
void MacroscopicModel::solve ()
{
  int N, M;
  double t;
  // Calcula o numero de subintervalos de discretizacao no espaco e no tempo
  M = nearbyint(this->t_max/this->dt);
  N = nearbyint(this->g->getXmax()/dx);
  cout << "[!] Solving monodomain equation ..." << endl;
  cout << "\tSubintervals in space: N = " << N << endl;
  cout << "\tSubintervals in time: M = " << M << endl;
  // Desvia para o metodo resolvedor
  switch (this->num_method)
  {
    case 1: cout << "\tUsing FTCS ..." << endl;
            break;
    case 2: cout << "\tUsing BTCS ..." << endl;
            // Constroi a matriz do implicito
            makeMatrix_A(this->g,beta,Cm,this->dt,this->dx);
            break;
  }
  // Loop no tempo
  for (int j = 0; j < M; j++)
  {
    //cout << "\nIteration " << j << endl;
    t = j*this->dt;
    writeGraphic(this->g,t,this->plot,num_equations,this->exec_number);
    if (M > 1000)
    {
      if (j % 10 == 0)
        writeVTK(this->g,j,this->exec_number);
    }
    else
      writeVTK(this->g,j,this->exec_number);
    switch (this->num_method)
    {
      // FTCS
      case 1: {
          // Para cada celula resolver a EDO primeiro, loop no espaco esta dentro desse procedimento
          solveODE(this->g,this->f,t,this->dt,num_equations);
          // Agora cada célula possui um V* e calculamos o V_n+1 de cada célula pela EDP
          solvePDE2(this->g,this->f,t,Cm,beta,this->dt,this->dx,num_equations);
          break;
        }
      // BTCS
      case 2: {
          // Resolve EDO primeiro
          solveODE_Imp(this->g,this->f,t,Cm,this->dt,num_equations);
          makeVector_b(this->g,beta,Cm,this->dt,this->dx);
          solvePDE_Imp(this->g);
          break;
        }
    }
    // Avanca no tempo
    nextTimestep(this->g,num_equations);
  }
  // Plota os graficos dos volumes
  makePlot(this->plot,this->exec_number);
  // *** Falta escrever o tempo e as informacoes sobre os parametros do modelo

  // Libera a memoria
  delete [] this->y0;
  delete [] this->f;
}
*/