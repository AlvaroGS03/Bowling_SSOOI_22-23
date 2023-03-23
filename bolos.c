#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/mman.h>

// if (sigaction(SIGTERM, 1, 1) == 0) {return pid[]} //devuelve pid del proceso que contiene el sigaction

int comportamiento();
void tryError(pid_t);
// int sigaction(int, const struct sigaction *, struct sigaction *);
pid_t waitpid(pid_t pid, int *stat_loc, int options);
void imprimirBolos(pid_t *pid);
void catcher(int sig);
void comprobarKill(int try);
void matar();

int main(int argc, char *argv[])
{
    
    struct sigaction action;
    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIGTERM);
    action.sa_flags = 0;
    action.sa_handler = &catcher;
    action.sa_mask = mask;
    sigaction(SIGTERM, &action, NULL);
    
    int temp;
    int a, b, c, d, e, f;

    int *pid = mmap(NULL, 40, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (pid == MAP_FAILED)
    {
        printf("Error al crear el mapa de memoria\n");
        return -1;
    }

    memset(argv[0], 0, strlen(argv[0]));

    if (temp = fork() == 0)
    {
        pid[0] = getpid();
        tryError(pid[0]);
        argv[0][0] = 65; // A
        a = comportamiento();

        printf("Soy A mi PID es %d y el de mi padre es %d\n", getpid(), getppid());

        if (temp = fork() == 0)
        {
            pid[1] = getpid();
            tryError(pid[1]);
            argv[0][0] = 72; // H
            printf("Soy H, mi PID es %d y el de mi padre es %d\n", getpid(), getppid());

            sigsuspend(&mask);
            printf("Pid proceso que accede a la mascara: %d\n", getpid());
            return getpid();
        }
        else if (temp = fork() == 0)
        {
            pid[2] = getpid();
            tryError(pid[2]);
            argv[0][0] = 73; // I
            printf("Soy I, mi PID es %d y el de mi padre es %d\n", getpid(), getppid());

            sigsuspend(&mask);
            printf("Pid proceso que accede a la mascara:: %d\n", getpid());
            return getpid();
        }

        else if (temp = fork() == 0)
        {
            pid[3] = getpid();
            tryError(pid[3]);
            argv[0][0] = 69; // E
            e = comportamiento();
            printf("Soy E, mi PID es %d y el de mi padre es %d\n", getpid(), getppid());
            //waitpid e
            sigsuspend(&mask);
            printf("Pid proceso que accede a la mascara:: %d\n", getpid());
            if (e == 1)
            {
                comprobarKill(kill(pid[2], SIGTERM)); 
                waitpid(pid[2], NULL, 0);            
                pid[2] = -99;                         
            }
            else if (e == 2)
            {
                comprobarKill(kill(pid[1], SIGTERM)); 
                waitpid(pid[1], NULL, 0);
                pid[1] = -99;
            }
            else if (e == 3)
            {
                printf("Pid proceso que accede a la mascara: i: %d\n", pid[2]);
                printf("Pid proceso que accede a la mascara: h: %d\n", pid[1]);
                comprobarKill(kill(pid[2], SIGTERM));
                waitpid(pid[2], NULL, 0);
                comprobarKill(kill(pid[1], SIGTERM));
                waitpid(pid[1], NULL, 0);
                pid[1] = -99;
                pid[2] = -99;
            }
            return getpid();
        }
        else if (temp = fork() == 0)
        {
            pid[4] = getpid();
            tryError(pid[4]);
            sigaction(SIGTERM, &action, NULL);
            argv[0][0] = 66; // B
            b = comportamiento();
            printf("Soy B, mi PID es %d y el de mi padre es %d\n", getpid(), getppid());

            if (temp = fork() == 0)
            {
                pid[7] = getpid();
                tryError(pid[7]);
                argv[0][0] = 68; // D
                d = comportamiento();
                printf("Soy G, mi PID es %d y el de mi padre es %d\n", getpid(), getppid());

                if (temp = fork() == 0)
                {
                    pid[6] = getpid();
                    tryError(pid[6]);
                    argv[0][0] = 71; // G
                    printf("\nSoy D, mi PID es %d y el de mi padre es %d\n", getpid(), getppid());
                    sigsuspend(&mask);
                    printf("Pid proceso que accede a la mascara: %d\n", getpid());
                    return getpid();
                }
                // waitpid de d
                sigsuspend(&mask);
                printf("pid: %d\n", getpid());
                if (pid[7] == getpid())
                {

                    printf("Pid proceso que accede a la mascara: %d\n", getpid());

                    if (d == 1)
                    {
                        comprobarKill(kill(pid[1], SIGTERM)); 
                        waitpid(pid[1], NULL, 0);
                        pid[1] = -99;
                    }
                    else if (d == 2)
                    {
                        comprobarKill(kill(pid[6], SIGTERM)); 
                        waitpid(pid[6], NULL, 0);
                        pid[6] = -99;
                    }
                    else if (d == 3)
                    {
                        comprobarKill(kill(pid[1], SIGTERM)); 
                        waitpid(pid[1], NULL, 0);
                        comprobarKill(kill(pid[6], SIGTERM));
                        waitpid(pid[6], NULL, 0);
                        pid[1] = -99;
                        pid[6] = -99;
                    }
                }
                return getpid();
            }
            sigsuspend(&mask);
            printf("Pid proceso que accede a la mascara: %d\n", getpid());
            
            if (b == 1)
            {
                comprobarKill(kill(pid[3], SIGTERM));
                waitpid(pid[3], NULL, 0);
                pid[3] = -99;
            }
            else if (b == 2)
            {
                comprobarKill(kill(pid[7], SIGTERM));
                waitpid(pid[7], NULL, 0);
                pid[7] = -99;
            }
            else if (b == 3)
            {
                comprobarKill(kill(pid[3], SIGTERM)); 
                waitpid(pid[3], NULL, 0);
                comprobarKill(kill(pid[7], SIGTERM));
                waitpid(pid[7], NULL, 0);
                pid[3] = -99;
                pid[7] = -99;
            }
            return getpid();
        }
        else if (temp = fork() == 0)
        {
            pid[5] = getpid();
            tryError(pid[5]);
            argv[0][0] = 67; // C
            c = comportamiento();
            printf("Soy C, mi PID es %d y el de mi padre es %d\n", getpid(), getppid());

            if (temp = fork() == 0)
            {
                pid[9] = getpid();
                tryError(pid[9]);

                argv[0][0] = 70; // F
                f = comportamiento();
                printf("Soy J, mi PID es %d y el de mi padre es %d\n", getpid(), getppid());

                if (temp = fork() == 0)
                {
                    pid[8] = getpid();
                    tryError(pid[8]);

                    argv[0][0] = 74; // J
                    printf("Soy F, mi PID es %d y el de mi padre es %d\n", getpid(), getppid());
                    sigsuspend(&mask);
                    return getpid();
                }
                // waitpid de f
                sigsuspend(&mask);

                if (f == 1)
                {
                    comprobarKill(kill(pid[8], SIGTERM)); 
                    waitpid(pid[8], NULL, 0);
                    pid[8] = -99;
                }
                else if (f == 2)
                {
                    comprobarKill(kill(pid[2], SIGTERM));
                    waitpid(pid[2], NULL, 0);
                    pid[2] = -99;
                }
                else if (f == 3)
                {
                    comprobarKill(kill(pid[8], SIGTERM));
                    waitpid(pid[8], NULL, 0);
                    comprobarKill(kill(pid[2], SIGTERM));
                    waitpid(pid[2], NULL, 0);
                    pid[8] = -99;
                    pid[2] = -99;
                }
                return getpid();
            }
            sigsuspend(&mask);
            //  waitpid de c
            printf("Pid proceso que accede a la mascara: %d\n", getpid());
            if (pid[5] == getpid())
            {
                if (c == 1)
                {
                    comprobarKill(kill(pid[9], SIGTERM)); 
                    waitpid(pid[9], NULL, 0);
                    pid[9] = -99;
                }
                else if (c == 2)
                {
                    comprobarKill(kill(pid[3], SIGTERM)); 
                    waitpid(pid[3], NULL, 0);
                    pid[3] = -99;
                }
                else if (c == 3)
                {
                    comprobarKill(kill(pid[9], SIGTERM));
                    waitpid(pid[9], NULL, 0);
                    comprobarKill(kill(pid[3], SIGTERM));
                    waitpid(pid[3], NULL, 0);
                    pid[9] = -99;
                    pid[3] = -99;
                }
            }
            return getpid();
        }
    }
    else
    {
        argv[0][0] = 80; // P
        printf("\nSoy P, mi PID es %d y el de mi padre es %d\n", getpid(), getppid());

        exit(EXIT_SUCCESS);
    }
    

    fflush(stdin);
    sigsuspend(&mask);
    sleep(4);

    //  waitpid a
    printf("Pid proceso que accede a la mascara: %d\n", getpid());

    if (getpid() == pid[0])
    {
        if (a == 1)
        {
            comprobarKill(kill(pid[5], SIGTERM));
            waitpid(pid[5], NULL, 0);
            pid[5] = -99;
        }
        else if (a == 2)
        {
            comprobarKill(kill(pid[4], SIGTERM));
            waitpid(pid[4], NULL, 0);
            pid[4] = -99;
        }
        else if (a == 3)
        {
            comprobarKill(kill(pid[5], SIGTERM));
            waitpid(pid[5], NULL, 0);
            comprobarKill(kill(pid[4], SIGTERM));
            waitpid(pid[4], NULL, 0);
            pid[5] = -99;
            pid[4] = -99;
        }
        imprimirBolos(pid);
        fflush(stdin);
        getchar();
        matar();
        fflush(stdin);
        return 0;
    }
}

void tryError(pid_t pid)
{
    if (pid == -1)
    {
        perror("Error");
        exit(-1);
    }
}

int comportamiento()
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL); 

    // variable milisegundos => current_time.tv_usec
    
    return current_time.tv_usec % 4;
}

void imprimirBolos(pid_t *pid)
{
    int i = 0;
    char array[10] = {"A"   // 0
                      "H"   // 1
                      "I"   // 2
                      "E"   // 3
                      "B"   // 4
                      "C"   // 5
                      "G"   // 6
                      "D"   // 7
                      "J"   // 8
                      "F"}; // 9
    printf("\n");
    printf("\n");
    printf("Procesos Vivos:\n");
    for (i = 0; i < 10; i++)
    {
        if (pid[i] != -99)
        {
            printf(" %c ", array[i]);
        }
        if (pid[i] == -99)
        {
            printf("   ");
        }
    }

}

void comprobarKill(int ret)
{

    if (ret == -1)
    {
        perror("kill");
        exit(EXIT_FAILURE);
    }
}

void catcher(int sig)
{
    write(1, "Soy la mascara\n", 15);
}

void matar(){
        //system("killall -9 bolosv6");
        kill(0, SIGKILL);
}