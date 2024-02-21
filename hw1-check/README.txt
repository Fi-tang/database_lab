0. set language to POSIX
   $ export LC_ALL="POSIX"

1. make sure ssh is running
   $ service ssh status

   if not, then run sshd (note that this is necessary in a docker container)
   $ service ssh start

2. make sure HDFS and HBase are successfully started
   $ start-dfs.sh
   $ start-hbase.sh

   check if hadoop and hbase are running correctly
   $ jps

   5824 Jps
   5029 HMaster
   5190 HRegionServer
   4950 HQuorumPeer
   4507 SecondaryNameNode
   4173 NameNode
   4317 DataNode

3. put input files into HDFS

   $ ./myprepare

4. check file name format

   $ ./check-group.pl <your-java-file>

5. check if the file can be compiled

   $ ./check-compile.pl <your-java-file>

6. run test

   $ ./run-test.pl ./score <your-java-file>

Your score will be in ./score.  The run-test.pl tests 3 input cases, you will
get one score for each case.  So the output full score is 3.

To run the test again, you need to first remove ./score

   $ rm ./score
   $ ./run-test.pl ./score <your-java-file>
