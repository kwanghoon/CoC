#include <stdio.h>
#include <stdlib.h>

#include "PropLogic.h"

int main()
{
	Formulae* var_P;
	Formulae* var_Q;
	Formulae* form1;
	Formulae* form2;

	// P & Q -> P | ~Q
	var_P = Var(formulaeAlloc(), "P");
	var_Q = Var(formulaeAlloc(), "Q");

	form1 = Impl(formulaeAlloc(), 
			Conj(formulaeAlloc(), var_P, var_Q),
			Disj(formulaeAlloc(), var_P, Neg(formulaeAlloc(), var_Q)));
	printf("// P & Q -> P | ~Q : ");
	printFormulae(form1);
	printf("\n");

	// NNF( ~(P -> ~(P & Q) ) = P & (P & Q)
	form2 = Neg(formulaeAlloc(),
				Impl(formulaeAlloc(),
					var_P,
					Neg(formulaeAlloc(),
						Conj(formulaeAlloc(),
							var_P,
							var_Q))));

	printf("// NNF( ~(P -> ~(P & Q) ) = ");
	printFormulae(NNF(form2));
	printf("\n");

#if SYSTEM == LINUX
	/* empty */ 
#else
	system("pause");
#endif
	return 0;
}

