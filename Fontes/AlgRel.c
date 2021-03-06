/********************************** AlgRel.c **********************************/

#include <stdio.h>
#include <string.h>
#include "AlgRel.h"

void cpyarq(char *out, char *in){
	char lin[256], nome[20];
	FILE *i, *o;

/* Vari?veis: lin -> armazena cada linha lida do arquivo de origem.
			  nome -> armazena o nome dos arquivos a serem abertos.
			  i -> arquivo de entrada (tanto .dad quanto .ctl).
			  o -> arquivo de sa?da (tanto .dad quanto .ctl). */

	strcpy(nome, in);		/*Abertura do arquivo .dad da tabela de entrada.*/
	strcat(nome, ".dad");
	i = fopen(nome, "r");
	strcpy(nome, out);		/*Abertura do arquivo .dad da tabela de sa?da.*/
	strcat(nome, ".dad");
	o = fopen(nome, "w");
	fgets(lin, 255, i);		/*Cada linha ? lida da origem e copiada.*/
	while (!feof(i))
	{
		fprintf(o, "%s", lin);
		fgets(lin, 255, i);
	}
	fclose(i);				/*Arquivo .dad copiado.*/
	fclose(o);
	strcpy(nome, in);		/*Abertura do arquivo .ctl da tabela de entrada.*/
	strcat(nome, ".ctl");
	i = fopen(nome, "r");
	strcpy(nome, out);		/*Abertura do arquivo .ctl da tabela de sa?da.*/
	strcat(nome, ".ctl");
	o = fopen(nome, "w");
	fgets(lin, 255, i);		/*Cada linha ? lida da origem e copiada.*/
	while (!feof(i))
	{
		fprintf(o, "%s", lin);
		fgets(lin, 255, i);
	}
	fclose(i);				/*Arquivo .ctl copiado.*/
	fclose(o);
}

void desloca(FILE *arq, int n)
{
    char temp[200];

/* Vari?veis: temp -> Usada apenas para receber as linhas
			   lidas. */

    fseek(arq, 0, 0);
    for (; n > 0; n--)
        fgets(temp, 199, arq);
}

int isIn(int k, int col[])
{
    int i;

    i = 0;
    while (col[i] && (col[i] != k))
       i++;

    return col[i];
}

