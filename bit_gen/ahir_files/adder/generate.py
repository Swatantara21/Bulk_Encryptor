#!/usr/bin/env python


# enter the number of intermediate pipes
# min value = 1
no_of_pipes = 100

print "\nInfo : generating loopback.Aa with no. of intermediate pipes =", no_of_pipes, "\n"


scr_file = open("loopback.Aa","w")
print >> scr_file, "$pipe Riffa_to_CPU: $uint<64> $depth 1"
print >> scr_file, "$pipe CPU_to_Riffa: $uint<64> $depth 1 \n"

for i in enumerate(range(no_of_pipes)):
	print >> scr_file, "$pipe temp" + str(i[0]) + ": $uint<64> $depth 1"

print >> scr_file, "\n$module [loopback_daemon] $in () $out () $is\n {"
print >> scr_file, "\t$branchblock[lOoP] { \n"
print >> scr_file, "\t\t$dopipeline $depth 1 $buffering 2  $fullrate"
print >> scr_file, "\t\t$merge $entry $loopback $endmerge \n"
print >> scr_file, "\t\t\tX := Riffa_to_CPU"
print >> scr_file, "\t\t\ttemp0 := (X*2)"

for i in range(0,no_of_pipes-1,1):
	print >> scr_file, "\t\t\ttemp" + str(i+1) + " := temp" + str(i)

print >> scr_file, "\t\t\tCPU_to_Riffa := temp" + str(no_of_pipes-1)
print >> scr_file, "\n\t\t$while 1\n\t}\n}"

scr_file.close()

