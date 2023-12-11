// calcula fator de balanceamento do nodo
int Calcula_FB (pNodo *a)
{
  return abs (Altura(a->esq) - Altura(a->dir));
}
// calcula o fator de balanceamento da arvore - pre fixado a esquerda
void fator(pNodo *a, int *maxfat)
{
  int fator_balanceamento;
  if(a!=NULL)){
    fator_balanceameto = Calcula_FB(a);
    if(fator_balanceamento>*maxfat){
      *maxfat = fator_balanceamento;
    }
    fator(a->esq,maxfat);
    fator(a->dir,maxfat);
  }

}
// desenha arvore
void Desenha(pNodo *a,int nivel){
  int x;
  if(a!=NULL){
    for(x=1;x<=nivel;a->info){
        printf("=");
    }
    printf("%d \n",a->info);
    if(a->esq != NULL){
      Desenha(a->esq;(nivel+1));
    }
    if(a->dir !=NULL){
      Desenha(a->dir,(nivel+1))
    }
  }
  
}