Cabecalho *decCatalogo (FILE *stream, int *n, int *m)
{
     char ch[2], strN[20], strM[20];
     int mode,i;
     Cabecalho *al;
     Atributo atributo;

/* Vari?veis: ch   -> armazena cada caractere lido do arquivo.
              strN -> armazena a string correspondente ao grau da rela??o.
              strM -> armazena a string correspondente ? cardinalidade
                      da rela??o.
              mode -> indica o que est? sendo lido do arquivo a cada instante.
              al   -> ponteiro p/ uma lista que armazena os atributos
                      da rela??o lidos do arquivo.
              atributo  -> armazena cada atributo lido do arquivo. */
    /*TEM q CONTAR PRA NAUM LER ATEH O EOF;PREENCHER AS F_REFERENCIADO*/
     mode = 1;
     strN[0] = '\0';
     strM[0] = '\0';
     ch[1] = '\0';
     ch[0] = getc(stream);
     while (ch[0] != '\n')        /*Enquanto n?o chegou ao fim da linha...*/
     {
           if (ch[0] == ',') mode++;         /*',' separa os valores de N e M.*/
           else if (mode == 1) strcat(strN, ch); /*mode == 1: l? o valor de N.*/
           else strcat(strM, ch);                /*mode == 2: l? o valor de M.*/
           ch[0] = getc(stream);
     }
     *n = atoi(strN);
     *m = atoi(strM);
     Atributo atributos[*n];
     initCabecalho(&al);
     ch[0] = getc(stream);      /*Inicia leitura dos aributos.*/
     int conta=0;
     while (ch[0] != EOF && conta<(*n))       /*Enquanto n?o chegou ao fim do arquivo...*/
     {
           conta++;
           mode = 1;
           atributo.nome[0] = '\0';
           atributo.tipo = 0;
           atributo.ord = 0;
           atributo.key = 0;
           atributo.conta_ref=0;
           atributo.fkey = 0;
           atributo.tipo_ref=RESTRICT;
           while (ch[0] != '\n')      /*Enquanto n?o chegou ao fim da linha...*/
           {
                 if (ch[0] == ',') mode++; /*',' separa nome, tipo e cl?usulas*/
                 else                      /*de cada atributo.*/
                     switch (mode)
                     {
                            case 1: strcat(atributo.nome, ch); break;
                            /*mode == 1: l? nome do atributo.*/
                            case 2: if (ch[0] == 'C') atributo.tipo = 1; break;
                            /*mode == 2: l? tipo do atributo.*/
                            case 3: if (ch[0] == 'o') atributo.ord = 1;
                                    if (ch[0] == 'c') atributo.key = 1;
                                    if (ch[0] == 'f'){
                                         atributo.fkey = 1;
                                         atributo.conta_ref=0;
                                         getc(stream);
                                         getc(stream);
                                         i=0;
                                         while((ch[0]=getc(stream))!=':')
                                              strM[i++]=ch[0];
                                         strM[i]='\0';
                                         strcpy(atributo.f_referencia,strM);
                                         ch[0]=getc(stream);
                                         getc(stream);getc(stream);getc(stream);
                                         if(ch[0]=='c')
                                              atributo.tipo_ref=CASCADE;
                                         else{
                                              atributo.tipo_ref=RESTRICT;
                                              getc(stream);
                                         }

                                    }else{
                                         getc(stream); getc(stream);
                                    } break;
                            /*mode == 3: cl?usula opcional [ord] ou [chv].*/
                            case 4: if (ch[0] == 'c') atributo.key = 1;
                                    if (ch[0] == 'f'){
                                         atributo.fkey = 1;
                                         atributo.conta_ref=0;
                                         getc(stream);
                                         getc(stream);
                                         i=0;
                                         while((ch[0]=getc(stream))!=':')
                                              strM[i++]=ch[0];
                                         strM[i]='\0';
                                         strcpy(atributo.f_referencia,strM);
                                         ch[0]=getc(stream);
                                         getc(stream);getc(stream);getc(stream);
                                         if(ch[0]=='c')
                                              atributo.tipo_ref=CASCADE;
                                         else{
                                              atributo.tipo_ref=RESTRICT;
                                              getc(stream);
                                         }

                                    }else{
                                         getc(stream); getc(stream);
                                    }
                            /*mode == 4: cl?usula opcional [chv].*/
                     }
                 ch[0] = getc(stream);
           }
           atributo.f_referenciada=NULL;
           atributo.conta_ref=0;
           atributos[conta-1]=atributo;
           ch[0] = getc(stream);
     }

     for(i=0;i<*n;i++){
          if(ch[0]!=EOF && atributos[i].key){
               Referenciada *f_referenciada =NULL;
               int x=0;
               int conta=0;
               f_referenciada = malloc(sizeof(Referenciada));
               f_referenciada->next = f_referenciada;
               f_referenciada->prev = f_referenciada;

               Referenciada *ref = f_referenciada,*nova;
               char tabela[21];
               while(ch[0]!='\n'){
                     if(ch[0]==','){
                           tabela[x]='\0';
                           if(conta==0){
                                 strcpy(ref->nome,tabela);
                           }else{
                                 nova = malloc(sizeof(Referenciada));
                                 strcpy(nova->nome,tabela);
                                 nova->next = f_referenciada;
                                 nova->prev = ref;
                                 ref->next = nova;
                                 ref = nova;
                            }
                            x=0;
                            conta++;
                     }else
                            tabela[x++]=ch[0];
                     ch[0]=getc(stream);
               }
               if(x>0){
                       tabela[x]='\0';
                       if(conta==0){
                             strcpy(ref->nome,tabela);
                       }else{
                             nova = malloc(sizeof(Referenciada));
                             strcpy(nova->nome,tabela);
                             nova->next = f_referenciada;
                             nova->prev = ref;
                             ref->next = nova;
                             ref = nova;
                       }
                       conta++;
               }
               atributos[i].f_referenciada = f_referenciada;
               atributos[i].conta_ref=conta;
          }
          insertCabecalho(&al, atributos[i]);
     }


     return al;
}

Relacao *decRelacao(FILE *stream)
{
     char ch[2], val[100];
     Tupla *vl;
     Relacao *tl;

/* Vari?veis: ch  -> armazena cada caractere lido do arquivo.
              val -> armazena cada valor lido do arquivo.
              vl  -> ponteiro p/ uma lista que armazena os valores
                     de cada tupla da rela??o lida do arquivo.
              tl  -> ponteiro p/ uma lista que armazena as tuplas
                     da rela??o lidas do arquivo. */

     val[0] = '\0';
     initRelacao(&tl);
     ch[1] = '\0';
     ch[0] = getc(stream);
     while (ch[0] != EOF)       /*Enquanto n?o chegou ao fim do arquivo...*/
     {
           initTupla(&vl);
           while (ch[0] != '\n')      /*Enquanto n?o chegou ao fim da linha...*/
           {
                 if (ch[0] == ',')     /*',' separa os valores de cada tupla.*/
                 {
                    insertTupla(&vl, val);     /*Insere cada valor lido em <vl>.*/
                    val[0] = '\0';
                 }
                 else strcat(val, ch); /*Armazena o caractere lido do arquivo.*/
                 ch[0] = getc(stream);
           }
           insertTupla(&vl, val);      /*Insere o ?ltimo valor da tupla em <vl>.*/
           val[0] = '\0';
           insertRelacao(&tl, vl);       /*Insere a tupla lida (<vl>) em <tl>.*/
           ch[0] = getc(stream);
     }
     return tl;
}

Tabela decTabela(FILE *ctl, FILE *dad)
{
     Tabela t;

     t.cabecalho = decCatalogo(ctl, &t.n, &t.m);
     if (t.m > 0) t.relacao = decRelacao(dad);
     else initRelacao(&t.relacao);

     t.chv = poschv(t.cabecalho);
     t.ord = posord(t.cabecalho);
     t.fchv = posfkey(t.cabecalho);

     return t;
}

