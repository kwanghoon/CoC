
#ifndef PROP_LOGIC_H
#define PROP_LOGIC_H

enum Tag { NOTHING=0, TRUE, FALSE, VAR, NEG, CONJ, DISJ, IMPL, IFF };


typedef struct _Formulae Formulae;

struct _Formulae {
	enum Tag tag;
	union {
		char* name;
		struct _Arg {
			Formulae* op1;
			Formulae* op2;
		} arg;
	} body;
};

Formulae* formulaeAlloc ();
Formulae* clone(Formulae* form);

void fprintFormulae(FILE* fp, Formulae* form);
#define printFormulae(form) fprintFormulae(stdout, form)

Formulae* True(Formulae* form);
Formulae* False(Formulae* form);
Formulae* Var(Formulae* form, char* name);
Formulae* Neg(Formulae* form, Formulae* arg);
Formulae* Conj(Formulae* form, Formulae* arg1, Formulae* arg2);
Formulae* Disj(Formulae* form, Formulae* arg1, Formulae* arg2);
Formulae* Impl(Formulae* form, Formulae* arg1, Formulae* arg2);
Formulae* Iff(Formulae* form, Formulae* arg1, Formulae* arg2);

int isLiteral(Formulae* form);
int isAtom(Formulae* form);

Formulae* NNF(Formulae* form);
#endif // PROP_LOGIC_H
