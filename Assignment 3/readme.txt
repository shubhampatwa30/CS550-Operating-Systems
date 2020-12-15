Name : Shubham Patwa
Bnumber : B00818310
================================

I have implemented both the kernel modules into a single module.
You have to give input to 'choice' parameter either 0 or 1 for selecting pf_probe_A or pf_probe_B. 

================================
How to run:
{
sudo make
sudo insmod pf_probe.ko tpid=<pid> choice=<0 or 1>
sudo rmmod pf_probe
dmesg
}
================================
Details:
pid    : input pid number.
choice : input 0 : for using pf_probe_A
choice : input 1 : for using pf_probe_B