#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>

/******************************************************************************
Demonstrates how to crack an encrypted password using a simple "brute force" algorithm. Works on passwords that consist only of 3 uppercase letters and a 2 digit integer. 

To Compile:
    cc -o Encrypt-password Encrypt-password.c -lcrypt

To Run:
    ./Encrypt-password > results.txt

Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/
int n_passwords = 4;

char *encrypted_passwords[] = {
  "$6$KB$vzTSu3Maa1.YpMdXwz8uTuT6aXf3AnewVv/VVvPf8BIfoV9rShaqc6BK1qlAlycksVwx.8EnE1qSJ0H048EK8.",
  "$6$KB$jKaNKDJ11DBKxqSNucLLHySWvx6O2oSWpmCQLm5XJeyk6p9cBOoIC096LNGwZV1jME1nYWPQ1wrscI0mW2HIh.",
  "$6$KB$mFJx2Kzic6WOiC2sTFC7UJlucB1ri6r5xGpr.JZHAJEzZ.6aOJUpm/z.oPOJ66EdgGer81kigMHdRGpARNdGi0",
  "$6$KB$BcluLD9iDmMVqkNrI2kGA801WATlmW.vxd5D39N8Qqa5titVvdqru6Gkj7a6ApDHmGqE8qPVPMilgfp21HGfa1"
};

//Due to lack of Standard function on C following below is required.

void sub(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

//This function crack the password and display # when password is found.

void aashish(char *salt_and_encrypted){
  int x, y, z,w;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  sub(salt, salt_and_encrypted, 0, 6);

  for(x='A'; x<='Z'; x++){
    for(y='A'; y<='Z'; y++){
      for(w='A'; w<='Z'; w++){
        for(z=0; z<=99; z++){
         sprintf(plain, "%c%c%c%02d", x, y,w, z); 
          enc = (char *) crypt(plain, salt);
          count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
      }
    }
  }
}
  printf("%d solutions explored\n", count);
}
// Used for calculating Time

int time_difference(struct timespec *start, struct timespec *finish,long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}
int main(int argc, char *argv[]){
  int i;
  struct timespec start, finish;   
  long long int time_elapsed;
  clock_gettime(CLOCK_MONOTONIC, &start);
  for(i=0;i<n_passwords;i<i++) {
    aashish(encrypted_passwords[i]);
  }
clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,(time_elapsed/1.0e9));
  return 0;
}
