//biblioteca de classes utilizadas nos programas k-difference prime

#ifndef _classes_H_
#define _classes_H_

#include <string>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <list>
#include <cstdlib>
#include <iostream>
#include <new>
#include <sys/resource.h>

//constantes definidas para uso de mensagens na tela
#define ERR_ARGS "\nErro: Argumentos errados!"
#define ERR_KMAIOR "Erro: O parametro k deve estar em um intervalo entre 1 e "
#define ERR_SAVEFILE "Erro: N�o foi possivel criar o arquivo de saida!"
#define ERR_TSAIDA "Erro: Tipo de saida incorreto! (1=simples, 2=completo, 3=XML e 4=JSON)"

#define MSG_0_OCCR "\nNao foi encontrado nenhuma ocorrencia com pelo menos "
#define MSG_N_OCCR "\nOcr "
#define MSG_MATRIZ "Mostrando a matriz de programacao dinamica das 10 primerias ocorrencias de primer"
#define USO "\nUso correto:\n -a -alpha -pattern -padrao -p -P \t(String)\n -b -beta -text -texto -t -T \t\t(String)\n -k -K \t\t\t\t\t(int) maior que 0 e menor que o tamanho de alfa\n [-sm] \t\tmostrar a matriz de programacao dinamica \t(Opcional, default=nao mostrar)\n [-vs%] \tversao do algoritmo (1,2 ou 3) \t\t(Opcional, default=1)\n [-st] \t\tmostrar o tempo de execucao e consumo de memoria \t(Opcional, default=nao mostrar)\n [-ts%] \tforma de saida do resultado (1=simples, 2=completo, 3=XML, 4=JSON) \t(Opcional, default=1)\n [-sf] \t\tlocal e nome do arquivo gerado como saida \t\t(Opcional, default=aM_bN_k_nomearquivo)\n [-log] \tmostrar log do status atual de processamento \n\n"

#define MSG_VERSAO_INCORRETA "\nVersao incorreta de implementacao do algoritmo\n"
#define MSG_VERSAO_K1_VS1 "\t-vs1 (default) versao que utiliza apenas uma linha para computar a matriz D.\n"
#define MSG_VERSAO_K1_VS2 "\t-vs2           versao que utiliza duas linhas para computar a matriz D.\n"
#define MSG_VERSAO_K1_VS3 "\t-vs3           versao original do algoritmo com m linhas para computar a matriz D.\n"
#define MSG_VERSAO_K2_VS1 "\t-vs1 (default) versao que utiliza apenas uma coluna para computar a matriz Lde.\n"
#define MSG_VERSAO_K2_VS2 "\t-vs2           versao original do algoritmo com e colunas para computar a matriz Lde.\n"
#define MSG_VERSAO_K2_VS3 "\t-vs3           versao que utiliza funcao directcomp para computar LCE em tempo O(n).\n"
#define MSG_VERSAO_K3_VS1 "\t-vs1 (default) versao que utiliza arvore de sufixo compressada e consulta LCE em tempo O(k).\n"
#define MSG_VERSAO_K4_VS1 "\t-vs1 (default) versao que utiliza array de sufixo + lcp + DirectMin.\n"
#define MSG_VERSAO_K4_VS2 "\t-vs2           versao que utiliza array de sufixo + lcp + consulta RMQ ao LCE em tempo O(1).\n"


/* FOREGROUND */
#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define FRED(x) KRED x RST
#define FGRN(x) KGRN x RST
#define FYEL(x) KYEL x RST
#define FBLU(x) KBLU x RST
#define FMAG(x) KMAG x RST
#define FCYN(x) KCYN x RST
#define FWHT(x) KWHT x RST

#define BOLD(x) "\x1B[1m" x RST
#define UNDL(x) "\x1B[4m" x RST

using namespace std;

class KdifferenceInexactMatch;
class KdifferencePrime;

struct JRprime{
   int j;
   int Jmax;
   int r;
};

string lerArquivo(const char *local);


long long int menorDeDois(long long int a,
                          long long int b){
   return a < b ? a : b;
}
static long long int maiorDeDois(long long int a,
                          long long int b){
   return a > b ? a : b;
}

