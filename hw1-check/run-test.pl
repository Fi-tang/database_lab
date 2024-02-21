#!/usr/bin/perl

#
# Written by Shimin Chen
#

use POSIX ":sys_wait_h";

my $script_path=`dirname $0`; chomp($script_path);

# 1. all the test input and output data

my @join_cmd, @join_col, @join_res;

     $join_cmd[0]= "R=/hw1/join_0R.tbl S=/hw1/join_0S.tbl join:R0=S0 res:R1,S1";
     $join_cmd[1]= "R=/hw1/join_1R.tbl S=/hw1/join_1S.tbl join:R0=S0 res:R1,S1";
     $join_cmd[2]= "R=/hw1/join_2R.tbl S=/hw1/join_2S.tbl join:R0=S0 res:R1,S1";

     $join_col[0]= "rowkey res:R1 res:S1";
     $join_col[1]= "rowkey res:R1 res:S1";
     $join_col[2]= "rowkey res:R1 res:S1";
     
     $join_res[0]= "join_0res.tbl";
     $join_res[1]= "join_1res.tbl";
     $join_res[2]= "join_2res.tbl";

my @groupby_cmd, @groupby_col, @groupby_res;

     $groupby_cmd[0]= "R=/hw1/groupby_0.tbl groupby:R0 'res:count'";
     $groupby_cmd[1]= "R=/hw1/groupby_1.tbl groupby:R1 'res:max(R0),count'";
     $groupby_cmd[2]= "R=/hw1/groupby_2.tbl groupby:R2 'res:max(R0),avg(R1),count'";

     $groupby_col[0]= "rowkey res:count";
     $groupby_col[1]= "rowkey 'res:max(R0)' res:count";
     $groupby_col[2]= "rowkey 'res:max(R0)' 'res:avg(R1)' res:count";

     $groupby_res[0]= "groupby_0res.tbl";
     $groupby_res[1]= "groupby_1res.tbl";
     $groupby_res[2]= "groupby_2res.tbl";

my @distinct_cmd, @distinct_col, @distinct_res;

     $distinct_cmd[0]= "R=/hw1/distinct_0.tbl select:R0,gt,3 distinct:R1";
     $distinct_cmd[1]= "R=/hw1/distinct_1.tbl select:R1,ge,30 distinct:R2,R0";
     $distinct_cmd[2]= "R=/hw1/distinct_2.tbl select:R2,lt,100 distinct:R3,R3";

     $distinct_col[0]= "res:R1";
     $distinct_col[1]= "res:R2 res:R0";
     $distinct_col[2]= "res:R3 res:R3";

     $distinct_res[0]= "distinct_0res.tbl";
     $distinct_res[1]= "distinct_1res.tbl";
     $distinct_res[2]= "distinct_2res.tbl";

if ($#ARGV < 1) {
   print "Usage: $0 <score output> <file...>\n";
   exit (0);
}

$| = 1; # set auto flush

if ( -f $ARGV[0] ) {
  print "$ARGV[0] already exists and cannot be score output!\n";
  exit (0);
}

open OUT, ">$ARGV[0]" or die "can't open $ARGV[0] for writing!\n";

for (my $i=1; $i<=$#ARGV; $i++) {
   my $file_name= $ARGV[$i];
   my $mydate=`date`; chomp($mydate);
   print "\n\n\n";
   print "------------------------------------------------------------\n";
   print "[$mydate] $i $file_name\n";
   print "------------------------------------------------------------\n";

   my $score= &grading($file_name);
   print OUT $file_name, " raw score: ", $score, "\n";
}

close(OUT);

# ---
# get the group and student id
# ---
sub getGroupID($)
{
   my ($file_name)= @_;

   if ($file_name =~ /(\d)_(\w+)_hw1.java/) {
      my $group= $1;
      my $student_id= $2;

      return ($group, $student_id);
   }
   else {
      print "Error: Bad file name $file_name\n";
      return (-1, -1);
   }
}

# ---
# print the command and run it
# ---
sub mysystem($)
{
  my ($command) = @_;
  print $command, "\n";
  return system($command);
}

# ---
# set up a single test
# &setupTest($source, $main_class);
# ---
sub setupTest($$)
{
   my ($source, $main_class)= @_;

   # 1. create a sandbox directory
   &mysystem("rm -rf sandbox; mkdir sandbox");
   &mysystem("cp $source sandbox/$main_class.java");

   # 2. compile
   print "------------------------------------------------------------\n";
   print "Compile\n";
   print "------------------------------------------------------------\n";
   &mysystem("cd sandbox; javac $main_class.java 2>&1; cd ..");

   # 3. check class file
   my $classfile= "sandbox/$main_class.class";
   if ( -f $classfile ) {
      return 0;
   }
   else {
      return -1;
   }
}

