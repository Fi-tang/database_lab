#!/usr/bin/perl

my $num_groups= 6;

if ($#ARGV < 0) {
   print "Usage: $0 <file1> <file2> ...\n";
   exit(0);
}

my $good= 0;
my $bad= 0;

for (my $i=0; $i<=$#ARGV; $i++) {
   my $file_name= $ARGV[$i];
   my $result= &checkGroup($file_name);
   if ($result ne "good") {
     print $file_name, " ", $result, "\n";
     $bad ++;
   }
   else {
     $good ++;
   }
}
print "Good files: $good, Bad files: $bad\n";

sub checkGroup($)
{
   my ($file_name)= @_;

   if ($file_name =~ /(\d)_(\w+)_hw1.java/) {
      my $group= $1;
      my $student_id= $2;

      my $last_6digits= 0;
      if ($file_name =~ /(\d\d\d\d\d\d)_hw1.java/) {
            $last_6digits= $1;
      }
      else {
            return "last 6 char of ID not digits?";
      }
      my $compute = $last_6digits % $num_groups;
      if ($compute != $group) {
         return "should be group $compute";
      }

      #my $first_line= `head -n 1 $file_name`; chomp($first_line);
      #if ($first_line =~ /(\d),\s*(\w+),/) {
      #   if (($1 ne $group) || ($2 ne $student_id)) {
      #      return "name and comment mismatch";
      #   }
      #}
      #else {
      #      return "bad comment format";
      #}

      #print "group: $group, student_id: $student_id\n";
      #print "last 6 digits: $last_6digits, compute: $compute\n";

      return "good";
   }
   else {
      return "bad file name format";
   }
}
