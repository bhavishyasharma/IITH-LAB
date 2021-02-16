/* External definitions for single-server queueing system. */

#include <stdio.h>
#include <stdlib.h>    
#include <math.h>
#include "lcgrand.h"  /* Header file for random-number generator. */

#define Q_LIMIT 1000000000  /* Limit on queue length. */
#define BUSY      1  /* Mnemonics for server's being busy */
#define IDLE      0  /* and idle. */

long   next_event_type, num_custs_delayed[2], num_delays_required, num_events,
      num_in_q[2], server_status[2];
int seedOffset = 0;
float area_num_in_q[2], area_server_status[2], mean_interarrival, mean_service,
      sim_time, time_arrival[Q_LIMIT + 1][2], time_last_event, time_next_event[4],
      total_of_delays[2];
//FILE  *infile, *outfile;

void simulation(void);
void  initialize(void);
void  timing(void);
void  arrive(void);
void  departQ1(void);
void  departQ2(void);
void  report(void);
void  update_time_avg_stats(void);
float expon(float mean, int seed);

int main(int argc, char **argv){
    long packetSize = atol(argv[1]);
    float linkSpeed = atof(argv[2]);
    num_delays_required = atol(argv[3]);
    seedOffset = atoi(argv[4]);


    /*mean_service = float(packetSize)/linkSpeed;
    mean_interarrival = mean_service;
    float arrival_rate = 1.0 / mean_interarrival;
    float increment = arrival_rate / 100;
    printf("InterArrival\tAvg. Delay(1)\tAvg. Delay(2)\t\tTotal Avg. Delay\tSim Time\n");
    //printf("Mean Service : %10.6f\nMean Arrival : %10.6f\n Increment : %10.6f\n", mean_service, mean_interarrival, increment);
    while(mean_interarrival > 0){
        simulation();
        arrival_rate -= increment;
        //mean_interarrival += increment;
        mean_interarrival = 1.0 / arrival_rate;
        //printf("Mean Service : %10.6f\nMean Arrival : %10.6f\n Increment : %10.6f\n", 1.0 / mean_service, arrival_rate, increment);
    }*/

    mean_service = float(packetSize)/linkSpeed;
    mean_interarrival = mean_service * 100;
    float increment = mean_service;
    printf("InterArrival\tAvg. Delay(1)\tAvg. Delay(2)\t\tTotal Avg. Delay\tSim Time\n");
    //printf("Mean Service : %10.6f\nMean Arrival : %10.6f\n Increment : %10.6f\n", mean_service, mean_interarrival, increment);
    while(mean_interarrival > 3){
        simulation();
        mean_interarrival -= increment;
        //printf("Mean Service : %10.6f\nMean Arrival : %10.6f\n Increment : %10.6f\n", 1.0 / mean_service, arrival_rate, increment);
    }
}

void simulation(void)  /* Main function. */
{
    /* Open input and output files. */

    //infile  = fopen("mm1.in",  "r");
    //outfile = fopen("mm1.out", "w");

    /* Specify the number of events for the timing function. */

    num_events = 3;

    /* Read input parameters. */

    //fscanf(infile, "%f %f %d", &mean_interarrival, &mean_service,
    //       &num_delays_required);

    /* Write report heading and input parameters. */

    //fprintf(outfile, "Single-server queueing system\n\n");
    //fprintf(outfile, "Mean interarrival time%11.3f minutes\n\n",
    //        mean_interarrival);
    //fprintf(outfile, "Mean service time%16.3f minutes\n\n", mean_service);
    //fprintf(outfile, "Number of customers%14d\n\n", num_delays_required);

    /* Initialize the simulation. */

    initialize();

    /* Run the simulation while more delays are still needed. */

    while (num_custs_delayed[0] < num_delays_required)     
    {
        /* Determine the next event. */

        timing();

        /* Update time-average statistical accumulators. */

        update_time_avg_stats();

        /* Invoke the appropriate event function. */

        switch (next_event_type) 
        {
            case 1:
                arrive();
                break;
            case 2:
                departQ1();
                break;
            case 3:
                departQ2();
                break;
        }
    }

    /* Invoke the report generator and end the simulation. */

    report();

    //fclose(infile);
    //fclose(outfile);

    return ;
}


