#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>
//second commit
#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum {
   lparen = 0,  /* ( 왼쪽 괄호 */
   rparen = 9,  /* ) 오른쪽 괄호*/
   times = 7,   /* * 곱셈 */
   divide = 6,  /* / 나눗셈 */
   plus = 5,    /* + 덧셈 */
   minus = 4,   /* - 뺄셈 */
   operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];    /* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];  /* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];     /* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];         /* 계산을 위해 필요한 스택 */


bool isValidOperand(char symbol);//입력된 문자가 유효한 숫자인지 확인하는 함수
bool isValidOperator(char symbol);//입력된 문자가 유효한 연산자인지 확인하는 함수
bool isValidExpression(); //infixExp에 입력된 수식이 유효한지 검사하는 함수

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;      /* evalStack용 top */
int evalResult = 0;      /* 계산 결과를 저장 */

void postfixpush(char x); //postfixStack에 요소를 추가하는 함수
char postfixPop(); //postfixStack에서 요소를 제거하고 반환하는 함수
void evalPush(int x); //evalstack에 요소를 추가하는 함수
int evalPop(); //evalstack에서 요소를 제거하고 반환하는 함수
int isEmpty(); //postfixstack이 비어있는지 확인하는 함수
void getInfix(); //infix를 입력받는 함수
precedence getToken(char symbol); //연산자의 우선순위를 반환하는 함수
precedence getPriority(char x); //연산자의 우선순위를 반환하는 함수
void charCat(char* c); //postfixexp에 문자를 추가하는 함수
void toPostfix(); //infixexp를 postfixexp로 변환하는 함수
void debug(); //디버그 정보를 출력하는 함수
void reset(); //모든 변수 및 스택을 초기화하는 함수
void evaluation(); //postfixexp를 evalue하ㅣ여 결과를 출력하는 함수

int main()
{
   printf("[----- [LeeSeungHun] [2023041045] -----]");

   char command;

   do {
      printf("----------------------------------------------------------------\n");
      printf("               Infix to Postfix, then Evaluation               \n");
      printf("----------------------------------------------------------------\n");
      printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
      printf("----------------------------------------------------------------\n");

      printf("Command = ");
      scanf(" %c", &command);

      switch (command) {
      case 'i': case 'I':
         getInfix(); //infix 입력 받기
         break;
      case 'p': case 'P':
         toPostfix(); //infix를 postfix로 변환
         break;
      case 'e': case 'E':
         evaluation(); // postfix를 evalue하여 결과 출력
         break;
      case 'd': case 'D':
         debug(); //디버그 정보 출력
         break;
      case 'r': case 'R':
         reset(); //모든 변수 및 스택 초기화
         break;
      case 'q': case 'Q':
         break;
      default:
         printf("\n       >>>>>   Concentration!!   <<<<<     \n");
         break;
      }

   } while (command != 'q' && command != 'Q');

   return 1;
}

void postfixPush(char x)
{
   postfixStack[++postfixStackTop] = x; //postfixStack에 값을 추가
}

char postfixPop()
{
   char x;
   if (postfixStackTop == -1)
      return '\0';
   else {
      x = postfixStack[postfixStackTop--]; //postfixStack에서 값을 꺼내어 반환
   }
   return x;
}

void evalPush(int x)
{
   evalStack[++evalStackTop] = x; //evalStack에 요소를 추가
}

int evalPop()
{
   if (evalStackTop == -1)
      return -1;
   else
      return evalStack[evalStackTop--]; //evalStack에서 값을 꺼내어 반환
}

int isEmpty() {
   return (postfixStackTop == -1); //postfixStackTop이 -1이면 비어있음
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
   printf("Type the expression >>> ");
   scanf("%s", infixExp); //infix 입력 받기
}

precedence getToken(char symbol)
{
   switch (symbol) {
   case '(': return lparen;
   case ')': return rparen;
   case '+': return plus;
   case '-': return minus;
   case '/': return divide;
   case '*': return times;
   default: return operand;
   }
}

