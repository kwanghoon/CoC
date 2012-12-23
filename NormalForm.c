#include <stdio.h>
#include "PropLogic.h"

// Negation Normal Form (NNF)

Formulae* NNF(Formulae* form)
{
	switch(form->tag)
	{
	case TRUE:
	case FALSE:
	case VAR:
			return clone(form);

	case NEG:
		// ~~F => F
		if (form->body.arg.op1->tag == NEG)
			return NNF(clone(form->body.arg.op1->body.arg.op1));

		// ~T => F
		if (form->body.arg.op1->tag == NEG)
			return NNF(clone(form->body.arg.op1->body.arg.op1));

		// ~F => T
		if(form->body.arg.op1->tag == FALSE)
			return False(formulaeAlloc());

		// ~(F1 & F2) => ~F1 | ~ F2
		if(form->body.arg.op1->tag == CONJ)
			return Disj(formulaeAlloc(),
					NNF(Neg(formulaeAlloc(), clone(form->body.arg.op1->body.arg.op1))),
					NNF(Neg(formulaeAlloc(), clone(form->body.arg.op1->body.arg.op2))));

		// ~(F1 | F2) => ~F1 & ~ F2
		if(form->body.arg.op1->tag == DISJ)
			return Conj(formulaeAlloc(),
					NNF(Neg(formulaeAlloc(), clone(form->body.arg.op1->body.arg.op1))),
					NNF(Neg(formulaeAlloc(), clone(form->body.arg.op1->body.arg.op2))));

		// ~(F1 -> F2) => ~(~F1 | F2) => F1 & ~F2  [NEW]
		if(form->body.arg.op1->tag == IMPL)
			return Conj(formulaeAlloc(),
					NNF(form->body.arg.op1->body.arg.op1),
					NNF(Neg(formulaeAlloc(),
							form->body.arg.op1->body.arg.op2)));

		// ~(F1 <-> F2) => ~((F1 -> F2) & (F2 -> F1))
		//              => ~((~F1 | F2) & (~F2 | F1)) => (F1 & ~F2) | (F2 & ~F1) [NEW]
		if(form->body.arg.op1->tag == IFF)
			return Disj(formulaeAlloc(),
					Conj(formulaeAlloc(),
						NNF(form->body.arg.op1->body.arg.op1),
						NNF(Neg(formulaeAlloc(), 
								form->body.arg.op1->body.arg.op2))),
					Conj(formulaeAlloc(),
						NNF(form->body.arg.op1->body.arg.op2),
						NNF(Neg(formulaeAlloc(),
								form->body.arg.op1->body.arg.op1))));

		// otherwise
			return Neg(formulaeAlloc(), NNF(form->body.arg.op1));

	case CONJ:
			return Conj(formulaeAlloc(),
					NNF(form->body.arg.op1),
					NNF(form->body.arg.op2));
	case DISJ:
			return Disj(formulaeAlloc(),
					NNF(form->body.arg.op1),
					NNF(form->body.arg.op2));
	case IMPL:
		// F1 -> F2 => ~F1 | F2
			return Disj(formulaeAlloc(),
					NNF(Neg(formulaeAlloc(), clone(form->body.arg.op1))),
					NNF(clone(form->body.arg.op2)));
	case IFF:
		// F1 <-> F2 => (F1 -> F2) & (F2 -> F1)
			Conj(formulaeAlloc(),
					Impl(formulaeAlloc(),
							NNF(clone(form->body.arg.op1)),
							NNF(clone(form->body.arg.op2))),
					Impl(formulaeAlloc(),
							NNF(clone(form->body.arg.op1)),
							NNF(clone(form->body.arg.op2))));
	default:
		fprintf(stderr, "[NNF] Unexpected tag: %d", form->tag);
		return NULL;
	}
}