void initialize(void)  /* Initialization function. */
{
    /* Initialize the simulation clock. */

    sim_time = 0.0;

    /* Initialize the state variables. */

    server_status[0] = server_status[1] = IDLE;
    num_in_q[0] = num_in_q[1] = 0;
    time_last_event = 0.0;

    /* Initialize the statistical counters. */

    num_custs_delayed[0] = num_custs_delayed[1] = 0;
    total_of_delays[0] = total_of_delays[1] = 0.0;
    area_num_in_q[0] = area_num_in_q[1] = 0.0;
    area_server_status[0] = area_server_status[1] = 0.0;

    /* Initialize event list.  Since no customers are present, the departure
       (service completion) event is eliminated from consideration. */

    time_next_event[1] = sim_time + expon(mean_interarrival, 1);
    time_next_event[2] = 1.0e+30;
    time_next_event[3] = 1.0e+30;
}


void timing(void)  /* Timing function. */
{
    int   i;
    float min_time_next_event = 1.0e+29;

    next_event_type = 0;

    /* Determine the event type of the next event to occur. */

    for (i = 1; i <= num_events; ++i)
        if (time_next_event[i] < min_time_next_event)
        {   
            min_time_next_event = time_next_event[i];
            next_event_type     = i;
        }

    /* Check to see whether the event list is empty. */

    if (next_event_type == 0)
    {
        /* The event list is empty, so stop the simulation. */

        printf("\nEvent list empty at time %f", sim_time);
        exit(1);
    }

    /* The event list is not empty, so advance the simulation clock. */

    sim_time = min_time_next_event;
}


void arrive(void)  /* Arrival event function. */
{
    float delay;

    /* Schedule next arrival. */

    time_next_event[1] = sim_time + expon(mean_interarrival, 1);

    /* Check to see whether server is busy. */

    if (server_status[0] == BUSY)
    {
        /* Server is busy, so increment number of customers in queue. */

        ++num_in_q[0];

        /* Check to see whether an overflow condition exists. */

        if (num_in_q[0] > Q_LIMIT)
        {
            /* The queue has overflowed, so stop the simulation. */

            printf("\nOverflow of the array time_arrival at");
            printf(" time %f", sim_time);
            exit(2);
        }

        /* There is still room in the queue, so store the time of arrival of the
           arriving customer at the (new) end of time_arrival. */

        time_arrival[num_in_q[0]][0] = sim_time;
    }

    else
    {
        /* Server is idle, so arriving customer has a delay of zero.  (The
           following two statements are for program clarity and do not affect
           the results of the simulation.) */

        delay            = 0.0;
        total_of_delays[0] += delay;

        /* Increment the number of customers delayed, and make server busy. */

        ++num_custs_delayed[0];
        server_status[0] = BUSY;

        /* Schedule a departure (service completion). */

        time_next_event[2] = sim_time + expon(mean_service, 1);
    }
}

void departQ1(void)  /* Departure event function. */
{
    int   i;
    float delay;

    /* Check to see whether the queue is empty. */

    if (num_in_q[0] == 0)
    {
        /* The queue is empty so make the server idle and eliminate the
           departure (service completion) event from consideration. */

        server_status[0]      = IDLE;
        time_next_event[2] = 1.0e+30;
    }

    else
    {
        /* The queue is nonempty, so decrement the number of customers in
           queue. */

        --num_in_q[0];

        /* Compute the delay of the customer who is beginning service and update
           the total delay accumulator. */

        delay            = sim_time - time_arrival[1][0];
        total_of_delays[0] += delay;

        /* Increment the number of customers delayed, and schedule departure. */

        ++num_custs_delayed[0];
        time_next_event[2] = sim_time + expon(mean_service, 1);

        /* Move each customer in queue (if any) up one place. */

        for (i = 1; i <= num_in_q[0]; ++i)
            time_arrival[i][0] = time_arrival[i + 1][0];
    }
    if (server_status[1] == BUSY)
    {
        /* Server is busy, so increment number of customers in queue. */

        ++num_in_q[1];

        /* Check to see whether an overflow condition exists. */

        if (num_in_q[1] > Q_LIMIT)
        {
            /* The queue has overflowed, so stop the simulation. */

            printf("\nOverflow of the array time_arrival at");
            printf(" time %f", sim_time);
            exit(2);
        }

        /* There is still room in the queue, so store the time of arrival of the
           arriving customer at the (new) end of time_arrival. */

        time_arrival[num_in_q[1]][1] = sim_time;
    }

    else
    {
        /* Server is idle, so arriving customer has a delay of zero.  (The
           following two statements are for program clarity and do not affect
           the results of the simulation.) */

        delay = 0.0;
        total_of_delays[1] += delay;

        /* Increment the number of customers delayed, and make server busy. */

        ++num_custs_delayed[1];
        server_status[1] = BUSY;

        /* Schedule a departure (service completion). */

        time_next_event[3] = sim_time + expon(mean_service, 3);
    }
}