int buscaSeq(FILE *arq, char *k, int pos, int ord, int tipo)
{
    char atrib[30], tupla[200];
    int i = 0;
    int ik = 0;
    int iatrib = 0;

/* Vari?veis: atrib -> armazena o atributo pesquisado de cada tupla.
			  tupla -> armazena cada tupla lida.
			  i -> contador (marca a posi??o no arquivo).
              ik -> representa??o inteira de k.
              iatrib -> representa??o inteira de atrib. */

    if (tipo == INTEGER)
        ik = atoi(k);

    fgets(tupla, 199, arq);
    tupla[strlen(tupla) - 1] = '\0';

    while (!feof(arq))	/*Compara o atributo em quest?o com o valor buscado*/
    {
        recuperaValor(tupla, pos, atrib);

        if (!strcmp(k, atrib)){
                        return(i);	/*At? que ele seja encontrado*/
                        }
        if (ord)
        {
           if (tipo == INTEGER)
           {
               iatrib = atoi(atrib);
               if (ik < iatrib)
                  return (-1);
           }
           else
               if (strcmp(k, atrib) < 0)
                  return (-1);
        }
        fgets(tupla, 199, arq);
        tupla[strlen(tupla) - 1] = '\0';
        i++;
    }										/*ou que o arquivo termine*/
	return(-1);		/*Se o arquivo terminou, o valor n?o existe*/
}

void selecao(char *a, char *z, char *atrib, int tipo, int pos, int chv,
                                                      int ord, int nTuplas)
{
	FILE *dadA, *dadZ;
	FILE *ctlA, *ctlZ;
	char temp[60], tupla[200], tatrib[30];
	int i, j, n = 0, tamA;

/* Vari?veis: dadA -> arquivo .dad da tabela de entrada.
			  dadZ -> arquivo .dad da tabela de sa?da.
			  ctlA -> arquivo .ctl da tabela de entrada.
			  ctlZ -> arquivo .ctl da tabela de sa?da.
			  temp -> armazena o nome dos arquivos que ser?o abertos.
			  tupla -> armazena uma tupla da rela??o.
			  tatrib -> armazena o valor do atributo da tupla atual.
			  i -> armazena a posi??o no arquivo de entrada.
			  j -> armazena o deslocamento no arquivo de entrada.
			  n -> armazena a posi??o no arquivo de sa?da. No final, ter? o
			  n?mero de tuplas na tabela resultante.
			  tamA -> n?mero de atributos da tabela de entrada. */

	if (strcmp(atrib, "*"))	/*Se atrib = "*", n?o existe condi??o.*/
	{
		strcpy(temp, a);	/*Abertura do arquivo .dad da tabela de entrada.*/
		strcat(temp, ".dad");
		dadA = fopen(temp, "r");
		strcpy(temp, z);	/*Abertura do arquivo .dad da tabela de sa?da.*/
		strcat(temp, ".dad");
		dadZ = fopen(temp, "a");
	    printf("%s\n",atrib);
        i = buscaSeq(dadA, atrib, pos, ord, tipo);
	    if (i != -1)
    	{
            desloca(dadA, i);
      	   	fgets(tupla, 199, dadA);
            tupla[strlen(tupla) - 1] = '\0';
	    	fprintf(dadZ, "%s\n", tupla);

    	    n++;
      	    if (!chv)
           	{
	            desloca(dadA, ++i);
    	        j = buscaSeq(dadA, atrib, pos, ord, tipo);
      	        while (j != -1)
      		    {
               		i += j;
                   	desloca(dadA, i);

		       		fgets(tupla, 199, dadA);
                    tupla[strlen(tupla) - 1] = '\0';
		        	fprintf(dadZ, "%s\n", tupla);
			        n++;
               	    desloca(dadA, ++i);
           		        	printf("%s | %s\n", tupla,atrib);
	       			j = buscaSeq(dadA, atrib, pos, ord, tipo);
		        }
    	   	}
        }

		fclose(dadA);	/*O arquivo de dados ? gerado.*/
		fclose(dadZ);
		strcpy(temp, a);
		strcat(temp, ".ctl");
		ctlA = fopen(temp, "r");
		strcpy(temp, z);
		strcat(temp, ".ctl");
		ctlZ = fopen(temp, "w");
		fscanf(ctlA, "\n%d,%d", &tamA, &i);
		fprintf(ctlZ, "%d,%d\n", tamA, n);
		fscanf(ctlA, "\n%s", temp);
		while (!feof(ctlA))	/*O arquivo .ctl ? id?ntico ao original.*/
		{
			fprintf(ctlZ, "%s\n", temp);
			fscanf(ctlA, "\n%s", temp);
		}
		fclose(ctlA);
		fclose(ctlZ);
	}
	else cpyarq(z, a);
}

