/* ordenação por seleção */
int x[10];

/* low nao eh declarada na tabela de simbolos, corrigir esse bug de sintaxe */
int minloc ( int a[], int low, int high ){
   int i;   int y;   int k;
   k = low;
   y = a[low];
   i = low + 1;
   while(i<high){
      if(a[i] < y){
         y = a[i];
         k = i;
      }
      i = i + 1;
   }
   return k;
}

void sort(int a[], int low, int high){
   int i;   int k;
   i = low;
   while(i <= high){
      int t;
      k = minloc(a,i,high);
      t = a[k];
      a[k] = a[i];
      a[i] = t;
      i = i + 1;
   }
}

void main(void){
   int i;
   i = 0;
   while(i<10){
      x[i] = input();
      i = i + 1;
   }
   sort(x,0,10);
   i = 0;
   while(i<10){
      output(x[i]);
      i = i + 1;
   }
}