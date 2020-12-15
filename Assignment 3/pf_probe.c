#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/time.h>
#include <linux/math64.h>
#define MAX_SYMBOL_LEN	64
int i;
int j=0;
char strings[70];
int temp[30][70];
static char symbol[MAX_SYMBOL_LEN] = "handle_mm_fault";
module_param_string(symbol, symbol, sizeof(symbol), 0644);
unsigned long addmin=0;
unsigned long addmax=0;
long timemax=0;
long timemin=0;
static int tpid;
static int choice;
ktime_t currenttime;
s64 time_ns;          //Current time
module_param(tpid,int,S_IRUGO);   //Input pid
module_param(choice,int,S_IRUGO);   //Choice for pf_probe_A or pf_probe_B
// To store address and time
struct myprobe{
  unsigned long address;
  long long signed time;

};
//Set limit to 500
static struct myprobe probes[500];
static struct kprobe kp = {
	.symbol_name	= symbol,
};

int func(void){ 
  int y;
  int a,b;
  // For scatter plot , a matrix of 30X70 is needed
  long unsigned addarray[31];   //For comparing the Y axis values
  long unsigned timearray[71];  //For comparing the X axis values
  int p=0,q=0;
  u64 y_step;                     //Get bins for x and y
  u64 x_step;
  memset(temp,0,sizeof(temp));        // 2D array for storing values where '*' should exist
  memset(addarray,0,sizeof(addarray));    //Address array
  memset(timearray,0,sizeof(timearray));  //Time array
  //Calculations for binning and plotting the scatter plot 
  timemin = probes[0].time;
  timemax = probes[i-1].time;
  addmax = addmax>>12;
  addmin = addmin>>12;
  y_step= addmax - addmin;
  y_step = div_u64(y_step,30);
  x_step = timemax - timemin;
  x_step = div_u64(x_step,70);
  for(y=0;y<i;y++){
    //Scaling
    probes[y].address = probes[y].address - addmin;
    probes[y].time = probes[y].time - timemin;
    }
  addmax = addmax - addmin;
  addmin = addmin - addmin;
  timemax = timemax - timemin;
  timemin = timemin - timemin;

  for(y=0;y<31;y++){
    //Input the values for y-axis
    addarray[y] = y_step*y;
  }
  for(y=0;y<71;y++){
    //Input the values for x axis 
    timearray[y] = x_step*y;
  }
  for(y=0;y<i;y++){
    for(a=1;a<31;a++){
      //Get x-axis value
      if(probes[y].address <= addarray[a] && probes[y].address > addarray[a-1]){
        p=a;
      }
    }
      //Get y-axis value
    for(b=1;b<71;b++){
      if(probes[y].time <= timearray[b] && probes[y].time > timearray[b-1]){
        q=b;
      }
    }
    //Store in 2D array that was created earlier
    temp[p][q] = 1;
  }
  printk("\n\n\n\n");
  printk("\t 'Scatter Plot:' ");
  printk("   \t_________________________________________________________________________");
  //Display the Scatter plot:
  for(a=30;a>=0;a--){
    for(b=70;b>=0;b--)
     {  
        if(temp[a][b]==1){
       strings[b] = '*';
       }
       else{
         strings[b] = ' ';
       }
   }
   printk("%d \t|%s|",a,strings);
   memset(strings,0,sizeof(strings));
}
printk("   \t\\_______________________________________________________________________/");
printk("\n\t\t  'X axis-> time vs Y-axis-> Virtual Page Number for PID : %d' \n\n\n",tpid);

  return 0;
}


/* kprobe pre_handler: called just before the probed instruction is executed */
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{///Referenced from :https://www.kernel.org/doc/html/latest/core-api/timekeeping.html 
currenttime = ktime_get();
time_ns = ktime_to_ns(currenttime);    //Convert to nanoseconds
  if(i<500){                           //Limited to 500
  if(current->pid == tpid){
    probes[i].time = time_ns;
    if(j==0){                         //For getting address max value and address min value
      addmin = regs->si;
      addmax = regs->si;
      j=1;                            //Run only once
    }
    probes[i].address = regs->si>>12;     //Store value without offset
    if((regs->si) < addmin){
      addmin = regs->si;
    }
    if((regs->si) > addmax){
      addmax = regs->si;
    }
    printk(KERN_INFO "'PID : %d ,PAGE FAULT AT  %lu'",current->pid,regs->si);
    i++;
      }
      }
 
  return 0;
}

static int __init kprobe_init(void)
{
	int ret;
	kp.pre_handler = handler_pre;       
	ret = register_kprobe(&kp);
	if (ret < 0) {
		pr_err("register_kprobe failed, returned %d\n", ret);
		return ret;
	}
	pr_info("Planted kprobe at %p\n", kp.addr);
	return 0;
}

static void __exit kprobe_exit(void)
{
if(choice==1){                            //For probe B
  memset(strings,0,sizeof(strings));  
  func();
  }
	unregister_kprobe(&kp);
	pr_info("kprobe at %p unregistered\n", kp.addr);


}

module_init(kprobe_init)
module_exit(kprobe_exit)
MODULE_LICENSE("GPL");