//fun��o auxiliar compara tr�s inteiros e devolve o menor
static long long int menorDeTres(unsigned long long int x,
                                   unsigned long long int b,
                                   unsigned long long int c){

  return menorDeDois(menorDeDois(x,b),c);
}

//n�o pode ser unsigned pois compara valores -1
//retorna o maior entre tr�s valores
static long long int maiorDeTres(long long int a,
                          long long int b,
                          long long int c){

  return maiorDeDois(maiorDeDois(a,b), c);
}

class Parametro{

public:
   string alpha, beta;
   int k;
   bool mostrarMatriz;
   bool mostrarTempo;
   int versao;
   bool escolheuVersao;
   bool argumentoErrado;
   string saida;
   int tipoSaida;
   bool mostrarLog;

   int total;

   Parametro(){
     k = 0;
     mostrarMatriz = false;
     mostrarTempo = false;
     versao = 0;
     total = 0;
     escolheuVersao = false;
     argumentoErrado = false;
     tipoSaida = 1; //sa�da simples (default)
     mostrarLog=false;
   };
};

class Primer{

public:
   int ocr;
   int j;
   int r;
   string sequencia;
//   cout matriz;

public:
   Primer(int ocr, int j, int r, string sequencia){
      this->ocr = ocr;
      this->j = j;
      this->r = r;
      this->sequencia = sequencia;
   };
   void escreverTela() const;
   string escreverArquivoCompleto();
   string escreverArquivoReduzido();
   string escreverArquivoJSON();
   string escreverArquivoXML();
};

bool comparar(const Primer* a, const Primer* b){
   return a->j < b->j;
}

class KdifferenceInexactMatch{
  protected:
    char *a;  //padr�o
    char *t;  //texto
    int m;  //tamanho de a
    int n;  //tamanho de t
    int k;  //quantidade de difer�n�as

    JRprime *jr;  //retorno do novo algoritmo

  public:
    int primerM; //tamanho original de alfa, extremamente necess�rio p/ computar corretamente o LCE
    int primerJ; //�ndice atual j de alfa
//  private:
  //  int rowPrint;//linha m�xima de impressao da matriz
public:
    KdifferenceInexactMatch(char *a, char *t, int *k){
       this->a = a;
       this->t = t;
       this->k = *k;

       this->m = strlen(a);
       this->n = strlen(t);

       primerJ = 0;
       primerM = m;
    }
    ~KdifferenceInexactMatch(){};
    virtual string name() const {return "K";};
    virtual int executar(int m){return -1;};//executa o algoritmo kdifference Inexact Match e retorna a linha da ocorr�ncia de prime
    virtual int executar(int m, int j){return -1;};
    virtual void imprimirMatrizTela(){};//escrever a matriz na tela
    virtual int getRowPrint(){
      return m;
    }
};

class KdifferenceInexactMatch234: public KdifferenceInexactMatch{

  private:
    int *L;   //Matriz L[-(k+1)..d]

  private:
      int dToMatriz(int d){
      return d + k + 1; //converte �ndice d para �ndices reais
    }

  public:
    void setL(int d, int valor){
      L[dToMatriz(d)] = valor; //L[d,e] = valor
    }

    int getL(int d){
      return L[dToMatriz(d)]; // L[d,e]
    }

    int getDimensaoD(){
      return k + n + 3; //k, n+1, 1 da posi��o 0
    }

  public:
    KdifferenceInexactMatch234(char *a, char *t, int *k): KdifferenceInexactMatch(a, t, k){
       //aloca��o din�mica de inteiros
       try{
         this->L = new int[getDimensaoD()];

       }catch(bad_alloc& ba){
          cout<<"Erro ao alocar matriz L: " << ba.what()<<endl;
       }
    }

    ~KdifferenceInexactMatch234(){
       //Ao destruir a classe desaloca toda mem�ria que foi usada
       delete [] L;
    }

