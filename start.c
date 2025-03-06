#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include<unistd.h>

#define COMPANY_COUNT 100
#define BOT_COUNT 1

double random_share_price()
{
    return (double)rand()/(double)RAND_MAX;
}

double get_direction(int dir)
{
    int d = rand()%100;
    int m = 0;
    if(d <= 10)
    {
        m = 10;
    }
    else if(d <= 50)
    {
        m = 5;
    }
    else if(d <= 80)
    {
        m = 2;
    }
    else if(d <= 99)
    {
        m = -2;
    }
    else
    {
        m = 50;
    }
    return m*dir;
}

typedef struct company_struct
{
    double share_price;
    int direction;
    
}Company;

typedef struct bot_struct
{
    int* company_shares;
    int* share_count;
    int strategy;
    double money;
    double in_market;
}Bot;

Company* companies;
Bot* bots;

void init_stock_market()
{
    companies = malloc(sizeof(*companies)*COMPANY_COUNT);
    
    for(int i=0; i < COMPANY_COUNT; i++)
    {
        companies[i].share_price = rand()%100;
        companies[i].direction = rand()%2*2-1;
    }
}

void stock_market_update()
{
    for(int i=0; i < COMPANY_COUNT; i++)
    {
        companies[i].share_price += random_share_price()*get_direction(companies[i].direction);
    }
}

void print_bot(Bot* b)
{
    int cc = 0;
    int sc = 0;
    for(int i=0; i < COMPANY_COUNT; i++)
    {
        if(b->share_count[i] > 0)
            cc++;
        sc += b->share_count[i];
    }
    
    printf("Number of companies: %d | Number of total shares: %d | Money: %lf | Money in market %lf\n", cc, sc, b->money, b->in_market);
}

void buy(Bot* b, int share, int count)
{
    if(b->money >= companies[share].share_price*count)
    {
        b->money -= companies[share].share_price*count;
        
        b->share_count[share] += count;
        //b->company_shares[]
    }
}

void sell(Bot* b, int share, int count)
{
    if(b->share_count[share] >= count)
    {
        b->share_count[share] -= count;
        b->money += companies[share].share_price*count;
    }
}

void init_bots()
{
    bots = malloc(sizeof(*bots)*BOT_COUNT);
    for(int i=0; i < BOT_COUNT; i++)
    {
        //bots[i].company_shares = malloc(sizeof(int)*100);
        bots[i].share_count = malloc(sizeof(int)*100);
        bots[i].strategy = rand()%1;
        bots[i].money = rand()%10000;
        bots[i].in_market = 0;
    }
}

void bot_update()
{
    for(int i=0; i < BOT_COUNT; i++)
    {
        switch(bots[i].strategy)
        {
            case 0: // RANDOM
            {
                int f = rand()%3;
                //printf("%d\n", f);
                if(f == 0)
                {
                    int share = rand()%COMPANY_COUNT;
                    int count = floor(bots[i].money/companies[share].share_price);
                    int val = 0;
                    if(count > 0)
                        val = rand()%count;
                        printf("BUYING %d at %lf for %lf\n", val, companies[share].share_price, companies[share].share_price*val);
                    buy(&(bots[i]), share, val);
                }
                else if(f == 1)
                {
                    //int share = rand()%COMPANY_COUNT;
                    int share = 0;
                    int cc = 0;
                    for(int j=0; j < COMPANY_COUNT; j++)
                    {
                        if(bots[i].share_count[j] > 0)
                            cc++;
                    }
                    int toSell = 0;
                    if(cc > 0)
                        toSell = rand()%cc;
                    /*do
                    {
                        if(bots[i].share_count[share] > 0)
                            break;
                        share++;
                    }while(share < COMPANY_COUNT);*/
                    int c = 0;
                    for(int j=0; j < COMPANY_COUNT; j++)
                    {
                        if(bots[i].share_count[j] > 0)
                        c++;
                        if(c == cc)
                        {
                            share = j;
                            break;
                        }
                    }
                    int count = bots[i].share_count[share];
                    int val = 0;
                    if(count > 0)
                        val = rand()%count;
                        printf("SELLING %d at %lf for %lf\n", val, companies[share].share_price, companies[share].share_price*val);
                    sell(&(bots[i]), share, val);
                }
                //else wait
            }
            break;
            case 1:
            {
                
            }
            break;
        }
        
        double p = 0;
        for(int j=0; j < COMPANY_COUNT; j++)
        {
            p += bots[i].share_count[j]*companies[j].share_price;
        }
        bots[i].in_market = p;
    }
}

void do_tick()
{
    stock_market_update();
    bot_update();
}

int main() {

    srand(time(NULL));
    
    
    init_stock_market();
    
    init_bots();
    int c = 0;
    print_bot(&(bots[0]));
    for(int i=0; i < 150; i++)
    
    //while(1)
    {
        do_tick();
    print_bot(&(bots[0]));
    sleep(1);
    c++;
    if(bots[0].money >= 20000)
    {
        printf("Only took %d\n", c);
        return 0;
    }
    }
    
    print_bot(&(bots[0]));
    
    
    //stock market bot
    //different bots
        //buy and sell as soon as you make money
        //buy and hold until 10% gain

    return 0;
}