void juncao(char *a, char *b, char *z, int tipo, int posA, int posB,
                 int chv, int ord, int nTuplas, char *nomeA)
{
	FILE *dadA, *dadB, *dadZ;
	FILE *ctlA, *ctlB, *ctlZ;
	Cabecalho *ca, *cb;
	char temp[200], atrib[30], valor[200], tatrib[30], *ptr;
	int i, j, n = 0, tamA, tamB, na, nb, ma, mb, v;

/* Vari?veis: dadA -> arquivo .dad da 1? tabela de entrada.
			  dadB -> arquivo .dad da 2? tabela de entrada.
			  dadZ -> arquivo .dad da tabela de sa?da.
			  ctlA -> arquivo .ctl da 1? tabela de entrada.
			  ctlB -> arquivo .ctl da 2? tabela de entrada.
			  ctlZ -> arquivo .ctl da tabela de sa?da.
			  temp -> armazena o nome dos arquivos que ser?o abertos. Tamb?m ?
			  usado para armazenar as tuplas lidas de a.
			  valor -> armazena uma tupla da rela??o b.
			  atrib -> armazena o valor do atributo da tupla atual de a.
			  tatrib -> armazena o valor do atributo da tupla atual de b.
			  ptr -> ponteiro usado na gera??o do novo arquivo .ctl.
			  i -> armazena a posi??o no arquivo de entrada.
			  j -> armazena o deslocamento no arquivo de entrada.
			  n -> armazena a posi??o no arquivo de sa?da. No final, ter? o
			  n?mero de tuplas na tabela resultante.
			  tamA -> n?mero de atributos da 1? tabela de entrada.
			  tamB -> n?mero de atributos da 2? tabela de entrada. */

    if (!nomeA[0])
       strcpy(nomeA, a);

    strcpy(temp, a);	/*Abertura do arquivo .dad da 1? tabela de entrada.*/
	strcat(temp, ".dad");
	dadA = fopen(temp, "r");
	strcpy(temp, b);	/*Abertura do arquivo .dad da 2? tabela de entrada.*/
	strcat(temp, ".dad");
	dadB = fopen(temp, "r");
	strcpy(temp, z);	/*Abertura do arquivo .dad da tabela de sa?da.*/
	strcat(temp, ".dad");
	dadZ = fopen(temp, "a");
	fgets(temp, 199, dadA);
    temp[strlen(temp) - 1] = '\0';
	while (!feof(dadA))		/*Cada tupla da 1? tabela de entrada ? lida.*/
	{
        desloca(dadB, 0);
		recuperaValor(temp, posA, atrib);

        i = buscaSeq(dadB, atrib, posB, ord, tipo);
		if (i == -1)	/*Se a busca falhar, a pr?xima tupla de a ? lida.*/
        {
              fgets(temp, 199, dadA);
              temp[strlen(temp) - 1] = '\0';
              continue;
        }
        desloca(dadB, i);		/*Caso contr?rio, procura-se por todas*/
		fgets(valor, 199, dadB);
        valor[strlen(valor) - 1] = '\0';/*as ocorr?ncias do valor.*/
		fprintf(dadZ, "%s\n", juntaTuplas(temp, valor, posB));
		n++;				/*Se o atributo ? chave, s? h? uma ocorr?ncia.*/
		if (chv)
        {
              fgets(temp, 199, dadA);
              temp[strlen(temp) - 1] = '\0';
              continue;
        }
        desloca(dadB, ++i);	/*Aqui, ? preciso fazer uma nova busca.*/
		j = buscaSeq(dadB, atrib, posB, ord, tipo);
		while (j != -1)
		{
           	i += j;
            desloca(dadB, i);
			fgets(valor, 199, dadB);
            valor[strlen(valor) - 1] = '\0';
		    fprintf(dadZ, "%s\n", juntaTuplas(temp, valor, posB));
			n++;
            desloca(dadB, ++i);
			j = buscaSeq(dadB, atrib, posB, ord, tipo);
		}

		fgets(temp, 199, dadA);
        temp[strlen(temp) - 1] = '\0';
	}
	fclose(dadA);	/*O arquivo de dados da tabela de sa?da est? pronto.*/
	fclose(dadB);
	fclose(dadZ);
	strcpy(temp, a);	/*Os arquivos .ctl s?o abertos.*/
	strcat(temp, ".ctl");
	ctlA = fopen(temp, "r");
	strcpy(temp, b);
	strcat(temp, ".ctl");
	ctlB = fopen(temp, "r");
	strcpy(temp, z);
	strcat(temp, ".ctl");
	ctlZ = fopen(temp, "a");

	ca = decCatalogo(ctlA, &na, &ma);
	cb = decCatalogo(ctlB, &nb, &mb);

	fseek(ctlA, 0, 0);
	fseek(ctlB, 0, 0);

	fscanf(ctlA, "\n%d,%d", &tamA, &i);	/*O n?mero de atributos da nova tabela*/
	fscanf(ctlB, "\n%d,%d", &tamB, &i);	/*ser? a soma dos atributos das*/
	fprintf(ctlZ, "%d,%d\n", tamA + tamB, n);/*tabelas a e b.*/
	fscanf(ctlA, "\n%s", temp);
	while (!feof(ctlA))	/*Ela ter? todos os atributos de a (sem chv).*/
	{
		if ((ptr = strstr(temp, "chv")) != NULL) *(--ptr) = 0;

		v = 0;
		do
        {
            ++v;
        } while (temp[v] != ',');
        strncpy(atrib, temp, v);
        atrib[v] = 0;
        if (posatr(atrib, cb))
           fprintf(ctlZ, "%s.", nomeA);
		fprintf(ctlZ, "%s\n", temp);
		fscanf(ctlA, "\n%s", temp);
	}
	for (n = 1; n <= tamB; n++)	/*E os atributos de b (sem chv, nem ord).*/
	{
		fscanf(ctlB, "\n%s", temp);
		if ((ptr = strstr(temp, "chv")) != NULL) *(--ptr) = 0;
		if ((ptr = strstr(temp, "ord")) != NULL) *(--ptr) = 0;

		v = 0;
		do
        {
            ++v;
        } while (temp[v] != ',');
        strncpy(atrib, temp, v);
        atrib[v] = 0;
        if (posatr(atrib, ca))
           fprintf(ctlZ, "%s.", b);

		fprintf(ctlZ, "%s\n", temp);
	}
	fclose(ctlA);	/*A tabela ? gerada com sucesso.*/
	fclose(ctlB);
	fclose(ctlZ);
}