    int executar(int m){
       this->m = m;
       int d,e, row;

       //inicializa��o da matriz
       for(d = -(k+1); d <= (n+1); d++)
           setL(d, -1);

       int long long pivo; //vari�vel auxiliar para troca de posi��es
       bool passou = true; //flag para controlar o caso de alcan�ar o fim de m antes de k diferen�as
       int linha = -1;     //vari�vel que guarda sempre o maior valor da coluna e
       for(e = 0; e < k && passou; e++){
           pivo = linha = -1;  //a cada nova coluna a vari�vel linha � reiniciada
           for(d = -e; d <= n && passou; d++){
              row = maiorDeTres(getL(d-1), getL(d) + 1, getL(d+1) + 1);
              row = menorDeDois(row, m);
              if(row + d < n)
                row += LCE(primerJ + row, primerM + 1 + row + d); //LCE
              //se j� alcancou 'm' e o erro � menor que 'k' pode parar e ir para o pr�ximo 'j'
              if(row == m){passou = false; continue;}
              setL(d-1, pivo); //atualiza a coluna e guardando o pivo no espa�o anterior
              if (row > linha) linha = row;
              pivo = row;
           }
           setL(n, row);
       }
       return (passou ? ++linha : -1); //retorna a linha de ocorr�ncia de primer ou -1 que indica n�o ocorr�ncia
   }
   void imprimirMatrizTela();

   virtual long int LCE(int x, int y){
      return 0;
   }
};

class KdifferencePrime{

    public:
      char *alpha, *beta;//seguindo o modelo encontrado no livro do GUSFIELD, nomes de vari�veis que representam o padr�o e o texto respectivamente
      int k;             //quantidade de difer�n�as
      int m;             //tamanho de alpha
      int n;             //tamanho de beta
      bool mostrarMatriz; //booleano 0,1 usado para imprime a matriz na tela
      int versao;     //booleano 0, 1 usado para executar a vers�o otimizada do algoritmo
      int j;             //�ndice de alpha necess�rio p/ posicionar corretamente na �rvore de sufixo
      string ocorrencia;
      bool tempo; //booleano para mostrar o tempo de execu��o na tela se o usu�rio setar o par�metro

      KdifferenceInexactMatch *c;

      list<Primer *> primers; //lista que guarda as ocorr�ncias de primers
      JRprime *jr;
      int *ocr; //vetor que guarda as ocorr�ncias

    public:
      KdifferencePrime(){
         //construtor seta os valores default se o usu�rio n�o escolher nada
         this->mostrarMatriz=false;
         this->versao=1; //apenas uma vers�o
         primers.clear();

      }
      ~KdifferencePrime(){};
      virtual void instanciar(){};
      virtual void processar();
      void mostrarOcorrencias(Parametro *p);
      void setaParametros(Parametro *p);
      void gerarOcorrencias();
    private:
      void mostrar(int j, int r){
        cout<< setfill(' ');
        cout<<"\nj:"<<setw(2)<<j;
        string o = ocorrencia.substr(j, r);
        cout<<", tam.: "<<setw(2)<<o.size()<<KBLU<<"-> ";
        cout<<KYEL<<o<<RST;
      };
};

