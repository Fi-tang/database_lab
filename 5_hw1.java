import java.util.HashMap;

import java.io.*;
import java.net.URI;
import java.net.URISyntaxException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FSDataOutputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IOUtils;

import java.io.IOException;

import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.HColumnDescriptor;
import org.apache.hadoop.hbase.HTableDescriptor;
import org.apache.hadoop.hbase.MasterNotRunningException;
import org.apache.hadoop.hbase.TableName;
import org.apache.hadoop.hbase.ZooKeeperConnectionException;
import org.apache.hadoop.hbase.client.HBaseAdmin;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Put;

import org.apache.log4j.*;

/**
 * @class:	ScanOperator, the first level in distinct Operation
 * 						  use to process every line read from HDFS,
 * 						  in which,to identify the number of line object, and remove line, 
 * 						  same Operation as Operator Tree's scan
 */
class ScanOperator{// preProcess
    // calculate total column Number, compute line Number
	/**
	 * @function: int totalColumnNumber
	 * @param String InputEveryLine: line read from HDFS
	 * @return:	int value, the number of colomns from file
	 * use the occurence of '|' to represent the total column number
	 * 
	 */
    public int totalColumnNumber(String InputEveryLine){
        String[] totalNumbercount = InputEveryLine.split("|");
        int count = 0;
        for(int i = 0; i < totalNumbercount.length; i++){
            if(totalNumbercount[i].charAt(0) == '|'){
                count++;
            }
        }
        return count;
    }
    /**
     * @function: String[] getNextLine
     * @param String InputEveryLine: line read from HDFS file 
     * @return String[]: After scan Operator, we get the form of 
     * for example: String InputEveryLine = "100|3|abc|def|best|10|" to
     * String[] scanOperatorResult = {"100","3","abc","def","best","10"};
     * remove the '|', and change and split it into String[]
     */
    // preProcess every input line
    public String[] getNextLine(String InputEveryLine){
        int totalLineNumber = totalColumnNumber(InputEveryLine);
        String[] returnResult = new String[totalLineNumber];
        for(int i = 0; i < totalLineNumber; i++){
            returnResult[i] = "";   // initialize String[] buffer, Every String is "" at first, instead of null
        }
        String[] beforeProcess = InputEveryLine.split("|");
        int returnResultCount = 0;
        for(int i = 0; i < beforeProcess.length; i++){
            if(beforeProcess[i].charAt(0) != '|'){
                returnResult[returnResultCount] += beforeProcess[i].charAt(0);
            }
            else{
                returnResultCount++;
            }
        }
        return returnResult;
    }
}

/**
 * @class:	FilterOperator, the second level in distinct Operation
 * 						  use to remove unstable line getting from ScanOperator
 * 						  include 6 types of value comparsion.
 */
//Is it ok? to transform all the value into double
class FilterOperator{
    // getNextLine(scanOperator, "1", "gt", "5.1");
	/**
	 * @function String[] getNextLine
	 * @param scanOperatorResult: Input from ScanOperatorResult's getNextLine
	 * @param columnNumber: which column to select?
	 * @param compareLevel: different kinds of compare include "gt","ge","eq","ne","le","lt"
	 * @param compareNumber: the value to compare
	 * @return: String[]: as the input for distinctOperator
	 * transfrom the message from String to int or float,
	 * compare whether scanOperatorResult[(int) columnNumber] [compareLevel] (float)compareNumber
	 */
    public String[] getNextLine(String[] scanOperatorResult, String columnNumber, String compareLevel, String compareNumber){
        float compareValue = Float.valueOf(compareNumber);
        int currentColumnNumber = Integer.parseUnsignedInt(columnNumber);
        float currentValue = Float.valueOf(scanOperatorResult[currentColumnNumber]);

        switch(compareLevel){
            case "gt":
                if(currentValue > compareValue){
                    return scanOperatorResult;
                }
                break;
            case "ge":
                if(currentValue >= compareValue){
                    return scanOperatorResult;
                }
                break;
            case "eq":
                if(currentValue == compareValue){
                    return scanOperatorResult;
                }
                break;
            case "ne":
                if(currentValue != compareValue){
                    return scanOperatorResult;
                }
                break;
            case "le":
                if(currentValue <= compareValue){
                    return scanOperatorResult;
                }
                break;
            case "lt":
                if(currentValue < compareValue){
                    return scanOperatorResult;
                }
                break;
            default:
        }
        return null;
    }
}

/**
 * @class:	DistinctOperator, the third level in distinct Operation			  
 */
