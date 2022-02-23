// Bareket Damari
// Michael Saar

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO {
public:
	virtual string read() = 0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;

	virtual void writeLine(string text) {
		this->write(text);
		this->write("\n");
	}
	virtual void writeLine(float f) {
		this->write(f);
		this->write("\n");
	}
	virtual string readLine() {
		string line;
		string s = this->read();
		while(s != "\n") {
			line = line + s;
			s = this->read();
		}
		line =  line + "\n";
		return line;
	}
    
	/* read using the DefaultIO read method, and write specifically into an ofstream object */
	virtual void writeFile(string fileName) {
		ofstream out;
		out.open(fileName);
        string done = "done";
		string doneN = "done\n";		
		string s;
		s = this->read();
		// continue reading and writing into file until the read output is "done"
		while(s != done && s != doneN) {
			out << s << std::endl;
			s = this->read();
		}
		out.close();
	}

	virtual void readFile(string fileName) {
		ifstream in;
		in.open(fileName);
		string s;
		// continue reding from the file until the read output is "done"
		while(getline(in, s)) {
			this->writeLine(s);
		}
		in.close();
	}

	virtual ~DefaultIO(){}
};

class FileIO : public DefaultIO {

public:
	ifstream in;
	ofstream out;
public:
	FileIO(string inputFile,string outputFile){
		in.open(inputFile);
		out.open(outputFile);
	}
	virtual string read(){
		string s;
		in>>s;
		return s;
	}
	virtual void write(string text){
		out<<text;
	}

	virtual void write(float f){
		out<<f;
	}

	virtual void read(float* f){
		in>>*f;
	}

	void close(){
		if(in.is_open())
			in.close();
		if(out.is_open())
			out.close();
	}

	virtual void readFromFile() {

	}
	virtual void writeToFile()=0;

	~FileIO(){
		close();
	}

};

class StandardIO : public DefaultIO {
   public:
    StandardIO() {}

    virtual string read() {
        string s;
        std::getline(std::cin, s);
        return s;
    }
	virtual void write(string text) {
		std::cout << text;
	}
	virtual void write(float f) {
		std::cout << f;
	}
	virtual void read(float* f) {
        std::cin >> (*f);
        // clear the rest
        string s;
        std::getline(std::cin, s);
    }

    ~StandardIO() {}
};

class Command {
    protected:
        string description;
    public:
        virtual void execute() = 0;
        string getDescription() {
             return this->description; 
        }
    virtual ~Command(){}
};

class IOCommand : public Command {
   protected:
	DefaultIO* dio;

   public:
    IOCommand(DefaultIO *dio) : dio(dio) {}

    virtual void execute() = 0;

    virtual ~IOCommand() {
        
    }
};

class UploadCommand : public IOCommand {
protected:
	const char* normalFile;
	const char* detectFile;

public:
    UploadCommand(DefaultIO *dio, const char *normalFile,
                  const char *detectFile)
        : IOCommand(dio), normalFile(normalFile), detectFile(detectFile) {
            this->description = "upload a time series csv file";
        }

    virtual void execute() {
        dio->writeLine("Please upload your local train CSV file.");
        dio->writeFile(normalFile);
        dio->writeLine("Upload complete.");

        dio->writeLine("Please upload your local test CSV file.");
        dio->writeFile(detectFile);
        dio->writeLine("Upload complete.");
    }

    ~UploadCommand() {
        // delete normalFile;
        // delete detectFile;
    }
};

class AnomalyDetectionCommand : public IOCommand {
protected:
    HybridAnomalyDetector *detector;

public:
    AnomalyDetectionCommand(DefaultIO *dio, HybridAnomalyDetector *detector)
        : IOCommand(dio), detector(detector) {}

    virtual void execute() = 0;

    ~AnomalyDetectionCommand() {
        
    }
};

class SetThresholdCommand : public AnomalyDetectionCommand {
public:
    SetThresholdCommand(DefaultIO *dio, HybridAnomalyDetector *detector)
        : AnomalyDetectionCommand(dio, detector) {
            this->description = "algorithm settings";
        }

    virtual void execute() {
        float originThreshold = this->detector->getThreshold();
        float threshold;
        bool isInputValid = false;
        dio->write("The current correlation threshold is ");
        dio->writeLine(originThreshold);
        dio->writeLine("Type a new threshold");
        do {
            dio->read(&threshold);
            if (threshold < 1.0 && threshold > 0.0) {
                detector->setThreshold(threshold);
                isInputValid = true;
            } else {
                dio->writeLine("please choose a value between 0 and 1.");
            }
        } while (!isInputValid);
    }
};

class DetectCommand : public AnomalyDetectionCommand {
    const char *fileName;

public:
    DetectCommand(DefaultIO *dio, HybridAnomalyDetector *detector,
                  const char *fileName)
        : AnomalyDetectionCommand(dio, detector), fileName(fileName) {
             this->description = "detect anomalies";
        }

    virtual void execute() {
        TimeSeries ts = TimeSeries(fileName);
        // learnNormal
        this->detector->learnNormal(ts);
		// this->detector->detect(ts);
        dio->writeLine("anomaly detection complete.");
    }

    ~DetectCommand() {
        // delete fileName;
    }
};

class DisplayAnomaliesCommand : public AnomalyDetectionCommand {
    const char *fileName;

