#ifndef CONST_DEFINITION
#define CONST_DEFINITION


static bool DEBUG = true;

/*MATRIX AREA DEFINITION*/

#define VIDE_DEF                0
#define CAR_DEF                 3
#define BLOC_DEF                1
#define CHEMIN_DEF              4
#define ARRIVE_DEF              2

/*AREA DEFINITION*/

#define CAM_EXTERN              1                   /*0 = default CAM, 1 = extern CAM*/
#define CAM_WIDTH               640                 
#define CAM_HEIGHT              480
#define MAX_BLOC_SCENE          30                  /*Maximum block on the area*/

#define MARGE_BLOCK_PROCESSING  30
#define MARKER_ID_CAR           1
#define MARKER_ID_ARRIVAL       0
#define MARKER_ID_AREA          12


/*SERVER DEFINITION*/

#define MAX_CONNECTION_CLIENT   5
#define PORT_SERVEUR            975

#define ID_PC                   "bFd6-a2e8-9Dff\n"          /*Module connect with code*/
#define ID_CAR                  "59ef-6F70-4bM7\n"
#define ID_REMOTE               "c3Z8-4b86-95Ax\n"          
#define ID_TABLET               "E34x-8Jkp-77Go\n"
#define ID_CONNECTION           "xxxx-xxxx-xxxx\n"

#define INTER_PROC              "<PROC_ESTABLISHED>"
#define REQ_CODE                "<ID-CODE>"
#define VAL_CODE                "<ID-VALID>"
#define INVAL_CODE              "<ID-INVALID>"
#define STOP_CONN               "<STOP>"
#define WAIT_CONN               "<WAIT>"

#endif