class DistinctOperator{
	/**
	 * @function: int[] preProcessIndex
	 * @param parameters
	 * @return: change String[] Type to int[] Type
	 */
    public int[] preProcessIndex(String[] parameters){
        int[] compareColumnIndex = new int[parameters.length];
        for(int i = 0; i < parameters.length; i++){
            compareColumnIndex[i] = Integer.parseUnsignedInt(parameters[i]);
        }
        return compareColumnIndex;
    }
    
    /**
     * @function: String[] preProcessValue
     * @param filterOperatorResult: Input from FilterOperator
     * @param compareColumnIndex: how and which are the columns to be distincted
     * @return: narrow the line to specific distinct Line,
     * for example ,if we want {"100","3","abc","def","42","good"}'s 2,3,5
     * then we get {"abc","def","good"}
     */
    public String[] preProcessValue(String[] filterOperatorResult, int[] compareColumnIndex){
        if(filterOperatorResult == null){
            return null;
        }
        else{
            String[] temp = new String[compareColumnIndex.length];
            for(int i = 0; i < compareColumnIndex.length; i++){
                temp[i] = "";
            }
            int k = 0;
            for(int i = 0; i < compareColumnIndex.length; i++){
                temp[i] += filterOperatorResult[compareColumnIndex[k++]];
            }
            return temp;
        }
    }
}

/**
 * 
 * @class Hw1Grp5: main class, used to read from HDFS and put to HBASE
 * because we want to use HashMap, and we must rewrite its equals() and hashCode() function
 * @member globalCount: record useful line
 * @member value: used to rewrite the class's equals() and hashCode() method
 * @member hashMap: use hash to finish distinct Operation
 * @member fileName: the name of input file
 * @member columnNumber: select from where
 * @member compareLevel: "gt" or "ge" or "le" or "lt" or "eq" or "ne"
 * @member compareNumber: parameters get from args[] start with select:
 * @member parameters: parameters get from args[] start with distinct:
 */
public class Hw1Grp5 {
    public int globalCount = 0;
    String value;
    HashMap<Integer, Hw1Grp5> hashMap = new HashMap<Integer, Hw1Grp5>();
    String fileName = "";		// R=/hw1/lineitem.tbl
    String columnNumber = "";	// select:R1,gt,5.1 -> 1
    String compareLevel = "";	// compareLevel: gt
    String compareNumber = "";	// compareNumber: 5.1
    String[] parameters;

    /**
     * @function: rewrite equals
     */
    @Override
    public boolean equals(Object anObject){
        if(!(anObject instanceof Hw1Grp5)){
            return false;
        }
        else{
            return this.value.equals(((Hw1Grp5)anObject).value);
        }
    }
    
    /**
     * @function: rewrite hashCode()
     */
    @Override
    public int hashCode(){
        return this.value.hashCode();
    }
    
    /**
     * @function: function used to print and debug, it is okay to get rid of it
     * @param returnResult
     * 
     */
    public void printString(String[] returnResult){
        if(returnResult == null){
            System.out.println("null");
            return;
        }
        else{
            for(int i = 0; i < returnResult.length; i++){
                System.out.print(returnResult[i] + "\t");
            }
            System.out.println();
        }
    }
    
    /**
     * @function : String[] AfterDistinctOperator
     * @param filterOperatorResult: result get from filterOperator
     * @param parameters: distinct column number
     * @return: distinct result
     * it is more efficient to rewrite the equals and hashCode() in this class,
     * so we change some function from distinctOperator to here
     */
    public String[] AfterDistinctOperator(String[] filterOperatorResult, String[] parameters){
        DistinctOperator distinctOperator = new DistinctOperator();
        int[] parametersIndex = distinctOperator.preProcessIndex(parameters);
        String temp[] = distinctOperator.preProcessValue(filterOperatorResult, parametersIndex);
        if(temp == null){
            return null;
        }
        else{
            String target = "";
            for(int i = 0; i < temp.length; i++){
                target += temp[i];
            }
            Hw1Grp5 Hw1Grp5Temp = new Hw1Grp5();
            Hw1Grp5Temp.value = target;
            if(this.hashMap.containsValue(Hw1Grp5Temp)){
                return null;
            }
            else{
                hashMap.put(this.globalCount, Hw1Grp5Temp);
                this.globalCount += 1;
                return temp;
            }
        }
    }
    