void projecao(char *a, char *z, int cols[],int isDistinct, int isOrderBy)
{
	FILE *dadA, *dadZ,*readZ;
	FILE *ctlA, *ctlZ;
	char temp1[256], *temp2,temp3[256], tupla[256], tatrib[30], *ptr,tcmp[256],test;

	int c, i, j, n = 0, tamA,achou;
/* Vari?veis: dadA -> arquivo .dad da tabela de entrada.
			  dadZ -> arquivo .dad da tabela de sa?da.
			  ctlA -> arquivo .ctl da tabela de entrada.
			  ctlZ -> arquivo .ctl da tabela de sa?da.
			  temp -> armazena o nome dos arquivos que ser?o abertos.
			  tupla -> armazena uma tupla da rela??o.
			  tatrib -> armazena o valor do atributo da tupla atual.
			  i -> armazena a posi??o no arquivo de entrada.
			  j -> armazena o deslocamento no arquivo de entrada.
			  n -> armazena a posi??o no arquivo de sa?da. No final, ter? o
			  n?mero de tuplas na tabela resultante.
			  tamA -> n?mero de atributos da tabela de entrada. */

	strcpy(temp1, a);	/*Abertura do arquivo .dad da tabela de entrada.*/
	strcat(temp1, ".dad");
	dadA = fopen(temp1, "r");
	strcpy(temp1, z);	/*Abertura do arquivo .dad da tabela de sa?da.*/
	strcat(temp1, ".dad");
	strcpy(temp3,temp1);
	fgets(temp1, 199, dadA);

	while (!feof(dadA))
	{

		temp2 = temp1;
	    tupla[0] = 0;

		i = 1;
		c = 0;
    	ptr = strchr(temp2, ',');
    	while (ptr)
    	{
        	if (isIn(i, cols))
        	{
                if (tupla[0])
                   strcat(tupla, ",");
                strncat(tupla, temp2, ptr - temp2);
                c++;
            }
            temp2 = ++ptr;
           	ptr = strchr(temp2, ',');
            i++;
        }

        if (isIn(i, cols))
        {
           if (tupla[0])
              strcat(tupla, ",");
           strcat(tupla, temp2);
           c++;
        }
        else
           strcat(tupla, "\n");

		dadZ = fopen(temp3, "r");
        achou=0;
        if(dadZ!=NULL){
		if(isDistinct){
		while((test = fgetc(dadZ)) != EOF)
        {
           strcpy(tcmp,"");
           tcmp[0]=test;
           tcmp[1]='\0';
           fgets(temp1,255,dadZ);
           strcat(tcmp,temp1);

          if(strcmp(tcmp,tupla)==0)achou=1;
        }

        }
        fclose(dadZ);
        }

        dadZ = fopen(temp3, "a");
        if(achou==0){

        fprintf(dadZ, "%s", tupla);
        }
        fclose(dadZ);
		fgets(temp1, 255, dadA);
	}

	/* GM: ORDER BY */
	if (isOrderBy)
	{
	char v[256][100];
	int ntuplas; /* Numero total de tuplas */
	ntuplas = 0;

	/* GM: Abre arquivo de saida para ler os dados e salva-los em um array para ordenar depois */
	dadZ = fopen(temp3, "r");
	while((test = fgetc(dadZ)) != EOF)
    {
      strcpy(tcmp,"");
      tcmp[0]=test;
      tcmp[1]='\0';
      fgets(temp1,255,dadZ);
      /* GM: tcmp = tupla (linha) */
      strcat(tcmp,temp1);
      strcpy(v[ntuplas], tcmp);
      ntuplas +=1;
    }
    fclose(dadZ);

    /* GM: Bubble Sort */
    int a;
    int b;
    int trocou;
    char auxxx[256];
    b = ntuplas;

    do
    {
      b--;
      trocou =0;
      for (a = 0; a < b; a++)
      {
        if(strcasecmp(v[a], v[a+1]) > 0)
        {
          strcpy(auxxx, v[a]);
          strcpy(v[a], v[a+1]);
          strcpy(v[a+1], auxxx);
          trocou =1;
        }

      }
    } while(trocou==1);
    /* GM: Fim Bubble Sort */


    remove(temp3); /* Remove o arquivo antigo */
    dadZ = fopen(temp3, "a"); /* Abre o arquivo novo para append */
    for (a = 0; a < ntuplas; a++)
    {
        /* v[acbde] = tupla j? ordenada */
        fprintf(dadZ, "%s", v[a]); /* Escreve no arquivo */
    }
    fclose(dadZ);

    }
	/* GM: FIM ORDER BY */







	fclose(dadA);	/*O arquivo de dados ? gerado.*/

	strcpy(temp1, a);	/*Abertura do arquivo .dad da tabela de entrada.*/
	strcat(temp1, ".ctl");
	ctlA = fopen(temp1, "r");
	strcpy(temp1, z);	/*Abertura do arquivo .dad da tabela de sa?da.*/
	strcat(temp1, ".ctl");
	ctlZ = fopen(temp1, "a");

	fscanf(ctlA, "\n%d,%d", &i, &n);
	fprintf(ctlZ, "%d,%d\n", c, n);

	i = 1;
	while (!feof(ctlA))
	{
		fscanf(ctlA, "\n%s", temp1);
        if (isIn(i, cols))
 	    {
            fprintf(ctlZ, "%s\n", temp1);
        }
        i++;
	}

	if (isIn(i, cols))
	{
	   fscanf(ctlA, "\n%s", temp1);
       fprintf(ctlZ, "%s\n", temp1);
    }
	fclose(ctlA);
	fclose(ctlZ);
}