precedence getPriority(char x)
{
   return getToken(x);
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
   if (postfixExp == '\0')
      strncpy(postfixExp, c, 1);
   else
      strncat(postfixExp, c, 1); //postfixExp에 문자 추가
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
   char* exp = infixExp;
   char x;

   while (*exp != '\0')
   {
      if (getToken(*exp) == operand) {
         charCat(exp); //피연산자는 postfixExp에 바로 추가한다
      }
      else if (*exp == '(') {
         postfixPush(*exp); // (를 스택에 push한다
      }
      else if (*exp == ')') {
         while (postfixStack[postfixStackTop] != '(') {
            charCat(&postfixStack[postfixStackTop--]); // (를 만날 때까지 스택에서 연산자를 pop하여 postfixExp에 추가한다
         }
         postfixPop(); // (를 pop하여 스택에서 제거
      }
      else {
         while (!isEmpty() && getPriority(*exp) <= getPriority(postfixStack[postfixStackTop])) {
            charCat(&postfixStack[postfixStackTop--]); //스택의 top에 있는 연산자의 우선순위가 높거나 같은 경우, pop하여 postfixExp에 추가한다
         }
         postfixPush(*exp); //현재 연산자를 스택에 push한다
      }
      exp++;
   }

   //스택에 남아있는 연산자를 모두 pop하여 postfixExp에 추가한다
   while (!isEmpty()) {
      charCat(&postfixStack[postfixStackTop--]);
   }
}


void debug()
{
   printf("\n---DEBUG\n");
   printf("infixExp =  %s\n", infixExp);
   printf("postExp =  %s\n", postfixExp);
   printf("eval result = %d\n", evalResult);

   printf("postfixStack : ");
   for (int i = 0; i < MAX_STACK_SIZE; i++)
      printf("%c  ", postfixStack[i]);

   printf("\n");

}

void reset()
{
   infixExp[0] = '\0';
   postfixExp[0] = '\0';

   for (int i = 0; i < MAX_STACK_SIZE; i++)
      postfixStack[i] = '\0';

   postfixStackTop = -1;
   evalStackTop = -1;
   evalResult = 0;
}

void evaluation()
{
   char* exp = postfixExp;
   char x;

   //postfixExp의 문자를 하나씩 읽어가면서 계산한다
   while (*exp != '\0') {
      if (getToken(*exp) == operand) {
         evalPush(*exp - '0'); //피연산자는 evalStack에 push
      }
      else {
         int operand2 = evalPop(); //두 번째 피연산자 pop
         int operand1 = evalPop(); //첫 번째 피연산자 pop
         switch (getToken(*exp)) {
         case plus: evalPush(operand1 + operand2); break;
         case minus: evalPush(operand1 - operand2); break;
         case times: evalPush(operand1 * operand2); break;
         case divide: evalPush(operand1 / operand2); break;
         }
      }
      exp++;
   }

   evalResult = evalPop(); //evalStack에 남아 있는 결과값을 가져오기
}

bool isValidOperand(char symbol) {
   return (symbol >= '0' && symbol <= '9');
}

bool isValidOperator(char symbol) {
   return (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/' || symbol == '(' || symbol == ')');
}

bool isValidExpression() {
   char* exp = infixExp;
   int parenCount = 0; // 괄호의 개수를 세는 변수

   while (*exp != '\0') {
      if (!isValidOperand(*exp) && !isValidOperator(*exp)) {
         // 숫자나 연산자가 아닌 문자가 입력되면 유효하지 않은 수식임
         return false;
      }

      if (*exp == '(') {
         parenCount++;
      }
      else if (*exp == ')') {
         parenCount--;
         if (parenCount < 0) {
            // 오른쪽 괄호가 왼쪽 괄호보다 더 많으면 유효하지 않은 수식임
            return false;
         }
      }

      exp++;
   }

   if (parenCount != 0) {
      // 왼쪽 괄호와 오른쪽 괄호의 개수가 일치하지 않으면 유효하지 않은 수식임
      return false;
   }

   return true;
}