    /**
     * 
     * @param preArgs: the same as public static void main's String[] args
     * to fill the Hw1Grp5's member
     */
    // The whole purpose is to preprocess InputLine
    public void preProcessArgs(String[] preArgs){
        // which means every " " is a different string

        // because it forms like: R=file
        for(int k = 2; k < preArgs[0].length(); k++){
            fileName += preArgs[0].charAt(k);
        }

        String compareTemp = "";
        for(int k = 7; k < preArgs[1].length(); k++){
            compareTemp += preArgs[1].charAt(k);
        }
        String[] compareMiddleTemp = compareTemp.split(",");
        for(int k = 1; k < compareMiddleTemp[0].length(); k++){
            if(compareMiddleTemp[0].charAt(k) == ','){
                break;
            }
            columnNumber += compareMiddleTemp[0].charAt(k);
        }

        for(int k = 0; k < compareMiddleTemp[1].length(); k++){
            if(compareMiddleTemp[1].charAt(k) == ','){
                break;
            }
            compareLevel += compareMiddleTemp[1].charAt(k);
        }

        for(int k = 0; k < compareMiddleTemp[2].length(); k++){
            if(compareMiddleTemp[2].charAt(k) == ','){
                break;
            }
            compareNumber += compareMiddleTemp[2].charAt(k);
        }
        String columnTemp = "";
        for(int k = 9; k < preArgs[2].length(); k++){
            columnTemp += preArgs[2].charAt(k);
        }
        String[] columnMiddleTemp = columnTemp.split(",");
        this.parameters = new String[columnMiddleTemp.length];
        for(int k = 0; k < columnMiddleTemp.length; k++){
            parameters[k] = "";
        }

        for(int k = 0; k < columnMiddleTemp.length; k++){
            for(int i = 1; i < columnMiddleTemp[k].length(); i++){
                if(columnMiddleTemp[k].charAt(i) == ','){
                    break;
                }
                parameters[k] += columnMiddleTemp[k].charAt(i);
            }
        }
    }

    /**
     * 
     * @param args
     * @throws IOException
     * @throws URISyntaxException
     * @throws MasterNotRunningException
     * @throws ZooKeeperConnectionException
     * main function starts here, to read from HDFS and write to HBASE,
     * and finish the scan\filter\distinct Operator
     */
    public static void main(String[] args) throws IOException, URISyntaxException, MasterNotRunningException,
            ZooKeeperConnectionException{
        // In real, it is from args
        Hw1Grp5 hw1Grp5 = new Hw1Grp5();
        hw1Grp5.preProcessArgs(args);
        // preprocess,to assign Every possible number

        ScanOperator scanOperator = new ScanOperator();
        FilterOperator filterOperator = new FilterOperator();

        // hdfs Read part
        String hdfsFilename = "hdfs://localhost:9000" + hw1Grp5.fileName;
        Configuration conf = new Configuration();
        FileSystem fs = FileSystem.get(URI.create(hdfsFilename),conf);
        FSDataInputStream in_stream = fs.open(new Path(hdfsFilename));
        BufferedReader in = new BufferedReader(new InputStreamReader(in_stream));
        String InputEveryLine;

        // hbase write part
        // create table descriptor
        String tableName = "Result";
        HTableDescriptor htd = new HTableDescriptor(TableName.valueOf(tableName));

        // create column descriptor
        HColumnDescriptor cf = new HColumnDescriptor("res");
        htd.addFamily(cf);

        // configure HBase
        Configuration configuration = HBaseConfiguration.create();
        HBaseAdmin hAdmin = new HBaseAdmin(configuration);

        hAdmin.createTable(htd);

        HTable table = new HTable(configuration, tableName);

        while((InputEveryLine = in.readLine()) != null){
            // Scan
            int columnNumber = scanOperator.totalColumnNumber(InputEveryLine);
            String[] scanOperatorResult = scanOperator.getNextLine(InputEveryLine);
            // Filter
            String[] filterOperatorResult = filterOperator.getNextLine(scanOperatorResult, hw1Grp5.columnNumber, hw1Grp5.compareLevel, hw1Grp5.compareNumber);
            // Distinct
            String[] distinctOperatorResult = hw1Grp5.AfterDistinctOperator(filterOperatorResult, hw1Grp5.parameters);
            if(distinctOperatorResult != null){
                for(int i = 0; i < distinctOperatorResult.length; i++){
                    String columnkey = "" + hw1Grp5.globalCount;
                    Put put = new Put(columnkey.getBytes());
                    String currentcolumn = "R" + hw1Grp5.parameters[i];
                    put.add("res".getBytes(),currentcolumn.getBytes(),distinctOperatorResult[i].getBytes());
                    table.put(put);
                }
            }
        }
        in.close();
        fs.close();
        hAdmin.close();
        table.close();
        System.out.println("put successfully");
    }
}