void executaSelecao(char *a, char *z, Condicao c)
{
	FILE *ctl;
	char temp[30];
	int ord, chv, tipo = 0, pos = 1, tam, nTuplas;

/* Vari?veis: ctl -> arquivo .ctl da tabela de entrada.
			  temp -> armazena o nome dos arquivos que ser?o abertos.
			  ord -> armazena se a tabela ? ou n?o ordenada pelo atributo.
			  chv -> armazena se o atributo ? ou n?o chave.
			  tipo -> tipo do atributo de compara??o.
			  pos -> armazena a posi??o do atributo nas tuplas.
			  tam -> n?mero de atributos da tabela de entrada.
			  nTuplas -> n?mero de tuplas na tabela de entrada. */

	strcpy(temp, a);	/*O arquivo .ctl da 1? tabela ? aberto.*/
	strcat(temp, ".ctl");
	ctl = fopen(temp, "r");
	fscanf(ctl, "\n%d,%d", &tam, &nTuplas);	/*N? de tuplas e de atributos ? */
	fscanf(ctl, "\n%s", temp);				/*lido.*/
	if (strcmp(c.val, "*"))
	{
		while (!feof(ctl))		/*Caso haja, ? preciso descobrir a posi??o do*/
		{						/*atributo.*/
			if (strstr(temp, c.atr1)) break;
			fscanf(ctl, "\n%s", temp);
			pos++;
		}
	}
	ord = (int) strstr(temp, "ord");	/*Testamos se a tabela est? ordenada*/
	chv = (int) strstr(temp, "chv");	/*e se o atributo ? chave.*/
	if (strstr(temp, "C")) tipo = 1;
	fclose(ctl);
	selecao(a, z, c.val, tipo, pos, chv, ord, nTuplas);

}

void executaJuncao(char *a, char *b, char *z, char *nomeA, Condicao c)
{
	FILE *ctlA, *ctlB;
	char temp[30];
	int ordA, ordB, chvA, chvB, tipo = 0, posA = 1, posB = 1,
        tamA, tamB, nTuplasA, nTuplasB;

/* Vari?veis: ctlA -> arquivo .ctl da 1? tabela de entrada.
			  ctlB -> arquivo .ctl da 2? tabela de entrada.
			  temp -> armazena o nome dos arquivos que ser?o abertos, al?m de
			  servir de vari?vel tempor?ria para dados que ser?o lidos.
			  ordA -> armazena se a 1? tabela ? ou n?o ordenada pelo atributo.
			  ordB -> armazena se a 2? tabela ? ou n?o ordenada pelo atributo.
			  chvA -> armazena se o atributo da 1? tabela ? ou n?o chave.
			  chvB -> armazena se o atributo da 2? tabela ? ou n?o chave.
			  tipo -> tipo dos atributos de compara??o.
			  posA -> armazena a posi??o do atributo nas tuplas da 1? tabela.
			  posB -> armazena a posi??o do atributo nas tuplas da 2? tabela.
			  tamA -> n?mero de atributos da 1? tabela de entrada.
			  tamB -> n?mero de atributos da 2? tabela de entrada.
			  nTuplasA -> n?mero de tuplas na 1? tabela de entrada.
			  nTuplasB -> n?mero de tuplas na 2? tabela de entrada. */

	strcpy(temp, a);	/*O arquivo .ctl da 1? tabela ? aberto.*/
	strcat(temp, ".ctl");
	ctlA = fopen(temp, "r");
	strcpy(temp, b);	/*O arquivo .ctl da 2? tabela ? aberto.*/
	strcat(temp, ".ctl");
	ctlB = fopen(temp, "r");
	fscanf(ctlA, "\n%d,%d", &tamA, &nTuplasA);	/*N? de tuplas e de atributos*/
	fscanf(ctlA, "\n%s", temp);					/*de A ? lido.*/
	while (!feof(ctlA))				/*? preciso achar a posi??o do atributo na*/
	{								/*tabela A.*/
		if (strstr(temp, c.atr1)) break;
		fscanf(ctlA, "\n%s", temp);
		posA++;
	}
	ordA = (int) strstr(temp, "ord");	/*E verificar se ela est? ordenada,*/
	chvA = (int) strstr(temp, "chv");	/*ou se o atributo ? chave.*/
	fscanf(ctlB, "\n%d,%d", &tamB, &nTuplasB);
	fscanf(ctlB, "\n%s", temp);		/*O mesmo ? feito para B.*/
	while (!feof(ctlB))
	{
		if (strstr(temp, c.atr2)) break;
		fscanf(ctlB, "\n%s", temp);
		posB++;
	}
	ordB = (int) strstr(temp, "ord");
	chvB = (int) strstr(temp, "chv");
	if (strstr(temp, "C")) tipo = 1;
	fclose(ctlA);
	fclose(ctlB);
	if (chvA)
	{
		if (chvB)
		{
			if (tamA < tamB)
               juncao(b, a, z, tipo, posB, posA, chvA, ordA, nTuplasA, nomeA);
			else
               juncao(a, b, z, tipo, posA, posB, chvB, ordB, nTuplasB, nomeA);
		}
		else
			juncao(b, a, z, tipo, posB, posA, chvA, ordA, nTuplasA, nomeA);
	}
    else
    	juncao(a, b, z, tipo, posA, posB, chvB, ordB, nTuplasB, nomeA);
}

