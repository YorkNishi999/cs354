#include <stdio.h>

/* それでは、自分で（とても）簡単なC言語のコードを書いて、
手を汚してみましょう。integer.cという名前のCプログラムを
書きます。(1, 100) - 1と100を含まない範囲の整数が与えられ
ると、プログラムは次のように表示します。
整数の16進数表示
整数の8進数表示
整数が素数であるか合成数であるか（素数 vs 合成数
整数が有効範囲内にない場合は、プログラムは「not in range」
と表示します。他の種類の入力（double、stringなど）はプロ
グラムに与えられないと仮定してもよいので、他の入力検証を
気にする必要はありません。
*/
int main()
{
	/* 整数を入力して変数として受けとる */
	int n, i;

	printf("Enter an integer between 1 to 100 (1 and 100 excluded:\n");
	scanf("%d", &n);
	
	if(n <= 1 || n >= 100)
		printf("not in range\n");
	else {
		printf("0x%x\n", n);
		printf("%03o\n", n);
		if (n == 2)
			printf("prime\n");
		for ( i = 2; i < n; ++i ) {
			//printf("i n %d %d\n", i, n);
			if (i == n-1)
				printf("prime\n");		
			if (n%i == 0) {
				printf("composit\n");
				break;
			}
		}
	}
	return 0;
}



