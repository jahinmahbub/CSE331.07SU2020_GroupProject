#include <reg51.h>
#define lcdport P2

sbit rs=P3^0;
sbit rw=P3^1;
sbit en=P3^2;
sbit start= P1^0;
sbit stop= P1^5;
sbit party1=P1^1;  //Candidate1
sbit party2=P1^2;  //Candidate2
sbit party3=P1^3;  //Candidate3
sbit party4=P1^4;  //Candidate4

void lcdcmd(char);
void lcdint();
void lcddata(char);
void lcdstring(char*);
void delay(unsigned int);
void longdelay(unsigned int);
void dispaly_vote(unsigned int);
void count();
void result();
void check();
unsigned int vote1,vote2,vote3,vote4 ;
char vote_no[4];

void main() {
    lcdport = 0x00;

    party1 = party2 = party3 = party4 = 0;
    vote1 = vote2 = vote3 = vote4 = 0;
    start = stop = 0;
    
    lcdint();
    lcdstring("press start ");
    lcdcmd(0xc0);
    lcdstring("to initiate");
    
    while(1) {
        if(start == 1) {
            lcdcmd(0x84);
            lcdcmd(0x01);
            lcdstring("WELCOME!!");
            longdelay(200);
            lcdcmd(0x01);
            lcdstring("press any key");
            lcdcmd(0xc0);
            lcdstring("to vote");
            longdelay(200);
            lcdcmd(0x01);
            lcdstring("BAL");
            delay(500);
            lcdcmd(0x84);
            lcdstring("BNP");
            delay(500);
            lcdcmd(0x88);
            lcdstring("JP");
            delay(500);
            lcdcmd(0x8C);
            lcdstring("WPB");
            count();
            lcdcmd(0x01);
            lcdcmd(0x80);
            lcdstring("thank you!!");
            longdelay(500);
            check();
        }
    }
}

void check() {
    if(party1 == 0 && party2 == 0 && party3 == 0 && party4 == 0) {
        if(stop != 0) {
            while(1)
                result();
        }
    }
}

void result() {
    int max = 0, flag = 0;
    
    lcdcmd(0x01);
    lcdstring("BAL");
    delay(500);

    lcdcmd(0x84);
    lcdstring("BNP");
    delay(500);

    lcdcmd(0x88);
    lcdstring("JP");
    delay(500);

    lcdcmd(0x8C);
    lcdstring("WPB");
    
    lcdcmd(0xc0);
    dispaly_vote(vote1);

    lcdcmd(0xc4);
    dispaly_vote(vote2);

    lcdcmd(0xc8);
    dispaly_vote(vote3);

    lcdcmd(0xcc);
    dispaly_vote(vote4);

    if(vote1 > max)
	max = vote1;
    
    if(vote2 > max)
	max = vote2;

    if(vote3 > max)
	max = vote3;

    if(vote4 > max)
	max=vote4;

    longdelay(500);
    
    if ( (vote1 == max) && ( vote2 != max) && (vote3 != max) && (vote4 != max) ) {
        flag = 1;
        lcdcmd(0x01);
        lcdcmd(0x80);
        lcdstring("BAL");
        lcdcmd(0xc5);
        lcdstring("wins");
        longdelay(500);
    }

    if ( (vote2 == max) && ( vote1 != max) && (vote3 != max) && (vote4 != max) ) {
        flag = 1;
        lcdcmd(0x01);
        lcdcmd(0x80);
        lcdstring("BNP");
        lcdcmd(0xc5);
        lcdstring("wins");
        longdelay(500);
    }

    if ( (vote3 == max) && ( vote2 != max) && (vote1 != max) && (vote4 != max) ) {
        flag = 1;
        lcdcmd(0x01);
        lcdcmd(0x80);
        lcdstring("JP");
        lcdcmd(0xc5);
        lcdstring("wins");
        longdelay(500);
    }

    if ( (vote4 == max) && ( vote2 != max) && (vote1 != max) && (vote3 != max) ) {
        flag = 1;
        lcdcmd(0x01);
        lcdcmd(0x80);
        lcdstring("WPB");
        lcdcmd(0xc5);
        lcdstring("wins");
        longdelay(500);
    }

    if(flag == 0) {
        lcdcmd(0x01);
        lcdcmd(0x80);
        lcdstring("clash between");
        lcdcmd(0xc0);
        
	if(vote1 == max)
	    lcdstring("BAL");
        
	if(vote2 == max)
	    lcdstring("BNP");

        if(vote3 == max)
	    lcdstring("JP");
        
        if(vote4 == max)
            lcdstring("WPB");

        longdelay(200);
    }
}

void dispaly_vote(unsigned int vote) {
    int k, p;
    for (k = 0; k <= 2; k++) {
        vote_no[k] = vote % 10;
        vote = vote / 10;
    }

    for (p = 2; p >= 0; p--)
        lcddata(vote_no[p]+48);
}

void count() {
    while(party1 == 0 && party2 == 0 && party3 == 0 && party4 == 0);

    if (party1 == 1)
        vote1 = vote1 + 1;
    
    if (party2 == 1)
	vote2 = vote2 + 1;

    if (party3 == 1)
        vote3 = vote3 + 1;

    if (party4 == 1)
        vote4 = vote4 + 1;
}

void delay(unsigned int x) {
    unsigned int i;
    for(i=0; i<x; i++);
}

void longdelay(unsigned int u) {
    unsigned int i,j;
    for(i=0; i<u; i++)
        for(j=0; j<1275; j++);
}

void lcdint() {
    lcdcmd(0x38);
    delay(500);

    lcdcmd(0x01);
    delay(500);

    lcdcmd(0x0c);
    delay(500);

    lcdcmd(0x80);
    delay(500);

    lcdcmd(0x0e);
    delay(500);
}

void lcdcmd(char value) {
    lcdport = value;
    rw=0;
    rs=0;
    en=1;
    delay(500);
    en=0;
}

void lcdstring(char *p) {
    while(*p != '\0') {
        lcddata(*p);
        delay(2000);
        p++;
    }
}

void lcddata(char value) {
    lcdport = value;
    rs=1;
    rw=0;
    en=1;
    delay(500);
    en=0;
}