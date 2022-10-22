import pandas as pd
from collections import OrderedDict
import pickle
import time
import os

def readData(symbol):
    os.mkdir(symbol)
    data = pd.read_csv(symbol + ".log",sep = "\t", header = None).values
    buy = dict()
    sell = dict()
    lastTradePrice = 0.0
    lastTradeQuantity = 0
    for ind,values in enumerate(data):
        values = values[2].split()
        price = float(values[3])
        quantity = int(values[4])
        if "SELL" in values:
            if "NEW" in values:
                if price in sell:
                    sell[price] += quantity
                else:
                    sell[price] = quantity
            elif "CANCEL" or "TRADE" in values:
                sell[price] -= quantity
                if "TRADE" in values:
                    lastTradePrice = price
                    lastTradeQuantity = quantity
                if sell[price] <= 0:
                    sell.pop(price)
        elif "BUY" in values:
            if "NEW" in values:
                if price in buy:
                    buy[price] += quantity
                else:
                    buy[price] = quantity
            elif "CANCEL" or "TRADE" in values:
                buy[price] -= quantity
                if "TRADE" in values:
                    lastTradePrice = price
                    lastTradeQuantity = quantity
                if buy[price] <= 0:
                    buy.pop(price)
        if ind % 100 == 0:
            dictionary = {'SELL': sell, 'BUY': buy, 'tradePrice':lastTradePrice, 'tradeQuantity':lastTradeQuantity}
            try:
                file = open("./" + symbol + "/" + str(ind) + ".txt", "wb")
                pickle.dump(dictionary, file)
                file.close()
            except:
                print("Unable to write to file")
    dictionary = {'SELL': sell, 'BUY': buy, 'tradePrice':lastTradePrice, 'tradeQuantity':lastTradeQuantity}
    try:
        file = open("./" + symbol + "/" + "lastOrderBook" + ".txt", "wb")
        pickle.dump(dictionary, file)
        file.close()
    except:
        print("Unable to write to file")


def getData(symbol,start,end):
    startFile = "./" + symbol + "/" + str((start-1) - ((start-1) % 100)) + '.txt'
    with open(startFile, 'rb') as handle:
        data = handle.read()
    data = pickle.loads(data)

    buy = data["BUY"]
    sell = data["SELL"]
    lastTradePrice = data["tradePrice"]
    lastTradeQuantity = data["tradeQuantity"]
    sell = OrderedDict(sorted(sell.items()))
    buy = OrderedDict(reversed(sorted(buy.items())))
    epoch = time.time_ns()
    askValQuant = list()
    bidValQuant = list()
    for i in range(len(sell)):
        askItem = list(sell.items())[i]
        askValQuant.append(askItem)
        if (i == 4):
            break
    for i in range(len(buy)):
        bidItem = list(buy.items())[i]
        bidValQuant.append(bidItem)
        if (i == 4):
            break
    if ((start-1) % 100 == 0):
        print("Time: " + str(start))
        print("Symbol: " + symbol)
        print("Epoch: " + str(epoch))
        print("Least " + str(len(askValQuant)) + " (Ask price, Ask quantity) pairs:", askValQuant)
        print("Highest " + str(len(bidValQuant)) + " (Bid price, Bid quantity) pairs:", bidValQuant)
        print("Last trade price: " + str(lastTradePrice))
        print("Last trade quantity: " + str(lastTradeQuantity))
        print("---------------------------------------------------------------------------------------------------------")
        startInd = start # Start from next element
        endInd = end
        printInd = start # Printing starts from "1" while indexing starts from 0
    else:
        startInd = (start-1) - ((start-1) % 100) + 1 # Start from next element
        endInd = end
        printInd = (start-1) - ((start-1) % 100) + 1 # Printing starts from "1" while indexing starts from 0
    data = pd.read_csv(symbol + ".log",sep = "\t", header = None).values
    for values in data[startInd:endInd]:
        values = values[2].split()
        price = float(values[3])
        quantity = int(values[4])
        if "SELL" in values:
            if "NEW" in values:
                if price in sell:
                    sell[price] += quantity
                else:
                    sell[price] = quantity
            elif "CANCEL" or "TRADE" in values:
                sell[price] -= quantity
                if "TRADE" in values:
                    lastTradePrice = price
                    lastTradeQuantity = quantity
                if sell[price] <= 0:
                    sell.pop(price)
        elif "BUY" in values:
            if "NEW" in values:
                if price in buy:
                    buy[price] += quantity
                else:
                    buy[price] = quantity
            elif "CANCEL" or "TRADE" in values:
                buy[price] -= quantity
                if "TRADE" in values:
                    lastTradePrice = price
                    lastTradeQuantity = quantity
                if buy[price] <= 0:
                    buy.pop(price)
        sell = OrderedDict(sorted(sell.items()))
        buy = OrderedDict(reversed(sorted(buy.items())))
        askValQuant = list()
        bidValQuant = list()
        for i in range(len(sell)):
            askItem = list(sell.items())[i]
            askValQuant.append(askItem)
            if (i == 4):
                break
        for i in range(len(buy)):
            bidItem = list(buy.items())[i]
            bidValQuant.append(bidItem)
            if (i == 4):
                break
        printInd += 1
        if (printInd >= start):
            print("Time: " + str(printInd))
            print("Symbol: " + symbol)
            print("Epoch: " + str(epoch))
            print("Least " + str(len(askValQuant)) + " (Ask price, Ask quantity) pairs:", askValQuant)
            print("Highest " + str(len(bidValQuant)) + " (Bid price, Bid quantity) pairs:", bidValQuant)
            print("Last trade price: " + str(lastTradePrice))
            print("Last trade quantity: " + str(lastTradeQuantity))
            print("---------------------------------------------------------------------------------------------------------")

def insertDeleteData(symbol,side,operation,price,quantity):
    startFile = "./" + symbol + '/lastOrderBook.txt'
    with open(startFile, 'rb') as handle:
        data = handle.read()
    data = pickle.loads(data)
    buy = data["BUY"]
    sell = data["SELL"]
    lastTradePrice = data["tradePrice"]
    lastTradeQuantity = data["tradeQuantity"]
    if side == "SELL":
        if operation == "NEW":
            if price in sell:
                sell[price] += quantity
            else:
                sell[price] = quantity
        elif (operation == "CANCEL") or (operation == "TRADE"):
            sell[price] -= quantity
            if operation == "TRADE":
                lastTradePrice = price
                lastTradeQuantity = quantity
            if sell[price] <= 0:
                sell.pop(price)
    elif side == "BUY":
        if operation == "NEW":
            if price in buy:
                buy[price] += quantity
            else:
                buy[price] = quantity
        elif (operation == "CANCEL") or (operation == "TRADE"):
            buy[price] -= quantity
            if operation == "TRADE":
                lastTradePrice = price
                lastTradeQuantity = quantity
            if buy[price] <= 0:
                buy.pop(price)
    dictionary = {'SELL': sell, 'BUY': buy, 'tradePrice':lastTradePrice, 'tradeQuantity':lastTradeQuantity}
    try:
        file = open("./" + symbol + "/" + "lastOrderBook" + ".txt", "wb")
        pickle.dump(dictionary, file)
        file.close()
    except:
        print("Unable to write to file")

if __name__ == "__main__":
    readData("SCH")
    readData("SCS")
    getData("SCH",10000,11000)
    getData("SCS",1175500-10001,1175500)   
    insertDeleteData(symbol = "SCH",side = "SELL",operation = "NEW",price = 107.7,quantity = 5)
    insertDeleteData(symbol = "SCS",side = "BUY",operation = "NEW",price = 108.9,quantity = 13)