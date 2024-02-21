import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.text.DecimalFormat;

public class Hw2Part1{
	/***
	 * Used for Map outputKey
	 * @param source : represent source
	 * @param destation: represent destation
	 * Because this KeyCountNT should be write to Context, so must declare Writable
	 * Also, I want to define myown grouping role: {source, destation}, so it must implement WritableComparable
	 */
    public static class KeyCountNT implements WritableComparable{
        private String source;
        private String destation;

        public KeyCountNT(){

        }

        public KeyCountNT(String source, String destation){
            this.source = source;
            this.destation = destation;
        }

        public void set(String source, String destation){
            this.source = source;
            this.destation = destation;
        }

        public String getSource(){
            return this.source;
        }

        public String getDestation(){
            return this.destation;
        }
        	
        @Override
        public void write(DataOutput dataOutput) throws IOException{
            dataOutput.writeUTF(source);
            dataOutput.writeUTF(destation);
        }

        @Override
        public void readFields(DataInput dataInput) throws IOException{
            this.source = dataInput.readUTF();
            this.destation = dataInput.readUTF();
        }

        /***
         * @return outputFormat , using '\t'
         */
        @Override
        public String toString() {
            return source + "\t" + destation;
        }
        
        /***
         * because we want to destinguish source and destation
         * so use '+' to add them together 
         * then use String's compareTo function
         * @param o
         * @return
         */
        @Override
        public int compareTo(Object o) {
            KeyCountNT oo = (KeyCountNT)o;
            String thisString = this.source + this.destation;
            String ooString = oo.source + oo.destation;
            int number = thisString.compareTo(ooString);
            if(number == 0){
                return 0;
            }
            else if(number > 0){
                return 1;
            }
            else{
                return -1;
            }
        }
    }

    /***
     * 
     * @class Used for Map OutputValue and Reduce's OutputValue and InputValue
     * do not need to compare
     * only need to override write and readFields function
     */
    public static class ValueCountNT implements Writable{
        private long count;
        private double total_time;

        public ValueCountNT(){

        }

        public ValueCountNT(long count, double total_time){
            this.count = count;
            this.total_time = total_time;
        }

        public void set(long count, double total_time){
            this.count = count;
            this.total_time = total_time;
        }

        public long getCount(){
            return this.count;
        }

        public double getTotal_time(){
            return this.total_time;
        }

        @Override
        public void write(DataOutput dataOutput) throws IOException{
            dataOutput.writeLong(count);
            dataOutput.writeDouble(total_time);
        }

        @Override
        public void readFields(DataInput dataInput) throws IOException{
            this.count = dataInput.readLong();
            this.total_time = dataInput.readDouble();
        }

        @Override
        public String toString() {
            return this.count + "\t" + this.total_time;
        }
    }
    
    /***
     * redefine compare rule
     * @function: same as KeyCountNT's compareTo
     * need to fullfill 0, -1 , 1 three category, or it will fail to pass input_1's test
     *
     */
    public static class keyGrouping extends WritableComparator{
        protected keyGrouping(){
            super(KeyCountNT.class, true);
        }

        @Override
        public int compare(WritableComparable a, WritableComparable b) {
            KeyCountNT aa = (KeyCountNT)a;
            KeyCountNT bb = (KeyCountNT)b;

            String aaString = aa.getSource() + aa.getDestation();
            String bbString = bb.getSource() + bb.getDestation();

            int number = aaString.compareTo(bbString);
            if(number == 0){
                return 0;
            }
            else if(number > 0){
                return 1;
            }
            else{
                return -1;
            }
        }
    }
    
    /***
     * @function Mapper
     * @param inputKey: LongWritable: bias of every line
     * @param inputValue: Text -> line
     * @param outputKey: KeyCountNT -> {source, destation}
     * @param outputValue: ValueCountNT -> {count, total_time}
     *
     */
    public static class CountNTMapper extends Mapper<LongWritable, Text, KeyCountNT, ValueCountNT>{
        KeyCountNT mapKeyResult = new KeyCountNT();
        ValueCountNT mapValueResult = new ValueCountNT();

        @Override
        protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
            String inputline = value.toString();
            String[] inputElement = inputline.split(" ");
            if(inputElement.length != 3){
                mapKeyResult.set("","");
                mapValueResult.set(0,0.0);
            }
            else{
                String source = inputElement[0];
                String destation = inputElement[1];
                double total_time = Double.parseDouble(inputElement[2]);

                mapKeyResult.set(source,destation);
                mapValueResult.set(1,total_time);
                context.write(mapKeyResult, mapValueResult);
            }
        }
    }

    /***
     * @function Reducer
     * @param inputKey:  KeyCountNT -> {source, destation}
     * @param inputValue: ValueCountNT -> {count, total_time}
     * @param outputKey: KeyCountNT -> {source, destation}
     * @param outputValue: ValueCountNT -> {count, averg_time}
     *
     */
    public static class CountNTReducer extends Reducer<KeyCountNT, ValueCountNT, KeyCountNT, ValueCountNT>{
        KeyCountNT reduceKeyResult = new KeyCountNT();
        ValueCountNT reduceValueResult = new ValueCountNT();

        @Override
        protected void reduce(KeyCountNT key, Iterable<ValueCountNT> values, Context context) throws IOException, InterruptedException {
            reduceKeyResult.set(key.getSource(), key.getDestation());
            long sum = 0;
            double total_time = 0;
            for(ValueCountNT val : values){
                sum += val.getCount();
                total_time += val.getTotal_time();
            }

            if(sum == 0){
                return;
            }
            else{
                double beforetime = total_time / (sum + 0.0);
                DecimalFormat dfm = new DecimalFormat("0.000");
                double aftertime = Double.parseDouble(dfm.format(beforetime));

                reduceValueResult.set(sum, aftertime);
                context.write(reduceKeyResult, reduceValueResult);
            }
        }
    }

    public static void main(String[] args) throws Exception{
        Configuration conf = new Configuration();
        String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
        if(otherArgs.length < 2){
            System.err.println("Usage: Hw2Part1 <in> [<in>...] <out>");
            System.exit(2);
        }

        Job job = Job.getInstance(conf, "Hw2Part1");

        job.setJarByClass(Hw2Part1.class);

        job.setMapperClass(CountNTMapper.class);
        job.setReducerClass(CountNTReducer.class);
        job.setGroupingComparatorClass(keyGrouping.class);

        job.setMapOutputKeyClass(KeyCountNT.class);
        job.setMapOutputValueClass(ValueCountNT.class);

        job.setOutputKeyClass(KeyCountNT.class);
        job.setOutputValueClass(ValueCountNT.class);

        for(int i = 0; i < otherArgs.length - 1; i++){
            FileInputFormat.addInputPath(job, new Path(otherArgs[i]));
        }

        FileOutputFormat.setOutputPath(job, new Path(otherArgs[otherArgs.length - 1]));

        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}