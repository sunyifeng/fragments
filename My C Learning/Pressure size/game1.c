#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

//函数声明
int rand10();							//产生10以内的随机数
int  dropDown();						//下注函数
void showEnding();						//输出开奖结果
void updateRecords();					//更新记录的开奖结果
void stories();							//显示投注记录
int running(int,int);					//跑起来 主循环
void endingUpdate(int);					//更新开奖结果并写入变量
int wizards();							//向导函数
int showTitle();						//显示软件欢迎信息菜单
int  choice();							//执行主菜单跳转
void viewStatus();						//显示当前状态：参数，金额等等
void pressKey();						//实现按任意键继续

//全局变量声明
int stepOne=0;							//每一局结束后是否暂停：0自动，1手动
int enjoys=0;							//当前期数
int totalCounts=10;						//总共玩多少期
int sleeps=5;							//开奖时间间隔秒：初始化0
int selfRand=0;							//当前开奖号码
double victoryRate=0;					//胜率百分比：初始化0
int totalVictorys=0;					//累胜局数：初始化0
int selfBSStatus=0;						//压大小状态：0初始化，1大，2小
int selfSDStatus=0;						//压单双状态：0初始化，1单，2双
int myBS=0;								//我压的大小：0初始化，1大，2小
int mySD=0;								//我压的单双：0初始化，1单，2双
int gameType=0;                         //游戏类型：0默认，1压大小，2压单双
int runType=0;                          //投注方式：0自动，1手动输入，2根据策略自动投注
double initMoney=5000;						//本金：初始0
double totalMoney=5000;					//总资金：初始0
double stepMoney=0;						//单局盈亏：初始0
double stepDrop=0;						//单局投注：初始0
double lastDrop=0;						//缓存上一局投注金额
double meanMoney=0;						//平均每局盈亏：初始0
double meanDrop=0;						//平均每局投注：初始0

//初始化字符串数组
char * strOne[]={"自动","手动"};
char * strBSStatus[]={"默认","大","小"};
char * strSDStatus[]={"默认","单","双"};
char * strgameType[]={"默认","压大小","压单双"};
char * strrunType[]={"自动","手动","使用内置策略投注"};

