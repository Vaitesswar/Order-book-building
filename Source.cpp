
#include <fstream>
#include <iostream>
#include <string>
#include <vector>  
#include <sstream> 
#include <list>
#include <unordered_map>
#include <map>
#include <chrono>
#include <algorithm>
using namespace std;

int sellComparator(pair<float, int> a, pair<float, int> b) {
	return a.first < b.first; /*Comparator for finding least 5 offer levels*/
}
int buyComparator(pair<float, int> a, pair<float, int> b) {
	return a.first > b.first; /*Comparator for finding highest 5 bid levels*/
}
void getData(string symbol, int start, int end) {
	/*Reads the sell,buy and trade data txt files*/
	fstream sellData;
	fstream buyData;
	fstream tradeData;
	string sellFilename = "./" + symbol + "/" + to_string((start - 1) - ((start - 1) % 100)) + "_sell.txt";
	string buyFilename = "./" + symbol + "/" + to_string((start - 1) - ((start - 1) % 100)) + "_buy.txt";
	string tradeFilename = "./" + symbol + "/" + to_string((start - 1) - ((start - 1) % 100)) + "_trade.txt";
	sellData.open(sellFilename, ios::in);
	buyData.open(buyFilename, ios::in);
	tradeData.open(tradeFilename, ios::in);
	unordered_map<float, int> buy;
	unordered_map<float, int> sell;

	/*Sell data is put into the map as {price:quantity}*/
	string line;
	int ind = 0;
	float key = 0.0;
	stringstream X(line);
	while (getline(sellData, line)) {
		string value;
		while (getline(X, value, ',')) {
			if (ind == 0) {
				sell[stof(value)] = 0;
				key = stof(value);
			}
			else if (ind == 1) {
				sell[key] = stoi(value);
			}
			ind += 1;
		}
	}

	/*Buy data is put into the map as {price:quantity}*/
	line = "";
	ind = 0;
	key = 0.0;
	stringstream Y(line);
	while (getline(buyData, line)) {
		string value;
		while (getline(Y, value, ',')) {
			if (ind == 0) {
				buy[stof(value)] = 0;
				key = stof(value);
			}
			else if (ind == 1) {
				buy[key] = stoi(value);
			}
			ind += 1;
		}
	}

	/*lastTradePrice and lastTradeQuantity are extracted.*/
	line = "";
	ind = 0;
	key = 0.0;
	float lastTradePrice = 0.0;
	int lastTradeQuantity = 0;
	stringstream Z(line);
	while (getline(tradeData, line)) {
		string value;
		while (getline(Z, value, ',')) {
			if (ind == 0) {
				lastTradePrice = stof(value);
				
			}
			else if (ind == 1) {
				lastTradeQuantity = stoi(value);
			}
			ind += 1;
		}
	}

	int startInd = 0;
	int endInd = 0;
	int printInd = 0;
	if ((start - 1) % 100 == 0) { /*If (start -1) is divisible by 100, then need to print first 
		before entering loop as no update is needed.*/
		cout << "Time: " << start << endl;
		cout << "Symbol: " << symbol << endl;
		cout << "Epoch: " << 0 << endl;
		cout << "Last trade price: " << lastTradePrice << endl;
		cout << "Last trade quantity: " << lastTradeQuantity << endl;
		vector< pair <float, int> > sellList(sell.begin(), sell.end());
		sort(sellList.begin(), sellList.end(), sellComparator); /*Sort by ascending comparator defined above*/
		cout << "Least 5 (Ask price, Ask quantity) pairs:" << "";
		for (int i = 0; i < sellList.size(); i += 1) {
			cout << "(" << sellList[i].first << "," << sellList[i].second << ")";
			if (i == 4 || i == sell.size() - 1) {
				break;
			}
			else {
				cout << ",";
			}
		}
		cout << "" << endl;
		vector< pair <float, int> > buyList(buy.begin(), buy.end());
		sort(buyList.begin(), buyList.end(), buyComparator); /*Sort by descending comparator defined above*/
		cout << "Highest 5 (Bid price, Bid quantity) pairs:" << ""; 
		for (int i = 0; i < buyList.size(); i += 1) {
			cout << "(" << buyList[i].first << "," << buyList[i].second << ")";
			if (i == 4 || i == buy.size() - 1) {
				break;
			}
			else {
				cout << ",";
			}
		}
		cout << "" << endl;
		cout << "----------------------------------------------------------------------" << endl;
		startInd = start; // Start from next operation
		endInd = end;
		printInd = start; // Printing starts from "1" while indexing starts from 0
	}
	else {
		/*If(start - 1) is not divisible by 100, then assign to find the nearest milestone text file containing the most updated order book
		for querying to avoid looping all over again.*/
		startInd = (start - 1) - ((start - 1) % 100) + 1; // Start from next operation
		endInd = end;
		printInd = (start - 1) - ((start - 1) % 100) + 1; // Printing starts from "1" while indexing starts from 0
	}
	
	fstream data;
	string filename = symbol + ".log";
	data.open(filename, ios::in);
	string str;
	ind = 0;
	while (getline(data, str)) {
		if (ind >= startInd and ind < endInd) { /*Only need data between startInd and endInd lines in the raw data file*/
			auto begin = std::chrono::high_resolution_clock::now();
			string value;
			list<string> strings;
			stringstream X(str);
			string side;
			string operation;
			float price = 0.0;
			int quantity = 0;
			int i = 0;
			/*From this line, the code is similar to readData function. Hence, please refer for the comments there.*/
			while (getline(X, value, ' ')) {
				/* X represents to read the string from stringstream, value use for store the token string and,
				 ' ' whitespace represents to split the string where whitespace is found. */
				if (!value.empty()) {
					if (i == 3) {
						side = value;
					}
					else if (i == 4) {
						operation = value;
					}
					else if (i == 5) {
						price = stof(value);
					}
					else if (i == 6) {
						quantity = stoi(value);
					}
					i += 1;
				}
			}
			if (side == "SELL") {
				if (operation == "NEW") {
					if (sell.count(price) > 0) {
						sell[price] += quantity;
					}
					else {
						sell[price] = quantity;
					}
				}
				else if (operation == "CANCEL" || operation == "TRADE") {
					sell[price] -= quantity;
					if (operation == "TRADE") {
						lastTradePrice = price;
						lastTradeQuantity = quantity;
					}
					if (sell[price] <= 0) {
						sell.erase(price);
					}
				}
			}
			else if (side == "BUY") {
				if (operation == "NEW") {
					if (buy.count(price) > 0) {
						buy[price] += quantity;
					}
					else {
						buy[price] = quantity;
					}
				}
				else if (operation == "CANCEL" || operation == "TRADE") {
					buy[price] -= quantity;
					if (operation == "TRADE") {
						lastTradePrice = price;
						lastTradeQuantity = quantity;
					}
					if (buy[price] <= 0) {
						buy.erase(price);
					}
				}
			}
			auto finish = std::chrono::high_resolution_clock::now();
			auto time_in_nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - begin).count();

			if (ind + 1 >= start && ind + 1 <= end) {
				cout << "Time: " << ind + 1 << endl;
				cout << "Symbol: " << symbol << endl;
				cout << "Epoch: " << time_in_nanoseconds << endl;
				cout << "Last trade price: " << lastTradePrice << endl;
				cout << "Last trade quantity: " << lastTradeQuantity << endl;
				vector< pair <float, int> > sellList(sell.begin(), sell.end());
				sort(sellList.begin(), sellList.end(), sellComparator);
				cout << "Least 5 (Ask price, Ask quantity) pairs:" << "";
				for (int i = 0; i < sellList.size(); i += 1) {
					cout << "(" << sellList[i].first << "," << sellList[i].second << ")";
					if (i == 4 || i == sell.size() - 1) {
						break;
					}
					else {
						cout << ",";
					}
				}
				cout << "" << endl;
				vector< pair <float, int> > buyList(buy.begin(), buy.end());
				sort(buyList.begin(), buyList.end(), buyComparator);
				cout << "Highest 5 (Bid price, Bid quantity) pairs:" << "";
				for (int i = 0; i < buyList.size(); i += 1) {
					cout << "(" << buyList[i].first << "," << buyList[i].second << ")";
					if (i == 4 || i == buy.size() - 1) {
						break;
					}
					else {
						cout << ",";
					}
				}
				cout << "" << endl;
				cout << "----------------------------------------------------------------------" << endl;
			}
		}
		else if (ind + 1 >= endInd) {
			break;
		}
		ind += 1;
	}
}
void readData(string symbol) { /* readData function runs through the whole time-series data 
	and saves the orderbook at regular intervals of 100 timesteps. This helps to optimize the search process later.*/
	fstream data;
	string filename = symbol + ".log";
	data.open(filename, ios::in);
	unordered_map<float, int> buy;
	unordered_map<float, int> sell;
	float lastTradePrice = 0.0;
	int lastTradeQuantity = 0;
	string str;
	int ind = 0;
	while (getline(data, str)) {
		string value;
		list<string> strings;
		stringstream X(str);
		string side;
		string operation;
		float price = 0.0;
		int quantity = 0;
		int i = 0;
		while (getline(X, value, ' ')) { /* This step gets the 4 parameters namely side, operation, price and quantity from each line.*/

			if (!value.empty()) {
				if (i == 3) {
					side = value;
				}
				else if (i == 4) {
					operation = value;
				}
				else if (i == 5) {
					price = stof(value);
				}
				else if (i == 6) {
					quantity = stoi(value);
				}
				i += 1;
			}
		}
		if (side == "SELL") {
			if (operation == "NEW") {
				if (sell.count(price) > 0) {
					sell[price] += quantity; /*If SELL & NEW & ALREADY EXISTS, then perform cumulative sum on quantity*/
				}
				else {
					sell[price] = quantity; /*If SELL & NEW & DOES NOT EXIST, then create a new key-value pair*/
				}
			}
			else if (operation == "CANCEL" || operation == "TRADE") {
				sell[price] -= quantity; /*If SELL & (CANCEL || TRADE), then subtract the particular quantity of that order*/
				if (operation == "TRADE") {
					lastTradePrice = price; /* Updates the last trade price and quantity*/
					lastTradeQuantity = quantity;
				}
				if (sell[price] <= 0) {
					sell.erase(price); /* Removes a price value from map if all shares are cancelled or traded.*/
				}
			}
		}
		else if (side == "BUY") { 
			if (operation == "NEW") {
				if (buy.count(price) > 0) {
					buy[price] += quantity; /*If BUY & NEW & ALREADY EXISTS, then perform cumulative sum on quantity*/
				}
				else {
					buy[price] = quantity; /*If BUY & NEW & DOES NOT EXIST, then create a new key-value pair*/
				}
			}
			else if (operation == "CANCEL" || operation == "TRADE") {
				buy[price] -= quantity; /*If BUY & (CANCEL || TRADE), then subtract the particular quantity of that order*/
				if (operation == "TRADE") {
					lastTradePrice = price; /* Updates the last trade price and quantity*/
					lastTradeQuantity = quantity;
				}
				if (buy[price] <= 0) {
					buy.erase(price); /* Removes a price value from map if all shares are cancelled or traded.*/
				}
			}
		}

		if (ind % 100 == 0) { /* Order book is saved at regular intervals of 100 timesteps.*/
			fstream sellOutput; /*sell data is stored as (value,quantity) pairs in a txt.file seperated by comma delimiter*/
			sellOutput.open("./" + symbol + "/" + to_string(ind) + "_sell.txt", ios::out);
			vector< pair <float, int> > sellList(sell.begin(), sell.end());
			if (sellOutput.is_open()) {
				for (int i = 0; i < sellList.size(); i += 1) {
					sellOutput << sellList[i].first << "," << sellList[i].second << "\n";
				}
				sellOutput.close();
			}

			fstream buyOutput; /*buy data is stored as (value,quantity) pairs in a txt.file seperated by comma delimiter*/
			buyOutput.open("./" + symbol + "/" + to_string(ind) + "_buy.txt", ios::out);
			vector< pair <float, int> > buyList(sell.begin(), sell.end());
			if (buyOutput.is_open()) {
				for (int i = 0; i < buyList.size(); i += 1) {
					buyOutput << buyList[i].first << "," << buyList[i].second << "\n";
				}
				buyOutput.close();
			}

			fstream tradeOutput; /*last trade price and quantity are stored in a txt.file seperated by comma delimiter*/
			tradeOutput.open("./" + symbol + "/" + to_string(ind) + "_trade.txt", ios::out);
			tradeOutput << lastTradePrice << "," << lastTradeQuantity;
			tradeOutput.close();
		}
		ind += 1;
	}
}
int main()
{	
	string symbol = "SCS";
	int start = 1175500-10001; // Start time step (starts from 1)
	int end = 1175500; // End time step (stops at maximum timesteps in the raw data)
	readData(symbol); // Create a folder manually before executing this step (e.g. "SCH")
	getData(symbol,start,end);
}