void executaProjecao(char *a, char *z, char *listaAtrib,int isDistinct, int isOrderBy)
{
	FILE *ctl;
	char temp[30], *ptr, atrib[30];
	Cabecalho *c;
	int n, m, i, cols[20];

/* Vari?veis: ctl -> arquivo .ctl da tabela de entrada.
			  temp -> armazena o nome dos arquivos que ser?o abertos. */

	strcpy(temp, a);	/*O arquivo .ctl da 1? tabela ? aberto.*/
	strcat(temp, ".ctl");
	ctl = fopen(temp, "r");

	c = decCatalogo(ctl, &n, &m);

	i = 0;
	ptr = strchr(listaAtrib, ',');
	while (ptr)
	{
    	strncpy(atrib, listaAtrib, ptr - listaAtrib);
    	atrib[ptr - listaAtrib] = 0;
    	listaAtrib = ++ptr;
    	cols[i] = posatr(atrib, c);
    	i++;
    	ptr = strchr(listaAtrib, ',');
    }
    strcpy(atrib, listaAtrib);
    cols[i] = posatr(atrib, c);

    cols[i + 1] = 0;

	fclose(ctl);
	/* HUGO: IF order by faz projecao ordenada*/
	projecao(a, z, cols,isDistinct, isOrderBy);
}

void decSelecao(char *s, char *a, char *z, Condicao *c)
{
	char *temp;
	char *ptr;

/* Vari?veis: temp -> usada na manipula??o dos atributos que ser?o lidos.
			  ptr -> usada para percorrer a string s. */

	ptr = strchr(s, ',');	/*O nome da primeira tabela ? delimitado por ','.*/
	strncpy(a, s, ptr - s);
	a[ptr - s] = 0;		/*O conte?do ? copiado para a.*/
	temp = ++ptr;
	ptr = strchr(temp, '='); /*O atributo da condi??o ? delimitado por '='.*/
	if (ptr == NULL)		/*Se o '=' n?o ? encontrado, n?o existe condi??o.*/
	{
		strcpy(c->val, "*");
		ptr = strchr(temp, ',');
	}
	else	/*Caso contr?rio, o atributo ? lido,*/
	{
		strncpy(c->atr1, temp, ptr - temp);
		c->atr1[ptr - temp] = 0;
		temp = ++ptr;
		ptr = strchr(temp, ',');	/*Assim como o valor a ser comparado.*/
		strncpy(c->val, temp, ptr - temp);
		c->val[ptr - temp] = 0;
        if (c->val[0] == '"')		/*Se o atributo ? uma string, ? preciso*/
        {							/*tirar as aspas.*/
            strncpy(c->val, &(c->val[1]), strlen(c->val) - 2);
            c->val[strlen(c->val) - 2] = 0;
        }
	}
	temp = ++ptr;		/*Por ?ltimo, temos o nome da tabela de destino,*/
	ptr = strchr(temp, ')');	/*delimitado por ')'.*/
	strncpy(z, temp, ptr - temp);
	z[ptr - temp] = 0;
}

void decJuncao(char *s, char *a, char *b, char *z, Condicao *c)
{
	char *temp;
	char *ptr;

/* Vari?veis: temp -> usada na manipula??o dos atributos que ser?o lidos.
			  ptr -> usada para percorrer a string s. */

	ptr = strchr(s, ',');	/*O nome da primeira tabela ? delimitado por ','.*/
	strncpy(a, s, ptr - s);
	a[ptr - s] = 0;
	temp = ++ptr;
	ptr = strchr(temp, ',');	/*Assim como o nome da segunda.*/
	strncpy(b, temp, ptr - temp);
	b[ptr - temp] = 0;
	temp = ++ptr;	/*Na jun??o, sempre existe uma condi??o.*/
	ptr = strchr(temp, '=');	/*O 1? atributo, delimitado por '='.*/
	strncpy(c->atr1, temp, ptr - temp);
	c->atr1[ptr - temp] = 0;
	temp = ++ptr;		/*O segundo, por ','.*/
	ptr = strchr(temp, ',');
	strncpy(c->atr2, temp, ptr - temp);
	c->atr2[ptr - temp] = 0;
	temp = ++ptr;		/*Por ?ltimo, o nome da tabela de destino.*/
	ptr = strchr(temp, ')');
	strncpy(z, temp, ptr - temp);
	z[ptr - temp] = 0;
}

