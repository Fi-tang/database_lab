#!/usr/bin/perl

my $script_path=`dirname $0`; chomp($script_path);

if ($#ARGV < 0) {
   print "Usage: $0 <file1> <file2> ...\n";
   exit(0);
}

my $good= 0;
my $bad= 0;
for (my $i=0; $i<=$#ARGV; $i++) {
   my $file_name= $ARGV[$i];
   my $result= &grading($file_name);
   if ($result ne "good") {
     print STDERR $file_name, " ", $result, "\n";
     $bad ++;
   }
   else {
     $good ++;
   }
}

print "Good files: $good, Bad files: $bad\n";

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

   print "------------------------------------------------------------\n";
   print "$source\n";
   print "------------------------------------------------------------\n";

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
# grading homework 1
# &grading($source)
# ---
sub grading($)
{
   my ($source)= @_;
   my ($group, $student_id)= &getGroupID($source);

   if ($group == -1) {return "file name error";}
   
   my $main_class= "Hw1Grp". $group;

   # 1. set up test
   if (&setupTest($source, $main_class) < 0) {return "compile error";}

   return "good";
}