char progTitle[]="欢迎来到赌博现场，激动人心的时刻即将到来······";
char progName[]="程序名称：开大小、单双 计算程序";
char progVersion[]="软件版本：Beta 0.1";
char progBuild[]="版本日期：Buid 20190309";
char progAuthor[]="作者：一叶扁舟";
char progURL[]="https://www.mazhiyuan.com";
char progMail[]="mail：tuantuanyuanyuan@hotmail.com";
char strMenu[]="功    能    清    单\n--------------------\n输入：w\t\t--------------------运行向导，设置投注（必须）；\n输入：r\t\t--------------------开始运行程序；\n输入：v\t\t--------------------查看当前状态；\n输入：m\t\t--------------------显示本菜单；\n输入：s\t\t--------------------查看开奖历史；\n输入：h\t\t--------------------查看帮助；\n输入：q\t\t--------------------退出程序；\n请输入您的选项，并按下键盘上的Return键：";
char MSGHELP[]="没有更多帮助！\n";
char MSG001[]="\033[43;31;5m程序结束，用户退出！\033[0m\n";
char MSG002[]="正等待指令，输入\'h\'查看帮助：";
char MSG003[]="应用程序暂停，请按任意键继续！\n";
char MSG004[]="好戏即将开始了，告诉我你想怎么玩，然后我们就开始！\n";
char MSG005[]="告诉我你打算玩几局？输入一个整数：";
char MSG006[]="每隔多少秒开一次奖呢？：";
char MSG007[]="游戏选择：1:压大小，2:压单双，0:随意：";
char MSG008[]="策略选择：1，每一局自己下注  2，使用内置策略  0，随意：";
char MSG009[]="你的本金（元）：";
char MSG010[]="（自动投注时）每一局结束后是否暂停？1.暂停  0.自动  ：";
char MSG011[]="\nOK!   按 r 键开始吧！\n";
char MSG012[]="\n以下是您当前的游戏状态信息：\n";
char MSG013[]=" 第 ";
char MSG014[]=" 期 ";
char MSG015[]="即将开奖···\n";
char MSG016[]="开奖号码为：";
char MSG017[]="大小：";
char MSG018[]="单双：";
char MSG019[]="\n系统状态：";
char MSG020[]="\n对局状态：";
char MSG021[]="\n财务状态：";
char MSG022[]="\n开奖时间间隔（秒）：";
char MSG023[]="\n游戏类型：";
char MSG024[]="\n投注方式：";
char MSG025[]="\n开始下一局：";
char MSG026[]="\n当前期数：";
char MSG027[]="\n总期数：";
char MSG028[]="\n总胜率（百分比）：";
char MSG029[]="\n累计获胜（局）：";
char MSG030[]="\n期初资金（元）：";
char MSG031[]="\n累计盈亏（元）：";
char MSG032[]="\n单局盈亏（平均）：";
char MSG033[]="\n单局投注（平均）：";
char MSG034[]="\n请输入下注金额（元）支持小数：";
char MSG035[]="\n输入错误，请输入正确的金额!";
char MSG036[]="\n恭喜，下注成功！\n";
char MSG037[]="\n当前余额：";
char MSG038[]=" 元 ";
char MSG039[]="\n请耐心等待开奖···\n";
char MSG040[]="\n压大还是压小？1大，2小\n";
char MSG041[]="\n压单还是压双？1单，2双\n";
char MSG042[]="\n恭喜您，中奖了！\n";
char MSG043[]="\n很遗憾，再接再厉哦！\n";
char MSG044[]="\n本局获利（元）：";
char MSG045[]="";
//定义错误信息
char ERRMSG1[]="出错了！\n";
char ERRMSG2[]="错误代码：";
char ERRMSG3[]="错误原因：";
char ERRMSG4[]="描述信息：";
//ERR1-100:程序执行错误
char ERR001[]="\n程序执行错误！\n";
char ERR002[]="\n输入错误，重新输入！\n";
//ERR100-199:资金出错
char ERR100[]="资金出错！";
char ERR101[]="\n提示：你没有那么多钱！";

int main(int argc,char *arg[],char *envp[]){
	while(1){
		showTitle();
		if(choice()==1){
			printf("%s",ERR002);
			sleep(5);
			continue;
		}
	}
	return 0;
}

int choice(){
	char strCMD[256];
	while(1){
		scanf("%c",strCMD);
		if(!strcmp(strCMD,"w")){
			if(wizards()!=0){
				break;
			}
		}else if(!strcmp(strCMD,"r")){
			running(sleeps,totalCounts);
		}else if(!strcmp(strCMD,"s")){
			stories();
		}else if(!strcmp(strCMD,"v")){
			viewStatus();
		}else if(!strcmp(strCMD,"m")){
			showTitle();
		}else if(!strcmp(strCMD,"h")){
			printf("%s",MSGHELP);
		}else if(!strcmp(strCMD,"q")){
			printf("%s",MSG001);
			exit(1);
		}else{
			printf("%s",MSG002);
		}
	}
	return 0;
}