# ---
# run a single test
# &runTest($main_class, $param, $col, $result);
# ---
sub runTest($$$$)
{
   my ($main_class, $param, $col, $out, $result)= @_;

   print "------------------------------------------------------------\n";
   print "Run $param\n";
   print "------------------------------------------------------------\n";

   # 1. remove hbase table
   &mysystem("hbase shell ./hbase-drop-result >/dev/null 2>&1");

   # 2. run
   print "cd sandbox; java $main_class $param >$out 2>&1; cd ..\n";

   my $pid= fork();
   die "fork() failed: $!" unless defined $pid;

   if ($pid == 0) {
      chdir("sandbox");
      exec "java $main_class $param >$out 2>&1";
      exit(0);
   }

   #print "wait for $pid\n";

   my $kid= -1;
   my $elapsed= 0;
   do {
      if ($elapsed >= 180) {# 3 min
        kill 9, $pid;
        print "Running for over 3 minutes, killed!\n";
      }
      sleep(1); $elapsed++;
      $kid = waitpid($pid, WNOHANG);
      #print "waitpid $kid $elapsed\n";
   } while ($kid == 0);

   # 3. obtain result
   &mysystem("cd bld; java HBase2TableAll Result $col > r.tbl; cd ..");
   &mysystem("mv bld/r.tbl $result");
}

# ---
# &checkResult($result, $good_result);
# ---
sub checkResult($$$)
{
   my ($result, $good_result)= @_;

   print "------------------------------------------------------------\n";
   print "checkResult $result $good_result\n";
   print "------------------------------------------------------------\n";

   # 1. sort the output
   &mysystem("sort $result > sandbox/tmp.tbl");

   # 2. diff
   &mysystem("diff sandbox/tmp.tbl $good_result >$result.diff 2>&1");

   # 3. check diff
   my $mydiff= `cat $result.diff`; chomp($mydiff);
   if ($mydiff eq '') {
      print "good\n";
      return 1;
   }
   else {
      print "different\n";
      return 0;
   }
}

# ---
# &checkGroupbyResult($result, $good_result);
# ---
sub checkGroupbyResult($$)
{
   my ($result, $good_result)= @_;

   print "------------------------------------------------------------\n";
   print "checkGroupbyResult $result $good_result\n";
   print "------------------------------------------------------------\n";

   # 1. sort the output
   &mysystem("sort $result > sandbox/tmp.tbl");

   # 2. diff
   print "compare sandbox/tmp.tbl and $good_result numerically\n";
   open IN1, "$good_result" or die "can't open $good_result!\n";
   open IN2, "sandbox/tmp.tbl" or die "can't open sandbox/tmp.tbl!\n";
   open DOUT, ">$result.diff" or die "can't open output for writing!\n";

   while(<IN1>) {
     my $line1= $_; chomp($line1);
     my $line2= <IN2>; chomp($line2);

     my @rec1= split(/\|/, $line1);
     my @rec2= split(/\|/, $line2);

     # a|b|c| will become {a,b,c} 3 components

     my $bad= 0;
     if ($rec1[0] eq $rec2[0]) {
       for (my $i=1; $i<=$#rec1; $i++) {
          my $d= $rec1[$i] - $rec2[$i];
          if (($d<-0.05) || ($d>0.05)) {
            $bad ++; last;
          }
       }
     }
     else {$bad=1;}

     if ($bad > 0) {
       print DOUT "<$line2\n";
       print DOUT ">$line1\n";
       last;
     }
   }

   close(DOUT);
   close(IN2);
   close(IN1);
   
   # 3. check diff
   my $mydiff= `cat $result.diff`; chomp($mydiff);
   if ($mydiff eq '') {
      print "good\n";
      return 1;
   }
   else {
      print "different\n";
      return 0;
   }
}

# ---
# grading homework 1
# &grading($source)
# ---
sub grading($)
{
   my ($source)= @_;
   my ($group, $student_id)= &getGroupID($source);

   if ($group == -1) {return 0;}
   
   my $main_class= "Hw1Grp". $group;

   # 1. set up test
   if (&setupTest($source, $main_class) < 0) {return 0;}

   # 2. run test
   for (my $i=0; $i<3; $i++) {
      my $out= "$i.out";
      my $result= "sandbox/$i.tbl";

      if (($group == 0) || ($group == 1)) {
        # join
        &runTest($main_class, $join_cmd[$i], $join_col[$i], $out, $result);
      }
      elsif (($group == 2) || ($group == 3)) {
        # groupby
        &runTest($main_class, $groupby_cmd[$i], $groupby_col[$i], $out, $result);
      }
      else {
        # distinct
        &runTest($main_class, $distinct_cmd[$i], $distinct_col[$i], $out, $result);
      }
   }
   
   # 3. check result
   my $score= 0;
   for (my $i=0; $i<3; $i++) {
      my $result= "sandbox/$i.tbl";
      my $r= 0;

      if (($group == 0) || ($group == 1)) {
        # join
        $r= &checkResult($result, "result-sorted/$join_res[$i]");
      }
      elsif (($group == 2) || ($group == 3)) {
        # groupby
        $r= &checkGroupbyResult($result, "result-sorted/$groupby_res[$i]");
      }
      else {
        # distinct
        $r= &checkResult($result, "result-sorted/$distinct_res[$i]");
      }

      $score += $r;
   }

   # 4. preserve the sandbox
   my $pos= index($source, ".java");
   if ($pos >= 0) {
       my $d= substr($source, 0, $pos);
       &mysystem("rm -rf $d; mv sandbox $d");
   }

   return $score;
}
