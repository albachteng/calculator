#include <stdio.h>

#define MAXOP 20    /* max size of operand, operator to be processed*/
#define NUMBER '0'  /* symbolic representation of number */
#define TOOBIG '9'  /* symbolic representation of too big error */
#define MAXVAL 100  /* max size of our stack */
#define BUFSIZE 100 /* max size of buffer */

static int sp = 0; /* pointer to top of stack */
static double val[MAXVAL];
static char buf[BUFSIZE];
static int bufp = 0; /* next free position in buf*/

int main() {
  char getop(char[], int);
  int type;
  void clear();
  char s[MAXOP];
  double op2, atof(char[]), pop(), push(double);

  while ((type = getop(s, MAXOP)) != EOF)
    switch (type) {
    case NUMBER:
      push(atof(s));
      break;
    case '+':
      push(pop() + pop());
      break;
    case '*':
      push(pop() * pop());
      break;
    case '-':
      op2 = pop();
      push(pop() - op2);
      break;
    case '/':
      op2 = pop();
      if (op2 != 0.0)
        push(pop() / op2);
      else
        printf("zero divisor\n");
      break;
    case '=':
      printf("\t%f\n", push(pop()));
      break;
    case 'c':
      clear();
      break;
    case TOOBIG:
      printf("%.20s ... is too long\n", s);
      break;
    default:
      printf("unknown command %c\n", type);
      break;
    }
}

// determines the predefined 'type' of an operator or operand
char getop(char s[], int lim) {
  void ungetch(int);
  int getch();
  static int i, c; /* index, character (int representation) */
  while ((c = getch()) == ' ' || c == '\t' || c == '\n')
    ; /* ignore spaces, tabs and newlines */
  if (c != '.' && (c < '0' || c > '9'))
    /* if c isn't a number or a dot, return it unchanged */
    return (c);
  /* now dealing with numbers: */
  s[0] = c;
  /* as long as we haven't reached maxval and we get numbers, add to s */
  for (i = 1; (c = getchar()) >= '0' && c <= '9'; i++)
    if (i < lim)
      s[i] = c;
  /* when we hit a decimal place: */
  if (c == '.') {
    if (i < lim)
      s[i] = c;
    /* for decimal values of c hereafter */
    for (i++; (c = getchar()) >= '0' && c <= '9'; i++)
      if (i < lim)
        s[i] = c;
  }
  /* if we're still under the limit, unget the last char and terminate s */
  if (i < lim) {
    ungetch(c);
    s[i] = '\0';
    /* if we got this far, it's a valid number */
    return (NUMBER);
  } else {
    while (c != '\n' && c != EOF)
      /* get the remaining chars */
      c = getchar();
    /* terminate the string before the max value */
    s[lim - 1] = '\0';
    /* string was too big, truncated value saved in buffer */
    return (TOOBIG);
  }
}

// get char from buffer, or, if empty, await input
int getch() { return ((bufp > 0) ? buf[--bufp] : getchar()); }

// return the character to the buffer, unless overflowing
void ungetch(int n) {
  if (bufp > BUFSIZE)
    printf("ungetch: too many characters\n");
  else
    buf[bufp++] = n;
}

void ungets(char s[]) {
  int i;
  for (i = 0; s[i] != '\0'; i++)
    ungetch(s[i]);
  // s[++i] = '\0';
}

// if not exceeding the maximum stack size, add the value on top and count up
double push(double f) {
  void clear();
  if (sp < MAXVAL)
    return (val[sp++] = f);
  else {
    printf("error stack full\n");
    clear();
    return (0);
  }
}

// if the stack isn't empty, return the top value and count down
double pop() {
  void clear();
  if (sp > 0)
    return (val[--sp]);
  else {
    printf("error stack empty\n");
    clear();
    return (0);
  }
}

// clear the stack
void clear() { sp = 0; }
