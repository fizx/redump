#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define BUF_SIZE 10
#define WL(A) A, strlen(A)
#define JUST_SAW(A) just_saw(WL(A))
#define BS '\\'
#define SQ '\''
#define DQ '"'
#define NL '\n'
#define TAB '\t'

static bool escaped = false;
static bool in_string = false;
static bool in_values = false;
static bool in_valueset = false;
static int buffer[BUF_SIZE];
static int pos = 0;

static int
ring_insert(int c) {
  buffer[pos] = c;
  pos++;
  if(pos == BUF_SIZE) pos = 0;
  return c;
}

static bool
just_saw(char* string, int len) {
  for(int i = pos - len; i < pos; i++){
    if(*string++ != buffer[(i < 0) ? i + BUF_SIZE : i]){
      return false;
    }
  }
  return true;
}

int
main (int argc, char **argv) {
  int c;
  while((c = getchar()) != EOF) {
    if(in_values) {
      if(in_valueset){
        if(in_string){
          if(escaped) {
            escaped = false;
            switch(c){
              case SQ: putchar(SQ); break;
              case DQ: putchar(DQ); break;
              case '%': putchar('%'); break;
              case '_': putchar('_'); break;
              default: putchar(BS); putchar(c);
            }
          } else {
            switch(c){
              case BS: escaped = true; break;
              case SQ: in_string = false; break;
              case NL: putchar(BS);putchar('n'); break;
              case TAB: putchar(BS);putchar('t'); break;
              default: putchar(c);
            }
          }
        } else {
          switch(c){
            case ')': putchar(NL); in_valueset = false; break;
            case ',': putchar(TAB); break;
            case SQ: in_string = true; break;
            default: putchar(c);
          };        
        }
      } else{
        switch(c){
          case ';': in_values = false; break;
          case '(': in_valueset = true; break;
        };
      }
    } else { 
      ring_insert(c);
      if(JUST_SAW("VALUES")) in_values = true;
    }
    
  }
  return 0;
}