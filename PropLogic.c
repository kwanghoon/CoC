
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "PropLogic.h"

#if SYSTEM == LINUX
#define _strdup strdup
#endif

Formulae* formulaeAlloc ()
{
	Formulae* form = (Formulae*)malloc(sizeof(Formulae));
	if (form == NULL)
		return NULL;
	
	form->tag			= NOTHING;
	form->body.name		= NULL;
	form->body.arg.op1	= NULL;
	form->body.arg.op2	= NULL;
	
	return form;
}

// void formulaeFree(Formulae* form);

Formulae* True(Formulae* form)
{
	form->tag = TRUE;
	return form;
}

Formulae* False(Formulae* form)
{
	form->tag = FALSE;
	return form;
}

Formulae* Var(Formulae* form, char* name)
{
	form->tag		= VAR;
	form->body.name = name;
	return form;
}

Formulae* Neg(Formulae* form, Formulae* arg)
{
	form->tag	  = NEG;
	form->body.arg.op1 = arg;
	return form;
}

Formulae* Conj(Formulae* form, Formulae* arg1, Formulae* arg2)
{
	form->tag	  = CONJ;
	form->body.arg.op1 = arg1;
	form->body.arg.op2 = arg2;
	return form;
}

Formulae* Disj(Formulae* form, Formulae* arg1, Formulae* arg2)
{
	form->tag	  = DISJ;
	form->body.arg.op1 = arg1;
	form->body.arg.op2 = arg2;
	return form;
}

Formulae* Impl(Formulae* form, Formulae* arg1, Formulae* arg2)
{
	form->tag	  = IMPL;
	form->body.arg.op1 = arg1;
	form->body.arg.op2 = arg2;
	return form;
}

Formulae* Iff(Formulae* form, Formulae* arg1, Formulae* arg2)
{
	form->tag	  = IFF;
	form->body.arg.op1 = arg1;
	form->body.arg.op2 = arg2;
	return form;
}

int isAtom(Formulae* form)
{
	return form->tag==TRUE || form->tag==FALSE || form->tag==VAR;
}

int isLiteral(Formulae* form)
{
	return isAtom(form) || form->tag==NEG && isAtom(form->body.arg.op1);
}


void fprintFormulae(FILE* fp, Formulae* form)
{
#define PROPENPAREN(x) if (isLiteral((x)) == 0) fprintf(fp, "(")
#define PRCLOSEPAREN(x) if (isLiteral((x)) == 0) fprintf(fp, ")")

	switch(form->tag)
	{
	case TRUE:
		fprintf(fp, "T");
		break;
	case FALSE:
		fprintf(fp, "F");
		break;
	case VAR:
		fprintf(fp, "%s", form->body.name);
		break;
	case NEG:
		fprintf(fp, "~");

		PROPENPAREN(form->body.arg.op1);
		fprintFormulae(fp, form->body.arg.op1);
		PRCLOSEPAREN(form->body.arg.op1);
		break;
	case CONJ:
		PROPENPAREN(form->body.arg.op1);
		fprintFormulae(fp, form->body.arg.op1);
		PRCLOSEPAREN(form->body.arg.op1);

		fprintf(fp, " & ");

		PROPENPAREN(form->body.arg.op2);
		fprintFormulae(fp, form->body.arg.op2);
		PRCLOSEPAREN(form->body.arg.op2);
		break;
	case DISJ:
		PROPENPAREN(form->body.arg.op1);
		fprintFormulae(fp, form->body.arg.op1);
		PRCLOSEPAREN(form->body.arg.op1);

		fprintf(fp, " | ");

		PROPENPAREN(form->body.arg.op2);
		fprintFormulae(fp, form->body.arg.op2);
		PRCLOSEPAREN(form->body.arg.op2);
		break;
	case IMPL:
		PROPENPAREN(form->body.arg.op1);
		fprintFormulae(fp, form->body.arg.op1);
		PRCLOSEPAREN(form->body.arg.op1);

		fprintf(fp, " -> ");

		PROPENPAREN(form->body.arg.op2);
		fprintFormulae(fp, form->body.arg.op2);
		PRCLOSEPAREN(form->body.arg.op2);
		break;
	case IFF:
		PROPENPAREN(form->body.arg.op1);
		fprintFormulae(fp, form->body.arg.op1);
		PRCLOSEPAREN(form->body.arg.op1);

		fprintf(fp, " <-> ");

		PROPENPAREN(form->body.arg.op2);
		fprintFormulae(fp, form->body.arg.op2);
		PRCLOSEPAREN(form->body.arg.op2);
		break;
	default:
		fprintf(stderr, "[fprintFormulae] Unexpected tag: %d", form->tag);
	}
}

// enum Tag { NOTHING=0, TRUE, FALSE, VAR, NEG, CONJ, DISJ, IMPL, IFF };
Formulae* clone(Formulae* form)
{
	switch(form->tag)
	{
	case TRUE:
		return True(formulaeAlloc());
	case FALSE:
		return False(formulaeAlloc());
	case VAR:
		return Var(formulaeAlloc(), _strdup(form->body.name));
	case NEG:
		return Neg(formulaeAlloc(), clone(form->body.arg.op1));
	case CONJ:
		return Conj(formulaeAlloc(), clone(form->body.arg.op1), clone(form->body.arg.op2));
	case DISJ:
		return Disj(formulaeAlloc(), clone(form->body.arg.op1), clone(form->body.arg.op2));
	case IMPL:
		return Impl(formulaeAlloc(), clone(form->body.arg.op1), clone(form->body.arg.op2));
	case IFF:
		return Iff(formulaeAlloc(), clone(form->body.arg.op1), clone(form->body.arg.op2));
	default:
		fprintf(stderr, "[Clone] Unexpected tag: %d", form->tag);
	}

	return NULL;
}