void decProjecao(char *s, char *a, char *z, char *listaAtrib)
{
	char *temp, *ptr, strn[3];
	int n;

/* Vari?veis: temp -> usada na manipula??o dos atributos que ser?o lidos.
			  ptr -> usada para percorrer a string s.
			  strn -> string representando no. de atributos.
              n -> no. de atributos. */

	ptr = strchr(s, ',');	/*O nome da primeira tabela ? delimitado por ','.*/
	strncpy(a, s, ptr - s);
	a[ptr - s] = 0;		/*O conte?do ? copiado para a.*/
	temp = ++ptr;

	ptr = strchr(temp, ',');	/*O no. de atributos ? delimitado por ','.*/
	strncpy(strn, temp, ptr - temp);
	strn[ptr - temp] = 0;		/*O conte?do ? copiado para strn.*/
	temp = ++ptr;
	n = atoi(strn);
	listaAtrib[0] = 0;
	while (n > 0)
	{
       ptr = strchr(temp, ',');
       if (listaAtrib[0])
          strcat(listaAtrib, ",");
	   strncat(listaAtrib, temp, ptr - temp);
       temp = ++ptr;
       n--;
    }
			/*Por ?ltimo, temos o nome da tabela de destino,*/
	ptr = strchr(temp, ')');	/*delimitado por ')'.*/
	strncpy(z, temp, ptr - temp);
	z[ptr - temp] = 0;
}

void recuperaValor(char *s, int pos, char *valor)
{
	int i;
	char *ptri = s;
	char *ptrf;

/* Vari?veis: i -> ?ndice de repeti??o.
			  ptri -> marca o come?o de um atributo na string s.
			  ptrf -> marca o fim de um atributo na string s. */

    ptrf = strchr(ptri, ','); /*O primeiro atributo ? delimitado por ','.*/
	for (i = 1; i != pos; i++)	/*Enquanto n?o alcan?amos a posi??o do*/
	{							/*atributo desejado, continuamos.*/
		ptri = ++ptrf;
		ptrf = strchr(ptri, ',');	/*Procuramos a pr?xima v?rgula.*/
	}
    if (ptrf != NULL)			/*Se o atributo n?o ? o ?ltimo, copiamos s?*/
    {							/*at? a pr?xima v?rgula.*/
        strncpy(valor, ptri, ((int) ptrf) - ((int) ptri));
        valor[((int) ptrf) - ((int) ptri)] = 0;
    }
    else
        strcpy(valor, ptri);	/*Sen?o, podemos copiar at? o fim.*/
}

char *juntaTuplas(char *t1, char *t2, int pos)
{
	char temp[60];

/* Vari?veis: temp -> armazena a tupla resultante. */

	strcpy(temp, t1);	/*temp ter? todos os atributos em t1*/
    strcat(temp, ","); /*e todos de t2.*/
    strcat(temp, t2);
    strcpy(t2, temp);
    return(t2);
}

void executaOperacao(char *s, char *nomeA,int isDistinct, int isOrderBy)
{
	char a[30], b[30], z[30], listaAtrib[100];
	Condicao c;

/* Vari?veis: a -> nome da 1? tabela de entrada.
			  b -> nome da 2? tabela de entrada.
			  z -> nome da tabela de sa?da.
			  listaAtrib -> lista de atributos da projecao.
			  c -> condi??o.*/

    if (s[0] == 'S') /*O primeiro caracter diz qual ? a opera??o.*/
	{

		decSelecao(&s[2], a, z, &c);	/*No caso da sele??o, os atributos s?o */
        executaSelecao(a, z, c);			/*lidos, e a opera??o ? executada.*/
        strcpy(nomeA, a);

	}
	else if (s[0] == 'J')
	{
		decJuncao(&s[2], a, b, z, &c);	/*O mesmo acontece com a jun??o.*/
		executaJuncao(a, b, z, nomeA, c);
	}
	else if (s[0] == 'P')
	{

		decProjecao(&s[2], a, z, listaAtrib);	/*O mesmo acontece com a jun??o.*/

		executaProjecao(a, z, listaAtrib,isDistinct, isOrderBy);
	}
}

void executaAlg(FILE *alg,int isDistinct, int isOrderBy)
{
	char buffer[255], nomeA[30];

/* Vari?veis: buffer -> armazena cada linha do arquivo com as opera??es.*/

    nomeA[0] = 0;
    fgets(buffer, 255, alg);	/*As linhas s?o lidas*/
	while (!feof(alg))
	{
		executaOperacao(buffer, nomeA,isDistinct, isOrderBy);		/*e interpretadas.*/
        fgets(buffer, 255, alg);
	}
	fclose(alg);	/*At? o final do arquivo.*/
}
