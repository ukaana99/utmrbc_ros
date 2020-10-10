
#ifndef STATE_H_
#define STATE_H_
#ifdef __cplusplus
extern "C" {
#endif

/*********************************************/
/*          Include Header                   */
/*********************************************/

#define USED_CAN1
/*********************************************/

/*********************************************/
/*          Define                           */
/*********************************************/

#define INVALID 	0

/*********************************************/

/*********************************************/
/*          Enumarator                       */
/*********************************************/
enum {
	RNS_S_PENDING = 0,
	RNS_S_VELOCITY,
	RNS_S_LINE_FOLLOW,
	RNS_S_PATH_PLAN,
	RNS_S_USER
};

enum {
	RNS_E_STOP = 0,
	RNS_E_VELOCITY,
	RNS_E_LINE_FOLLOW,
	RNS_E_PATH_PLAN,
	RNS_E_USER,
	RNS_E_PARAM,
	RNS_E_ENQ,
	RNS_E_FINISH
};
/*********************************************/

/*********************************************/
/*          Variable                         */
/*********************************************/

/*********************************************/

/*********************************************/
/*           Function Prototype              */
/*********************************************/
void STTStateInit(void);
void STTEventChecker(void);
void STTStop(void);

//Pending State
void STTPendingStateVelocity(void);
void STTPendingStateLineFollow(void);
void STTPendingStatePathPlan(void);
void STTPendingStateUser(void);
void STTPendingStateParam(void);
void STTPendingStateEnq(void);

//Velocity State
void STTVelocityStateVelocity(void);
void STTVelocityStateLineFollow(void);
void STTVelocityStatePathPlan(void);
void STTVelocityStateUser(void);
void STTVelocityStateParam(void);
void STTVelocityStateEnq(void);

//Line Follow State
void STTLineFollowStateParam(void);
void STTLineFollowStateEnq(void);

//Path Plan State
void STTPathPlanStateStop(void);
void STTPathPlanStateParam(void);
void STTPathPlanStateEnq(void);

//User State
void STTUserStateStop(void);
void STTUserStateParam(void);
void STTUserStateEnq(void);
void STTUserStateFinish(void);

/*********************************************/


#ifdef __cplusplus
}
#endif
#endif /* STATE_H_ */