    virtual void printReport(vector<AnomalyReport> &report) {
        // sort(report.begin(), report.end(), reportCompare());
        for (AnomalyReport r : report) {
            dio->write(r.timeStep);
            dio->write("\t");
            dio->writeLine(r.description);
        }
        dio->writeLine("Done.");
    }

public:
    DisplayAnomaliesCommand(DefaultIO *dio, HybridAnomalyDetector *detector,
                            const char *fileName)
        : AnomalyDetectionCommand(dio, detector), fileName(fileName) {
             this->description = "display results";
        }

    virtual void execute() {
        TimeSeries ts = TimeSeries(fileName);
        // detect to get the report
		this->detector->detect(ts); //this->detector->getDetectedAnomalyReport(); //
        vector<AnomalyReport> report = this->detector->getDetectedAnomalyReport();

        printReport(report);
    }

    ~DisplayAnomaliesCommand() {
        // delete fileName;
    }
};

class AnalyzeCommand : public AnomalyDetectionCommand {
    struct AnomalyUnit {
        string description;
        long startTime;
        long endTime;
        long range() {
            return endTime - startTime + 1;
        }
    };
	vector<AnomalyUnit> combinedAnomalies;
	vector<AnomalyUnit> realTimeAnomalies;
	long p, tp = 0;
	long n, fp = 0;
    const char *fileName;
    const char *detectFile;

	void combineDetectedAnomalies(vector<AnomalyReport> report) {	
        bool isFirst = true;
        for_each(
            report.begin(), report.end(),
            [&isFirst ,this](AnomalyReport rep) {
                AnomalyUnit &lastUnit = combinedAnomalies.back();
                if (!isFirst && lastUnit.description == rep.description &&
                    lastUnit.endTime + 1 == rep.timeStep) {
                    ++lastUnit.endTime;
                } else {
                    isFirst = false;
                    AnomalyUnit au{rep.description, rep.timeStep, rep.timeStep};
                    combinedAnomalies.push_back(au);
                }
            });
        sort(combinedAnomalies.begin(), combinedAnomalies.end(), [](const AnomalyUnit& au1, const AnomalyUnit& au2){
			return au1.startTime < au2.startTime;
		});
	}

    void getRealTimeAnomalies() {
        string rt = "real-time";
        string del = ",";
        string line; 
        line = dio->read();
        while (line != "done" && line != "done\n") {
            int splitter = line.find(del);
            int lineLen = line.length();
            int splitIndex = lineLen - (splitter + 1);
            AnomalyUnit au = {rt, stol(line.substr(0, splitIndex-1)),
                            stol(line.substr(splitIndex , lineLen - 1))};
            realTimeAnomalies.push_back(au);
            line = dio->read();
        }
    }

public:
    AnalyzeCommand(DefaultIO *dio, HybridAnomalyDetector *detector, const char *fileName, const char* detectFile)
        : AnomalyDetectionCommand(dio, detector), fileName(fileName), detectFile(detectFile) {
             this->description = "upload anomalies and analyze results";
        }

    virtual void execute() {
        dio->writeLine("Please upload your local anomalies file.");
        getRealTimeAnomalies();
        dio->writeLine("Upload complete.");

        TimeSeries ts = TimeSeries(detectFile);
        vector<AnomalyReport> report =
            this->detector->getDetectedAnomalyReport(ts);
            // cout << ",,.." << endl; // FOR DEBUG
            cout << "";
        combineDetectedAnomalies(report);    

        p = realTimeAnomalies.size();
        n = ts.getNumOfRows();
        // check : iterate through realtime vector and substract range from fp
        for_each(realTimeAnomalies.begin(), realTimeAnomalies.end(),
                 [this](AnomalyUnit au) { n -= au.range(); });

      
        countAccurateDetections();

        // print results
        printRes("True",  newPrecision(float(tp) / float(p)));
        printRes("False", newPrecision(float(fp) / float(n)));
        
        clear();

    }

        void countAccurateDetections() {
		int i = 0;
        for_each(
            combinedAnomalies.begin(), combinedAnomalies.end(),
            [&i, this](AnomalyUnit au) {
				// AnomalyUnit rtAnomaly = realTimeAnomalies[i];
                bool isFalseAlarm = true;
                for_each(realTimeAnomalies.begin(), realTimeAnomalies.end(),
                    [&isFalseAlarm, &au, this](AnomalyUnit rtAnomaly) {
                        if (isRangesIntersect(rtAnomaly, au)) {
                            ++tp;
                            isFalseAlarm = false;
                        }
                });
                if (isFalseAlarm) {
                    ++fp;
                }
            });
	}

	bool isRangesIntersect(AnomalyUnit a, AnomalyUnit b) {
		if (a.startTime <= b.endTime &&
			(a.startTime >= b.startTime || a.endTime >= b.startTime)) {
			return true;
		}		
		return false;
	}

	void printRes(string desc, float value) {
		dio->write(desc + " Positive Rate: ");
		dio->writeLine(value);
	}

    float newPrecision(float n) {
        return floor(n * 1000.0) / 1000.0;;
    }
    
    void clear() {
        realTimeAnomalies.clear();
        combinedAnomalies.clear();
        tp = 0;
        p = 0;
        n = 0;
        fp = 0;
    }

    ~AnalyzeCommand() {
        // delete fileName;
    }
};

class ExitCommand : public Command {
    bool *shouldStop;

public:
    ExitCommand(bool *shouldStop) : shouldStop(shouldStop) {
         this->description = "exit";
    }

    virtual void execute() { *(this->shouldStop) = true; 
        // cout << "ExitCommand" << endl; FOR DEBUG
    }

    ~ExitCommand() {
        
    }
};

#endif /* COMMANDS_H_ */