int wizards(){
	printf("%s",MSG004);
	printf("%s",MSG005);
	scanf("%d",&totalCounts);
	printf("%s",MSG006);
	scanf("%d",&sleeps);
	printf("%s",MSG007);
	scanf("%d",&gameType);
	printf("%s",MSG008);
	scanf("%d",&runType);
	printf("%s",MSG009);
	scanf("%lf",&initMoney);
	printf("%s",MSG010);
	scanf("%d",&stepOne);
	printf("%s",MSG011);

	//同步资金
	totalMoney=initMoney;

	return 0;
}
int showTitle(){
    system("clear");
	printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s","\033[40;33;5m",progTitle,"\n",progName,"\n",progVersion,"\n",progBuild,"\n",progAuthor,"\n",progMail,"\n",progURL,"\033[0m\n");
	printf("%s",strMenu);
	return 0;
}

int running(int isleep , int icounts){
	int i;
	enjoys=enjoys==0?1:enjoys;
	for (i=1;i<=icounts;i++){
			printf("\n%s%s%d%s%s%s","\033[;35;1m",MSG013,enjoys,MSG014,"\033[0m",MSG015);
			//下注
			if(dropDown()!=0){
				printf("%s",ERR001);
				break;
			}

			sleep(isleep);
			endingUpdate(rand10());
			showEnding();
			if (stepOne==1){
				pressKey();
			}
			enjoys++;
		}
	return 0;
}

int rand10(){
	int basenums[10]={5,6,4,7,3,8,2,9,1,0};
	unsigned long int i,j,k,l;
	srand(time(NULL));
	i=rand();
	j=time(NULL);
	k=time(NULL)/3600;
	l=time(NULL)/3.1415926;
	srand((i+j+k+l));
	return (rand()%10+1);
}

void endingUpdate(int x){
	selfRand=x;
	selfBSStatus=selfRand>5?1:2;
	selfSDStatus=(selfRand%2)==0?2:1;

	//结算
	stepMoney=0;
	if(selfBSStatus==myBS){
		double winMoney;
		winMoney=stepDrop*2-(stepDrop*0.02);     //手续费
		totalMoney=totalMoney+winMoney;
		stepMoney=winMoney-stepDrop;
		totalVictorys++;
	}else{
		stepMoney=stepMoney-stepDrop;
	}
	victoryRate=(totalVictorys/(double)enjoys)*100;
}

void showEnding(){
	printf("%s%s%d%s%s","\033[;35;1m",MSG013,enjoys,MSG014,"\033[0m");
	printf("%s%s%d%s",MSG016,"\033[;31;1m ",selfRand,"  \033[0m ");
	if (gameType<=1){
		printf("%s%s%s%s%s",MSG017,MSG018,"\033[;31;1m",strBSStatus[selfBSStatus],"\033[0m  ");
	}
	if(gameType!=1){
		printf("%s%s%s%s%s",MSG017,MSG018,"\033[;31;1m",strSDStatus[selfSDStatus],"\033[0m");	
	}
	printf("\n\n");

	if(stepMoney<0){
		printf("%s",MSG043);
	}else{
		printf("%s",MSG042);
	}
	printf("%s%f%s%f%s%d%s%f",MSG037,totalMoney,MSG044,stepMoney,MSG029,totalVictorys,MSG028,victoryRate);
}

int dropDown(){
	printf("%s",MSG034);
	scanf("%lf",&stepDrop);
	printf("%s",MSG040);
	scanf("%d",&myBS);
	//printf("%s",MSG041);
	//scanf("%d",&mySD);
	if(stepDrop<=0){
		printf("%s",MSG035);
		return 1;	
	}else if(stepDrop>totalMoney){
		printf("%s",ERR101);
		return 1;
	}else if(stepDrop<=totalMoney){
		totalMoney=totalMoney-stepDrop;
		printf("%s%s%f%s%s",MSG036,MSG037,totalMoney,MSG038,MSG039);

	}
	return 0;
}

void stories(){
	;
}

void viewStatus(){
	printf("%s",MSG012);
	printf("%s",MSG019);
	printf("%s%d",MSG022,sleeps);
	printf("%s%s",MSG023,strgameType[gameType]);
	printf("%s%s",MSG024,strrunType[runType]);
	printf("%s%s",MSG025,strOne[stepOne]);

	printf("%s",MSG020);
	printf("%s%d",MSG026,enjoys);
	printf("%s%d",MSG027,totalCounts);
	printf("%s%f",MSG028,victoryRate);
	printf("%s%d",MSG029,totalVictorys);

	printf("%s",MSG021);
	printf("%s%f",MSG030,initMoney);
	printf("%s%f",MSG031,(totalMoney-initMoney));
	printf("%s%f",MSG032,meanMoney);
	printf("%s%f",MSG033,meanDrop);
	printf("\n");
}

void pressKey(){
	printf("%s",MSG003);
	getchar();
}