//fun��o auxiliar recebe os par�metros que o usu�rio digitou, valida e transforma os valores
Parametro *parseParametros(int argc, char** argv){
   string argA[6] = {"-a", "-alpha", "-pattern", "-padrao", "-p", "-P"};
   string argB[6] = {"-b", "-beta", "-text", "-texto", "-t", "-T"};
   string argK[2] = {"-k", "-K"};

   //string aceitos[9] = {"-sm", "-st", "-ts", "-sf","-vs1", "-vs2", "-vs3", "-h", "-help"};

   Parametro *p = new Parametro();

   int temA, temB, temK;
   temA = temB = temK = 0;

   int z = 1;
   while( z < argc ){
      //cout<<argv[z]<<" ";
      if(strcmp(argv[z], "-sm") == 0){
        p->mostrarMatriz = true;
        z++;
        continue;
      }

      if(strcmp(argv[z], "-log") == 0){
        p->mostrarLog = true;
        z++;
        continue;
      }

      if(strcmp(argv[z], "-ts") == 0){
        try{
          if(z + 1 < argc) p->tipoSaida = atoi(argv[z + 1]);
          //1 = simples, 2 = completo, 3 = XML e 4 = JSON
        }catch(int e){
           p->tipoSaida = 1;
        }
        z+=2;
        continue;
      }

      if(strcmp(argv[z], "-sf") == 0){
         if(z + 1 < argc) p->saida = argv[z + 1];
         z+=2;
        continue;
      }

      if(strcmp(argv[z], "-vs1") == 0){
        if(p->escolheuVersao) p->versao=0; else p->versao=1;
        p->escolheuVersao = true;
        z++;
        continue;
      }
      if(strcmp(argv[z], "-vs2") == 0){
        if(p->escolheuVersao) p->versao=0; else p->versao=2;
        p->escolheuVersao = true;
        z++;
        continue;
      }

      if(strcmp(argv[z], "-vs3") == 0){
        if(p->escolheuVersao) p->versao=0; else p->versao=3;
        p->escolheuVersao = true;
        z++;
        continue;
      }

      if(strcmp(argv[z], "-st") == 0){
        p->mostrarTempo=true;
        z++;
        continue;
      }

      bool achou = false;
      for(int w = 0; w < 6 && !achou; w++){
        if(argA[w].compare(argv[z]) == 0){
            if(z + 1 < argc) p->alpha = argv[z + 1];
            temA++;
            achou = true;
        }
        else if(argB[w].compare(argv[z]) == 0){
            if(z + 1 < argc) p->beta = argv[z + 1];
            temB++;
            achou = true;
        }
        else if(w < 2 && argK[w].compare(argv[z]) == 0){
            achou = true;
            try{
              if(z + 1 < argc) p->k = atoi(argv[z + 1]);
              if(p->k > 0) temK++;
            }catch(int e){
              p->k = 0;
            }
        }
      }
      if(achou){
        z+=2;
        continue;
      };

      //verificamos se h� outro par�metro errado vamos verificar se algum par�metro passado est� fora da lista
      //se chegou at� aqui
      p->argumentoErrado=true;
      cout<<FRED(ERR_ARGS)<<" "<<argv[z];
      cout<<USO;
      z++;
      break;
   }
   p->total = temA + temB + temK;

   p->alpha = lerArquivo(p->alpha.c_str());
   p->beta  = lerArquivo(p->beta.c_str());
   if(!p->escolheuVersao) p->versao = 1; //se n�o escolheu vers�o ent�o seta default como 1

   return p;
}

//devemos abrir o arquivo em tempo de execu��o, ou seja, processar enquanto abre o arquivo,
 string lerArquivo(const char *local){

  ifstream arquivo(local, std::ifstream::in);

  if(!arquivo.good()) {
    arquivo.close();

    return local;
  }
  cout<<"Abrindo arquivo "<<local<<"...\n";

  std::ostringstream contents;
  contents << arquivo.rdbuf();

  arquivo.close();

  return(contents.str());
}

void formataTempo(time_t hora, bool inicio){
   struct tm *timeinfo;

   timeinfo = localtime(&hora);
   char buffer[80];
   if(inicio) strftime(buffer, 80, "Inicio: %d/%m/%y %X", timeinfo);
   else       strftime(buffer, 80, "Fim   : %d/%m/%y %X", timeinfo);
   cout<<buffer<<endl;
}

void formataSegundos(double segundos){
  long int valor = (int) segundos;
  int sec = 0, minu = 0, dia = 0, hor = 0;
  double _tempo = valor;

  if(valor > 60){//minutos
   sec = valor % 60;
   valor = (valor - sec)/60;
  } else{
    sec = valor;
    valor = 0;
  }

  if(valor > 60){//horas
   minu = valor % 60;
   valor = (valor - minu)/60;
  } else{
    minu = valor;
    valor = 0;
  }

  if(valor > 24){//dias
    hor = valor % 24;
    valor = (valor - hor)/24;
  } else{
    hor = valor;
    valor = 0;
  }
  if(valor > 0) dia = valor;

  string tempo = "";
  if(dia > 0)  tempo += to_string(dia)  + " dia(s) ";
  if(hor > 0)  tempo += to_string(hor) + " hora(s) ";
  if(minu > 0) tempo += to_string(minu) + " minuto(s) ";
  if(sec > 0)  tempo += to_string(sec)  + " segundo(s) ";

  if(minu > 0){
     tempo += "(" + to_string(segundos) + " segundos )";
  }

  cout<<"Tempo de execucao: "<< tempo  << endl;
}

 /*
  * fun��o criado usando uma estrutura pr�-definida de dados
  * que retorna o total de mem�ria utilizado no processo. Baseado em:
  * http://man7.org/linux/man-pages/man2/getrusage.2.html
  * http://linux.die.net/man/2/getrusage
  */