void departQ2(void)  /* Departure event function. */
{
    int   i;
    float delay;

    /* Check to see whether the queue is empty. */

    if (num_in_q[1] == 0)
    {
        /* The queue is empty so make the server idle and eliminate the
           departure (service completion) event from consideration. */

        server_status[1]      = IDLE;
        time_next_event[3] = 1.0e+30;
    }

    else
    {
        /* The queue is nonempty, so decrement the number of customers in
           queue. */

        --num_in_q[1];

        /* Compute the delay of the customer who is beginning service and update
           the total delay accumulator. */

        delay            = sim_time - time_arrival[1][1];
        total_of_delays[1] += delay;

        /* Increment the number of customers delayed, and schedule departure. */

        ++num_custs_delayed[1];
        time_next_event[3] = sim_time + expon(mean_service, 3);

        /* Move each customer in queue (if any) up one place. */

        for (i = 1; i <= num_in_q[1]; ++i)
            time_arrival[i][1] = time_arrival[i + 1][1];
    }
}


void report(void)  /* Report generator function. */
{
    /* Compute and write estimates of desired measures of performance. */

    /* fprintf(outfile, "\n\nAverage delay in queue(1)%11.3f minutes\n\n",
            total_of_delays[0] / num_custs_delayed[0]);
    fprintf(outfile, "Average number in queue(1)%10.3f\n\n",
            area_num_in_q[0] / sim_time);
    fprintf(outfile, "Server(1) utilization%15.3f\n\n",
            area_server_status[0] / sim_time);
    fprintf(outfile, "\n\nAverage delay in queue(2)%11.3f minutes\n\n",
            total_of_delays[1] / num_custs_delayed[1]);
    fprintf(outfile, "Average number in queue(2)%10.3f\n\n",
            area_num_in_q[1] / sim_time);
    fprintf(outfile, "Server(2) utilization%15.3f\n\n",
            area_server_status[1] / sim_time);
    fprintf(outfile, "Time simulation ended%12.3f minutes", sim_time); */
    printf("%10.6f\t%10.6f\t%10.6f\t%10.6f\t%10.6f\n", mean_interarrival, total_of_delays[0]/num_custs_delayed[0], total_of_delays[1]/num_custs_delayed[1], (total_of_delays[0]/num_custs_delayed[0])+(total_of_delays[1]/num_custs_delayed[1]), sim_time);
}


void update_time_avg_stats(void)  /* Update area accumulators for time-average
                                     statistics. */
{
    float time_since_last_event;

    /* Compute time since last event, and update last-event-time marker. */

    time_since_last_event = sim_time - time_last_event;
    time_last_event       = sim_time;

    /* Update area under number-in-queue function. */

    area_num_in_q[0]      += num_in_q[0] * time_since_last_event;
    area_num_in_q[1]      += num_in_q[1] * time_since_last_event;

    /* Update area under server-busy indicator function. */

    area_server_status[0] += server_status[0] * time_since_last_event;
    area_server_status[1] += server_status[1] * time_since_last_event;
}


float expon(float mean, int seed)  /* Exponential variate generation function. */
{
    /* Return an exponential random variate with mean "mean". */

    return -mean * log(lcgrand(seed+seedOffset));
}