void mostrarMemoria(){

    struct rusage r_usage;
    getrusage(RUSAGE_SELF,&r_usage);
    printf("Uso de memoria   : %ld\n",r_usage.ru_maxrss);
}


/*
 * m�todo extremamente importante quando utilizado o algoritmo para diminuir o j
 * deve ser utilizado antes de mostrar as ocorr�ncias
 */
void KdifferencePrime::gerarOcorrencias(){
  // modificado temporariamente apenas para
  int r;
  for(int v = 0; v < m-k+1; v++){
     //printf("j: %d Jmax:%d r: %d\n", jr[v].j, jr[v].Jmax, jr[v].r);
     ocr[v] = -1;
     r = jr[v].r;
     if(r != -1){             // se h� uma ocorr�ncia de primer

        for(int w = jr[v].j;        //  deve ir da primeira posi��o
                w < jr[v].j + jr[v].Jmax; //  at� Jmax com a mesma ocorr�ncia r de primer
                /*&& (jr[v].r + w) <= m */ //enquanto n�o extrapolar o tamanho de alfa
                w++){
           //printf("==> %d, %d\n", v, w);
          ocr[w] = r--;
        }

        v += jr[v].Jmax-1;

      } else ocr[v] = -1;
  }
}

void KdifferencePrime::mostrarOcorrencias(Parametro *par){

  int nOcr = 0; int r;
  for(int v = 0; v < m-k+1; v++){
     r = ocr[v];
     if(r != -1){
         Primer *pr = new Primer(++nOcr, v, r, ocorrencia.substr(v, r));
         primers.insert(primers.end(), pr);
      }
  }

  if(primers.size() == 0){
     cout<<KRED<<MSG_0_OCCR<<RST<<k<<KRED<<" diferenca(s)"<<RST<<"\n";
     cout<<"alpha: "<<KCYN<<to_string(m)<<RST
         <<", beta: "<<KMAG<<to_string(n)<<RST
         <<",k: "<<to_string(k)
         <<" ("<<c->name()<<")\n";
  }else{
     cout<<KYEL<<"Encontrado "<<RST<<primers.size()<<KYEL<<" ocorrencia(s) de primers!"<<RST;

     if(primers.size() > 10){
       cout<<" Ocorrencias gravadas em ";
       string fileName;
       if(par->saida.empty())
          fileName = "saida/a" + to_string(m) + "_b" + to_string(n) + "_k" + to_string(k) + "_" + c->name();
       else
          fileName = par->saida;

       try{
         fstream out;

         out.open(fileName, ios::out | ios::trunc);
         if(par->tipoSaida == 1)       out<<"j|tamanho|sequencia\n";
         else if(par->tipoSaida == 2)  out<<"ocorrencia|j|tamanho|sequencia|j+r\n";

         for(Primer *p : primers){
             if(par->tipoSaida == 1)      out<<p->escreverArquivoReduzido();
             else if(par->tipoSaida == 2) out<<p->escreverArquivoCompleto();
             else if(par->tipoSaida == 3) out<<p->escreverArquivoXML();
             else if(par->tipoSaida == 4) out<<p->escreverArquivoJSON();
         }
         out.close();

       } catch(std::ifstream::failure e){
         cout<<FRED(ERR_SAVEFILE);
       }
       cout<<fileName<<endl;
     }else{
         primers.sort(comparar);
         if(par->tipoSaida == 1)       cout<<"\nj|tamanho|sequencia\n";
         else if(par->tipoSaida == 2)  cout<<"\nocorrencia|j|tamanho|sequencia|j+r\n";
         for(Primer *p : primers){
             if(par->tipoSaida == 1)      cout<<p->escreverArquivoReduzido();
             else if(par->tipoSaida == 2) cout<<p->escreverArquivoCompleto();
             else if(par->tipoSaida == 3) cout<<p->escreverArquivoXML();
             else if(par->tipoSaida == 4) cout<<p->escreverArquivoJSON();
         }
         cout<<endl;
     }
  }
}

void KdifferencePrime::setaParametros(Parametro *p){

    try{
      alpha = new char [p->alpha.length() + 1];
      strcpy(alpha, p->alpha.c_str());
      beta = new char [p->beta.length() + 1];
      strcpy(beta, p->beta.c_str());
    } catch(bad_alloc& ba){
      cout<<"Erro ao alocar mem�ria de alpha e beta: " << ba.what()<<endl;
    }
    k = p->k;
    mostrarMatriz = p->mostrarMatriz;
    versao = p->versao;

    m = p->alpha.size();
    n = p->beta.size();
    ocorrencia = p->alpha;
    try{
      int maxOcr = abs(m-k+1); //calcula o total de ocorr�ncias de primer poss�veis
      ocr = new int[maxOcr];   //aloca o m�ximo espa�o para todas as poss�veis ocorr�ncias de primer
      memset(ocr, -1, maxOcr); //seta todas as posi��es com valores -1 (n�o ocorr�ncia)
    } catch(bad_alloc& ba){
      cout<<"Erro ao alocar mem�ria de ocorr�ncias: " << ba.what()<<endl;
    }
    try{
       jr = new JRprime[m];
       for(j = 0; j < m; j++){
          jr[j].j = j;
          jr[j].Jmax = 1;
          jr[j].r = -1;
       }
    } catch(bad_alloc& ba){
      cout<<"Erro ao alocar mem�ria JRprime: " << ba.what()<<endl;
    }
    tempo = p->mostrarTempo;
    p->alpha.clear();
    p->beta.clear();
}

//m�todo que processa o algoritmo principal chamado a partir do procedimento MAIN
//IMPORTANTE: n�o h� execu��o sem a invoca��o deste m�todo
void KdifferencePrime::processar(){
   instanciar(); //essa chamada depende diretamente da implementa��o do kdifferenceInexactMatch que ser� utilizado
   //O(m), onde m � o tamanho de alpha
   int print = 0;
   c->primerM = m;
   for(j = 0; j < (m - k) + 1; j++){
       //guardo em um array todos as ocorr�ncias r de primer para cada j
       c->primerJ = j;
       ocr[j] = c->executar(m - j);
       if(print < 10 && mostrarMatriz && ocr[j] != -1){
         mostrar(j, ocr[j]);
         c->imprimirMatrizTela();
         print++; //contador de matriz impressas na tela, apenas as primeiras 10 devem ser mostradas
       }
   }
}

void Primer::escreverTela() const{
  cout<< setfill(' ');
  cout<<"\nj:"<<setw(2)<<j;
  cout<<", tam.: "<<setw(2)<<sequencia.size()<<KBLU<<" -> ";
  cout<<KYEL<<sequencia<<RST;
}

string Primer::escreverArquivoCompleto(){
    //ocorrencia;j;tamanho;sequencia;j+tamanho
    string *retorno = new string(to_string(ocr));
    retorno->append(";");
    retorno->append(to_string(j));
    retorno->append(";");
    retorno->append(to_string(r));
    retorno->append(";");
    retorno->append(sequencia);
    retorno->append(";");
    retorno->append(to_string(j+r));
    retorno->append("\n");
    return *retorno;
}

string Primer::escreverArquivoReduzido(){
    //j;tamanho;sequencia
    string *retorno = new string(to_string(j));
    retorno->append(";");
    retorno->append(to_string(r));
    retorno->append(";");
    retorno->append(sequencia);
    retorno->append("\n");
    return *retorno;
}

string Primer::escreverArquivoXML(){
    string *retorno = new string(to_string(j));
    return *retorno;
}

string Primer::escreverArquivoJSON(){
    string *retorno = new string(to_string(j));
    return *retorno;
}

//m�todo que imprimi a matriz na tela
void KdifferenceInexactMatch234::imprimirMatrizTela(){
   cout << setfill(' ');     //setar o tamanho de espa�amento de elementos na tela
   int vr;
   cout<<KGRN<<"\n  d:";

   for(int d = -(k+1); d <= (n+1); d++){
    cout<<setw(2)<<d<<" ";
   }
   cout<<KMAG<<"\nk-1:"<<RST;
   for(int d = -(k+1); d <= (n+1); d++){
     vr = getL(d);
     cout<<setw(2)<<vr<<" ";
   }
   cout<<endl;
}



/*
   constru�do o main apenas para testar as fun��es
   int main(){
     formataSegundos(59590);
     return 0;
   }
*/

#endif
